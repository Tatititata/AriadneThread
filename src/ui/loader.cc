
#include "loader.h"

bool MazeFileLoader::LoadMazeFromFile(QWidget* parent, Maze* maze, char c) {
  if (!maze) return false;
  QString fileName = QFileDialog::getOpenFileName(
      parent, "Open Maze File", "", "Text Files (*.txt);;All Files (*)");

  if (fileName.isEmpty()) return false;

  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QMessageBox::warning(parent, "Error", "Cannot open file for reading");
    return false;
  }

  QTextStream in(&file);
  std::stringstream ss;

  while (!in.atEnd()) {
    ss << in.readLine().toStdString() << "\n";
  }
  file.close();

  Maze tmp;
  if (tmp.Load(ss, c)) {
    *maze = tmp;
    return true;
  } else {
    QMessageBox::warning(parent, "Error", "Failed to load maze from file");
    return false;
  }
}

bool MazeFileLoader::SaveMazeToFile(QWidget* parent, const Maze* maze, char c) {
  if (!maze) return false;

  QString file_name = QFileDialog::getSaveFileName(
      parent, "Save Maze File",
      QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
      "Text Files (*.txt);;All Files (*)");
  if (file_name.isEmpty()) return false;

  QFileInfo fileInfo(file_name);
  if (fileInfo.suffix().isEmpty()) {
    file_name += ".txt";
  }

  QFile file(file_name);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QMessageBox::warning(parent, "Error", "Cannot open file for writing");
    return false;
  }

  QTextStream out(&file);
  std::stringstream ss;

  if (!maze->Save(ss, c)) {
    QMessageBox::warning(parent, "Error", "Failed to save maze to file");
    file.close();
    return false;
  }

  out << QString::fromStdString(ss.str());
  file.close();
  return true;
}