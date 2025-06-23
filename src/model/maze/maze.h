#ifndef MAZE_H_
#define MAZE_H_

#include <array>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <queue>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../cell.h"

constexpr int kMaxSize = 50;
// class QLearning;

class Maze {
  friend class QLearning;

 public:
  Maze();
  explicit Maze(int rows, int cols);
  Maze(const Maze &);
  Maze &operator=(const Maze &other) noexcept;
  Maze(Maze &&other) noexcept;
  Maze &operator=(Maze &&other) noexcept;

  void GenerateMaze();
  void GenerateCave(const double chance);
  bool SolveCave(const int birth, const int death);
  std::vector<Cell> SolveMaze(Cell end, Cell start);
  std::vector<std::vector<Cell>> DistanceMatrix(Cell start);
  bool Load(std::istream &stream, char c);
  bool Save(std::ostream &stream, char c) const;
  int GetRows() const { return _rows; }
  int GetCols() const { return _cols; }
  static int GetBit(uint64_t n, int bit) { return (n >> bit) & 1u; }

  std::array<uint64_t, kMaxSize> GetVerticals() const { return _verticals; }
  std::array<uint64_t, kMaxSize> GetHorizontals() const { return _horizontals; }

  void SetVerticals(std::array<uint64_t, kMaxSize> verticals) {
    _verticals = verticals;
  }
  void SetHorizontals(std::array<uint64_t, kMaxSize> horizontals) {
    _horizontals = horizontals;
  };
  bool SetRowsCols(int rows, int cols);
  static void InitRandom();
  static std::mt19937 _gen;

 private:
  bool LoadMatrix(std::istream &stream, char c);
  bool LoadMatrices(std::istream &stream, char c);
  bool SaveMatrix(std::ostream &stream, char c) const;

  void MakeLastLine(std::array<int, kMaxSize> &set);
  void CheckhorizontalPass(const std::array<int, kMaxSize> &set, int i,
                           int uniques);
  void MakeVerticalWalls(std::array<int, kMaxSize> &set, int i);
  static void SetBit1(uint64_t &n, int pos);
  static void SetBit0(uint64_t &n, int pos);

  inline bool CanGo(const Cell &start, const Cell &end) const {
    if (!ValidPoint(end))
      return false;
    else if (start.r == end.r && start.c == end.c + 1)  // left
      return !GetBit(_verticals[end.r], end.c);
    else if (start.r == end.r && start.c + 1 == end.c)  // right
      return !GetBit(_verticals[start.r], start.c);
    else if (start.c == end.c && start.r + 1 == end.r)
      return !GetBit(_horizontals[start.r], start.c);
    else if (start.c == end.c && start.r == end.r + 1)
      return !GetBit(_horizontals[end.r], end.c);
    else
      return false;
  }

  inline bool ValidPoint(const Cell &point) const {
    return (point.r >= 0 && point.c >= 0 && point.r < _rows && point.c < _cols);
  }

  static bool EmptyPoint(const Cell &point);
  void ProceedDead(const int i, const int j, const int birth);
  void ProceedAlive(const int i, const int j, const int death);

  int _rows;
  int _cols;
  std::array<uint64_t, kMaxSize> _verticals;
  std::array<uint64_t, kMaxSize> _horizontals;

  static std::uniform_int_distribution<> _dist_bit;
  static std::uniform_real_distribution<> _dist_real;

  static int RandomBit() { return _dist_bit(_gen); }
  static double RandomReal() { return _dist_real(_gen); }
};

#endif
