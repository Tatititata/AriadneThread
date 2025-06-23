#ifndef MAZE_WINDOW_H_
#define MAZE_WINDOW_H_

#include <QMainWindow>
#include <QPushButton>

#include "cave_widget.h"
#include "maze_style.h"
#include "maze_widget.h"

/**
 * @class MazeWindow
 * @brief Main window class for the Maze Project application.
 *
 * Provides the main menu with options to select Maze or Cave views,
 * and manages switching between different widgets.
 */
class MazeWindow : public QMainWindow {
  Q_OBJECT

 public:
  /**
   * @brief Constructs the MazeWindow.
   * @param parent Optional parent widget.
   */
  explicit MazeWindow(QWidget* parent = nullptr);

 private:
  /**
   * @brief Creates the main menu with buttons for Maze and Cave.
   */
  void CreateMainMenu();

  /**
   * @brief Shows the MazeWidget or CaveWidget depending on the selected mode.
   * @param c Character indicating mode: 'm' for Maze, 'c' for Cave.
   */
  void ShowMazeWidget(const char c);

  /**
   * @brief Adjusts the font size of a given button based on the window size.
   * @param button Pointer to the QPushButton to adjust.
   */
  void AdjustButtonFonts(QPushButton* button);
};

#endif  // MAZE_WINDOW_H_
