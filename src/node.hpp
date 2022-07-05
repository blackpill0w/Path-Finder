#ifndef NODE_HPP
#define NODE_HPP

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#include <vector>
#include "constants.hpp"

using std::vector;

class Node {
protected:
  bool explored;
  sf::RectangleShape rect;
  Node *parent;
public:
  vector<unsigned> neighbours;
  static const sf::Color BLACK;
  static const sf::Color WHITE;
  static const sf::Color BLUE;
  static const sf::Color GREEN;
protected:
  void setNeighbours(const unsigned index);
public:
  Node(const Node &other);
  Node& operator=(const Node &other) = delete;

  Node(const sf::Vector2f &pos, const unsigned index);
  sf::Color getColor() const;
  void setColor(const sf::Color &color);
  void setParent(Node *parent);
  Node* getParent() const;
  bool hasParent() const;
  bool isNodeAt(const sf::Vector2f &pos) const;
  void draw(sf::RenderWindow &window) const;
  bool isExplored() const;
  void markAsExplored();
};

#endif
