#ifndef A_STAR_H
#define A_STAR_H

#include <vector>
#include "SDL.h"
#include <limits>

using std::vector;

enum class State { kEmpty, kClosed, kOpen, kPath, };

class Node {
public:
    Node *parent = nullptr;
    int x = 0;
    int y = 0;
    int g_cost = 0;
    int h_cost = std::numeric_limits<int>::max();
    State state = State::kEmpty;
};

class AStar {
public:
    AStar(const std::size_t grid_width, const std::size_t grid_height);

    ~AStar() {} 

    vector<SDL_Point> Search(const SDL_Point &start_point, const SDL_Point &end_point);

private:
    vector<vector<Node>> nodes_;
    vector<Node> open_list_{};
    constexpr static int delta_[4][2]{ {-1, 0}, {0, -1}, {1, 0}, {0, 1} };

    bool IsInRange(const int x, const int y);

    inline int Heuristic(int x1, int y1, int x2, int y2) {
      return std::abs(x2 - x1) + std::abs(y2 - y1);
    }

    //void AddToOpen(int x, int y, int g, int h,
        //std::vector<std::vector<int>>& open_nodes,
        //std::vector<std::vector<State>>& grid );
};

#endif
