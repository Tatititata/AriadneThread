#ifndef QLEARNING_DIALOG_H_
#define QLEARNING_DIALOG_H_

#include <QDialog>
#include <QElapsedTimer>

#include "../model/q_learning/q_learning.h"
#include "maze_style.h"

/**
 * @class QLearningDialog
 * @brief Dialog window for Q-Learning training progress and Q-values
 * visualization.
 *
 * Provides UI to display training progress with a progress bar,
 * allows stopping the training, and shows training duration.
 * Also supports displaying Q-values text visualization.
 */
class QLearningDialog : public QDialog {
  Q_OBJECT

 public:
  /**
   * @brief Constructs the dialog for training progress display.
   * @param agent Pointer to the QLearning agent.
   * @param parent Optional parent widget.
   */
  explicit QLearningDialog(const QLearning* agent, QWidget* parent = nullptr);

  /**
   * @brief Constructs the dialog for displaying Q-values as text.
   * @param text Text content to display (e.g., Q-values).
   * @param parent Optional parent widget.
   */
  explicit QLearningDialog(const QString& text, QWidget* parent = nullptr);

 protected:
  /**
   * @brief Handles the close event to emit appropriate signals.
   * @param event Close event.
   */
  void closeEvent(QCloseEvent* event) override;

 signals:
  /**
   * @brief Signal emitted to request stopping the learning process.
   */
  void AgentStopLearning();

  /**
   * @brief Signal emitted when training has finished.
   */
  void TrainingFinished();

 private:
  const QLearning* m_pagent_;      ///< Pointer to the QLearning agent.
  QElapsedTimer* m_elapsed_timer;  ///< Timer to measure training duration.
};

#endif  // QLEARNING_DIALOG_H_
