#ifndef STYLE_H_
#define STYLE_H_

#include <QString>
/**
 * @class MazeStyle
 * @brief Provides CSS styles for various UI elements in the Maze application.
 *
 * Contains static methods returning style sheets for buttons, menus, and
 * progress bars.
 */
class MazeStyle {
 public:
  /**
   * @brief Returns the main style for QPushButton.
   * @return CSS style string.
   */
  static QString GetStyle();

  /**
   * @brief Returns the style for menu buttons.
   * @return CSS style string.
   */
  static QString GetMenuStyle();

  /**
   * @brief Returns the style for QProgressBar.
   * @return CSS style string.
   */
  static QString GetProgressStyle();
};
#endif