#include <array>
#include <queue>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "constants.hpp"
#include "node.hpp"

using std::array;
using std::queue;

void drawGrid(sf::RenderWindow &window, array<Node, constants::nodesNum> &nodes);
unsigned getNodeIndexAtPos(array<Node, constants::nodesNum> &nodes, sf::Vector2f &pos);
unsigned getClickedButtonIndex(array<sf::RectangleShape, 5> &buttons,
                               sf::Vector2f &mousePos);
bool isBlockingNode(Node &node);
void addNeighboursToQueue(queue<unsigned> &nodesQueue, array<Node, constants::nodesNum> &nodes, unsigned nodeIndex);
void exploreNode(queue<unsigned> &nodesQueue, array<Node, constants::nodesNum> &nodes, unsigned nodeIndex);
void highlightPath(array<Node, constants::nodesNum> &nodes, unsigned nodeToHighlight, unsigned source, unsigned destination);
void resetGrid(array<Node, constants::nodesNum> &nodes, unsigned source, unsigned destination);


int main() {
   sf::RenderWindow window(
      sf::VideoMode(constants::wWidth, constants::wHeight), "Path Finder",
      sf::Style::Titlebar | sf::Style::Close
   );
   window.setPosition({100, 100});

   array<sf::RectangleShape, 5> buttons;
   std::fill( buttons.begin(),buttons.end(), sf::RectangleShape( sf::Vector2f(constants::buttonSize, constants::buttonSize)) );

   // Initialize button (position...)
   for (unsigned i=0; i < buttons.size(); ++i) {
     buttons[i].setPosition({constants::wWidth - 70, 20 + constants::buttonSize*((float) i) +((float) i)*10});
     buttons[i].setOutlineColor(Node::WHITE);
   }
   enum : unsigned {fromButton = 0u, destinationButton, barrierButton, eraseButton, startSearchButton};
   buttons[fromButton].setFillColor(Node::GREEN);
   buttons[destinationButton].setFillColor(Node::BLUE);
   buttons[barrierButton].setFillColor(Node::BLACK);
   buttons[eraseButton].setFillColor(Node::WHITE);
   buttons[startSearchButton].setFillColor(sf::Color(0, 255, 255));

   array<Node, constants::nodesNum> nodes;

   int x{ 0 };
   int y{ 0 };
   unsigned nodeIndex{ 0u };
   while (y < constants::gridHeight) {
     x = 0;
     while (x < constants::gridWidth) {
       nodes[nodeIndex].setPosition(sf::Vector2f(x, y));
       nodes[nodeIndex].setNeighbours(nodeIndex);
       ++nodeIndex;
       x += constants::squareSize;
     }
     y += constants::squareSize;
   }

   // Helpers
   unsigned             activeButton{ fromButton };
   unsigned             source{ 0 };
   unsigned             destination{ 40 };
   bool                 mouseClicked{ false };
   bool                 searching{ false };
   sf::Vector2f         mousePos;
   queue<unsigned>     nodesQueue;

   buttons[activeButton].setOutlineThickness(2.f);
   nodes[source].setColor(Node::GREEN);
   nodes[destination].setColor(Node::BLUE);

   while (window.isOpen()) {
     sf::Event event;
     while (window.pollEvent(event)) {
       if (event.type == sf::Event::Closed) {
         window.close();
       }
       else if (event.type == sf::Event::MouseButtonReleased) {
         mouseClicked = false;
       }
       else if (event.type == sf::Event::MouseButtonPressed) {
         mouseClicked = true;
         unsigned clickedItem = getClickedButtonIndex(buttons, mousePos);
         if (clickedItem != constants::invalidIndex) {
           // Deactivate the active button
           buttons[activeButton].setOutlineThickness(0.f);
           // Activate the clicked one
           activeButton = clickedItem;
           buttons[activeButton].setOutlineThickness(2.f);
           if (activeButton == startSearchButton) {
             searching = true;
           }
         }
       }
     }

     window.clear(sf::Color(50, 50, 50));
     mousePos = sf::Vector2f{ sf::Mouse::getPosition(window) };

     if (!searching && mouseClicked) {
       unsigned temp{ getNodeIndexAtPos(nodes, mousePos) };
       if (temp != constants::invalidIndex) {
         if (activeButton == barrierButton || activeButton == eraseButton) {
           if (temp != source && temp != destination) {
             nodes[temp].setColor(buttons[activeButton].getFillColor());
           }
         }
         else {
           unsigned oldItem = (activeButton == fromButton) ? source : destination;
           nodes[oldItem].setColor(Node::WHITE);
           nodes[temp].setColor(buttons[activeButton].getFillColor());
           if (oldItem == source) {
             source = temp;
           }
           else {
             destination = temp;
           }
         }
       }
     }
     else if (searching) {
       unsigned destinationAfterSearch{ constants::invalidIndex };
       nodesQueue.emplace(source);
       if (!nodesQueue.empty()) {
         unsigned currentNode{ nodesQueue.front() };
         nodes[currentNode].markAsExplored();
         nodesQueue.pop();

         if (currentNode == destination) {
           destinationAfterSearch = currentNode;
           nodesQueue = {};
           searching = false;
         }
         if (currentNode != source && currentNode != destination) {
           nodes[currentNode].setColor(sf::Color(0, 255, 255));
         }
         addNeighboursToQueue(nodesQueue, nodes, currentNode);
       }
       else {
         searching = false;
       }
       if (destinationAfterSearch != constants::invalidIndex) {
         highlightPath(nodes, destinationAfterSearch, source, destination);
       }
     }

     drawGrid(window, nodes);
     for (auto& button: buttons) {
       window.draw(button);
     }
     window.display();
   }
}

void drawGrid(sf::RenderWindow &window, array<Node, constants::nodesNum> &nodes) {
  for (auto& node: nodes) {
    node.draw(window);
  }
}

unsigned getNodeIndexAtPos(array<Node, constants::nodesNum> &nodes, sf::Vector2f &pos) {
  for (unsigned i=0; i < nodes.size(); ++i) {
    if (nodes[i].isNodeAt(pos)) {
      return i;
    }
  }
  return constants::invalidIndex;
}

unsigned getClickedButtonIndex(array<sf::RectangleShape, 5> &buttons, sf::Vector2f &mousePos) {
  for (unsigned i=0; i < buttons.size(); ++i) {
    if (buttons[i].getGlobalBounds().contains(mousePos)) {
      return i;
    }
  }
  return constants::invalidIndex;
}

bool isBlockingNode(Node &node) { return (node.getColor() == Node::BLACK); }

void addNeighboursToQueue(queue<unsigned> &nodesQueue, array<Node, constants::nodesNum> &nodes, unsigned nodeIndex) {
  for (auto& i: nodes[nodeIndex].neighbours) {
    if (!nodes[i].isExplored() && !isBlockingNode(nodes[i])) {
      nodes[i].markAsExplored();
      nodesQueue.emplace(i);
      nodes[i].setParent(nodeIndex);
    }
  }
}

void highlightPath(array<Node, constants::nodesNum> &nodes, unsigned nodeToHighlight, unsigned source, unsigned destination) {
  if (nodeToHighlight != source && nodeToHighlight != destination) {
    nodes[nodeToHighlight].setColor(sf::Color(255, 0, 0));
  }
  if (nodes[nodeToHighlight].hasParent()) {
    highlightPath(nodes, nodes[nodeToHighlight].getParent(), source, destination);
  }
}

void resetGrid(array<Node, constants::nodesNum> &nodes, unsigned source, unsigned destination) {
  for (unsigned i=0; i < nodes.size(); ++i) {
    nodes[i].markAsUnxplored();
    if (i != source && i != destination) {
      nodes[i].setColor(Node::WHITE);
    }
  }
}
