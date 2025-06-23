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

/**
 * @brief The CaveWidget class provides a complete interface for cave generation
 * and simulation.
 *
 * This widget combines visualization (CaveDrawWidget) with controls for:
 * - Setting cave dimensions and generation parameters
 * - Configuring cellular automata rules (birth/death limits)
 * - Manual and automatic simulation stepping
 * - Loading/saving cave configurations
 */
class CaveWidget : public QWidget {
  Q_OBJECT
 public:
  /**
   * @brief Constructs a CaveWidget with default parameters.
   * @param parent The parent widget (optional).
   *
   * Initializes a cave with default size (kMaxSize/2) and random generation.
   */
  explicit CaveWidget(QWidget* parent = nullptr);

 signals:
  /**
   * @brief Emitted when the back button is pressed.
   */
  void BackRequested();

  /**
   * @brief Emitted when cave dimensions are changed.
   */
  void CaveSizeChanged();

  /**
   * @brief Emitted when cave simulation starts.
   * @param birth Birth limit parameter
   * @param death Death limit parameter
   */
  void SolvingStarted(int birth, int death);

  /**
   * @brief Emitted to switch between manual/auto simulation tabs.
   * @param index Tab index to switch to (0 = manual, 1 = auto)
   */
  void SwitchStackedPage(int index);

 private:
  Maze* m_pcave_;                 ///< The cave model being visualized
  double m_chance_;               ///< Initial cell spawn probability (0.0-1.0)
  QSpinBox* m_birth_;             ///< Control for birth limit parameter
  QSpinBox* m_death_;             ///< Control for death limit parameter
  CaveDrawWidget* m_pcave_view_;  ///< Visualization widget for the cave

  /**
   * @brief Creates the side control panel.
   * @return Configured widget containing all controls.
   */
  QWidget* CreateSideMenu();

  /**
   * @brief Creates the cave size configuration group.
   * @param menu Parent widget for the group.
   * @return Configured group box with size controls.
   */
  QGroupBox* CreateSizeMenu(QWidget* menu);

  /**
   * @brief Creates the simulation control group.
   * @param menu Parent widget for the group.
   * @return Configured group box with simulation controls.
   */
  QGroupBox* CreateSolveMenu(QWidget* menu);

  /**
   * @brief Creates the button row (back/load/save).
   * @return Configured layout with action buttons.
   */
  QHBoxLayout* CreateButtonsLayout();

  /**
   * @brief Creates the simulation parameters panel.
   * @param solve_box Parent widget for the panel.
   * @return Configured widget with birth/death parameters.
   */
  QWidget* CreateParamsMenu(QGroupBox* solve_box);

  /**
   * @brief Creates the simulation control panel.
   * @param solve_box Parent widget for the panel.
   * @return Configured widget with manual/auto controls.
   */
  QWidget* CreateControlMenu(QGroupBox* solve_box);

  /**
   * @brief Creates the manual simulation tab.
   * @param parent Parent widget for the tab.
   * @return Configured manual control widget.
   */
  QWidget* CreateManualTab(QWidget* parent);

  /**
   * @brief Creates the automatic simulation tab.
   * @param parent Parent widget for the tab.
   * @return Configured auto control widget.
   */
  QWidget* CreateAutoTab(QWidget* parent);

  /**
   * @brief Creates the tab switcher for manual/auto modes.
   * @param parent Parent widget for the switcher.
   * @return Configured toolbox widget.
   */
  QWidget* CreatePageSwitcher(QWidget* parent);
};

#endif