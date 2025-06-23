#ifndef DRAW_BONUS_H_
#define DRAW_BONUS_H_

#include <QPainter>
#include <QTimer>
#include <QWidget>

#include "../model/maze/maze.h"

/**
 * @brief The BonusDrawWidget class provides visualization of maze solution
 * paths.
 *
 * This widget animates the progression through multiple solution paths in a
 * maze, showing each step with a color gradient from blue (start) to green to
 * red (end).
 */
class BonusDrawWidget : public QWidget {
  Q_OBJECT
 public:
  /**
   * @brief Constructs a BonusDrawWidget.
   * @param parent The parent widget (optional).
   */
  explicit BonusDrawWidget(QWidget* parent = nullptr);

  /**
   * @brief Sets the solution paths to be visualized.
   * @param solution Vector of solution paths where each path is a vector of
   * Cells.
   * @param rows Number of rows in the maze.
   * @param cols Number of columns in the maze.
   */
  void SetSolution(const std::vector<std::vector<Cell>>& solution, int rows,
                   int cols);

 protected:
  /**
   * @brief Handles paint events for the widget.
   * @param event The paint event (unused).
   */
  void paintEvent(QPaintEvent*) override;

 private slots:
  /**
   * @brief Advances the animation to the next step.
   */
  void NextStep();

 private:
  /**
   * @brief Draws a single point in the maze.
   * @param painter The QPainter to use for drawing.
   * @param point The cell coordinates to draw.
   * @param cell_width Width of a single maze cell in pixels.
   * @param cell_height Height of a single maze cell in pixels.
   * @param size Size of the point to draw (unused in current implementation).
   * @param color The color to use for drawing.
   */
  void DrawPoint(QPainter& painter, const Cell& point, int cell_width,
                 int cell_height, int size, const QColor& color);

  int m_rows_;  ///< Number of rows in the maze.
  int m_cols_;  ///< Number of columns in the maze.
  std::vector<std::vector<Cell>>
      m_solution_;      ///< Collection of solution paths to visualize.
  int m_current_step_;  ///< Current animation step index.
};

#endif  // DRAW_BONUS_H_