#include "path_draw.h"

PathDrawWidget::PathDrawWidget(int rows, int cols, QWidget* parent)
    : QWidget(parent), m_rows_(rows), m_cols_(cols) {
  setAttribute(Qt::WA_TranslucentBackground);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_current_step_ = 0;
}

void PathDrawWidget::SetSolution(const std::vector<Cell>& solution, int rows,
                                 int cols) {
  m_rows_ = rows;
  m_cols_ = cols;
  m_solution_ = solution;
  m_current_step_ = 0;
  update();
  if (!m_solution_.empty())
    QTimer::singleShot(25, this, &PathDrawWidget::NextStep);
}

void PathDrawWidget::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  painter.fillRect(rect(), Qt::transparent);
  if (m_solution_.empty()) return;

  int cell_width = width() / m_cols_;
  int cell_height = height() / m_rows_;
  int size = std::min(cell_width, cell_height) * 0.4;

  DrawPoint(painter, m_solution_.front(), cell_width, cell_height, size,
            Qt::green);
  DrawPoint(painter, m_solution_.back(), cell_width, cell_height, size,
            Qt::red);

  for (int i = 1; i < m_current_step_ && i < (int)m_solution_.size() - 1; ++i) {
    DrawPoint(painter, m_solution_[i], cell_width, cell_height, size,
              QColor(255, 0, 0, 255));
  }
}

void PathDrawWidget::NextStep() {
  if (m_current_step_ < (int)m_solution_.size()) {
    ++m_current_step_;
    update();
    if (m_current_step_ < (int)m_solution_.size())
      QTimer::singleShot(50, this, &PathDrawWidget::NextStep);
  }
}

void PathDrawWidget::DrawPoint(QPainter& painter, const Cell& point,
                               int cell_width, int cell_height, int size,
                               const QColor& color) {
  qreal x = (point.c + 0.5) * cell_width - size / 2;
  qreal y = (point.r + 0.5) * cell_height - size / 2;
  painter.setPen(Qt::NoPen);
  painter.setBrush(color);
  painter.drawEllipse(QRectF(x, y, size, size));
}
