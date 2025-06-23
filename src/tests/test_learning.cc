#include <gtest/gtest.h>

#include "../model/maze/maze.h"
#include "../model/q_learning/q_learning.h"

TEST(QLearningAgentTest, LearnAndFindPathInSimpleRowMaze) {
  Maze maze(1, 5);
  maze.GenerateMaze();

  QLearning agent;
  Cell goal{0, 4};
  agent.Init(&maze, goal);
  agent.Train();
  Cell start{0, 0};
  std::vector<Cell> path = agent.FindPath(start);
  ASSERT_FALSE(path.empty());
  EXPECT_EQ(path.front(), start);
  EXPECT_EQ(path.back(), goal);
  EXPECT_EQ(path.size(), maze.GetCols());

  for (size_t i = 0; i < path.size(); ++i) {
    EXPECT_EQ(path[i].r, 0);
    EXPECT_EQ(path[i].c, static_cast<int>(i));
  }
}

TEST(QLearningTest, QValuesToStringBasic) {
  Maze maze(2, 2);
  QLearning agent;
  Cell goal{1, 1};
  agent.Init(&maze, goal);
  agent.Train();
  std::string q_values_str = agent.QValuesToString();
  EXPECT_FALSE(q_values_str.empty());
  EXPECT_NE(q_values_str.find('.'), std::string::npos);
  EXPECT_NE(q_values_str.find('0'), std::string::npos);
}