#include "maze.h"

void Maze::GenerateCave(const double chance) {
  for (int i = 0; i < _rows; ++i)
    for (int j = 0; j < _cols; ++j)
      RandomReal() < chance ? SetBit1(_verticals[i], j)
                            : SetBit0(_verticals[i], j);
}

bool Maze::SolveCave(const int birth, const int death) {
  bool result = true;
  for (int i = 0; i < _rows; ++i) {
    for (int j = 0; j < _cols; ++j) {
      if (GetBit(_verticals[i], j))
        ProceedAlive(i, j, death);
      else
        ProceedDead(i, j, birth);
    }
    result = result && (_verticals[i] == _horizontals[i]);
  }
  std::swap(_verticals, _horizontals);
  return result;
}

void Maze::ProceedAlive(const int i, const int j, const int death) {
  int sum = -1;
  for (int di = -1; di < 2; ++di) {
    int new_i = i + di;
    for (int dj = -1; dj < 2; ++dj) {
      int new_j = j + dj;
      if (new_i < 0 || new_i >= _rows || new_j < 0 || new_j >= _cols)
        sum++;
      else {
        sum += GetBit(_verticals[new_i], new_j);
      }
    }
  }
  sum >= death ? SetBit1(_horizontals[i], j) : SetBit0(_horizontals[i], j);
}

void Maze::ProceedDead(const int i, const int j, const int birth) {
  int sum = 0;
  for (int di = -1; di < 2; ++di) {
    int new_i = i + di;
    for (int dj = -1; dj < 2; ++dj) {
      int new_j = j + dj;
      if (new_i < 0 || new_i >= _rows || new_j < 0 || new_j >= _cols)
        sum++;
      else {
        sum += GetBit(_verticals[new_i], new_j);
      }
    }
  }
  sum > birth ? SetBit1(_horizontals[i], j) : SetBit0(_horizontals[i], j);
}
