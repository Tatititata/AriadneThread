#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QByteArray>
#include <QDateTime>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include "../model/maze/maze.h"

class TcpServer : public QWidget {
  Q_OBJECT

 public:
  explicit TcpServer(quint16 port, QWidget* parent = nullptr);
  ~TcpServer();

 private slots:
  void OnNewConnection();
  void OnReadyRead(QTcpSocket* client);
  void OnClientDisconnected(QTcpSocket* client_socket);

 private:
  bool ValidJson(QTcpSocket* client, const QJsonObject& obj);
  Cell GetPoint(const QJsonObject& obj, const QString& point);
  static bool ValidPoint(const Cell& point, const int& rows, const int& cols);
  void SendPassResponce(QTcpSocket* client, const std::vector<Cell>& pass);
  QString GetContentType(const QString& filePath);
  void ProceedPostRequest(QTcpSocket* client, const QString& path,
                          const QByteArray& body);
  void ProceedOptionRequest(QTcpSocket* client);
  void ProceedGetRequest(QTcpSocket* client, const QString& path);
  void SendGeneratedMaze(QTcpSocket* client, int rows, int cols);
  void ProceedGenerate(QTcpSocket* client, const QByteArray& body);
  void ProceedPath(QTcpSocket* client, const QByteArray& body);
  void SendHttpResponse(QTcpSocket* client, int statusCode,
                        const QString& statusText, const QByteArray& body,
                        const QString& contentType);

  QTcpServer* m_ptcp_server_;
  QTextEdit* m_ptxt_;
  QList<QTcpSocket*> m_clients_;
};

#endif  // TCPSERVER_H
