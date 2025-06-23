#ifndef MAZE_WINDOW_H_
#define MAZE_WINDOW_H_

#include <QApplication>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QResizeEvent>
#include <QWidget>

#include "cave_widget.h"
#include "maze_style.h"
#include "maze_widget.h"

class MazeWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MazeWindow(QWidget* parent = nullptr);

 private:
  void CreateMainMenu();
  void ShowMazeWidget(const char c);
  void AdjustButtonFonts(QPushButton* button);
};

#endif