#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include "headers/game.h"

#include <sys/socket.h>

#include <QString>
#include <QTcpSocket>

#define MAX_NAME_LENGTH (25)
#define MAX_MESSAGE_LENGTH (350)
#define MESSAGE_WAIT_TIME (1)

class GameClient : public QThread
{
    Q_OBJECT
  public:
    GameClient(QObject* parent = nullptr);

    GameClient(QObject* parent, Game* game, QString name, bool isHost);

    ~GameClient();

    bool isHost();

    // creates a socket and connects to server
    // on the given address, and sends client's name
    void connectToServer(QString address);

    void run();

    bool isMyTurn();

    // returns a message sent from server
    QString getMessage();

    // loads initial information to start game
    void loadGame();

    // sends turn information
    void sendMessage(QString message);

    Game* getGame();

    int sockfd;

    QVector<QString> names;

  private:
    Game* _game;
    int _myIndex;
    QString _myName;
    bool _isHost;
};

#endif // GAMECLIENT_H
