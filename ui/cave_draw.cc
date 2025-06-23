#include "cave_draw.h"

CaveDrawWidget::CaveDrawWidget(Maze* cave, QWidget* parent)
    : QWidget(parent), m_pcave_(cave) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void CaveDrawWidget::paintEvent(QPaintEvent*) {
  if (!m_pcave_) return;
  QPainter painter(this);

  int rows = m_pcave_->GetRows();
  int cols = m_pcave_->GetCols();

  int cell_width = width() / cols;
  int cell_height = height() / rows;

  if (cell_width < 1) cell_width = 1;
  if (cell_height < 1) cell_height = 1;

  painter.setBrush(QBrush(Qt::black));
  painter.setPen(Qt::NoPen);
  auto matrix = m_pcave_->GetVerticals();
  painter.fillRect(rect(), Qt::white);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (m_pcave_->GetBit(matrix[i], j)) {
        int x = j * cell_width;
        int y = i * cell_height;
        painter.fillRect(x, y, cell_width, cell_height, Qt::black);
      }
    }
  }
}
