#ifndef CAVE_WIDGET_H_
#define CAVE_WIDGET_H_

#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QStackedLayout>
#include <QStackedWidget>
#include <QTabWidget>
#include <QTimer>
#include <QToolBox>
#include <QWidget>

#include "../model/maze/maze.h"
#include "cave_draw.h"
#include "loader.h"
#include "maze_style.h"

class CaveWidget : public QWidget {
  Q_OBJECT
 public:
  explicit CaveWidget(QWidget* parent = nullptr);

 signals:
  void BackRequested();
  void CaveSizeChanged();
  void SolvingStarted(int birth, int death);
  void SwitchStackedPage(int index);

 private:
  Maze* m_pcave_;
  double m_chance_;
  int m_steps_;
  int m_current_step_;
  QSpinBox* m_birth_;
  QSpinBox* m_death_;
  CaveDrawWidget* m_pcave_view_;

  QWidget* CreateSideMenu();
  QGroupBox* CreateSizeMenu(QWidget* menu);
  QGroupBox* CreateSolveMenu(QWidget* menu);
  QHBoxLayout* CreateButtonsLayout();
  QWidget* CreateParamsMenu(QGroupBox* solve_box);
  QWidget* CreateControlMenu(QGroupBox* solve_box);
  QWidget* CreateManualTab(QWidget* parent);
  QWidget* CreateAutoTab(QWidget* parent);
  QWidget* CreatePageSwitcher(QWidget* parent);
};

#endif
