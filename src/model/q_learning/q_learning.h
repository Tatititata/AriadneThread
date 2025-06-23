#ifndef Q_LEARNING_H
#define Q_LEARNING_H

#ifndef TESTING
#include <QObject>
#endif

#include <array>
#include <cmath>
#include <ctime>
#include <vector>

#include "../maze/maze.h"

constexpr int kAction = 4;
constexpr int kPercents = 100;
constexpr double kAlpha = 0.1;
constexpr double kGamma = 0.95;
constexpr double kEpsilon = 0.1;

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
class QLearning {
 public:
  explicit QLearning();
#else
class QLearning : public QObject {
  Q_OBJECT
 public:
  explicit QLearning(QObject *parent = nullptr);
#endif
  std::vector<Cell> FindPath(Cell start);
  std::string QValuesToString() const;
  void Init(Maze *maze, Cell goal);
  void Train();
  bool IsLearning() const { return m_is_learning_; }

#ifndef TESTING
 signals:
  void Progress(int episode);
  void EndLearningProgress();

 public slots:
  void StopLearning();
#endif

 private:
  bool m_stop_requested_;
  bool m_is_learning_;
  Maze *m_pmaze_;
  Cell m_goal_;
  std::array<std::array<std::array<double, 4>, kMaxSize>, kMaxSize> m_table_;
  std::uniform_int_distribution<> dist_action_;
  Cell ChooseAction(const Cell &cur, int &action);
  void Update(const Cell &cur, int action, const Cell &next, double reward);
  Cell GetNext(const Cell &cur, int action);
};

#endif
