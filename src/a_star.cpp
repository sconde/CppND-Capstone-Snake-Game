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
            // cout << static_cast<int>(nodes_[i][j].state) << ", ";
        }
        // cout << endl;
    }

// /****search test****/
    // cout << "nodes_.size(): " << nodes_.size() << endl;
    // cout << "nodes_[0].size(): " << nodes_[0].size() << endl;
//     SDL_Point start, end;
//     start.x = 0;
//     start.y = 0;
//     end.x = 10;
//     end.y = 10;
//     vector<SDL_Point> SDL_Points = Search(start, end);

//     cout << "SDL_Points.size(): " << SDL_Points.size() << endl;

//     for (int i = 0; i < grid_height; i++) {
//         for (int j = 0; j < nodes_[i].size(); j++) {
//             if (State::kPath == nodes_[i][j].state) {
//                 cout << "*, ";
//             } else {
//                 cout << static_cast<int>(nodes_[i][j].state) << ", ";
//             }
//         }
//         cout << endl;
//     }
// /****search test****/

    cout << "AStar is Done\n" << endl;
}

AStar::~AStar() {
    cout << "~AStar is Done" << endl;
}

int AStar::Heuristic(const int start_point_x, const int start_point_y, const int end_point_x, const int end_point_y) {
    return abs(end_point_x - start_point_x) + abs(end_point_y - start_point_y);
}

bool AStar::IsInRange(const int x, const int y) {
    const bool on_board_x = (x >= 0 && x < nodes_.size());
    const bool on_board_y = (y >= 0 && y < nodes_[0].size());

    if (on_board_x && on_board_y) {
        return true;
    } else {
        return false;
    }
}

vector<SDL_Point> AStar::Search(const SDL_Point &start_point, const SDL_Point &end_point) {
    open_list_.clear();
    vector<SDL_Point> SDL_Points{};
    SDL_Point SDL_point;
    int search_count = 0;

    Node node;
    node.parent = nullptr;
    node.x = start_point.x;
    node.y = start_point.y;
    node.g_cost = 0;
    node.h_cost = Heuristic(start_point.x, start_point.y, end_point.x, end_point.y);
    node.state = State::kOpen;
    nodes_[node.x][node.y].x = node.x;
    nodes_[node.x][node.y].y = node.y;
    open_list_.push_back(node);

    while (not open_list_.empty()) {
        search_count++;
        std::sort(open_list_.begin(), open_list_.end(), [](const Node &a, const Node &b){ return (a.g_cost + a.h_cost) > (b.g_cost + b.h_cost); });
        Node *current = &nodes_[open_list_.back().x][open_list_.back().y];
        current->state = State::kClosed;
        open_list_.pop_back();

        for (int i = 0;  i < 4; i++) {
            const int x_next = current->x + delta_[i][0];
            const int y_next = current->y + delta_[i][1];

            if (not IsInRange(x_next, y_next)) {
                continue;
            }

            Node *node_next = &nodes_[x_next][y_next];
            node_next->x = x_next;
            node_next->y = y_next;

            if (x_next == end_point.x && y_next == end_point.y) {
                node_next->parent = current;
                node_next->state = State::kPath;
                node_next->g_cost = current->g_cost + 1;
                node_next->h_cost = 0;

                while (nullptr != node_next) {
                    SDL_point.x = node_next->x;
                    SDL_point.y = node_next->y;
                    SDL_Points.push_back(SDL_point);
                    node_next->state = State::kPath;
                    node_next = node_next->parent;
                }

                std::reverse(SDL_Points.begin(), SDL_Points.end());
                cout << "A* search OK! search_count: "  << search_count << endl;
                SDL_Points.pop_back();
                // /****search test****/
                // for (int i = 0; i < 32; i++) {
                //     for (int j = 0; j < nodes_[i].size(); j++) {
                //         if (State::kPath == nodes_[i][j].state) {
                //             cout << "*, ";
                //         } else {
                //             cout << static_cast<int>(nodes_[i][j].state) << ", ";
                //         }
                //     }
                //     cout << endl;
                // }
                // /****search test****/

                return SDL_Points;
            }

            if (State::kClosed == node_next->state) {
                continue;
            }

            bool should_add_to_openlist = false;

            if (State::kOpen != node_next->state) {
                should_add_to_openlist = true;
            } else if (current->g_cost + 1 < node_next->g_cost) {
                should_add_to_openlist = true;
            } else {
                continue;
            }

            if (should_add_to_openlist) {
                node_next->parent = current;
                node_next->state = State::kOpen;
                node_next->g_cost = current->g_cost + 1;
                node_next->h_cost = Heuristic(x_next, y_next, end_point.x, end_point.y);
                open_list_.push_back(*node_next);
            }
        }
    }

    cout << "A* search FAILED! search_count: "  << search_count << endl;

    return SDL_Points;
}