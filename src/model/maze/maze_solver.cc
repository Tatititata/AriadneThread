#include "maze.h"

bool Maze::EmptyPoint(const Cell& point) {
  return point.r == -1 && point.c == -1;
}

std::vector<Cell> Maze::SolveMaze(Cell end, Cell start) {
  std::vector<std::vector<Cell>> prev(_rows,
                                      std::vector<Cell>(_cols, {-1, -1}));

  const std::array<Cell, 4> d = {{{-1, 0}, {1, 0}, {0, 1}, {0, -1}}};
  std::queue<Cell> q;
  q.push(start);
  prev[start.r][start.c] = start;
  bool found = false;
  while (!q.empty() && !found) {
    Cell current = q.front();
    q.pop();
    if (current == end)
      found = true;
    else {
      for (int i = 0; i < 4; ++i) {
        Cell tmp = current + d[i];
        if (CanGo(current, tmp) && EmptyPoint(prev[tmp.r][tmp.c])) {
          prev[tmp.r][tmp.c] = current;
          q.push(tmp);
        }
      }
    }
  }
  std::vector<Cell> pass;
  if (found) {
    pass.push_back(end);
    while (pass.back() != start) {
      pass.push_back(prev[pass.back().r][pass.back().c]);
    }
  }
  return pass;
}

std::vector<std::vector<Cell>> Maze::DistanceMatrix(Cell start) {
  std::vector<std::vector<Cell>> matrix{};
  std::vector<std::vector<bool>> visited(_rows,
                                         std::vector<bool>(_cols, false));
  const std::array<Cell, 4> delta = {{{-1, 0}, {1, 0}, {0, 1}, {0, -1}}};
  int i = 0;
  matrix.push_back({start});
  visited[start.r][start.c] = true;
  while (!matrix[i].empty()) {
    std::vector<Cell> next_level;
    for (auto m : matrix[i]) {
      for (auto d : delta) {
        Cell tmp = m + d;
        if (CanGo(m, tmp) && !visited[tmp.r][tmp.c]) {
          visited[tmp.r][tmp.c] = true;
          next_level.push_back(tmp);
        }
      }
    }
    matrix.push_back(next_level);
    ++i;
  }
  return matrix;
}