#ifndef LOBBY_H
#define LOBBY_H

#include "client.h"
#include "gameGUI.h"
#include "gameParameters.h"
#include "server.h"

#include <QWidget>

namespace Ui {
class Lobby;
}

class Lobby : public QWidget
{
    Q_OBJECT

  public:
    Lobby(QWidget* parent, GameParameters* gameParams);

    ~Lobby();

  public slots:
    void initLobby();
    void initNames();

    void startGame();
    void toggleVolume();

  private slots:
    void startGameBtn_clicked();

  private:
    Ui::Lobby* ui;
    GameParameters* _gameParams;
    GameGUI* gameGUI;
    QVector<QFrame*> frames;

    void init();

    void initFrames(QString row);
    void initBotFrames(int numOfPlayers);

    Server* server;
    Game* serverSide;
    Client* client;
    QMediaPlayer* mediaPlayer;
    QAudioOutput* audioOutput;

    bool _volumeInd = true;
};

#endif // LOBBY_H
