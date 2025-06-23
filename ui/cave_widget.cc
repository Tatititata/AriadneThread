#include "cave_widget.h"

CaveWidget::CaveWidget(QWidget* parent)
    : QWidget(parent), m_steps_(), m_current_step_() {
  Maze::InitRandom();
  m_pcave_ = new Maze(kMaxSize / 2, kMaxSize / 2);
  m_pcave_->GenerateCave(0.5);

  QHBoxLayout* main_layout = new QHBoxLayout(this);
  main_layout->addWidget(CreateSideMenu(), 0);

  m_pcave_view_ = new CaveDrawWidget(m_pcave_, this);
  main_layout->addWidget(m_pcave_view_);

  main_layout->setContentsMargins(0, 0, 0, 0);
  main_layout->setSpacing(0);
  main_layout->setAlignment(Qt::AlignTop);
}

QWidget* CaveWidget::CreateSideMenu() {
  QWidget* menu = new QWidget(this);
  menu->setFixedWidth(170);

  QVBoxLayout* layout = new QVBoxLayout(menu);
  layout->setAlignment(Qt::AlignTop);

  QHBoxLayout* button_layout = CreateButtonsLayout();

  QGroupBox* size = CreateSizeMenu(menu);
  size->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  QGroupBox* solve = CreateSolveMenu(menu);
  solve->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  layout->addLayout(button_layout);
  layout->addWidget(size);
  layout->addWidget(solve);
  layout->addStretch();
  menu->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
  layout->setAlignment(Qt::AlignTop);
  return menu;
}

QGroupBox* CaveWidget::CreateSolveMenu(QWidget* menu) {
  QGroupBox* solve = new QGroupBox(menu);
  QVBoxLayout* solve_layout = new QVBoxLayout(solve);
  QStackedWidget* stacked_widget = new QStackedWidget(solve);
  QWidget* params = CreateParamsMenu(solve);
  QWidget* toolbox_menu = CreatePageSwitcher(solve);
  stacked_widget->addWidget(params);
  stacked_widget->addWidget(toolbox_menu);
  solve_layout->addWidget(stacked_widget);
  connect(this, &CaveWidget::SolvingStarted, this,
          [stacked_widget](int, int) { stacked_widget->setCurrentIndex(1); });
  connect(this, &CaveWidget::SwitchStackedPage, stacked_widget,
          &QStackedWidget::setCurrentIndex);
  solve->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  solve_layout->setAlignment(Qt::AlignTop);
  return solve;
}

QWidget* CaveWidget::CreateParamsMenu(QGroupBox* solve_box) {
  QWidget* params = new QWidget(solve_box);
  params->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  QVBoxLayout* params_layout = new QVBoxLayout(params);
  QHBoxLayout* birth_death_layout = new QHBoxLayout();
  QVBoxLayout* birth_layout = new QVBoxLayout();
  QLabel* birth_label = new QLabel("Birth:", params);
  m_birth_ = new QSpinBox(params);
  m_birth_->setRange(0, 7);
  m_birth_->setValue(4);
  birth_layout->addWidget(birth_label);
  birth_layout->addWidget(m_birth_);

  QVBoxLayout* death_layout = new QVBoxLayout();
  QLabel* death_label = new QLabel("Death:", params);
  m_death_ = new QSpinBox(params);
  m_death_->setRange(0, 7);
  m_death_->setValue(3);
  death_layout->addWidget(death_label);
  death_layout->addWidget(m_death_);
  birth_death_layout->addLayout(birth_layout);
  birth_death_layout->addLayout(death_layout);
  params_layout->addLayout(birth_death_layout);
  QPushButton* solve_button = new QPushButton("Solve", params);
  params_layout->addWidget(solve_button);
  params_layout->setContentsMargins(0, 0, 0, 0);
  params_layout->setAlignment(Qt::AlignTop);

  connect(solve_button, &QPushButton::clicked, this, [this]() {
    m_steps_ = 7 + (m_pcave_->GetRows() + m_pcave_->GetCols()) / 3;
    m_current_step_ = 0;
    emit SolvingStarted(m_birth_->value(), m_death_->value());
  });
  return params;
}

QWidget* CaveWidget::CreateControlMenu(QGroupBox* solve_box) {
  QWidget* control_widget = new QWidget(solve_box);
  QVBoxLayout* control_layout = new QVBoxLayout(control_widget);

  QLabel* manual_label = new QLabel("Manual solving", control_widget);
  QPushButton* next_step_btn = new QPushButton("Next step", control_widget);
  QLabel* auto_label = new QLabel("Auto solving", control_widget);
  QPushButton* auto_run_btn = new QPushButton("Run", control_widget);
  QSpinBox* interval_spinbox = new QSpinBox(control_widget);
  interval_spinbox->setRange(50, 500);
  interval_spinbox->setValue(300);
  QLabel* interval_label = new QLabel("Step in mc:", control_widget);
  QLabel* space_label = new QLabel("___", control_widget);

  QHBoxLayout* interval_layout = new QHBoxLayout();
  interval_layout->addWidget(interval_label);
  interval_layout->addWidget(interval_spinbox);

  control_layout->addWidget(manual_label, 0, Qt::AlignHCenter);
  control_layout->addWidget(next_step_btn);
  control_layout->addWidget(space_label, 0, Qt::AlignHCenter);
  control_layout->addWidget(auto_label, 0, Qt::AlignHCenter);
  control_layout->addLayout(interval_layout);
  control_layout->addWidget(auto_run_btn);
  control_layout->setContentsMargins(0, 0, 0, 0);
  control_layout->setAlignment(Qt::AlignTop);
  return control_widget;
}

QGroupBox* CaveWidget::CreateSizeMenu(QWidget* menu) {
  QGroupBox* size = new QGroupBox("Cave Size", menu);
  QVBoxLayout* main_layout = new QVBoxLayout(size);

  QHBoxLayout* size_layout = new QHBoxLayout();
  QVBoxLayout* rows_layout = new QVBoxLayout();
  QLabel* rows_label = new QLabel("Rows:", size);
  QSpinBox* rows = new QSpinBox(size);
  rows->setRange(1, kMaxSize);
  rows->setValue(m_pcave_->GetRows());
  rows_layout->addWidget(rows_label);
  rows_layout->addWidget(rows);
  QVBoxLayout* cols_layout = new QVBoxLayout();
  QLabel* cols_label = new QLabel("Columns:", size);
  QSpinBox* cols = new QSpinBox(size);
  cols->setRange(1, kMaxSize);
  cols->setValue(m_pcave_->GetCols());
  cols_layout->addWidget(cols_label);
  cols_layout->addWidget(cols);
  size_layout->addLayout(rows_layout);
  size_layout->addLayout(cols_layout);
  size_layout->setAlignment(Qt::AlignTop);
  main_layout->addLayout(size_layout);

  // chance
  QHBoxLayout* chance_layout = new QHBoxLayout();
  QLabel* chance_label = new QLabel("Chance:", size);
  QDoubleSpinBox* chance_spinbox = new QDoubleSpinBox(size);
  chance_spinbox->setRange(0.0, 1.0);
  chance_spinbox->setSingleStep(0.01);
  chance_spinbox->setDecimals(2);
  chance_spinbox->setValue(0.5);

  chance_layout->addWidget(chance_label);
  chance_layout->addWidget(chance_spinbox);
  chance_layout->setAlignment(Qt::AlignTop);
  main_layout->addLayout(chance_layout);

  // generate button
  QPushButton* generate = new QPushButton("Generate", menu);
  generate->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  main_layout->addWidget(generate);
  size->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
  main_layout->setAlignment(Qt::AlignTop);

  connect(generate, &QPushButton::clicked, this,
          [this, rows, cols, chance_spinbox]() {
            m_chance_ = chance_spinbox->value();
            m_pcave_->SetRowsCols(rows->value(), cols->value());
            m_pcave_->GenerateCave(m_chance_);
            m_pcave_view_->update();
            emit SwitchStackedPage(0);
          });

  connect(this, &CaveWidget::CaveSizeChanged, menu, [this, rows, cols]() {
    rows->setValue(m_pcave_->GetRows());
    cols->setValue(m_pcave_->GetCols());
    m_pcave_view_->update();
  });
  return size;
}

QHBoxLayout* CaveWidget::CreateButtonsLayout() {
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

  connect(back_btn, &QPushButton::clicked, this, &CaveWidget::BackRequested);
  connect(load_btn, &QPushButton::clicked, this, [this]() {
    if (MazeFileLoader::LoadMazeFromFile(this, m_pcave_, 'c')) {
      emit CaveSizeChanged();
      m_pcave_view_->update();
    }
  });
  connect(save_btn, &QPushButton::clicked, this,
          [this]() { MazeFileLoader::SaveMazeToFile(this, m_pcave_, 'c'); });

  return button_layout;
}

QWidget* CaveWidget::CreateManualTab(QWidget* parent) {
  QWidget* manual_tab = new QWidget(parent);
  QVBoxLayout* layout = new QVBoxLayout(manual_tab);
  QLabel* manual_label = new QLabel("Manual solving", manual_tab);
  manual_label->setAlignment(Qt::AlignCenter);
  QPushButton* next_step_btn = new QPushButton("Next step", manual_tab);
  layout->addWidget(manual_label);
  layout->addWidget(next_step_btn);
  layout->addStretch();
  manual_tab->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  layout->setAlignment(Qt::AlignTop);
  connect(next_step_btn, &QPushButton::clicked, this, [this]() {
    m_pcave_->SolveCave(m_birth_->value(), m_death_->value());
    m_pcave_view_->update();
  });
  return manual_tab;
}

QWidget* CaveWidget::CreateAutoTab(QWidget* parent) {
  QWidget* auto_tab = new QWidget(parent);
  QVBoxLayout* layout = new QVBoxLayout(auto_tab);
  QSpinBox* interval_spinbox = new QSpinBox(auto_tab);
  interval_spinbox->setRange(50, 500);
  interval_spinbox->setValue(300);
  QPushButton* auto_run_btn = new QPushButton("Run", auto_tab);
  layout->addWidget(new QLabel("Interval (ms):", auto_tab));
  layout->addWidget(interval_spinbox);
  layout->addWidget(auto_run_btn);
  auto_tab->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  layout->setAlignment(Qt::AlignTop);

  QTimer* timer = new QTimer(auto_tab);
  timer->setInterval(interval_spinbox->value());

  connect(interval_spinbox, QOverload<int>::of(&QSpinBox::valueChanged), timer,
          [timer](int val) { timer->setInterval(val); });
  connect(timer, &QTimer::timeout, this, [this, timer]() {
    bool done = m_pcave_->SolveCave(m_birth_->value(), m_death_->value());
    ++m_current_step_;
    m_pcave_view_->update();
    if (done || m_current_step_ > m_steps_) {
      timer->stop();
    }
  });
  connect(auto_run_btn, &QPushButton::clicked, this, [this, timer]() {
    if (timer->isActive()) {
      timer->stop();
    } else {
      timer->start();
    }
  });
  return auto_tab;
}

QWidget* CaveWidget::CreatePageSwitcher(QWidget* parent) {
  QToolBox* tool_box = new QToolBox(parent);
  tool_box->addItem(CreateManualTab(tool_box), "Manual");
  tool_box->addItem(CreateAutoTab(tool_box), "Auto");

  tool_box->setFixedHeight(170);
  return tool_box;
}
