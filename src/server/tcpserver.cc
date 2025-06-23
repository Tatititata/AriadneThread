#include "tcpserver.h"

TcpServer::TcpServer(quint16 port, QWidget* parent) : QWidget(parent) {
  m_ptcp_server_ = new QTcpServer(this);
  m_ptxt_ = new QTextEdit(this);
  m_ptxt_->setReadOnly(true);

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->addWidget(m_ptxt_);
  setLayout(layout);

  connect(m_ptcp_server_, &QTcpServer::newConnection, this,
          &TcpServer::OnNewConnection);

  if (!m_ptcp_server_->listen(QHostAddress::Any, port)) {
    m_ptxt_->append("Unable to start the server: " +
                    m_ptcp_server_->errorString());
    return;
  }
  m_ptxt_->append("Server started on port " + QString::number(port));
  setWindowTitle("TCP Server - Port " + QString::number(port));
  resize(500, 700);
}

TcpServer::~TcpServer() {
  for (QTcpSocket* client : m_clients_) {
    client->close();
    client->deleteLater();
  }
  m_clients_.clear();
  m_ptcp_server_->close();
}

void TcpServer::OnNewConnection() {
  while (m_ptcp_server_->hasPendingConnections()) {
    QTcpSocket* client_socket = m_ptcp_server_->nextPendingConnection();
    m_clients_.append(client_socket);

    connect(client_socket, &QTcpSocket::readyRead, this,
            [this, client_socket]() { OnReadyRead(client_socket); });
    connect(client_socket, &QTcpSocket::disconnected, this,
            [this, client_socket]() { OnClientDisconnected(client_socket); });

    m_ptxt_->append("New client connected from " +
                    client_socket->peerAddress().toString());
  }
}

void TcpServer::OnClientDisconnected(QTcpSocket* client_socket) {
  if (!client_socket) return;

  m_ptxt_->append("Client disconnected: " +
                  client_socket->peerAddress().toString());
  m_clients_.removeAll(client_socket);
  client_socket->deleteLater();
}

void TcpServer::OnReadyRead(QTcpSocket* client) {
  if (!client) return;

  QByteArray request_data = client->readAll();
  QString request(request_data);

  QStringList lines = request.split("\r\n");
  if (lines.isEmpty()) return;

  QStringList request_line = lines[0].split(' ');
  if (request_line.size() < 2) return;

  QString method = request_line[0];
  QString path = request_line[1];

  m_ptxt_->append(method + " " + path + " from " +
                  client->peerAddress().toString());
  m_ptxt_->append("Received " + QString::number(request_data.size()) +
                  " bytes from " + client->peerAddress().toString());

  int empty_line_idx = lines.indexOf("");
  QByteArray body;
  if (empty_line_idx != -1 && empty_line_idx + 1 < lines.size()) {
    body = lines.mid(empty_line_idx + 1).join("\r\n").toUtf8();
  }
  if (method == "GET") {
    ProceedGetRequest(client, path);
  } else if (method == "OPTIONS") {
    ProceedOptionRequest(client);
  } else if (method == "POST") {
    ProceedPostRequest(client, path, body);
    m_ptxt_->append("POST body: " + QString(body.left(100)));
  } else {
    SendHttpResponse(client, 405, "Method Not Allowed",
                     QByteArray("Only POST supported"), "text/plain");
  }
}

void TcpServer::ProceedGetRequest(QTcpSocket* client, const QString& path) {
  QString filePath = "server/web" + path;

  if (filePath.contains("..")) {
    SendHttpResponse(client, 403, "Forbidden", "Access denied", "text/plain");
    return;
  }
  if (path == "/") filePath = "server/web/index.html";

  QFile file(filePath);
  if (file.open(QIODevice::ReadOnly)) {
    QByteArray content = file.readAll();
    QString contentType = GetContentType(filePath);
    SendHttpResponse(client, 200, "OK", content, contentType);
  } else {
    SendHttpResponse(client, 404, "Not Found", "File not found", "text/plain");
  }
}
void TcpServer::ProceedOptionRequest(QTcpSocket* client) {
  m_ptxt_->append("OPTIONS request processed for " +
                  client->peerAddress().toString());
  QByteArray response;
  response.append("HTTP/1.1 204 No Content\r\n");
  response.append("Access-Control-Allow-Origin: *\r\n");
  response.append("Access-Control-Allow-Methods: POST, OPTIONS\r\n");
  response.append("Access-Control-Allow-Headers: Content-Type\r\n");
  response.append("Access-Control-Max-Age: 86400\r\n");
  response.append("Connection: close\r\n\r\n");
  client->write(response);
  client->flush();
  client->disconnectFromHost();
}

void TcpServer::ProceedPostRequest(QTcpSocket* client, const QString& path,
                                   const QByteArray& body) {
  m_ptxt_->append("POST to " + path + " from " +
                  client->peerAddress().toString());
  if (path == "/generate") {
    ProceedGenerate(client, body);
  } else if (path == "/pass") {
    ProceedPath(client, body);
  } else {
    SendHttpResponse(client, 404, "Not Found", QByteArray("Path not found"),
                     "text/plain");
    m_ptxt_->append("POST: Unknown path " + path);
  }
}

QString TcpServer::GetContentType(const QString& filePath) {
  if (filePath.endsWith(".html")) return "text/html";
  if (filePath.endsWith(".css")) return "text/css";
  if (filePath.endsWith(".js")) return "application/javascript";
  if (filePath.endsWith(".svg")) return "image/svg+xml";
  return "text/plain";
}

void TcpServer::ProceedGenerate(QTcpSocket* client, const QByteArray& body) {
  QJsonDocument doc = QJsonDocument::fromJson(body);
  if (!doc.isObject()) {
    SendHttpResponse(client, 400, "Bad Request", QByteArray("Invalid JSON"),
                     "text/plain");
    return;
  }
  QJsonObject obj = doc.object();

  if (!obj.contains("rows") || !obj.contains("cols")) {
    SendHttpResponse(client, 400, "Bad Request",
                     QByteArray("Missing rows or cols"), "text/plain");
    return;
  }

  int rows = obj.value("rows").toInt(-1);
  int cols = obj.value("cols").toInt(-1);
  m_ptxt_->append("Generating maze: rows=" + QString::number(rows) +
                  ", cols=" + QString::number(cols));

  if (rows <= 0 || cols <= 0 || rows > kMaxSize || cols > kMaxSize) {
    SendHttpResponse(client, 400, "Bad Request",
                     QByteArray("Invalid rows or cols"), "text/plain");
    m_ptxt_->append("Maze generation error: invalid parameters");

    return;
  }
  SendGeneratedMaze(client, rows, cols);
}

void TcpServer::SendGeneratedMaze(QTcpSocket* client, int rows, int cols) {
  Maze maze(rows, cols);
  maze.GenerateMaze();

  QJsonObject responseObj;
  QJsonArray verticals_array;
  QJsonArray horizontals_array;

  auto verticals = maze.GetVerticals();
  auto horizontals = maze.GetHorizontals();

  for (int i = 0; i < rows; ++i) {
    verticals_array.append(QString::number(verticals[i]));
    horizontals_array.append(QString::number(horizontals[i]));
  }

  responseObj["verticals"] = verticals_array;
  responseObj["horizontals"] = horizontals_array;

  QJsonDocument responseDoc(responseObj);
  QByteArray responseData = responseDoc.toJson();

  SendHttpResponse(client, 200, "OK", responseData, "application/json");
  m_ptxt_->append("Maze sent to " + client->peerAddress().toString());
}

void TcpServer::ProceedPath(QTcpSocket* client, const QByteArray& body) {
  QJsonDocument doc = QJsonDocument::fromJson(body);
  if (!doc.isObject()) {
    SendHttpResponse(client, 400, "Bad Request", QByteArray("Invalid JSON"),
                     "text/plain");
    return;
  }
  QJsonObject obj = doc.object();
  if (!ValidJson(client, obj)) return;

  int rows = obj.value("rows").toInt(-1);
  int cols = obj.value("cols").toInt(-1);
  if (rows <= 0 || cols <= 0 || rows > kMaxSize || cols > kMaxSize) {
    SendHttpResponse(client, 400, "Bad Request",
                     QByteArray("Invalid rows or cols"), "text/plain");
    return;
  }

  auto start = GetPoint(obj, "start");
  auto end = GetPoint(obj, "end");
  m_ptxt_->append("Path request: start=(" + QString::number(start.r) + "," +
                  QString::number(start.c) + "), end=(" +
                  QString::number(end.r) + "," + QString::number(end.c) + ")");

  if (!ValidPoint(start, rows, cols) || !ValidPoint(end, rows, cols)) {
    SendHttpResponse(client, 400, "Bad Request",
                     QByteArray("Invalid start or end"), "text/plain");
    return;
  }

  QJsonArray verticals_array = obj.value("verticals").toArray();
  QJsonArray horizontals_array = obj.value("horizontals").toArray();
  if (verticals_array.size() < rows || horizontals_array.size() < rows) {
    SendHttpResponse(client, 400, "Bad Request",
                     QByteArray("Invalid walls arrays"), "text/plain");
    return;
  }

  std::array<uint64_t, kMaxSize> verticals{};
  std::array<uint64_t, kMaxSize> horizontals{};

  for (int i = 0; i < rows; ++i) {
    bool ok1 = false, ok2 = false;
    uint64_t v = verticals_array[i].toString().toULongLong(&ok1);
    uint64_t h = horizontals_array[i].toString().toULongLong(&ok2);
    if (!ok1 || !ok2) {
      SendHttpResponse(client, 400, "Bad Request",
                       QByteArray("Invalid wall data"), "text/plain");
      return;
    }
    verticals[i] = v;
    horizontals[i] = h;
  }

  Maze maze(rows, cols);
  maze.SetVerticals(verticals);
  maze.SetHorizontals(horizontals);

  auto pass = maze.SolveMaze(start, end);
  SendPassResponce(client, pass);
  m_ptxt_->append("Path found, length: " + QString::number(pass.size()));
}

Cell TcpServer::GetPoint(const QJsonObject& obj, const QString& point) {
  QJsonArray p = obj.value(point).toArray();
  if (p.size() != 2) {
    return {-1, -1};
  }
  return {p[0].toInt(-1), p[1].toInt(-1)};
}

void TcpServer::SendPassResponce(QTcpSocket* client,
                                 const std::vector<Cell>& pass) {
  if (!pass.size()) {
    SendHttpResponse(client, 404, "Not Found", QByteArray("Path not found"),
                     "text/plain");
    return;
  }

  QJsonArray passArray;
  for (const auto& p : pass) {
    QJsonArray coord;
    coord.append(p.r);
    coord.append(p.c);
    passArray.append(coord);
  }

  QJsonObject responseObj;
  responseObj["pass"] = passArray;

  QJsonDocument responseDoc(responseObj);
  QByteArray responseData = responseDoc.toJson();

  SendHttpResponse(client, 200, "OK", responseData, "application/json");
}

void TcpServer::SendHttpResponse(QTcpSocket* client, int statusCode,
                                 const QString& statusText,
                                 const QByteArray& body,
                                 const QString& contentType) {
  m_ptxt_->append("Sent response: " + QString::number(statusCode) + " " +
                  statusText);

  QByteArray response;
  response.append("HTTP/1.1 " + QByteArray::number(statusCode) + " " +
                  statusText.toUtf8() + "\r\n");
  response.append("Content-Type: " + contentType.toUtf8() + "\r\n");
  response.append("Content-Length: " + QByteArray::number(body.size()) +
                  "\r\n");
  response.append("Access-Control-Allow-Origin: *\r\n");
  response.append("Connection: close\r\n");
  response.append("\r\n");
  response.append(body);

  client->write(response);
  client->flush();
  client->disconnectFromHost();
}

bool TcpServer::ValidJson(QTcpSocket* client, const QJsonObject& obj) {
  if (!obj.contains("rows") || !obj.contains("cols") ||
      !obj.contains("start") || !obj.contains("end") ||
      !obj.contains("verticals") || !obj.contains("horizontals")) {
    SendHttpResponse(client, 400, "Bad Request",
                     QByteArray("Missing parameters"), "text/plain");
    return false;
  }
  return true;
}

bool TcpServer::ValidPoint(const Cell& point, const int& rows,
                           const int& cols) {
  return point.r >= 0 && point.r < rows && point.c >= 0 && point.c < cols;
}