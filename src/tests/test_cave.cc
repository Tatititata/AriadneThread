#include <gtest/gtest.h>

#include "../model/maze/maze.h"

int CountAliveCells(const Maze *cave) {
  auto verticals = cave->GetVerticals();
  int count = 0;
  for (int i = 0; i < cave->GetRows(); ++i) {
    for (int j = 0; j < cave->GetCols(); ++j) {
      if (cave->GetBit(verticals[i], j)) count++;
    }
  }
  return count;
}

TEST(CaveTest, GenerateCaveBasic) {
  Maze::InitRandom();
  Maze cave(10, 10);
  cave.GenerateCave(0.5);
  auto verticals = cave.GetVerticals();

  bool has_zero = false, has_one = false;
  for (int i = 0; i < cave.GetRows(); ++i) {
    for (int j = 0; j < cave.GetCols(); ++j) {
      if (!cave.GetBit(verticals[i], j)) has_zero = true;
      if (cave.GetBit(verticals[i], j)) has_one = true;
    }
  }
  EXPECT_TRUE(has_zero && has_one);
}

TEST(CaveTest, GenerateCaveExtremeChance) {
  Maze cave(5, 5);

  cave.GenerateCave(0.0);
  EXPECT_EQ(CountAliveCells(&cave), 0);

  cave.GenerateCave(1.0);
  EXPECT_EQ(CountAliveCells(&cave), 25);
}

TEST(CaveTest, SolveCaveBasic) {
  Maze cave(3, 3);

  std::array<uint64_t, kMaxSize> verticals = {};

  cave.GenerateCave(0.0);
  verticals = cave.GetVerticals();
  auto set_bit = [](uint64_t &row, int pos) { row |= (1ULL << pos); };
  set_bit(verticals[0], 0);
  set_bit(verticals[0], 2);
  set_bit(verticals[1], 1);
  set_bit(verticals[2], 0);
  set_bit(verticals[2], 2);

  cave.SetVerticals(verticals);

  bool result = cave.SolveCave(3, 2);
  EXPECT_FALSE(result);

  auto final_verticals = cave.GetVerticals();
  EXPECT_EQ(cave.GetBit(final_verticals[0], 0), 1);
  EXPECT_EQ(cave.GetBit(final_verticals[0], 1), 1);
  EXPECT_EQ(cave.GetBit(final_verticals[1], 0), 1);
  EXPECT_EQ(cave.GetBit(final_verticals[1], 1), 1);
  EXPECT_EQ(cave.GetBit(final_verticals[2], 2), 1);
}

TEST(CaveTest, ProceedAliveAndDeadBehavior) {
  Maze cave(3, 3);

  cave.GenerateCave(0.0);

  auto verticals = cave.GetVerticals();
  auto set_bit = [](uint64_t &row, int pos) { row |= (1ULL << pos); };
  set_bit(verticals[0], 0);
  set_bit(verticals[0], 2);
  set_bit(verticals[1], 1);
  set_bit(verticals[2], 0);
  set_bit(verticals[2], 2);
  cave.SetVerticals(verticals);

  cave.SolveCave(3, 4);

  auto new_verticals = cave.GetVerticals();
  EXPECT_EQ(cave.GetBit(new_verticals[1], 1), 1);

  cave.SetVerticals(verticals);  // сброс
  cave.SolveCave(3, 5);
  new_verticals = cave.GetVerticals();
  EXPECT_EQ(cave.GetBit(new_verticals[1], 1), 0);
}

TEST(CaveTest, SetRowsCols) {
  Maze cave;
  EXPECT_TRUE(cave.SetRowsCols(10, 10));
  EXPECT_EQ(cave.GetRows(), 10);
  EXPECT_EQ(cave.GetCols(), 10);

  EXPECT_FALSE(cave.SetRowsCols(kMaxSize + 1, 10));
  EXPECT_FALSE(cave.SetRowsCols(10, kMaxSize + 1));
}
