#include "main_window.h"

MazeWindow::MazeWindow(QWidget* parent) : QMainWindow(parent) {
  setWindowTitle("MAZE PROJECT");
  setMinimumSize(670, 500);
  CreateMainMenu();
}

void MazeWindow::CreateMainMenu() {
  QWidget* button_widget = new QWidget(this);
  QHBoxLayout* layout = new QHBoxLayout(button_widget);

  QPushButton* btn_maze = new QPushButton("Maze");
  QPushButton* btn_cave = new QPushButton("Cave");

  btn_maze->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  btn_cave->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  layout->addWidget(btn_maze);
  layout->addWidget(btn_cave);
  AdjustButtonFonts(btn_maze);
  AdjustButtonFonts(btn_cave);
  button_widget->setStyleSheet(MazeStyle::GetStyle());
  connect(btn_maze, &QPushButton::clicked, this,
          [this]() { ShowMazeWidget('m'); });
  connect(btn_cave, &QPushButton::clicked, this,
          [this]() { ShowMazeWidget('c'); });
  setCentralWidget(button_widget);
}

void MazeWindow::AdjustButtonFonts(QPushButton* button) {
  int base_width = this->width();
  int base_height = this->height();

  int font_size = qMin(base_height / 7, base_width / 7);

  QFont button_font = button->font();
  button_font.setPixelSize(font_size > 1 ? font_size : 1);
  button->setFont(button_font);
}

void MazeWindow::ShowMazeWidget(const char c) {
  QWidget* container = new QWidget(this);
  QVBoxLayout* layout = new QVBoxLayout(container);

  QWidget* content = nullptr;

  if (c == 'm') {
    MazeWidget* maze_widget = new MazeWidget(container);
    connect(maze_widget, &MazeWidget::BackRequested, this,
            &MazeWindow::CreateMainMenu);
    content = maze_widget;

  } else {
    CaveWidget* cave_widget = new CaveWidget(container);
    connect(cave_widget, &CaveWidget::BackRequested, this,
            &MazeWindow::CreateMainMenu);
    content = cave_widget;
  }
  content->setStyleSheet(MazeStyle::GetMenuStyle());
  layout->addWidget(content, 1);
  container->setLayout(layout);

  setCentralWidget(container);
}
