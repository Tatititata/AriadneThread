#include <gtest/gtest.h>

#include "../model/maze/maze.h"

TEST(MazeTest, DefaultMazeGeneration) {
  Maze maze(50, 50);
  EXPECT_NO_THROW(maze.GenerateMaze());
}

TEST(MazeTest, SingleRowMazeGeneration) {
  Maze maze(1, 5);
  maze.GenerateMaze();
  for (int j = 0; j < maze.GetCols() - 1; ++j) {
    EXPECT_EQ(Maze::GetBit(maze.GetVerticals()[0], j), 0);
  }
}

TEST(MazeTest, SingleColumnMazeGeneration) {
  Maze maze(5, 1);
  maze.GenerateMaze();

  for (int i = 0; i < maze.GetRows() - 1; ++i) {
    EXPECT_EQ(Maze::GetBit(maze.GetHorizontals()[i], 0), 0);
  }
  EXPECT_EQ(Maze::GetBit(maze.GetHorizontals()[maze.GetRows() - 1], 0), 1);
}

TEST(MazeTest, BfsSingleRow) {
  Maze maze(1, 3);
  maze.GenerateMaze();

  Cell start{0, 0};
  Cell end{0, 2};

  std::vector<Cell> path = maze.SolveMaze(end, start);
  EXPECT_EQ(path.size(), 3);
  EXPECT_EQ(path.front(), end);
  EXPECT_EQ(path.back(), start);
}

TEST(MazeTest, DistanceMatrixBasic) {
  Maze maze(3, 3);
  maze.GenerateMaze();

  Cell start{1, 1};
  auto dist_matrix = maze.DistanceMatrix(start);

  ASSERT_FALSE(dist_matrix.empty());
  ASSERT_FALSE(dist_matrix[0].empty());
  EXPECT_EQ(dist_matrix[0][0], start);

  int total_cells = 0;
  for (const auto& level : dist_matrix) {
    total_cells += static_cast<int>(level.size());
  }
  EXPECT_GE(total_cells, maze.GetRows() * maze.GetCols());
}

TEST(MazeCopyMoveTest, CopyConstructor) {
  Maze original(5, 7);
  original.GenerateMaze();

  Maze copy = original;

  EXPECT_EQ(copy.GetRows(), original.GetRows());
  EXPECT_EQ(copy.GetCols(), original.GetCols());

  for (int i = 0; i < original.GetRows(); ++i) {
    EXPECT_EQ(copy.GetVerticals()[i], original.GetVerticals()[i]);
    EXPECT_EQ(copy.GetHorizontals()[i], original.GetHorizontals()[i]);
  }
}

TEST(MazeCopyMoveTest, CopyAssignment) {
  Maze original(4, 6);
  original.GenerateMaze();

  Maze assigned;
  assigned = original;

  EXPECT_EQ(assigned.GetRows(), original.GetRows());
  EXPECT_EQ(assigned.GetCols(), original.GetCols());

  for (int i = 0; i < original.GetRows(); ++i) {
    EXPECT_EQ(assigned.GetVerticals()[i], original.GetVerticals()[i]);
    EXPECT_EQ(assigned.GetHorizontals()[i], original.GetHorizontals()[i]);
  }
}

TEST(MazeCopyMoveTest, MoveConstructor) {
  Maze original(3, 5);
  original.GenerateMaze();

  Maze moved = std::move(original);

  EXPECT_EQ(moved.GetRows(), 3);
  EXPECT_EQ(moved.GetCols(), 5);

  EXPECT_EQ(original.GetRows(), 0);
  EXPECT_EQ(original.GetCols(), 0);
}

TEST(MazeCopyMoveTest, MoveAssignment) {
  Maze original(6, 4);
  original.GenerateMaze();

  Maze moved;
  moved = std::move(original);

  EXPECT_EQ(moved.GetRows(), 6);
  EXPECT_EQ(moved.GetCols(), 4);

  EXPECT_EQ(original.GetRows(), 0);
  EXPECT_EQ(original.GetCols(), 0);
}

TEST(MazeCopyMoveTest, CopySelfAssignment) {
  Maze maze(5, 5);
  maze.GenerateMaze();

  Maze& ref = maze;
  maze = ref;

  EXPECT_EQ(maze.GetRows(), 5);
  EXPECT_EQ(maze.GetCols(), 5);
}

TEST(MazeCopyMoveTest, MoveSelfAssignment) {
  Maze maze(5, 5);
  maze.GenerateMaze();

  Maze& ref = maze;
  maze = std::move(ref);

  EXPECT_EQ(maze.GetRows(), 5);
  EXPECT_EQ(maze.GetCols(), 5);
}
