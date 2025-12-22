#pragma once

#include <functional>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "sgui/Render/PrimitiveShapeRender.h"

namespace sgui
{
/**
 * @brief plot range for an axis
 */
struct PlotRange {
  PlotRange () = default;
  PlotRange (const float mi, const float ma)
    : min (mi), max (ma) {}
  float min = 0.f; ///< Minimum of the range
  float max = 1.f; ///< Maximum of the range
};


/**
 * @brief plot function in a given range and level of sampling.
 */
class Plotter : public sf::Drawable, public sf::Transformable
{
public:
  /**
   * @brief set points to sample function, default is 50 points.
   * @param sample is the number of points used to sample curve.
   */
  void setSample (const uint32_t sample);
  /**
   * @brief unset bound size of the plot
   */
  void unsetBound ();
  /**
   * @brief set bound in which the function will be drawn
   */
  void setBound (const sf::Vector2f& plotBound);
  /**
   * @brief set border rendering for bounded plot
   * @param count is the number of subdivision for abscissa and ordinate
   */
  void setBorderTick (const uint32_t count);
  /**
   * @brief set border widtj
   * @param thickness of the border
   */
  void setBorderWidth (const float thickness);
  /**
   * @brief set color of the border
   * @param color of the border
   */
  void setBorderColor (const sf::Color& color);
  /**
   * @brief clear all plotted function
   */
  void clear ();
  /**
   * @brief plot a function y = f(x)
   * @param slope is a lambda of the form y = f(x)
   * @param position is the position of the plot
   * @param lineColor set the color of the line
   * @param thickness set the thickness of the line
   */
  void plot (const std::function<float (float)>& slope, const sf::Vector2f& position, const sf::Color& lineColor, const float thickness = 1.f);
  /**
   * @brief plot a parametric function (x, y) = f(t)
   * @param slope is a lambda of the form (x, y) = f(t)
   * @param position is the position of the plot
   * @param lineColor set the color of the line
   * @param thickness set the thickness of the line
   */
  void plot (const std::function<sf::Vector2f (float)>& slope, const sf::Vector2f& position, const sf::Color& lineColor, const float thickness = 1.f);
  /**
   * @brief plot a set of point
   * @param points is the set of point to be drawn
   * @param position is the position of the plot
   * @param lineColor set the color of the line/dot
   * @param thickness set the thickness of the line/dot
   */
  void plot (const std::vector <sf::Vector2f>& points, const sf::Vector2f& position, const sf::Color& lineColor, const float thickness);
public:
  PlotRange xRange = {};       ///< Plot range along x
  PlotRange yRange = {};       ///< Plot range along y
  PrimitiveShapeRender render; ///< Render used
private:
  // to remap value in the draw area
  sf::Vector2f toPlot (const float pointX, const float pointY) const;
  sf::Vector2f toPlot (const sf::Vector2f& point) const;
  // plot boundaries and axes for bounded plot
  void drawBorderAndAxes (const sf::Vector2f& position);
  // draw all function plotted
  void draw (sf::RenderTarget& target, sf::RenderStates states) const override;
private:
  bool mBounded = false;
  uint32_t mSample = 50;
  uint32_t mBorderTick = 5;
  float mBorderWidth = 4.f;
  sf::Vector2f mBound = {};
  sf::Color mBorderColor = sf::Color::White;
};

} // namespace sgui
