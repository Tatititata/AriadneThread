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

/**
 * @brief The MazeWidget class provides a complete interface for maze
 * generation, solving and Q-learning.
 *
 * This widget combines visualization components (MazeDrawWidget,
 * PathDrawWidget, BonusDrawWidget) with controls for:
 * - Maze generation with customizable dimensions
 * - Pathfinding between start and target points
 * - Q-learning algorithm training and path visualization
 * - Loading/saving maze configurations
 */
class MazeWidget : public QWidget {
  Q_OBJECT

 public:
  /**
   * @brief Constructs a MazeWidget with default parameters.
   * @param parent The parent widget (optional).
   *
   * Initializes a maze with default size (10x10) and random generation.
   */
  explicit MazeWidget(QWidget* parent = nullptr);

 signals:
  /**
   * @brief Emitted when the back button is pressed.
   */
  void BackRequested();

  /**
   * @brief Emitted when maze dimensions are changed.
   */
  void MazeSizeChanged();

  /**
   * @brief Emitted when secret back button is pressed.
   */
  void SecretBackPressed();

  /**
   * @brief Emitted when solve tab becomes active.
   */
  void SolveTabActivated();

  /**
   * @brief Emitted when Q-learning tab becomes active.
   */
  void QLearnTabActivated();

  /**
   * @brief Emitted when Q-learning agent finishes training.
   */
  void AgentReady();

 private:
  Maze* m_pmaze_;                   ///< The maze model being visualized
  QLearning* m_pagent_;             ///< Q-learning agent for pathfinding
  MazeDrawWidget* m_pmaze_view_;    ///< Visualization widget for the maze
  PathDrawWidget* m_ppath_view_;    ///< Widget for displaying solution paths
  BonusDrawWidget* m_pbonus_view_;  ///< Widget for bonus visualization
  std::vector<Cell> m_solution_;    ///< Current solution path

  /**
   * @brief Creates the side control panel.
   * @return Configured widget containing all controls.
   */
  QWidget* CreateSideMenu();

  /**
   * @brief Creates the solve tab with pathfinding controls.
   * @param menu Parent widget for the tab.
   * @return Configured solve tab widget.
   */
  QWidget* CreateSolveTab(QWidget* menu);

  /**
   * @brief Creates the Q-learning tab with training controls.
   * @param menu Parent widget for the tab.
   * @return Configured Q-learning tab widget.
   */
  QWidget* CreateQLearnTab(QWidget* menu);

  /**
   * @brief Creates the main maze visualization area.
   * @return Configured widget with maze visualization.
   */
  QWidget* MazeDrawing();

  /**
   * @brief Updates solution visualization for single point.
   * @param cell The cell to visualize.
   */
  void UpdateSolution(const Cell& cell);

  /**
   * @brief Updates solution visualization for two points.
   * @param cell1 First cell to visualize.
   * @param cell2 Second cell to visualize.
   */
  void UpdateSolution(const Cell& cell1, const Cell& cell2);

  /**
   * @brief Creates the button row (back/load/save).
   * @return Configured layout with action buttons.
   */
  QHBoxLayout* CreateButtonsLayout();

  /**
   * @brief Creates the tab widget for solve/Q-learning modes.
   * @param menu Parent widget for the tab widget.
   * @return Configured tab widget.
   */
  QTabWidget* CreateTabWidget(QWidget* menu);

  /**
   * @brief Creates the secret visualization widget.
   * @param menu Parent widget for the secret widget.
   * @return Configured secret widget.
   */
  QWidget* CreateSecretWidget(QWidget* menu);

  /**
   * @brief Creates the maze generation controls layout.
   * @return Configured layout with generation controls.
   */
  QVBoxLayout* CreateGenerateLayout();

  /**
   * @brief Sets random values for spinboxes based on maze size.
   * @param spinbox The spinbox to configure.
   * @param c Dimension type ('r' for rows, 'c' for columns).
   */
  void SetSpinboxValues(QSpinBox* spinbox, const char c);

  /**
   * @brief Creates a labeled spinbox layout.
   * @param spinbox The spinbox to include.
   * @param label_text The label text.
   * @param parent The parent widget.
   * @return Configured layout with label and spinbox.
   */
  QVBoxLayout* CreateSpinBox(QSpinBox* spinbox, const QString& label_text,
                             QWidget* parent);

  /**
   * @brief Sets initial values for solve tab spinboxes.
   * @param target_row Target row spinbox.
   * @param target_col Target column spinbox.
   * @param start_row Start row spinbox.
   * @param start_col Start column spinbox.
   */
  void SetSolveSpinboxValues(QSpinBox* target_row, QSpinBox* target_col,
                             QSpinBox* start_row, QSpinBox* start_col);

  /**
   * @brief Connects solve tab spinboxes to update events.
   * @param target_row Target row spinbox.
   * @param target_col Target column spinbox.
   * @param start_row Start row spinbox.
   * @param start_col Start column spinbox.
   */
  void ConnectSolveTabSpins(QSpinBox* target_row, QSpinBox* target_col,
                            QSpinBox* start_row, QSpinBox* start_col);

  /**
   * @brief Connects Q-learning widgets to their events.
   * @param target_row Target row spinbox.
   * @param target_col Target column spinbox.
   * @param start_row Start row spinbox.
   * @param start_col Start column spinbox.
   * @param train_button Train button.
   * @param find_path_button Find path button.
   * @param start_group Start position group box.
   * @param new_target_button New target button.
   * @param q_values_button Q-values visualization button.
   */
  void ConnectQLearnWidgets(QSpinBox* target_row, QSpinBox* target_col,
                            QSpinBox* start_row, QSpinBox* start_col,
                            QPushButton* train_button,
                            QPushButton* find_path_button,
                            QGroupBox* start_group,
                            QPushButton* new_target_button,
                            QPushButton* q_values_button);

  /**
   * @brief Starts Q-learning training process.
   * @param target_row Target row spinbox.
   * @param target_col Target column spinbox.
   */
  void StartTraining(QSpinBox* target_row, QSpinBox* target_col);

  /**
   * @brief Creates a group box with labeled spinboxes for Q-learning.
   * @param qlearn_tab Parent widget.
   * @param row Row spinbox.
   * @param col Column spinbox.
   * @param label_text Group box label text.
   * @return Configured group box.
   */
  QGroupBox* CreateQLearnGroup(QWidget*& qlearn_tab, QSpinBox*& row,
                               QSpinBox*& col, const QString& label_text);

  /**
   * @brief Shows path found by Q-learning agent.
   * @param start_row Start row spinbox.
   * @param start_col Start column spinbox.
   */
  void ShowAgentPath(QSpinBox* start_row, QSpinBox* start_col);

  /**
   * @brief Connects spinboxes to maze size changes.
   * @param target_row Target row spinbox.
   * @param target_col Target column spinbox.
   * @param start_row Start row spinbox.
   * @param start_col Start column spinbox.
   */
  void ConnectSpinboxesToMazeSize(QSpinBox* target_row, QSpinBox* target_col,
                                  QSpinBox* start_row, QSpinBox* start_col);

  /**
   * @brief Connects spinboxes to solution updates.
   * @param target_row Target row spinbox.
   * @param target_col Target column spinbox.
   * @param start_row Start row spinbox.
   * @param start_col Start column spinbox.
   */
  void ConnectSpinboxesToSolution(QSpinBox* target_row, QSpinBox* target_col,
                                  QSpinBox* start_row, QSpinBox* start_col);
};

#endif  // MAZEWIDGET_H_