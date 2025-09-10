#ifndef GAMESERVER_H
#define GAMESERVER_H

#include "headers/game.h"

#include <qthread.h>

#include <QTcpServer>
#include <QTcpSocket>

#define MAX_NAME_LENGTH (25)
#define MAX_MESSAGE_LENGTH (350)
#define MAX_WAIT_IN_LOOP (1)
#define MESSAGE_WAIT_TIME (1)

/*
    Current plan for GameServer is for every player,
    including the lobby leader, to be connected as clients
*/

class GameServer : public QThread
{
  public:
    GameServer(QObject* parent = nullptr);

    GameServer(QObject* parent, Game* game);

    ~GameServer();

    void stopWaiting();

    // establishes a connection to clients
    void wait();

    void run();

    void setNumberOfPlayers(int number);

    // sends initial state of the game to all other players
    void sendInitial();

    // sends a message to all other players,
    // except for player in position index
    void sendMessage(std::string message, int index);

    // receives a message from player whose turn it is
    std::string getMessage();

    // processes received message
    // and returns the message that needs to be sent
    std::string processTurn(std::string message);

    std::string getDeckOrder() const;

    bool isBotsTurn() const;

    void refill();

    Game* getGame() const;

    QVector<int> _connections;

    int sockfd;

  private:
    Game* _game;
    int _numberOfPlayers;
    bool _stopWaiting;
};

#endif // GAMESERVER_H
