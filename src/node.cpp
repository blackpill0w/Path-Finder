#include "node.hpp"
#include "constants.hpp"
#include <SFML/System/Vector2.hpp>
#include <locale>

const sf::Color Node::BLACK = sf::Color(0, 0, 0);
const sf::Color Node::WHITE = sf::Color(255, 255, 255);
const sf::Color Node::BLUE = sf::Color(0, 0, 255);
const sf::Color Node::GREEN = sf::Color(0, 255, 0);

// Copy constructor
Node::Node(const Node &other)
  : explored(other.explored), rect( sf::Vector2f(constants::squareSize, constants::squareSize) ),
    parent(other.parent), neighbours(other.neighbours)
{
   rect.setPosition(other.rect.getPosition());
   rect.setOutlineThickness(1.f);
   rect.setOutlineColor(BLACK);
   rect.setFillColor(other.rect.getFillColor());
}

Node::Node(const sf::Vector2f &pos, const unsigned index)
  : explored(false), rect( sf::Vector2f(constants::squareSize, constants::squareSize) ),
    parent(nullptr), neighbours()
{
   rect.setPosition(pos);
   rect.setOutlineThickness(1.f);
   rect.setOutlineColor(BLACK);

   setNeighbours(index);
}

sf::Color Node::getColor() const { return rect.getFillColor(); }

void Node::setColor(const sf::Color &color) { rect.setFillColor(color); }

void Node::setParent(Node *parent) { this->parent = parent; }

Node* Node::getParent() const { return parent; }

bool Node::hasParent() const { return (parent != nullptr); }

bool Node::isNodeAt(const sf::Vector2f &pos) const { return ( rect.getGlobalBounds().contains(pos) ); }

void Node::draw(sf::RenderWindow &window) const { window.draw(rect); }

bool Node::isExplored() const { return explored; };

void Node::markAsExplored() { explored = true; }

void Node::setNeighbours(const unsigned index) {
  sf::Vector2f pos = rect.getPosition();
  if (pos.x > 1) { // Floats can be tricky
    neighbours.emplace_back(index - 1); // Left
    if (pos.y > 1) {
      neighbours.emplace_back(index - 1 - constants::nodesInLine); // Top left
    }
    if (pos.y < constants::gridHeight - constants::squareSize - 1) {
      neighbours.emplace_back(index - 1 + constants::nodesInLine); // Bottom left
    }
  }
  if (pos.x < constants::gridWidth - constants::squareSize - 1) {
     neighbours.emplace_back(index + 1); // Right
    if (pos.y > 1) {
      neighbours.emplace_back(index + 1 - constants::nodesInLine); // Top right
    }
    if (pos.y < constants::gridHeight - constants::squareSize - 1) {
      neighbours.emplace_back(index + 1 + constants::nodesInLine); // Bottom right
    }
  }
  if (pos.y < constants::gridHeight - constants::squareSize - 1) {
    neighbours.emplace_back(index + constants::nodesInLine); // Down
  }
  if (pos.y > 1) {
    neighbours.emplace_back(index - constants::nodesInLine); // Up
  }

}
