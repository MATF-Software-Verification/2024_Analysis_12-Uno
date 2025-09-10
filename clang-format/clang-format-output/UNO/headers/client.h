#ifndef CLIENT_H
#define CLIENT_H

#include "headers/game.h"

#include <sys/socket.h>

#include <QString>
#include <QTcpSocket>

#define MAX_NAME_LENGTH (25)
#define MAX_MESSAGE_LENGTH (350)
#define MESSAGE_WAIT_TIME (1)

class Client : public QThread
{
    Q_OBJECT
  public:
    Client(QObject* parent = nullptr);

    Client(QObject* parent, QString address, QString name, bool isHost);

    ~Client();

    bool isHost();

    // creates a socket and connects to server
    // on the given address, and sends client's name
    void run();

    // returns a message sent from server
    QString getMessage();

    // sends turn information
    void sendMessage(QString message);

    int sockfd;

    QVector<QString> names;

    int totalNumOfPlayers;

    Game::GAME_MODE _gameMode;

    int mainPlayerIndex;

  signals:
    void signalInitLobby();

    void signalInitNames();

    void signalStartGame();

    void signalClose();

  private:
    QString _serverAddress;
    QString _myName;
    bool _isHost;
};

void
connect1(int sockfd, struct sockaddr* sockaddr, socklen_t size);

#endif // CLIENT_H
