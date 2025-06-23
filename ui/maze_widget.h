#ifndef MAZE_WIDGET_H_
#define MAZE_WIDGET_H_

#include <QBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QScrollArea>
#include <QSpinBox>
#include <QStackedLayout>
#include <QTabWidget>
#include <QThread>
#include <QWidget>
#include <span>

#include "../model/maze/maze.h"
#include "../model/q_learning/q_learning.h"
#include "bonus_draw.h"
#include "loader.h"
#include "maze_draw.h"
#include "maze_style.h"
#include "path_draw.h"
#include "qlearning_dialog.h"

class MazeWidget : public QWidget {
  Q_OBJECT

 public:
  explicit MazeWidget(QWidget* parent = nullptr);

 signals:
  void BackRequested();
  void MazeSizeChanged();
  void SecretBackPressed();
  void SolveTabActivated();
  void QLearnTabActivated();
  void AgentReady();

 private:
  Maze* m_pmaze_;
  QLearning* m_pagent_;
  MazeDrawWidget* m_pmaze_view_;
  PathDrawWidget* m_ppath_view_;
  BonusDrawWidget* m_pbonus_view_;
  std::vector<Cell> m_solution_;

  QWidget* CreateSideMenu();
  QWidget* CreateSolveTab(QWidget* menu);
  QWidget* CreateQLearnTab(QWidget* menu);
  QWidget* MazeDrawing();
  void UpdateSolution(const Cell& cell);
  void UpdateSolution(const Cell& cell1, const Cell& cell2);
  QHBoxLayout* CreateButtonsLayout();
  QTabWidget* CreateTabWidget(QWidget* menu);
  QWidget* CreateSecretWidget(QWidget* menu);
  QVBoxLayout* CreateGenerateLayout();
  void SetSpinboxValues(QSpinBox* spinbox, const char c);
  QVBoxLayout* CreateSpinBox(QSpinBox* spinbox, const QString& label_text,
                             QWidget* parent);
  void SetSolveSpinboxValues(QSpinBox* target_row, QSpinBox* target_col,
                             QSpinBox* start_row, QSpinBox* start_col);
  void ConnectSolveTabSpins(QSpinBox* target_row, QSpinBox* target_col,
                            QSpinBox* start_row, QSpinBox* start_col);
  void ConnectQLearnWidgets(QSpinBox* target_row, QSpinBox* target_col,
                            QSpinBox* start_row, QSpinBox* start_col,
                            QPushButton* train_button,
                            QPushButton* find_path_button,
                            QGroupBox* start_group,
                            QPushButton* new_target_button,
                            QPushButton* q_values_button);
  void StartTraining(QSpinBox* target_row, QSpinBox* target_col);
  QGroupBox* CreateQLearnGroup(QWidget*& qlearn_tab, QSpinBox*& row,
                               QSpinBox*& col, const QString& label_text);
  void ShowAgentPath(QSpinBox* start_row, QSpinBox* start_col);
  void ConnectSpinboxesToMazeSize(QSpinBox* target_row, QSpinBox* target_col,
                                  QSpinBox* start_row, QSpinBox* start_col);
  void ConnectSpinboxesToSolution(QSpinBox* target_row, QSpinBox* target_col,
                                  QSpinBox* start_row, QSpinBox* start_col);
};
#endif  // MAZEWIDGET_H
