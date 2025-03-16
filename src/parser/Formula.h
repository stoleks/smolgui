#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <string>
#include <iostream>

#include "MathParser.h"

namespace sgui 
{
class Formula {
public:
  Formula (sf::Font& font) : mathFont (font) {}
 
  //////
  void printFormula (const std::string& str, sf::RenderWindow& screen, Style style)
  {
    characterSize = style.fontSize.normal;
    color = style.fontColor;
    auto* root = buildTree (str);
    auto x = 0.f;
    buildFormulaCoordinates (root, 0, x);
    auto xOffset = (screen.getSize ().x - root->size.width) * 0.5f;
    buildFormulaCoordinates (root, std::max (0.f, screen.getSize().y * 0.1f - root->height.up), xOffset);
    drawFormula (root, screen);
  }
  
  //////
  float getCharacterSize()
  {
    return characterSize;
  }

private:
  sf::Font& mathFont;
  sf::Color color;
  float characterSize = 40.f;
  float spacing = characterSize * 0.25f;
  /**
   *
   */
  struct FormulaTree 
  {
    char operation;
    bool parentheses;
    std::string formula;
    Parser::Function function;
    FormulaTree* leftTree;
    FormulaTree* rightTree;
 
    FormulaTree (std::string treeForm)
    {
      leftTree = nullptr;
      rightTree = nullptr;
      parentheses = false;
      operation = 0;
      formula = treeForm;
    }

    struct position {
      float xFormula = 0, yFormula = 0;
      float xOperator = 0, yOperator = 0;
    } position;
 
    struct size {
      float width = 0;
      float height = 0;
    } size;
 
    struct height {
      float down = 0;
      float up = 0;
    } height;
  };

 //Misc
 //////
 bool isOnlyTerm (FormulaTree* node)
 {
   return (node->leftTree == nullptr && node->rightTree == nullptr); 
 }

 //////
 std::string deleteParentheses (const std::string& expression)
 {
   auto str = expression;
   str.pop_back ();
   return str.substr(1);
 }

 //////
 float getWidthOfString (const std::string& str, float height = 0.f)
 {
   auto txt = sf::Text (mathFont);
   txt.setCharacterSize (characterSize + height);
   txt.setString (str);
   return txt.getLocalBounds().size.x;
 }
 
 //////
 float getHeightOfString (const std::string& str, float height = 0.f)
 {
   auto txt = sf::Text (mathFont);
   txt.setCharacterSize (characterSize + height);
   txt.setString (str);
   return txt.getLocalBounds().size.y;
 }
 
 //////
 float getWidthOfChar (const char c, float height = 0.f)
 {
   return getWidthOfString (std::string (1, c), height);
 }
 
 //////
 float getHeightOfChar (const char c, float height = 0.f)
 {
   return getHeightOfString (std::string (1, c), height);
 }

 
 //////
 void drawSquareRoot(sf::RenderWindow& window, const float xDown, const float yDown, const float xUp, const float yUp)
 {
   sf::VertexArray squareRoot (sf::PrimitiveType::LineStrip, 4);

   squareRoot[0].position = sf::Vector2f (xDown, yDown - (yDown - yUp) * 0.4);
   squareRoot[1].position = sf::Vector2f (xDown + characterSize * 0.66, yDown);
   squareRoot[2].position = sf::Vector2f (xDown + characterSize * 1.33, yUp);
   squareRoot[3].position = sf::Vector2f (xUp, yUp);

   // Set the color for the square root symbol
   for (size_t i = 0; i < squareRoot.getVertexCount (); ++i) {
     squareRoot[i].color = color;
   }

   // Draw the square root symbol on the window
   window.draw (squareRoot);
 }


 //--------------------------------------------------------------------------------------
 FormulaTree* buildTree (const std::string& expression)
 {
   //Create new node of tree
   auto formula = Parser::removeSpace (expression);
   FormulaTree* node = new FormulaTree (formula);
   
   // If it has parentheses then we eliminate them and go to the next node
   int numberOfOperators = 0;
   int numberOfMulDivOperators = 0;
   if (formula[0] == '(' && formula[int(formula.size() - 1)] == ')') {
     if (Parser::isNextParenthesesValid (formula)) {
       node->parentheses = true;
       node->leftTree = buildTree (deleteParentheses (formula));
       return node;
     }
   }

   // Check if the input is a function
   node->function = Parser::parseNextFunction (formula);
   
   if (node->function != Parser::Function::None) {
     const auto functionSize = Parser::toString (node->function).size () + 1;
     if (node->function == Parser::Function::sqrt) {
       node->leftTree = buildTree (deleteParentheses (formula.substr (functionSize)));
     }
     else {
       node->leftTree = buildTree (formula.substr (functionSize));
     }
     return node;
   }
   
   // Check what is the lowest priority operation in the current string. Addition
   int addSubIndex = -1;
   int addSubPriority = 1e9;
   // Multiplication
   int mulIndex = -1;
   int mulPriority = 1e9; 
   // Division
   int divIndex = -1;
   int divPriority = 1e9;
   // Power
   int powIndex = -1;
   int powPriority = 1e9;
   // Group
   int minimumPriority = 1e9;
   int currentPriority = 0;
   
   int index = 0;
   for (const auto character : formula) {
     // Group
     if (character == '(') currentPriority++;
     if (character == ')') currentPriority--;
     // Addition
     if ((character == '+' || character == '-') && currentPriority < addSubPriority) {
       addSubPriority = currentPriority;
       addSubIndex = index;
     }
     // Multiplication
     if ((character == '*' || character == sgui::Parser::dot) && currentPriority < mulPriority) {
       mulPriority = currentPriority;
       mulIndex = index;
     }
     // Division
     if ((character == '/') && currentPriority < divPriority) {
       divPriority = currentPriority;
       divIndex = index;
     }
     // Power
     if ((character == '^') && currentPriority < powPriority) {
       powPriority = currentPriority;
       powIndex = index;
     }
     index++;
   }
   
   // Check if there is at least an operator
   if (addSubIndex == -1 && mulIndex == -1 && powIndex == -1 && divIndex == -1) {
     return node;
   }
   
   // Find which operator has the lowest priority and it's location
   minimumPriority = std::min (std::min (addSubPriority, mulPriority), std::min(powPriority, divPriority));
   
   int operatorIndex = 0;
   if (minimumPriority == addSubPriority) {
     operatorIndex = addSubIndex;
   }
   else if (minimumPriority == mulPriority) {
     operatorIndex = mulIndex;
   }
   else if (minimumPriority == divPriority) {
     operatorIndex = divIndex;
   }
   else {
     operatorIndex = powIndex;
   }
   
   // Break the current formula in two parts -> the left part of the operation and the right part
   node->operation = formula [operatorIndex];
   node->leftTree = buildTree (formula.substr (0, operatorIndex));
   node->rightTree = buildTree (formula.substr (operatorIndex + 1));
   return node;
 }

 
 //////
 void buildFormulaCoordinates (FormulaTree* node, float heightPos, float& widthPos)
 {
   // Quit if there is no formula
   if (node == nullptr) return;

   // Check if node is a leaf and mark the coordinates
   if (isOnlyTerm (node)) {
     node->position.xFormula = widthPos;
     node->position.yFormula = heightPos;
     node->height.up = heightPos;
     node->height.down = heightPos;
     node->size.width = getWidthOfString (node->formula);
     node->size.height = 0;
     widthPos += getWidthOfString (node->formula);
     return;
   }

   if (node->operation == '/') {
     // Get height and width coordinates
     const float savedWidthPos = widthPos;
     buildFormulaCoordinates (node->leftTree, heightPos - 0.7f * characterSize, widthPos);
     widthPos = savedWidthPos;
     buildFormulaCoordinates (node->rightTree, heightPos + 0.7f * characterSize, widthPos);
     widthPos = savedWidthPos;

     // Rebuild the position with correct settings
     auto maxWidth = std::max (node->leftTree->size.width, node->rightTree->size.width); 
     widthPos += (maxWidth - node->leftTree->size.width) * 0.5f;
     buildFormulaCoordinates (node->leftTree, (heightPos - 0.7f * characterSize) * 2 - node->leftTree->height.down, widthPos);

     maxWidth = std::max (node->leftTree->size.width, node->rightTree->size.width); 
     widthPos = savedWidthPos + (maxWidth - node->rightTree->size.width) * 0.5f;
     buildFormulaCoordinates (node->rightTree, (heightPos + 0.7f * characterSize) * 2 - node->rightTree->height.up + characterSize * 0.28f, widthPos);

     node->height.up = node->leftTree->height.up;
     node->height.down = node->rightTree->height.down;
     node->size.width = maxWidth;
     node->size.height = std::abs (node->height.down - node->height.up);

     node->position.xOperator = savedWidthPos;
     node->position.yOperator = heightPos + 0.7f * characterSize;

     //Reset position after fraction
     widthPos = savedWidthPos + std::max (node->leftTree->size.width, node->rightTree->size.width);
     return;
   }
   else if (node->operation == '^') {
     // Get base coordinates
     buildFormulaCoordinates(node->leftTree, heightPos, widthPos);
     const float savedWidthPos = widthPos;

     // Calculate the proportion of the exponent
     buildFormulaCoordinates(node->rightTree, heightPos - characterSize*0.7, widthPos);
     widthPos = savedWidthPos;

     // Recalculate the position depending on the size of the exponent
     buildFormulaCoordinates(node->rightTree, (heightPos - characterSize*0.7) * 2 - node->rightTree->height.down - node->leftTree->size.height, widthPos);

     // Save the information of the current formula
     node->height.up = node->rightTree->height.up;
     node->height.down = node->leftTree->height.down;
     node->size.width = node->leftTree->size.width + node->rightTree->size.width;
     node->size.height = std::max(0.f, node->height.down - node->height.up);
     return;
   }
   else if (node->parentheses) {
     node->position.xOperator = widthPos;
     // Calculate the proportion of the formula within the parantheses
     buildFormulaCoordinates (node->leftTree, heightPos, widthPos);

     widthPos = node->position.xOperator + getWidthOfChar ('(', node->leftTree->size.height)
      + getHeightOfChar ('(', node->leftTree->size.height) * 0.15f;

     // Recalculate the positions
     buildFormulaCoordinates (node->leftTree, heightPos, widthPos);

     // Adjust height of parantheses depending on the proportions of the formula
     node->position.yOperator = (node->leftTree->height.down + node->leftTree->height.up
      - std::abs(node->leftTree->height.down - node->leftTree->height.up)) * 0.5f
      - (node->leftTree->size.height > 0 ? (getHeightOfChar ('(', node->leftTree->size.height) * 0.25f) : 0)
      + (node->leftTree->operation == '^' ? characterSize * 0.7f : 0);
     widthPos = widthPos + getHeightOfChar (')', node->leftTree->size.height) * 0.15f + getWidthOfChar(')', node->leftTree->size.height);

     // Save the information of the current formula
     node->size.width = widthPos - node->position.xOperator;
     node->height.up = node->leftTree->height.up;
     node->height.down = node->leftTree->height.down;
     node->size.height = std::abs (node->height.up - node->height.down);
     return;
   }
   else if (node->function != Parser::Function::None) {
     node->position.xOperator = widthPos;
     node->position.yOperator = heightPos;
     widthPos += getWidthOfString (Parser::toString (node->function));

     if (node->function == Parser::Function::sqrt) {
       // widthPos = node->position.xOperator + getWidthOfString ("LL");
       buildFormulaCoordinates (node->leftTree, heightPos + characterSize * 0.2f, widthPos);
     }
     else {
       buildFormulaCoordinates (node->leftTree, heightPos, widthPos);
     }

     node->size.width = widthPos - node->position.xOperator;
     node->size.height = node->leftTree->size.height;
     node->height.up = node->leftTree->height.up - characterSize * 0.20f;
     node->height.down = node->leftTree->height.down;
     return;
   }
   else if (!node->parentheses) {
     // Build the first term
     buildFormulaCoordinates(node->leftTree, heightPos, widthPos);

     // Build the operator
     widthPos += spacing;
     node->position.xOperator = widthPos;
     node->position.yOperator = heightPos;
     widthPos += (spacing + getWidthOfChar(node->operation));

     //Build the second term
     buildFormulaCoordinates(node->rightTree, heightPos, widthPos);

     //Save the information of the current formula
     node->height.down = std::max (node->leftTree->height.down, node->rightTree->height.down);
     node->height.up = std::min (node->leftTree->height.up, node->rightTree->height.up);
     node->size.width = 
       node->leftTree->size.width + node->rightTree->size.width + 2 * spacing + getWidthOfChar (node->operation);
     node->size.height = std::abs (node->height.down - node->height.up);
     return;
   }
 }
 
 //////
 void drawFormula (FormulaTree* node, sf::RenderWindow& window)
 {
   // quit if node is empty
   if (node == nullptr) return;

   // set global text properties 
   auto text = sf::Text (mathFont);
   text.setCharacterSize (characterSize);
   text.setFillColor (color);

   // Base node
   if (isOnlyTerm (node)) {
     text.setPosition ({node->position.xFormula, node->position.yFormula});
     text.setString (node->formula);
     window.draw (text);
     return;
   }
   // Parentheses
   else if (node->parentheses) {
     // left parenthesis
     text.setCharacterSize (characterSize + node->leftTree->size.height * 1.2f);
     text.setPosition ({node->position.xOperator, node->position.yOperator});
     text.setString ("(");
     window.draw (text);
 
     // formula
     drawFormula (node->leftTree, window);
 
     // right parenthesis
     text.setString (")");
     const auto x = node->position.xOperator 
       + node->leftTree->size.width 
       + 0.15f  * getHeightOfChar (')', node->leftTree->size.height)
       + 0.609f * getWidthOfChar (')', node->leftTree->size.height) 
       + int(characterSize * 3 / getWidthOfChar (')', node->leftTree->size.height));
     text.setPosition ({x, node->position.yOperator});
     window.draw (text);
     return;
   }
   // Functions
   else if (node->function != Parser::Function::None) {
     if (node->function == Parser::Function::sqrt) {
       text.setPosition ({node->position.xOperator, node->position.yOperator});
       text.setCharacterSize (characterSize + node->leftTree->size.height * 1.2f);
       text.setString (Parser::sqrt);
       window.draw (text);
     }
     else {
       text.setPosition ({node->position.xOperator, node->position.yOperator});
       text.setString (Parser::toString (node->function));
       window.draw (text);
     }
     drawFormula (node->leftTree, window);
     return;
   }
   // Operations
   else if (node->operation == '^') {
     drawFormula (node->leftTree, window);
     drawFormula (node->rightTree, window);
   }
   else if (node->operation == '/') {
     drawFormula (node->leftTree, window);
     sf::RectangleShape line ({node->size.width, characterSize * 0.1f});
     line.setPosition ({node->position.xOperator, node->position.yOperator});
     line.setFillColor (color);
     window.draw (line);
     drawFormula (node->rightTree, window);
   }
   else if (node->operation != '^' && node->operation != '/') {
     text.setPosition ({node->position.xOperator, node->position.yOperator});
     text.setString (node->operation);
     drawFormula (node->leftTree, window);
     window.draw (text);
     drawFormula (node->rightTree, window);
   }
 }
};

} // namespace sgui
