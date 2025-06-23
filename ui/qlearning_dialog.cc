#include "qlearning_dialog.h"

QLearningDialog::QLearningDialog(const QLearning* agent, QWidget* parent)
    : QDialog(parent), m_pagent_(agent) {
  hide();
  setModal(true);

  setFixedWidth(250);
  setWindowTitle("Agent traning");
  m_elapsed_timer = new QElapsedTimer();
  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setAlignment(Qt::AlignHCenter);

  QProgressBar* progress_bar = new QProgressBar(this);
  progress_bar->setRange(0, 100);
  progress_bar->setValue(0);
  progress_bar->setStyleSheet(MazeStyle::GetProgressStyle());

  QPushButton* stop_button = new QPushButton("Stop", this);

  stop_button->setStyleSheet(MazeStyle::GetMenuStyle());
  QPushButton* ok_button = new QPushButton("OK", this);
  layout->addWidget(progress_bar);
  QLabel* finished_label = new QLabel("Training complete", this);
  finished_label->setAlignment(Qt::AlignCenter);
  QLabel* time_label = new QLabel(this);
  time_label->setAlignment(Qt::AlignCenter);
  time_label->hide();

  layout->addWidget(progress_bar);
  layout->addWidget(stop_button);
  layout->addWidget(finished_label);
  layout->addWidget(time_label);
  layout->addWidget(ok_button);
  finished_label->hide();
  ok_button->hide();
  connect(ok_button, &QPushButton::clicked, this, [this]() {
    emit TrainingFinished();
    this->accept();
  });

  connect(stop_button, &QPushButton::clicked, this, [this]() {
    emit AgentStopLearning();
    this->reject();
  });
  connect(m_pagent_, &QLearning::EndLearningProgress, this,
          [this, progress_bar, finished_label, ok_button, stop_button,
           time_label]() {
            progress_bar->hide();
            stop_button->hide();
            finished_label->show();
            ok_button->show();

            qint64 ms = m_elapsed_timer->elapsed();
            int seconds = ms / 1000;
            int milliseconds = ms % 1000;

            time_label->setText(QString("Training time: %1.%2 s")
                                    .arg(seconds)
                                    .arg(milliseconds, 3, 10, QChar('0')));
            time_label->show();
          });
  m_elapsed_timer->start();
  connect(m_pagent_, &QLearning::Progress, progress_bar,
          &QProgressBar::setValue);
  show();
}

void QLearningDialog::closeEvent(QCloseEvent* event) {
  if (m_pagent_) {
    if (m_pagent_->IsLearning()) {
      emit AgentStopLearning();
    } else {
      emit TrainingFinished();
    }
  }
  event->accept();
}

QLearningDialog::QLearningDialog(const QString& text, QWidget* parent)
    : QDialog(parent), m_pagent_(nullptr) {
  setModal(true);
  setWindowTitle("Q-values visualization");
  resize(600, 400);

  QVBoxLayout* layout = new QVBoxLayout(this);

  QPlainTextEdit* text_edit = new QPlainTextEdit(this);
  text_edit->setReadOnly(true);

  QFont font("Courier New");
  font.setStyleHint(QFont::Monospace);
  text_edit->setFont(font);

  text_edit->setPlainText(text);
  layout->addWidget(text_edit);

  QPushButton* ok_button = new QPushButton("OK", this);
  layout->addWidget(ok_button);
  connect(ok_button, &QPushButton::clicked, this, &QDialog::accept);

  show();
}
