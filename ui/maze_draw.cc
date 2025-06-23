#include "maze_draw.h"

MazeDrawWidget::MazeDrawWidget(Maze* maze, QWidget* parent)
    : QWidget(parent), m_pmaze_(maze) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void MazeDrawWidget::SetMaze(Maze* maze) {
  m_pmaze_ = maze;
  update();
}

void MazeDrawWidget::paintEvent(QPaintEvent*) {
  if (!m_pmaze_) return;
  QPainter painter(this);

  int rows = m_pmaze_->GetRows();
  int cols = m_pmaze_->GetCols();
  int cell_width = width() / cols;
  int cell_height = height() / rows;

  painter.setPen(Qt::NoPen);
  painter.setBrush(Qt::white);
  painter.drawRect(0, 0, cols * cell_width, rows * cell_height);

  QPen pen(Qt::black);
  pen.setWidth(2);
  painter.setPen(pen);
  painter.setBrush(Qt::NoBrush);

  auto vWalls = m_pmaze_->GetVerticals();
  auto hWalls = m_pmaze_->GetHorizontals();

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols - 1; j++) {
      if (m_pmaze_->GetBit(vWalls[i], j)) {
        int x = (j + 1) * cell_width;
        painter.drawLine(x, i * cell_height, x, (i + 1) * cell_height);
      }
    }
  }

  for (int i = 0; i < rows - 1; i++) {
    for (int j = 0; j < cols; j++) {
      if (m_pmaze_->GetBit(hWalls[i], j)) {
        int y = (i + 1) * cell_height;
        painter.drawLine(j * cell_width, y, (j + 1) * cell_width, y);
      }
    }
  }
}
