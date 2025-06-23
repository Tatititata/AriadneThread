#ifndef DRAW_CAVE_H_
#define DRAW_CAVE_H_

#include <QPainter>
#include <QWidget>

#include "../model/maze/maze.h"

class CaveDrawWidget : public QWidget {
  Q_OBJECT
 public:
  explicit CaveDrawWidget(Maze* cave, QWidget* parent = nullptr);

 protected:
  void paintEvent(QPaintEvent*) override;

 private:
  Maze* m_pcave_;
};

#endif  // DRAW_CAVE_H_