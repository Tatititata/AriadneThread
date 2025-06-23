#include "bonus_draw.h"

BonusDrawWidget::BonusDrawWidget(QWidget* parent)
    : QWidget(parent),
      m_rows_(),
      m_cols_(),
      m_solution_({}),
      m_current_step_() {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void BonusDrawWidget::paintEvent(QPaintEvent*) {
  if (m_solution_.empty()) return;
  QPainter painter(this);

  int cell_width = width() / m_cols_;
  int cell_height = height() / m_rows_;
  int size = std::min(cell_width, cell_height) * 0.3;
  float step = 1.0f / (float)m_solution_.size();
  for (int i = 0; i < m_current_step_; ++i) {
    float ratio = i * step;
    QColor heat_color;

    if (ratio < 0.5) {
      heat_color.setRgbF(0, 2 * ratio, 1 - 2 * ratio);
    } else {
      heat_color.setRgbF(2 * (ratio - 0.5), 1 - 2 * (ratio - 0.5), 0);
    }
    heat_color.setAlphaF(0.5);
    for (auto point : m_solution_[i]) {
      DrawPoint(painter, point, cell_width, cell_height, size, heat_color);
    }
  }
}

void BonusDrawWidget::NextStep() {
  if (m_current_step_ < (int)m_solution_.size()) {
    ++m_current_step_;
    update();
    if (m_current_step_ < (int)m_solution_.size())
      QTimer::singleShot(30, this, &BonusDrawWidget::NextStep);
  }
}

void BonusDrawWidget::DrawPoint(QPainter& painter, const Cell& point,
                                int cell_width, int cell_height, int /*size*/,
                                const QColor& color) {
  int x = point.c * cell_width;
  int y = point.r * cell_height;
  painter.setPen(Qt::NoPen);
  painter.setBrush(color);
  painter.drawRect(x, y, cell_width, cell_height);
}

void BonusDrawWidget::SetSolution(
    const std::vector<std::vector<Cell>>& solution, int rows, int cols) {
  m_rows_ = rows;
  m_cols_ = cols;
  m_solution_ = solution;
  m_current_step_ = 0;
  update();
  if (!m_solution_.empty())
    QTimer::singleShot(25, this, &BonusDrawWidget::NextStep);
}