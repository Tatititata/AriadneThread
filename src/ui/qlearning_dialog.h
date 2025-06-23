#ifndef QLEARNING_DIALOG_H_
#define QLEARNING_DIALOG_H_

#include <QBoxLayout>
#include <QCloseEvent>
#include <QDialog>
#include <QElapsedTimer>
#include <QLabel>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QPushButton>

#include "../model/q_learning/q_learning.h"
#include "maze_style.h"

class QLearningDialog : public QDialog {
  Q_OBJECT
 public:
  explicit QLearningDialog(const QLearning* agent, QWidget* parent = nullptr);
  explicit QLearningDialog(const QString& text, QWidget* parent = nullptr);

 protected:
  void closeEvent(QCloseEvent* event) override;

 signals:
  void AgentStopLearning();
  void TrainingFinished();

 private:
  const QLearning* m_pagent_;
  QElapsedTimer* m_elapsed_timer;
};

#endif