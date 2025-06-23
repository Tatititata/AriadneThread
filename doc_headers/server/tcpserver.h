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

/**
 * @brief The TcpServer class provides a TCP server implementation with
 * HTTP-like functionality.
 *
 * This class handles incoming TCP connections, processes HTTP-like requests
 * (GET, POST, OPTIONS), and serves responses. It's designed to work with maze
 * generation and pathfinding operations.
 */
class TcpServer : public QWidget {
  Q_OBJECT

 public:
  /**
   * @brief Constructs a TCP server listening on the specified port.
   * @param port The port number to listen on.
   * @param parent The parent QWidget (optional).
   */
  explicit TcpServer(quint16 port, QWidget* parent = nullptr);

  /**
   * @brief Destructor that cleans up all client connections and server
   * resources.
   */
  ~TcpServer();

 private slots:
  /**
   * @brief Handles new incoming connections.
   */
  void OnNewConnection();

  /**
   * @brief Processes data received from a client.
   * @param client The socket representing the client connection.
   */
  void OnReadyRead(QTcpSocket* client);

  /**
   * @brief Handles client disconnection.
   * @param client_socket The socket representing the disconnected client.
   */
  void OnClientDisconnected(QTcpSocket* client_socket);

 private:
  /**
   * @brief Validates JSON request structure for pathfinding requests.
   * @param client The client socket for sending error responses.
   * @param obj The JSON object to validate.
   * @return true if JSON is valid, false otherwise.
   */
  bool ValidJson(QTcpSocket* client, const QJsonObject& obj);

  /**
   * @brief Extracts a cell coordinate from JSON object.
   * @param obj The JSON object containing the point.
   * @param point The key name for the point in the JSON object.
   * @return Cell structure with coordinates.
   */
  Cell GetPoint(const QJsonObject& obj, const QString& point);

  /**
   * @brief Validates if a point is within maze boundaries.
   * @param point The cell coordinates to validate.
   * @param rows The number of rows in the maze.
   * @param cols The number of columns in the maze.
   * @return true if point is valid, false otherwise.
   */
  static bool ValidPoint(const Cell& point, const int& rows, const int& cols);

  /**
   * @brief Sends pathfinding solution to client.
   * @param client The client socket to send response to.
   * @param pass The solution path as vector of cells.
   */
  void SendPassResponce(QTcpSocket* client, const std::vector<Cell>& pass);

  /**
   * @brief Determines content type based on file extension.
   * @param filePath The path to the file.
   * @return The MIME content type as string.
   */
  QString GetContentType(const QString& filePath);

  /**
   * @brief Processes POST requests.
   * @param client The client socket.
   * @param path The request path.
   * @param body The request body.
   */
  void ProceedPostRequest(QTcpSocket* client, const QString& path,
                          const QByteArray& body);

  /**
   * @brief Processes OPTIONS requests (for CORS).
   * @param client The client socket.
   */
  void ProceedOptionRequest(QTcpSocket* client);

  /**
   * @brief Processes GET requests (serves static files).
   * @param client The client socket.
   * @param path The request path.
   */
  void ProceedGetRequest(QTcpSocket* client, const QString& path);

  /**
   * @brief Generates and sends a new maze to client.
   * @param client The client socket.
   * @param rows Number of rows in the maze.
   * @param cols Number of columns in the maze.
   */
  void SendGeneratedMaze(QTcpSocket* client, int rows, int cols);

  /**
   * @brief Processes maze generation requests.
   * @param client The client socket.
   * @param body The request body containing generation parameters.
   */
  void ProceedGenerate(QTcpSocket* client, const QByteArray& body);

  /**
   * @brief Processes pathfinding requests.
   * @param client The client socket.
   * @param body The request body containing maze and path parameters.
   */
  void ProceedPath(QTcpSocket* client, const QByteArray& body);

  /**
   * @brief Sends an HTTP response to the client.
   * @param client The client socket.
   * @param statusCode HTTP status code.
   * @param statusText HTTP status text.
   * @param body Response body.
   * @param contentType Response content type.
   */
  void SendHttpResponse(QTcpSocket* client, int statusCode,
                        const QString& statusText, const QByteArray& body,
                        const QString& contentType);

  QTcpServer* m_ptcp_server_;     ///< The TCP server instance.
  QTextEdit* m_ptxt_;             ///< Text edit for logging server activity.
  QList<QTcpSocket*> m_clients_;  ///< List of connected client sockets.
};

#endif  // TCPSERVER_H