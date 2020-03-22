#include "a_star.h"
#include <iostream>
#include <algorithm>

using std::cout;
using std::endl;

AStar::AStar(const std::size_t grid_width, const std::size_t grid_height) {

    open_list_.clear();
    nodes_.clear();

    nodes_.resize(grid_height);

    for (int i = 0; i < grid_height; i++) {
        nodes_[i].resize(grid_width);
        for (int j = 0; j < nodes_[i].size(); j++) {
            nodes_[i][j].state = State::kEmpty;
        }
    }

}


bool AStar::CheckValidCell(const int x, const int y){
  if ( ( x >= 0 and x < nodes_.size() ) and ( y >= 0 and y < nodes_[0].size()) )
    return ( nodes_[x][y].state == State::kEmpty );

  return false;
}


std::vector<SDL_Point>
AStar::Search(const SDL_Point &start_point, const SDL_Point &end_point) {

    std::vector<SDL_Point> results{};

    open_list_.clear();
    SDL_Point SDL_point;

    Node node;
    node.x = start_point.x;
    node.y = start_point.y;
    node.h_cost = Heuristic(start_point.x, start_point.y, end_point.x, end_point.y);

    node.state = State::kOpen;
    nodes_[node.x][node.y].x = node.x;
    nodes_[node.x][node.y].y = node.y;

    open_list_.push_back(node);

    while (not open_list_.empty()) {

        std::sort(open_list_.begin(), open_list_.end(), 
                   [](const Node &a, const Node &b)
                   { return (a.g_cost + a.h_cost) > (b.g_cost + b.h_cost); }
                  );

        auto *current = &nodes_[open_list_.back().x][open_list_.back().y];
        current->state = State::kClosed;
        open_list_.pop_back();

        for (int i = 0;  i < 4; i++) {
            const auto x_next = current->x + delta_[i][0];
            const auto y_next = current->y + delta_[i][1];

            if (not CheckValidCell(x_next, y_next)) continue;

            auto *node_next = &nodes_[x_next][y_next];
            node_next->x = x_next;
            node_next->y = y_next;

            if (end_point.x == x_next and end_point.y == y_next) {
                node_next->parent = current;
                node_next->state = State::kPath;
                node_next->g_cost = current->g_cost + 1;
                node_next->h_cost = 0;

                while (node_next != nullptr) {
                    SDL_point.x = node_next->x;
                    SDL_point.y = node_next->y;
                    results.push_back(SDL_point);
                    node_next->state = State::kPath;
                    node_next = node_next->parent;
                }

                std::reverse(results.begin(), results.end());
                results.pop_back();
                return results;
            }

            if ( node_next->state == State::kClosed) continue;

            if ( (State::kOpen != node_next->state) or
                (current->g_cost + 1 < node_next->g_cost) 
               )
            {
              node_next->parent = current;
              node_next->state = State::kOpen;
              node_next->g_cost = current->g_cost + 1;
              node_next->h_cost = Heuristic(x_next, y_next, end_point.x, end_point.y);
              open_list_.push_back(*node_next);
            }
        }
    }

    return results;
}
