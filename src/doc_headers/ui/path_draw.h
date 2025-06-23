#ifndef DRAW_PATH_H_
#define DRAW_PATH_H_

#include <QColor>
#include <QWidget>
#include <vector>

#include "../model/maze/maze.h"

/**
 * @class PathDrawWidget
 * @brief Widget for animating and displaying a solution path on a maze.
 *
 * Draws the path step-by-step with a timer, highlighting start and end points.
 */
class PathDrawWidget : public QWidget {
  Q_OBJECT

 public:
  /**
   * @brief Constructs the PathDrawWidget.
   * @param rows Number of rows in the maze grid.
   * @param cols Number of columns in the maze grid.
   * @param parent Optional parent widget.
   */
  PathDrawWidget(int rows, int cols, QWidget* parent = nullptr);

  /**
   * @brief Sets the solution path to display and resets animation.
   * @param solution Vector of cells representing the solution path.
   * @param rows Number of rows in the maze grid.
   * @param cols Number of columns in the maze grid.
   */
  void SetSolution(const std::vector<Cell>& solution, int rows, int cols);

 protected:
  /**
   * @brief Paint event handler to draw the current state of the solution path.
   * @param event Paint event pointer (unused).
   */
  void paintEvent(QPaintEvent* event) override;

 private slots:
  /**
   * @brief Advances the animation to the next step of the path.
   */
  void NextStep();

 private:
  int m_rows_;                    ///< Number of rows in the maze.
  int m_cols_;                    ///< Number of columns in the maze.
  std::vector<Cell> m_solution_;  ///< Cells representing the solution path.
  int m_current_step_;            ///< Current step index in the animation.

  /**
   * @brief Draws a single point (ellipse) representing a cell in the path.
   * @param painter QPainter object used for drawing.
   * @param point Cell to draw.
   * @param cell_width Width of one maze cell.
   * @param cell_height Height of one maze cell.
   * @param size Diameter of the ellipse to draw.
   * @param color Color of the ellipse.
   */
  void DrawPoint(QPainter& painter, const Cell& point, int cell_width,
                 int cell_height, int size, const QColor& color);
};

#endif  // DRAW_PATH_H_
