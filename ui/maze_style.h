#ifndef STYLE_H_
#define STYLE_H_

#include <QString>

class MazeStyle {
 public:
  static QString GetStyle() {
    return R"(
            QPushButton {
                background-color:rgb(117, 55, 233);
                color: white;
                font-weight: bold;
                border-radius: 5px;
                padding: 20px 20px;
            }
            QPushButton:hover {
                background-color:rgb(146, 82, 241);
            }
            QPushButton:pressed {
                background-color:rgb(56, 2, 127);
            }
        )";
  }

  static QString GetMenuStyle() {
    return R"(
            QPushButton {
                background-color:rgb(117, 55, 233);
                color: white;
                font-weight: normal;
                border-radius: 5px;
                padding: 5px;
            }
            QPushButton:hover {
                background-color:rgb(146, 82, 241);
            }
            QPushButton:pressed {
                background-color:rgb(56, 2, 127);
            }
        )";
  }

  static QString GetProgressStyle() {
    return R"(
            QProgressBar {
                border-radius: 5px;
                background-color: rgb(117, 55, 233);
                color: green;
                padding: 5px;
                text-align: center;
            }             
            QProgressBar::chunk {
                background-color: white;
                border-radius: 5px;
            }
        )";
  }
};

#endif
