#include <gtest/gtest.h>

#include "../model/maze/maze.h"

const char* maze_data_manual =
    "\n"
    "3 3\n"
    "0 1 0\n"
    "1 0 1\n"
    "0 1 0\n"
    "\n"
    "1 1 1\n"
    "1 1 1\n"
    "1 1 1\n";

TEST(MazeParserTest, LoadManualMazeValid) {
  std::istringstream ss(maze_data_manual);
  Maze maze;
  bool success = maze.Load(ss, 'm');
  EXPECT_TRUE(success);
  EXPECT_EQ(maze.GetRows(), 3);
  EXPECT_EQ(maze.GetCols(), 3);

  EXPECT_EQ(Maze::GetBit(maze.GetVerticals()[1], 0), 1);
  EXPECT_EQ(Maze::GetBit(maze.GetHorizontals()[2], 2), 1);
}

TEST(MazeParserTest, LoadManualMazeInvalidSize) {
  std::istringstream ss("a b\n");
  Maze maze;
  bool success = maze.Load(ss, 'm');
  EXPECT_FALSE(success);
}

TEST(MazeParserTest, LoadManualMazeInvalidData) {
  std::istringstream ss("3 3\n\n 1 1 0\n 1 1\n");
  Maze maze;
  bool success = maze.Load(ss, 'm');
  EXPECT_FALSE(success);
}

TEST(MazeParserTest, SaveAndLoadRoundtrip) {
  Maze maze(3, 3);
  maze.GenerateMaze();

  std::ostringstream oss;
  ASSERT_TRUE(maze.Save(oss, 'm'));

  std::istringstream iss(oss.str());
  Maze loaded;
  ASSERT_TRUE(loaded.Load(iss, 'm'));

  EXPECT_EQ(maze.GetRows(), loaded.GetRows());
  EXPECT_EQ(maze.GetCols(), loaded.GetCols());

  for (int i = 0; i < maze.GetRows(); ++i) {
    EXPECT_EQ(maze.GetVerticals()[i], loaded.GetVerticals()[i]);
    EXPECT_EQ(maze.GetHorizontals()[i], loaded.GetHorizontals()[i]);
  }
}

TEST(MazeParserTest, SaveInvalidStream) {
  Maze maze(3, 3);
  maze.GenerateMaze();
  std::ostringstream oss;
  oss.setstate(std::ios::badbit);
  EXPECT_FALSE(maze.Save(oss, 'm'));
}

TEST(MazeParserTest, LoadInvalidStream) {
  std::istringstream iss;
  iss.setstate(std::ios::badbit);
  Maze maze;
  EXPECT_FALSE(maze.Load(iss, 'm'));
}

TEST(MazeLoaderTest, LoadFailsOnInvalidSizeFormat) {
  std::istringstream iss("invalid data\n0 1 0\n1 0 1\n");
  Maze maze;
  bool result = maze.Load(iss, 'c');
  EXPECT_FALSE(result);
}

TEST(MazeLoaderTest, LoadFailsOnInvalidMatrixData) {
  std::istringstream iss(
      "\n"
      "2 2\n"
      "0 1\n"
      "2 0\n"
      "\n");
  Maze maze;
  bool result = maze.Load(iss, 'c');
  EXPECT_FALSE(result);
}

TEST(MazeLoaderTest, LoadFailsOnNegativeSize) {
  std::istringstream iss("\n0 -1 0\n1 0 1\n");
  Maze maze;
  bool result = maze.Load(iss, 'c');
  EXPECT_FALSE(result);
}