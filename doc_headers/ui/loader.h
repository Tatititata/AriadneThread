#ifndef LOADER_H_
#define LOADER_H_

#include <QWidget>

#include "../model/maze/maze.h"

/**
 * @class MazeFileLoader
 * @brief Provides static methods to load and save Maze objects using file
 * dialogs.
 *
 * This class handles opening file dialogs for selecting maze files and manages
 * reading from and writing to files, including error handling via message
 * boxes.
 */
class MazeFileLoader {
 public:
  /**
   * @brief Opens a file dialog to select a maze file and loads the maze from
   * it.
   * @param parent Pointer to the parent QWidget for dialog modality.
   * @param maze Pointer to the Maze object to load data into.
   * @param c Character flag indicating the loading mode (e.g., 'c' or 'm').
   * @return true if the maze was successfully loaded; false otherwise.
   */
  static bool LoadMazeFromFile(QWidget* parent, Maze* maze, char c);

  /**
   * @brief Opens a file dialog to select a location and saves the maze to a
   * file.
   * @param parent Pointer to the parent QWidget for dialog modality.
   * @param maze Pointer to the Maze object to save.
   * @param c Character flag indicating the saving mode (e.g., 'c' or 'm').
   * @return true if the maze was successfully saved; false otherwise.
   */
  static bool SaveMazeToFile(QWidget* parent, const Maze* maze, char c);
};

#endif  // LOADER_H_
