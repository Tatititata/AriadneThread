#ifndef DRAW_MAZE_H_
#define DRAW_MAZE_H_

#include <QWidget>

#include "../model/maze/maze.h"

/**
 * @class MazeDrawWidget
 * @brief Widget for visualizing a Maze object.
 *
 * Draws the maze grid with walls using Qt painting system.
 */
class MazeDrawWidget : public QWidget {
  Q_OBJECT

 public:
  /**
   * @brief Constructs the MazeDrawWidget.
   * @param maze Pointer to the Maze object to visualize.
   * @param parent Optional parent widget.
   */
  explicit MazeDrawWidget(Maze* maze, QWidget* parent = nullptr);

  /**
   * @brief Sets the Maze object to visualize.
   * @param maze Pointer to the Maze.
   */
  void SetMaze(Maze* maze);

 protected:
  /**
   * @brief Handles the paint event to draw the maze.
   * @param event Paint event pointer (unused).
   */
  void paintEvent(QPaintEvent* event) override;

 private:
  Maze* m_pmaze_;  ///< Pointer to the Maze to draw.
};

#endif  // DRAW_MAZE_H_
