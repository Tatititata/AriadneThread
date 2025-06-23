#include <QtWidgets>

#include "tcpserver.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  TcpServer server(8080);
  server.show();
  return app.exec();
}
