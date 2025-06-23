#ifndef DRAW_CAVE_H_
#define DRAW_CAVE_H_

#include <QPainter>
#include <QWidget>

#include "../model/maze/maze.h"

/**
 * @brief The CaveDrawWidget class provides visualization for a cave structure.
 *
 * This widget renders a cave represented by a Maze object, displaying live
 * cells as black rectangles and dead cells as white space. The visualization
 * automatically scales to fit the available widget space.
 */
class CaveDrawWidget : public QWidget {
  Q_OBJECT
 public:
  /**
   * @brief Constructs a CaveDrawWidget for the specified cave.
   * @param cave Pointer to the Maze object representing the cave to visualize.
   * @param parent The parent widget (optional).
   *
   * @note The CaveDrawWidget does not take ownership of the Maze object.
   * The caller is responsible for ensuring the Maze remains valid during the
   * widget's lifetime.
   */
  explicit CaveDrawWidget(Maze* cave, QWidget* parent = nullptr);

 protected:
  /**
   * @brief Handles paint events for the widget.
   * @param event The paint event (unused).
   *
   * Renders the cave by drawing each live cell (bit set to 1) as a black
   * rectangle and dead cells (bit set to 0) as white space. The cells are
   * scaled proportionally to fit the current widget dimensions.
   */
  void paintEvent(QPaintEvent*) override;

 private:
  Maze* m_pcave_;  ///< Pointer to the Maze object representing the cave to
                   ///< visualize.
};

#endif  // DRAW_CAVE_H_