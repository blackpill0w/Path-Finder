#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>

static const std::string sep{ "/" };
#ifdef _WIN32
   sep = "\\"
#endif

namespace constants {

inline constexpr int squareSize{ 18 };
inline constexpr int gridWidth{ 540 };
inline constexpr int gridHeight{ 540 };
inline constexpr int wWidth{ gridWidth + 100 };
inline constexpr int wHeight{ gridHeight };
inline constexpr int nodesInLine{ gridWidth / squareSize };
inline constexpr int nodesInColumn{ gridHeight / squareSize };
inline constexpr int nodesNum{ nodesInLine*nodesInColumn };
inline constexpr int buttonSize{ 50 };
inline constexpr unsigned invalidIndex{ 900000u };
inline const std::string fontFile{ "assets" + sep + "fonts" + sep + "UbuntuMono-R.ttf" };

}; // namespace constants

#endif
