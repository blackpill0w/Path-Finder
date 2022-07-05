#ifndef NODE_HPP
#define NODE_HPP

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#include <SFML/System/Vector2.hpp>
#include <vector>
#include "constants.hpp"

using std::vector;

class Node {
protected:
  bool explored;
  sf::RectangleShape rect;
  unsigned parent;
public:
  vector<unsigned> neighbours;
  static const sf::Color BLACK;
  static const sf::Color WHITE;
  static const sf::Color BLUE;
  static const sf::Color GREEN;
public:
  // Constructors
  Node();
  Node(const sf::Vector2f &pos, const unsigned index);

  void setPosition(const sf::Vector2f &pos);
  void setNeighbours(const unsigned index);
  sf::Color getColor() const;
  void setColor(const sf::Color &color);
  void setParent(unsigned parent);
  unsigned getParent() const;
  bool hasParent() const;
  bool isNodeAt(const sf::Vector2f &pos) const;
  void draw(sf::RenderWindow &window) const;
  bool isExplored() const;
  void markAsExplored();
  void markAsUnxplored();
};

#endif
