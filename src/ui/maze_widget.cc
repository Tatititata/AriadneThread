
#include "maze_widget.h"

MazeWidget::MazeWidget(QWidget* parent) : QWidget(parent), m_pagent_() {
  Maze::InitRandom();
  m_pmaze_ = new Maze(10, 10);
  m_pmaze_->GenerateMaze();

  QHBoxLayout* main_layout = new QHBoxLayout(this);
  main_layout->setContentsMargins(0, 0, 0, 0);
  main_layout->setSpacing(0);

  main_layout->addWidget(MazeDrawing());
  main_layout->insertWidget(0, CreateSideMenu());
  main_layout->setAlignment(Qt::AlignTop);
  setLayout(main_layout);
}

QWidget* MazeWidget::CreateSideMenu() {
  QWidget* menu = new QWidget(this);
  menu->setFixedWidth(170);
  QVBoxLayout* layout = new QVBoxLayout(menu);

  QHBoxLayout* button_layout = CreateButtonsLayout();
  QVBoxLayout* generate_layout = CreateGenerateLayout();

  QStackedLayout* has_secret_layout = new QStackedLayout();
  QTabWidget* tab_widget = CreateTabWidget(menu);
  QWidget* secret_widget = CreateSecretWidget(menu);

  QPushButton* secret_button = new QPushButton(menu);
  secret_button->setFixedSize(20, 20);
  secret_button->setFlat(true);
  secret_button->setStyleSheet("background: transparent; border: none;");
  secret_button->move(60, 40);

  has_secret_layout->addWidget(tab_widget);
  has_secret_layout->addWidget(secret_widget);
  has_secret_layout->setAlignment(Qt::AlignTop);

  layout->addLayout(button_layout);
  layout->addLayout(generate_layout);
  layout->addLayout(has_secret_layout);
  layout->addStretch(1);
  layout->setAlignment(Qt::AlignTop);

  connect(
      secret_button, &QPushButton::clicked, this,
      [this, has_secret_layout]() { has_secret_layout->setCurrentIndex(1); });

  connect(this, &MazeWidget::SecretBackPressed, this,
          [has_secret_layout]() { has_secret_layout->setCurrentIndex(0); });
  secret_button->raise();
  return menu;
}

QHBoxLayout* MazeWidget::CreateButtonsLayout() {
  QHBoxLayout* button_layout = new QHBoxLayout();

  QPushButton* back_btn = new QPushButton("Back");
  QPushButton* load_btn = new QPushButton("Load");
  QPushButton* save_btn = new QPushButton("Save");

  back_btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  load_btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  save_btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

  button_layout->addWidget(back_btn);
  button_layout->addWidget(load_btn);
  button_layout->addWidget(save_btn);

  connect(back_btn, &QPushButton::clicked, this, &MazeWidget::BackRequested);
  connect(load_btn, &QPushButton::clicked, this, [this]() {
    if (MazeFileLoader::LoadMazeFromFile(this, m_pmaze_, 'm')) {
      emit MazeSizeChanged();
      m_pmaze_view_->SetMaze(m_pmaze_);
      m_pbonus_view_->hide();
      m_ppath_view_->show();
    }
  });
  connect(save_btn, &QPushButton::clicked, this,
          [this]() { MazeFileLoader::SaveMazeToFile(this, m_pmaze_, 'm'); });

  return button_layout;
}

QWidget* MazeWidget::MazeDrawing() {
  QWidget* drawing_container = new QWidget(this);
  QGridLayout* layout = new QGridLayout(drawing_container);
  layout->setContentsMargins(0, 0, 0, 0);

  m_pmaze_view_ = new MazeDrawWidget(m_pmaze_, drawing_container);
  m_ppath_view_ = new PathDrawWidget(m_pmaze_->GetRows(), m_pmaze_->GetCols(),
                                     drawing_container);
  m_pbonus_view_ = new BonusDrawWidget(drawing_container);

  layout->addWidget(m_pmaze_view_, 0, 0);
  layout->addWidget(m_ppath_view_, 0, 0);
  layout->addWidget(m_pbonus_view_, 0, 0);
  m_pbonus_view_->hide();

  return drawing_container;
}

QVBoxLayout* MazeWidget::CreateSpinBox(QSpinBox* spinbox,
                                       const QString& label_text,
                                       QWidget* parent) {
  QVBoxLayout* layout = new QVBoxLayout;
  QLabel* label = new QLabel(label_text, parent);
  layout->addWidget(label);
  layout->addWidget(spinbox);
  return layout;
}

QVBoxLayout* MazeWidget::CreateGenerateLayout() {
  QVBoxLayout* generate_layout = new QVBoxLayout();
  QHBoxLayout* size_layout = new QHBoxLayout();

  QSpinBox* rows = new QSpinBox();
  QSpinBox* cols = new QSpinBox();

  rows->setRange(1, kMaxSize);
  rows->setValue(m_pmaze_->GetRows());
  cols->setRange(1, kMaxSize);
  cols->setValue(m_pmaze_->GetCols());

  size_layout->addLayout(CreateSpinBox(rows, "Rows:", nullptr));
  size_layout->addLayout(CreateSpinBox(cols, "Columns:", nullptr));

  generate_layout->addLayout(size_layout);

  QPushButton* generate = new QPushButton("Generate");
  generate->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  generate_layout->addWidget(generate);

  connect(generate, &QPushButton::clicked, this, [this, rows, cols]() {
    m_pmaze_->SetRowsCols(rows->value(), cols->value());
    m_pmaze_->GenerateMaze();
    emit MazeSizeChanged();
    m_pmaze_view_->SetMaze(m_pmaze_);
    // emit SecretBackPressed();
    m_pbonus_view_->hide();
    m_ppath_view_->show();
  });
  connect(this, &MazeWidget::MazeSizeChanged, this, [this, rows, cols]() {
    rows->setValue(m_pmaze_->GetRows());
    cols->setValue(m_pmaze_->GetCols());
  });
  return generate_layout;
}

QWidget* MazeWidget::CreateSecretWidget(QWidget* menu) {
  QWidget* secret_widget = new QWidget(menu);
  secret_widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  QVBoxLayout* secret_layout = new QVBoxLayout(secret_widget);

  QSpinBox* secret_row = new QSpinBox(secret_widget);
  QSpinBox* secret_col = new QSpinBox(secret_widget);

  QHBoxLayout* point_layout = new QHBoxLayout();
  point_layout->addLayout(CreateSpinBox(secret_row, "Row:", secret_widget));
  point_layout->addLayout(CreateSpinBox(secret_col, "Column:", secret_widget));

  SetSpinboxValues(secret_row, 'r');
  SetSpinboxValues(secret_col, 'c');

  QPushButton* run = new QPushButton("Run", secret_widget);
  run->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  QPushButton* back = new QPushButton("Back", secret_widget);
  back->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  secret_layout->addLayout(point_layout);
  secret_layout->addWidget(run);
  secret_layout->addWidget(back);
  secret_layout->setAlignment(Qt::AlignTop);

  connect(this, &MazeWidget::MazeSizeChanged, secret_row,
          [this, secret_row]() { SetSpinboxValues(secret_row, 'r'); });
  connect(this, &MazeWidget::MazeSizeChanged, secret_col,
          [this, secret_col]() { SetSpinboxValues(secret_col, 'c'); });
  connect(run, &QPushButton::clicked, this, [this, secret_row, secret_col] {
    m_ppath_view_->hide();
    m_pbonus_view_->show();
    m_pbonus_view_->SetSolution(
        m_pmaze_->DistanceMatrix(
            {secret_row->value() - 1, secret_col->value() - 1}),
        m_pmaze_->GetRows(), m_pmaze_->GetCols());
  });
  connect(back, &QPushButton::clicked, this, [this]() {
    if (m_pbonus_view_) {
      m_pbonus_view_->hide();
    }
    m_ppath_view_->show();
    emit SecretBackPressed();
  });
  return secret_widget;
}

void MazeWidget::SetSpinboxValues(QSpinBox* spinbox, const char c) {
  int value;
  if (c == 'r')
    value = m_pmaze_->GetRows();
  else
    value = m_pmaze_->GetCols();
  std::uniform_int_distribution<> dist(1, value);
  spinbox->setRange(1, value);
  spinbox->setValue(dist(Maze::_gen));
}

QTabWidget* MazeWidget::CreateTabWidget(QWidget* menu) {
  QTabWidget* tab_widget = new QTabWidget(menu);
  tab_widget->setTabPosition(QTabWidget::North);

  QWidget* solve_tab = CreateSolveTab(tab_widget);
  QWidget* qlearn_tab = CreateQLearnTab(tab_widget);

  tab_widget->addTab(solve_tab, "Solve");
  tab_widget->addTab(qlearn_tab, "Q-learn");
  tab_widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  connect(this, &MazeWidget::MazeSizeChanged, tab_widget, [this, tab_widget]() {
    tab_widget->setCurrentIndex(0);
    emit SolveTabActivated();
  });
  connect(tab_widget, &QTabWidget::currentChanged, this, [this](int index) {
    if (index == 0) {
      emit SolveTabActivated();
    } else if (index == 1) {
      emit QLearnTabActivated();
    }
  });
  return tab_widget;
}

QWidget* MazeWidget::CreateSolveTab(QWidget* menu) {
  QWidget* solve_tab = new QWidget(menu);
  QVBoxLayout* layout = new QVBoxLayout(solve_tab);

  QSpinBox* target_row = new QSpinBox(solve_tab);
  QSpinBox* target_col = new QSpinBox(solve_tab);
  QHBoxLayout* target_cell = new QHBoxLayout();
  target_cell->addLayout(CreateSpinBox(target_row, "Row:", solve_tab));
  target_cell->addLayout(CreateSpinBox(target_col, "Column:", solve_tab));
  QSpinBox* start_row = new QSpinBox(solve_tab);
  QSpinBox* start_col = new QSpinBox(solve_tab);
  QHBoxLayout* end_cell = new QHBoxLayout();
  end_cell->addLayout(CreateSpinBox(start_row, "Row:", solve_tab));
  end_cell->addLayout(CreateSpinBox(start_col, "Column:", solve_tab));
  SetSolveSpinboxValues(target_row, target_col, start_row, start_col);

  QPushButton* solve_button = new QPushButton("Solve", solve_tab);
  solve_tab->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);

  layout->addWidget(new QLabel("Target", solve_tab), 0, Qt::AlignHCenter);
  layout->addLayout(target_cell);
  layout->addSpacing(10);
  layout->addWidget(new QLabel("Start", solve_tab), 0, Qt::AlignHCenter);
  layout->addLayout(end_cell);
  layout->addWidget(solve_button);
  layout->setAlignment(Qt::AlignTop);
  ConnectSolveTabSpins(target_row, target_col, start_row, start_col);
  connect(solve_button, &QPushButton::clicked, this,
          [this, target_row, target_col, start_row, start_col]() {
            Cell target = {target_row->value() - 1, target_col->value() - 1};
            Cell start = {start_row->value() - 1, start_col->value() - 1};
            m_solution_ = m_pmaze_->SolveMaze(start, target);
            if (!m_solution_.empty()) {
              m_ppath_view_->SetSolution(m_solution_, m_pmaze_->GetRows(),
                                         m_pmaze_->GetCols());
            } else {
              QMessageBox::information(this, "Sorry", "Can't find path.");
            }
          });
  UpdateSolution({start_row->value(), start_col->value()},
                 {target_row->value(), target_col->value()});
  return solve_tab;
}

void MazeWidget::SetSolveSpinboxValues(QSpinBox* target_row,
                                       QSpinBox* target_col,
                                       QSpinBox* start_row,
                                       QSpinBox* start_col) {
  SetSpinboxValues(target_row, 'r');
  SetSpinboxValues(target_col, 'c');
  SetSpinboxValues(start_row, 'r');
  SetSpinboxValues(start_col, 'c');
}

void MazeWidget::ConnectSolveTabSpins(QSpinBox* target_row,
                                      QSpinBox* target_col, QSpinBox* start_row,
                                      QSpinBox* start_col) {
  connect(this, &MazeWidget::MazeSizeChanged, this,
          [this, target_row, target_col, start_row, start_col]() {
            SetSolveSpinboxValues(target_row, target_col, start_row, start_col);
          });
  auto update_lambda = [this, target_row, target_col, start_row, start_col]() {
    UpdateSolution({start_row->value(), start_col->value()},
                   {target_row->value(), target_col->value()});
  };
  connect(target_row, QOverload<int>::of(&QSpinBox::valueChanged), this,
          update_lambda);
  connect(target_col, QOverload<int>::of(&QSpinBox::valueChanged), this,
          update_lambda);
  connect(start_row, QOverload<int>::of(&QSpinBox::valueChanged), this,
          update_lambda);
  connect(start_col, QOverload<int>::of(&QSpinBox::valueChanged), this,
          update_lambda);
  connect(this, &MazeWidget::SolveTabActivated, this, update_lambda);
}

void MazeWidget::UpdateSolution(const Cell& cell) {
  m_solution_.clear();
  m_solution_.push_back((Cell){cell.r - 1, cell.c - 1});
  m_ppath_view_->SetSolution(m_solution_, m_pmaze_->GetRows(),
                             m_pmaze_->GetCols());
}

void MazeWidget::UpdateSolution(const Cell& cell1, const Cell& cell2) {
  m_solution_.clear();
  m_solution_.push_back((Cell){cell1.r - 1, cell1.c - 1});
  m_solution_.push_back((Cell){cell2.r - 1, cell2.c - 1});
  m_ppath_view_->SetSolution(m_solution_, m_pmaze_->GetRows(),
                             m_pmaze_->GetCols());
}

void MazeWidget::StartTraining(QSpinBox* target_row, QSpinBox* target_col) {
  Cell target = {target_row->value() - 1, target_col->value() - 1};

  if (m_pagent_) {
    m_pagent_->deleteLater();
    m_pagent_ = nullptr;
  }

  m_pagent_ = new QLearning();
  m_pagent_->Init(m_pmaze_, target);

  QLearningDialog* dialog = new QLearningDialog(m_pagent_, this);
  QThread* thread = new QThread;
  m_pagent_->moveToThread(thread);

  connect(thread, &QThread::started, m_pagent_, &QLearning::Train);
  connect(dialog, &QLearningDialog::TrainingFinished, this,
          [this, thread, dialog]() {
            if (thread->isRunning()) {
              thread->quit();
              thread->wait();
            }
            dialog->close();
            dialog->deleteLater();
            emit AgentReady();
          });
  connect(thread, &QThread::finished, thread, &QThread::deleteLater);
  connect(dialog, &QLearningDialog::AgentStopLearning, this, [this, dialog]() {
    if (m_pagent_) {
      m_pagent_->StopLearning();
      m_pagent_->deleteLater();
      m_pagent_ = nullptr;
    }
    dialog->close();
    dialog->deleteLater();
  });
  thread->start();
  dialog->exec();
}

QWidget* MazeWidget::CreateQLearnTab(QWidget* menu) {
  QWidget* qlearn_tab = new QWidget(menu);
  QVBoxLayout* layout = new QVBoxLayout(qlearn_tab);

  QSpinBox* target_row = new QSpinBox(qlearn_tab);
  QSpinBox* target_col = new QSpinBox(qlearn_tab);
  QSpinBox* start_row = new QSpinBox(qlearn_tab);
  QSpinBox* start_col = new QSpinBox(qlearn_tab);
  QPushButton* train_button = new QPushButton("Train", qlearn_tab);
  QPushButton* find_path_button = new QPushButton("Find path", qlearn_tab);
  QPushButton* new_target_button = new QPushButton("New Target", qlearn_tab);
  QPushButton* q_values_button =
      new QPushButton("Visualize Q-values", qlearn_tab);
  QGroupBox* target_group =
      CreateQLearnGroup(qlearn_tab, target_row, target_col, "Target cell:");
  QGroupBox* start_group =
      CreateQLearnGroup(qlearn_tab, start_row, start_col, "Start cell:");

  layout->addWidget(target_group);
  layout->addWidget(train_button);
  layout->addWidget(q_values_button);
  layout->addWidget(start_group);
  layout->addWidget(find_path_button);
  layout->addSpacing(5);
  layout->addWidget(new_target_button);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->setAlignment(Qt::AlignTop);
  start_group->hide();
  find_path_button->hide();
  q_values_button->hide();
  new_target_button->hide();

  ConnectQLearnWidgets(target_row, target_col, start_row, start_col,
                       train_button, find_path_button, start_group,
                       new_target_button, q_values_button);

  qlearn_tab->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
  return qlearn_tab;
}
void MazeWidget::ConnectSpinboxesToMazeSize(QSpinBox* target_row,
                                            QSpinBox* target_col,
                                            QSpinBox* start_row,
                                            QSpinBox* start_col) {
  connect(this, &MazeWidget::MazeSizeChanged, target_row,
          [this, target_row]() { SetSpinboxValues(target_row, 'r'); });
  connect(this, &MazeWidget::MazeSizeChanged, target_col,
          [this, target_col]() { SetSpinboxValues(target_col, 'c'); });
  connect(this, &MazeWidget::MazeSizeChanged, start_row,
          [this, start_row]() { SetSpinboxValues(start_row, 'r'); });
  connect(this, &MazeWidget::MazeSizeChanged, start_col,
          [this, start_col]() { SetSpinboxValues(start_col, 'c'); });
}

void MazeWidget::ConnectSpinboxesToSolution(QSpinBox* target_row,
                                            QSpinBox* target_col,
                                            QSpinBox* start_row,
                                            QSpinBox* start_col) {
  auto update_solution_lambda = [this, target_row, target_col, start_row,
                                 start_col]() {
    if (start_row->isVisible()) {
      UpdateSolution({start_row->value(), start_col->value()},
                     {target_row->value(), target_col->value()});
    } else {
      UpdateSolution({target_row->value(), target_col->value()});
    }
  };
  connect(target_row, QOverload<int>::of(&QSpinBox::valueChanged), this,
          update_solution_lambda);
  connect(target_col, QOverload<int>::of(&QSpinBox::valueChanged), this,
          update_solution_lambda);
  connect(start_row, QOverload<int>::of(&QSpinBox::valueChanged), this,
          update_solution_lambda);
  connect(start_col, QOverload<int>::of(&QSpinBox::valueChanged), this,
          update_solution_lambda);
  connect(this, &MazeWidget::QLearnTabActivated, this, update_solution_lambda);
  connect(this, &MazeWidget::AgentReady, this, update_solution_lambda);
}

void MazeWidget::ConnectQLearnWidgets(
    QSpinBox* target_row, QSpinBox* target_col, QSpinBox* start_row,
    QSpinBox* start_col, QPushButton* train_button,
    QPushButton* find_path_button, QGroupBox* start_group,
    QPushButton* new_target_button, QPushButton* q_values_button) {
  ConnectSpinboxesToMazeSize(target_row, target_col, start_row, start_col);
  ConnectSpinboxesToSolution(target_row, target_col, start_row, start_col);

  connect(this, &MazeWidget::MazeSizeChanged, q_values_button,
          &QPushButton::hide);

  connect(train_button, &QPushButton::clicked, this,
          [this, target_row, target_col]() {
            StartTraining(target_row, target_col);
          });
  connect(
      find_path_button, &QPushButton::clicked, this,
      [this, start_row, start_col]() { ShowAgentPath(start_row, start_col); });

  connect(q_values_button, &QPushButton::clicked, this, [this]() {
    QString qvalues_text = QString::fromStdString(m_pagent_->QValuesToString());
    QLearningDialog dlg(qvalues_text, this);
    dlg.exec();
  });

  connect(this, &MazeWidget::AgentReady, this,
          [start_group, find_path_button, new_target_button, train_button,
           target_row, target_col, q_values_button]() {
            start_group->show();
            find_path_button->show();
            new_target_button->show();
            train_button->hide();
            q_values_button->show();
            target_row->setEnabled(false);
            target_col->setEnabled(false);
          });
  connect(this, &MazeWidget::MazeSizeChanged, this,
          [start_group, find_path_button, new_target_button, train_button,
           target_row, target_col, q_values_button]() {
            start_group->hide();
            find_path_button->hide();
            new_target_button->hide();
            q_values_button->hide();
            train_button->show();
            target_row->setEnabled(true);
            target_col->setEnabled(true);
          });
  connect(new_target_button, &QPushButton::clicked, this,
          [this, start_group, find_path_button, new_target_button, train_button,
           target_row, target_col, q_values_button]() {
            start_group->hide();
            find_path_button->hide();
            new_target_button->hide();
            q_values_button->hide();
            train_button->show();
            target_row->setEnabled(true);
            target_col->setEnabled(true);
            UpdateSolution({target_row->value(), target_col->value()});
          });
}

void MazeWidget::ShowAgentPath(QSpinBox* start_row, QSpinBox* start_col) {
  m_solution_ =
      m_pagent_->FindPath({start_row->value() - 1, start_col->value() - 1});
  if (!m_solution_.empty()) {
    m_ppath_view_->SetSolution(m_solution_, m_pmaze_->GetRows(),
                               m_pmaze_->GetCols());
  } else {
    QMessageBox::information(this, "Sorry", "Can't find path.");
  }
}

QGroupBox* MazeWidget::CreateQLearnGroup(QWidget*& qlearn_tab, QSpinBox*& row,
                                         QSpinBox*& col,
                                         const QString& label_text) {
  QGroupBox* group = new QGroupBox();
  QVBoxLayout* layout = new QVBoxLayout(group);
  QHBoxLayout* target_layout = new QHBoxLayout();
  SetSpinboxValues(row, 'r');
  SetSpinboxValues(col, 'c');
  target_layout->addLayout(CreateSpinBox(row, "Row:", qlearn_tab));
  target_layout->addLayout(CreateSpinBox(col, "Col:", qlearn_tab));
  layout->addWidget(new QLabel(label_text), 0, Qt::AlignHCenter);
  layout->addLayout(target_layout);
  return group;
}