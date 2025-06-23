#ifndef LOADER_H_
#define LOADER_H_

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QString>

#include "../model/maze/maze.h"

class MazeFileLoader {
 public:
  static bool LoadMazeFromFile(QWidget* parent, Maze* maze, char c);
  static bool SaveMazeToFile(QWidget* parent, const Maze* maze, char c);
};

#endif