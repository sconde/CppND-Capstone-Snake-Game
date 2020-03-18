#ifndef A_STAR_CPP
#define A_STAR_CPP 
/*
 * Purpose:
 * A* Search
*/

#include "a_star.hpp"
#include <iostream>
#include <algorithm> // sort
#include <string>
#include <sstream> // istringstream
#include <fstream> // ifstream
#include <vector>


AStar::AStar (const std::size_t grid_w, std::size_t grid_h) {

  open_lists_.clear();
  nodes_.clean();

  nodes_.resize( grid_h );
  for(std::size_t i = 0; i < grid_h, i++) {
    nodes_[i].resize( grid_w );
    for(std::size_t j = 0; j < grid_q; j++ )
      nodes_[i][j].state = state::kEmpty;
  }
}

AStar::IsInRange(const int x, const in y) {
  const auto in_x = (x >= 0 and x < nodes_.size()   );
  const auto in_y = (y >= 0 and y < node_[0].size() );
  return ( in_x and in y );
}

std::string CellString(State instate){
  switch (instate) {
    case State::kObstacle: return "⛰️   ";
    case State::kPath: return "🚗   ";
    case State::kStart: return "🚦   ";
    case State::kFinish: return "🏁   ";
    default: return "0   ";
  }
}


inline bool Compare(const std::vector<int> first, const std::vector<int> second){
 return ( first[2]+first[3]) > (second[2]+second[3]); 
}

// sort the two-dimentional vector of ints in descending order.
inline void CellSort(std::vector<std::vector<int>> *v){
  std::sort(v->begin(), v->end(), Compare);
}


/* accepts `board` as argument and print each row with new line */
void PrintBoard(const std::vector<std::vector<State> >& in_vec){
  for(auto row : in_vec){
    for(auto e: row) std::cout << CellString( e );
    std::cout << std::endl;
  }
}

std::vector<State> ParseLine(std::string in_string){

  std::vector<State> result;
  std::istringstream my_stream(in_string);
  int num;
  char c;
  while( my_stream >> num >> c && c == ',' ){
    result.push_back(  (num==0) ? State::kEmpty : State::kObstacle  );
  }

  return result;
}

std::vector<std::vector<State> > ReadBoardFile(std::string f_path){

  std::vector<std::vector<State>> result;
  std::ifstream f_stream(f_path);

  if (f_stream) {
    std::string line;
    while ( getline(f_stream, line)) {
      result.push_back( ParseLine( line ) );
    }
  }

  return result;
}

void AddToOpen(int x, int y, int g, int h,
               std::vector<std::vector<int>>& open_nodes,
               std::vector<std::vector<State>>& grid ) 
{

  std::vector<int> node{x, y, g, h};
  open_nodes.push_back( node );
  grid[x][y] = State::kClosed;
}

inline bool CheckValidCell(int x, int y,std::vector<std::vector<State>>& grid ){
  if ( ( x >= 0 and x < grid.size() ) and ( y >= 0 and y < grid[0].size()) )
    return ( grid[x][y] == State::kEmpty );

  return false;
}

// Expand current node's neighbors and add them to the open list
void ExpandNeighbors(const std::vector<int>& current, int end[2],
                     std::vector<std::vector<int>>& open, 
                     std::vector<std::vector<State>>& grid) {

  // Get current node's data
  int x = current[0], y = current[1], g = current[2];

  // Loop through current node's potential neighbors
  for (int i = 0; i < 4; ++i) {
   int x2 = x + delta[i][0];
   int y2 = y + delta[i][1];

   // Check that the potential neighbor's x2 and y2 values are ont he grid and
   // not closed
   if (CheckValidCell(x2, y2, grid)){
     // Increment g value and add neighbor to the open list
     int g2 = g +1;
     int h2 = Heuristic(x2, y2, end[0], end[1]);
     AddToOpen(x2, y2, g2, h2, open, grid);
   }

  }
}


// Implementation of A* search algorithm
std::vector<SDL_Point> 
AStar::Search(const SDL_Point & start,const SDL_Point &end) {

  open_lists_.clear();
  std::vector<SDL_Point> SDL_points {} ;

  SDL_Point point;
  int search_count = 0;

  // Initialize the starting node
  Node node;
  node.parent = nullptr;
  node.x = start.x;
  node.y = start.y;
  node.g_cost = 0;
  node.h_value = Heuristic( start.x, start.y, end.x, end.y);
  node.state = State::kEmpty; // kEmpty = kOpen;

  nodes_[node.x][node.y]x = node.x;
  nodes_[node.x][node.y].y = node.y;

  open_lists_.push_back( node );


  std::vector<std::vector<State> > results;

  // Create the vector of open nodes
  std::vector<std::vector<int>> open{};


  // while open vector is non empty
  while ( not open.empty() ) {
    search_count++;

    std::sort(open_lists_.begin(), open_lists_.end(),
              [](const Node &a, const Node &b) { return (a.g_cost + a.h_value) > (b.g_cost + b.h_value); }
             );

    auto *current = &nodes_[open_lists_.back().x][open_lists_.back().y];
    current->state = State::kClosed;
    open_lists_.pop_back();

    for(int i = 0; i < 4; i++) {
      const int x_next = current->x + delta_[i][0];
      const int y_next = current->y + delta_[i][1];
      
      if (not IsInRange(x_next, y_next) ) continue;

      Node *node_next = &nodes_[x_next][y_next];
      node_next->x = x_next;
      node_next->y = y_next;

      if ( x_next == end.x and y_next == end.y) {
        node_next->parent  = current;
        node_next->state   = State::kPath;
        node_next->g_cost  = current->g_cost + 1;
        node_next->h_value = 0;

        while (node_next != nullptr ) {
          point.x = node_next->x;
          point.y = node_next->y;
          SDL_points.push_back ( point );
          node_next->state = State::kPath;
          node_next = node_next->parent;
        }

        std::reverse( SDL_points.begin(), SDL_points.end() );
        SDL_points.pop_back();

        return SDL_points;
      }

      if ( State::kClosed == node_next->state ) continue;

      bool should_add_to_openlist = false;

      if ( State::kOpen != node_next->state ) should_add_to_openlist = true;
      else if ( current->g_cost + 1 < node_next->g_cost ) should_add_to_openlist = true;
      else continue;

      if ( should_add_to_openlist ) {
        node_next->parent = current;
        node_next->state = State::kOpen;
        node_next->g_cost = current->g_cost + 1;
        node_next->h_value = Heuristic(x_next, y_next, end.x, end.y);
        open_lists_.push_back(*node_next);
      }

    }

  }  // end of while loop

  std::cout << "No path found!" << std::endl;
  return SDL_points;

} // end of search function

#endif /* ifndef A_STAR_CPP */
