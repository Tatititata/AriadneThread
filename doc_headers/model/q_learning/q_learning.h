#ifndef Q_LEARNING_H
#define Q_LEARNING_H

#ifndef TESTING
#include <QObject>
#endif

#include <array>
#include <cmath>
#include <ctime>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "../maze/maze.h"

/// Number of possible actions (up, down, left, right).
constexpr int kAction = 4;
constexpr int kPercents = 100;
constexpr double kAlpha = 0.1;    ///< Learning rate.
constexpr double kGamma = 0.95;   ///< Discount factor.
constexpr double kEpsilon = 0.1;  ///< Exploration rate.

#ifdef TESTING
constexpr int kMaxStepPerEpisode = 100;
constexpr int kMaxEpisodes = 100;
constexpr double kReward = 100.0;
#else
constexpr int kMaxStepPerEpisode = 100000;
constexpr int kMaxEpisodes = 1000;  // total = kMaxEpisodes * kPercents !!!
constexpr double kReward = 35500.0;
#endif

#ifdef TESTING
/**
 * @class QLearning
 * @brief Q-Learning algorithm implementation for maze solving (non-GUI/testing
 * mode).
 */
class QLearning {
 public:
  /**
   * @brief Constructor.
   */
  explicit QLearning();
#else
/**
 * @class QLearning
 * @brief Q-Learning algorithm implementation for maze solving with Qt signals
 * and slots.
 */
class QLearning : public QObject {
  Q_OBJECT
 public:
  /**
   * @brief Constructor.
   * @param parent Optional Qt parent object.
   */
  explicit QLearning(QObject *parent = nullptr);
#endif

  /**
   * @brief Finds the path from a start cell to the goal using the learned
   * Q-table.
   * @param start Starting cell.
   * @return Vector of cells representing the path. Empty if no path found.
   */
  std::vector<Cell> FindPath(Cell start);

  /**
   * @brief Returns a string representation of the maximum Q-values for each
   * cell.
   * @return Formatted string of Q-values.
   */
  std::string QValuesToString() const;

  /**
   * @brief Initializes the Q-learning with the maze and goal cell.
   * @param maze Pointer to the Maze object.
   * @param goal Goal cell to reach.
   */
  void Init(Maze *maze, Cell goal);

  /**
   * @brief Starts the Q-learning training process.
   */
  void Train();

  /**
   * @brief Checks if the learning process is currently active.
   * @return true if learning is in progress, false otherwise.
   */
  bool IsLearning() const { return m_is_learning_; }

#ifndef TESTING
 signals:
  /**
   * @brief Signal emitted to indicate progress of learning (percentage).
   * @param episode Current episode number.
   */
  void Progress(int episode);

  /**
   * @brief Signal emitted when learning has finished.
   */
  void EndLearningProgress();

 public slots:
  /**
   * @brief Slot to request stopping the learning process.
   */
  void StopLearning();
#endif

 private:
  bool m_stop_requested_;  ///< Flag to indicate if stop was requested.
  bool m_is_learning_;     ///< Flag to indicate if learning is in progress.
  Maze *m_pmaze_;          ///< Pointer to the maze.
  Cell m_goal_;            ///< Goal cell.

  /// Q-table: 3D array [row][col][action] storing Q-values.
  std::array<std::array<std::array<double, kAction>, kMaxSize>, kMaxSize>
      m_table_;

  /// Distribution to randomly select an action.
  std::uniform_int_distribution<> dist_action_;

  /**
   * @brief Chooses the next action to take from the current cell using an
   * epsilon-greedy policy.
   * @param cur Current cell.
   * @param action Output parameter to store chosen action index.
   * @return Next cell after taking the action.
   */
  Cell ChooseAction(const Cell &cur, int &action);

  /**
   * @brief Updates the Q-table based on the observed transition.
   * @param cur Current cell.
   * @param action Action taken.
   * @param next Next cell after action.
   * @param reward Reward received.
   */
  void Update(const Cell &cur, int action, const Cell &next, double reward);

  /**
   * @brief Returns the next cell given the current cell and action.
   * @param cur Current cell.
   * @param action Action index (0=up,1=down,2=left,3=right).
   * @return Next cell.
   */
  Cell GetNext(const Cell &cur, int action);
};

#endif  // Q_LEARNING_H
