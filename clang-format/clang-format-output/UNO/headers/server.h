#ifndef SERVER_H
#define SERVER_H

#include "headers/game.h"

#include <qthread.h>

#include <QTcpServer>
#include <QTcpSocket>

#define MAX_NAME_LENGTH (25)
#define MAX_MESSAGE_LENGTH (350)
#define MAX_WAIT_IN_LOOP (1)
#define MESSAGE_WAIT_TIME (1)

/*
    First we load client and server for the lobby,
    then when the game starts, we load GameServer once
    on host's side, and GameClient on every player's side.
*/

class Server : public QThread
{
  public:
    Server(QObject* parent = nullptr);

    Server(QObject* parent, Game* game);

    ~Server();

    void stopWaiting();

    void run();

    void setNumberOfPlayers(int number);

    // sends a message to all other players,
    // except for player in position index
    void sendMessage(std::string message, int index);

    // receives a message from player whose turn it is
    std::string getMessage();

    Game* getGame() const;

    QVector<int> _connections;

    int sockfd;

  private:
    Game* _game;
    int _numberOfPlayers;
    bool _stopWaiting;
};

#endif // SERVER_H
