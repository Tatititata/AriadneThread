#ifndef DRAW_PATH_H_
#define DRAW_PATH_H_

#include <QPainter>
#include <QTimer>
#include <QWidget>

#include "../model/maze/maze.h"

class PathDrawWidget : public QWidget {
  Q_OBJECT
 public:
  PathDrawWidget(int rows, int cols, QWidget* parent = nullptr);
  void SetSolution(const std::vector<Cell>& solution, int rows, int cols);

 protected:
  void paintEvent(QPaintEvent*) override;

 private slots:
  void NextStep();

 private:
  int m_rows_;
  int m_cols_;
  std::vector<Cell> m_solution_;
  int m_current_step_;

  void DrawPoint(QPainter& painter, const Cell& point, int cell_width,
                 int cell_height, int size, const QColor& color);
};

#endif
