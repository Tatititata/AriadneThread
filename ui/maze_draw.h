#ifndef DRAW_MAZE_H_
#define DRAW_MAZE_H_

#include <QPainter>
#include <QWidget>

#include "../model/maze/maze.h"

class MazeDrawWidget : public QWidget {
  Q_OBJECT
 public:
  explicit MazeDrawWidget(Maze* maze, QWidget* parent = nullptr);
  void SetMaze(Maze* maze);

 protected:
  void paintEvent(QPaintEvent*) override;

 private:
  Maze* m_pmaze_;
};

#endif  // DRAW_MAZE_H_