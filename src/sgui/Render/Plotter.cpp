#include "sgui/Render/Plotter.h"
#include "sgui/Core/Interpolation.h"

namespace sgui
{
/////////////////////////////////////////////////
void Plotter::setSample (const uint32_t sample)
{
  mSample = sample;
}

/////////////////////////////////////////////////
void Plotter::unsetBound ()
{
  mBounded = false;
}

/////////////////////////////////////////////////
void Plotter::setBound (const sf::Vector2f& plotBound)
{
  mBounded = true;
  mBound = plotBound;
}

/////////////////////////////////////////////////
void Plotter::setBorderTick (const uint32_t count)
{
  mBorderTick = count;
}

/////////////////////////////////////////////////
void Plotter::setBorderWidth (const float thickness)
{
  mBorderWidth = thickness;
}

/////////////////////////////////////////////////
void Plotter::setBorderColor (const sf::Color& color)
{
  mBorderColor = color;
}

/////////////////////////////////////////////////
void Plotter::clear ()
{
  render.clear ();
}

/////////////////////////////////////////////////
void Plotter::plot (
  const std::function<float (float)>& slope,
  const sf::Vector2f& position,
  const sf::Color& lineColor,
  const float thickness)
{
  // draw n=sample lines with the wanted slope
  const auto max = static_cast<float> (mSample);

  // we use int to avoid dubious bug with i-1
  for (int i = 0; i < static_cast<int>(mSample); i++) {
    const auto xA = lerp (xRange.min, xRange.max, i / max);
    const auto xB = lerp (xRange.min, xRange.max, (i + 1) / max);
    const auto xPrev = lerp (xRange.min, xRange.max, (i - 1) / max);
    const auto xNext = lerp (xRange.min, xRange.max, (i + 2) / max);
    const auto a = position + toPlot (xA, slope (xA));
    const auto b = position + toPlot (xB, slope (xB));
    render.loadConnected (
      LineFloat (a, b),
      position + toPlot (xPrev, slope (xPrev)),
      position + toPlot (xNext, slope (xNext)),
      thickness,
      lineColor
    );
  }

  // draw border and axis on top
  if (mBounded) {
    drawBorderAndAxes (position);
  }
}

/////////////////////////////////////////////////
void Plotter::plot (
  const std::function<sf::Vector2f (float)>& slope,
  const sf::Vector2f& position,
  const sf::Color& lineColor,
  const float thickness)
{
  // draw n=sample lines with the wanted slope
  const auto max = static_cast<float> (mSample);

  // we use int to avoid dubious bug with i-1
  for (int i = 0; i < static_cast<int>(mSample); i++) {
    const auto a = slope (lerp (xRange.min, xRange.max, i / max));
    const auto b = slope (lerp (xRange.min, xRange.max, (i + 1) / max));
    const auto prev = slope (lerp (xRange.min, xRange.max, (i - 1) / max));
    const auto next = slope (lerp (xRange.min, xRange.max, (i + 2) / max));
    render.loadConnected (
      LineFloat (position + toPlot (a), position + toPlot (b)),
      position + toPlot (prev),
      position + toPlot (next),
      thickness,
      lineColor
    );
  }

  // draw border and axis on top
  if (mBounded) {
    drawBorderAndAxes (position);
  }
}

/////////////////////////////////////////////////
void Plotter::plot (
  const std::vector<sf::Vector2f>& points,
  const sf::Vector2f& position,
  const sf::Color& lineColor,
  const float thickness)
{
  // we use int to avoid dubious bug with i-1
  for (uint32_t i = 1; i < points.size () - 2; i++) {
    const auto a = points[i];
    const auto b = points[i + 1];
    const auto prev = points[i - 1];
    const auto next = points[i + 2];
    render.loadConnected (
      LineFloat (position + toPlot (a), position + toPlot (b)),
      position + toPlot (prev),
      position + toPlot (next),
      thickness,
      lineColor
    );
  }

  // draw border and axis on top
  if (mBounded) {
    drawBorderAndAxes (position);
  }
}

/////////////////////////////////////////////////
sf::Vector2f Plotter::toPlot (
  const float pointX,
  const float pointY) const
{
  if (mBounded) {
    const auto x = remap (xRange.min, xRange.max, 0.f, mBound.x, pointX);
    const auto y = remap (-yRange.max, -yRange.min, 0.f, mBound.y, -pointY);
    return sf::Vector2f (x, y);
  }
  return sf::Vector2f (pointX, pointY);
}

/////////////////////////////////////////////////
sf::Vector2f Plotter::toPlot (const sf::Vector2f& point) const
{
  return toPlot (point.x, point.y);
}

/////////////////////////////////////////////////
void Plotter::drawBorderAndAxes (
  const sf::Vector2f& pos)
{
  // draw axis with a given tick count
  const auto max = static_cast<float> (mBorderTick + 1);
  const auto dx = mBound.x / max;
  const auto dy = mBound.y / max;
  const auto length = mBound.y / 20.f;
  const auto bottom = sf::Vector2f (pos.x, pos.y + mBound.y);
  for (uint32_t i = 0; i < (mBorderTick + 1); i++) {
    const auto xTick1 = bottom + sf::Vector2f (i * dx, 0.f);
    const auto xTick2 = bottom + sf::Vector2f (i * dx, -length);
    const auto yTick1 = pos + sf::Vector2f (0.f, i * dy);
    const auto yTick2 = pos + sf::Vector2f (length, i * dy);
    render.load (LineFloat (xTick1, xTick2), mBorderWidth, mBorderColor);
    render.load (LineFloat (yTick1, yTick2), mBorderWidth, mBorderColor);
  }

  // draw border
  const auto border = sf::FloatRect (pos, mBound);
  render.load (border, mBorderWidth, mBorderColor);
}

/////////////////////////////////////////////////
void Plotter::draw (
  sf::RenderTarget& target,
  sf::RenderStates states) const
{
  states.transform *= getTransform ();
  target.draw (render, states);
}

} // namespace sgui
