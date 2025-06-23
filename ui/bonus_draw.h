#ifndef DRAW_BONUS_H_
#define DRAW_BONUS_H_

#include <QPainter>
#include <QTimer>
#include <QWidget>

#include "../model/maze/maze.h"

class BonusDrawWidget : public QWidget {
  Q_OBJECT
 public:
  explicit BonusDrawWidget(QWidget* parent = nullptr);
  void SetSolution(const std::vector<std::vector<Cell>>& solution, int rows,
                   int cols);

 protected:
  void paintEvent(QPaintEvent*) override;

 private slots:
  void NextStep();

 private:
  void DrawPoint(QPainter& painter, const Cell& point, int cell_width,
                 int cell_height, int size, const QColor& color);
  int m_rows_;
  int m_cols_;
  std::vector<std::vector<Cell>> m_solution_;
  int m_current_step_;
};

#endif  // DRAW_BONUS_H_