#include <QApplication>

#include "main_window.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  MazeWindow w;
  w.show();

  return app.exec();
}