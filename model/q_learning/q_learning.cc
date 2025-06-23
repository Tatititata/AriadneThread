#include "q_learning.h"

#ifdef TESTING
QLearning::QLearning()
    :
#else
QLearning::QLearning(QObject *parent)
    : QObject(parent),
#endif
      m_stop_requested_(false),
      m_is_learning_(false),
      m_pmaze_(nullptr),
      m_goal_({}),
      dist_action_(0, kAction - 1) {
}

void QLearning::Init(Maze *maze, Cell goal) {
  m_pmaze_ = maze;
  m_goal_ = goal;

  m_stop_requested_ = false;
  for (auto &matrix2d : m_table_) {
    for (auto &array4 : matrix2d) {
      array4.fill(0.0);
    }
  }
}

Cell QLearning::GetNext(const Cell &cur, int action) {
  Cell next = cur;
  if (action == 0)
    next.r -= 1;
  else if (action == 1)
    next.r += 1;
  else if (action == 2)
    next.c -= 1;
  else
    next.c += 1;
  return next;
}

Cell QLearning::ChooseAction(const Cell &cur, int &action) {
  Cell next = cur;
  if (Maze::_dist_real(Maze::_gen) < kEpsilon) {
    bool cango = false;
    int steps = 0;
    while (!cango && steps < kAction) {
      action = dist_action_(Maze::_gen);
      next = GetNext(cur, action);
      cango = m_pmaze_->CanGo(cur, next);
      ++steps;
    }
    if (!cango) next = cur;
  } else {
    double max_q = -std::numeric_limits<double>::infinity();
    for (int a = 0; a < kAction; a++) {
      Cell tmp = GetNext(cur, a);
      if (m_pmaze_->CanGo(cur, tmp)) {
        if (m_table_[cur.r][cur.c][a] > max_q) {
          max_q = m_table_[cur.r][cur.c][a];
          next = tmp;
          action = a;
        }
      }
    }
  }
  return next;
}

void QLearning::Update(const Cell &cur, int action, const Cell &next,
                       double reward) {
  double max_next_q = -std::numeric_limits<double>::infinity();
  for (int a = 0; a < kAction; a++) {
    if (m_table_[next.r][next.c][a] > max_next_q) {
      max_next_q = m_table_[next.r][next.c][a];
    }
  }
  m_table_[cur.r][cur.c][action] =
      (1 - kAlpha) * m_table_[cur.r][cur.c][action] +
      kAlpha * (reward + kGamma * max_next_q);
}

#ifndef TESTING
void QLearning::StopLearning() { m_stop_requested_ = true; }
#endif

void QLearning::Train() {
  m_is_learning_ = true;
  std::uniform_int_distribution<> dist_row(0, m_pmaze_->GetRows() - 1);
  std::uniform_int_distribution<> dist_col(0, m_pmaze_->GetCols() - 1);

  double penalty = (double)kReward / 7.0 / (double)kMaxStepPerEpisode;

  for (int p = 0; p < kPercents; ++p) {
#ifndef TESTING
    emit Progress(p);
#endif
    for (int episode = 0; episode < kMaxEpisodes; episode++) {
      if (m_stop_requested_) return;
      Cell current = {dist_row(Maze::_gen), dist_col(Maze::_gen)};
      int steps = 0;
      while (current != m_goal_ && steps < kMaxStepPerEpisode) {
        int action;
        Cell next = ChooseAction(current, action);
        double reward = penalty;
        if (next == m_goal_) {
          reward = kReward;
        }
        Update(current, action, next, reward);
        steps++;
        current = next;
      }
    }
  }
  m_is_learning_ = false;
#ifndef TESTING
  emit EndLearningProgress();
#endif
}

std::vector<Cell> QLearning::FindPath(Cell start) {
  size_t max_steps = m_pmaze_->GetRows() * m_pmaze_->GetCols();
  std::vector<Cell> pass;

  Cell cur = start;

  pass.push_back(cur);
  while (cur != m_goal_ && pass.size() < max_steps) {
    double max_q = -std::numeric_limits<double>::infinity();
    Cell next = cur;

    for (int a = 0; a < kAction; a++) {
      Cell tmp = GetNext(cur, a);
      if (m_pmaze_->CanGo(cur, tmp)) {
        if (m_table_[cur.r][cur.c][a] > max_q) {
          max_q = m_table_[cur.r][cur.c][a];
          next = tmp;
        }
      }
    }
    cur = next;
    pass.push_back(cur);
  }
  if (cur != m_goal_) pass.clear();
  return pass;
}

std::string QLearning::QValuesToString() const {
  std::ostringstream out;
  for (int i = 0; i < m_pmaze_->GetRows(); i++) {
    for (int j = 0; j < m_pmaze_->GetCols(); j++) {
      double max_q = -std::numeric_limits<double>::infinity();
      for (int a = 0; a < kAction; a++) {
        if (m_table_[i][j][a] > max_q) {
          max_q = m_table_[i][j][a];
        }
      }
      out.width(10);
      out.precision(3);
      out << std::fixed << max_q << " ";
    }
    out << "\n";
  }
  return out.str();
}
