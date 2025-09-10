#include "headers/lobby.h"
#include "ui_lobby.h"
#include <QThread>
#include <iostream>
#include <unistd.h>

int frameWidth = 350;
int frameHeight = 350;

Lobby::Lobby(QWidget* parent, GameParameters* gameParams)
  : QWidget(parent)
  , ui(new Ui::Lobby)
  , _gameParams(gameParams)
{
    if (gameParams->isOnline && gameParams->isHost) {
        int num = gameParams->numOfPlayers;

        gameParams->numOfPlayers = 0;

        serverSide = new Game(this, gameParams);

        client =
          new Client(this, "127.0.0.1", gameParams->mainPlayerName, true);

        gameParams->numOfPlayers = num;

        server = new Server(this, serverSide);

        server->setNumberOfPlayers(gameParams->numOfPlayers);

        init();

        server->start();

        client->start();
    } else if (!gameParams->isHost && gameParams->isOnline) {
        client =
          new Client(this, "127.0.0.1", gameParams->mainPlayerName, false);

        init();

        client->start();
    }
}

void
Lobby::init()
{
    ui->setupUi(this);
    this->setWindowTitle("Lobby");
    QWidget::showMaximized();

    mediaPlayer = new QMediaPlayer();
    audioOutput = new QAudioOutput();
    mediaPlayer->setAudioOutput(audioOutput);
    mediaPlayer->setSource(QUrl::fromUserInput("qrc:/Music/lobby.mp3"));
    mediaPlayer->play();

    initFrames("up");
    initFrames("down");

    connect(ui->startGameBtn,
            &QPushButton::clicked,
            this,
            &Lobby::startGameBtn_clicked);
    connect(
      ui->turnVolumeBtn, &QPushButton::clicked, this, &Lobby::toggleVolume);
    connect(client, &Client::signalInitLobby, this, &Lobby::initLobby);
    connect(client, &Client::signalInitNames, this, &Lobby::initNames);
    connect(client, &Client::signalStartGame, this, &Lobby::startGame);
    connect(client, &Client::signalClose, this, &Lobby::close);
}

void
Lobby::initLobby()
{
    initBotFrames(client->names.size());

    // TODO: logic when a player joins the lobby,
    // change the required bot name to player name
    initNames();
}

void
Lobby::initFrames(QString row)
{
    auto* frame = new QFrame(this);
    if (row == "up")
        frame->setGeometry(50, 50, frameWidth, frameHeight);
    else
        frame->setGeometry(
          50, height() - frameHeight - 50, frameWidth, frameHeight);
    if (row == "up")
        frame->setStyleSheet(
          "QFrame {border-image: url(:/backgrounds/hacker.png)}");
    else
        frame->setStyleSheet(
          "QFrame {border-image: url(:/backgrounds/unknown.png)}");
    frame->show();
    frames.push_back(frame);

    int i = row == "up" ? 1 : 5;
    int sz = row == "up" ? 4 : 8;
    int yPos = row == "up" ? 50 : height() - frameHeight - 50;
    for (; i < sz; i++) {
        auto* nFrame = new QFrame(this);
        nFrame->setGeometry(frames[i - 1]->geometry().x() + frameWidth + 140,
                            yPos,
                            frameWidth,
                            frameHeight);
        nFrame->setStyleSheet(
          "QFrame {border-image: url(:/backgrounds/unknown.png)}");
        nFrame->show();
        frames.push_back(nFrame);
    }
}

void
Lobby::initBotFrames(int numOfHumans)
{
    for (int i = numOfHumans; i < client->totalNumOfPlayers; i++)
        frames[i]->setStyleSheet(
          "QFrame {border-image: url(:/backgrounds/ai.png)}");
}

void
Lobby::initNames()
{
    for (int i = 0; i < 8; i++) {
        QString name = i == 0 ? "[HOST] " : "";
        if (i < client->names.size()) {
            if (client->names[i] == _gameParams->mainPlayerName)
                name.append("[ME] ");
            name.append(client->names[i]);
            frames[i]->setStyleSheet(
              "QFrame {border-image: url(:/backgrounds/hacker.png)}");
        } else if (i >= client->totalNumOfPlayers)
            name.append("NOT ENOUGH OPEN SPOTS");
        else
            name.append("PC" + QString::number(i + 1));

        auto* lbl = new QLabel(name, this);
        if (lbl->text().contains("[HOST]"))
            lbl->setStyleSheet("QLabel {background-color: orange}");
        else if (lbl->text().contains("[ME]"))
            lbl->setStyleSheet("QLabel {background-color: green}");
        else
            lbl->setStyleSheet(
              "QLabel {background-color: qlineargradient(spread:pad, "
              "x1:0.492, y1:1, x2:0.513, y2:0, "
              "stop:0 rgba(168, 168, 168, 255), stop:0.402116 rgba(229, 229, "
              "229, 255), "
              "stop:0.619048 rgba(240, 240, 240, 255), stop: 0.994709 "
              "rgba(172, 172, 172, 255))};");

        lbl->setAlignment(Qt::AlignHCenter);
        lbl->setFont(QFont("URW Bookman", 14, QFont::Bold));
        int frameXPos = frames[i]->geometry().x();
        int frameYPos = frames[i]->geometry().y();
        int lblWidth = 300;
        int lblHeight = 30;
        lbl->setGeometry(frameXPos + frameWidth / 2 - lblWidth / 2,
                         frameYPos + frameHeight + lblHeight,
                         lblWidth,
                         lblHeight);
        lbl->show();
    }
}

void
Lobby::startGame()
{
    gameGUI = new GameGUI(nullptr, _gameParams, server, client);
    gameGUI->show();
    delete audioOutput;
    delete mediaPlayer;
    this->close();
}

void
Lobby::toggleVolume()
{
    if (_volumeInd) {
        audioOutput->setMuted(true);
        QPixmap pixmap(":/backgrounds/mute.png");
        QIcon buttonIcon(pixmap);
        ui->turnVolumeBtn->setIcon(buttonIcon);
        ui->turnVolumeBtn->setIconSize(QSize(40, 40));
    } else {
        audioOutput->setMuted(false);
        QPixmap pixmap(":/backgrounds/sound.png");
        QIcon buttonIcon(pixmap);
        ui->turnVolumeBtn->setIcon(buttonIcon);
        ui->turnVolumeBtn->setIconSize(QSize(40, 40));
    }
    _volumeInd = !_volumeInd;
}

Lobby::~Lobby()
{
    delete ui;
}

void
Lobby::startGameBtn_clicked()
{
    // TODO: logic here for server/client
    if (_gameParams->isHost) {
        server->stopWaiting();
        //        gameGUI = new GameGUI(nullptr, &_gameParams);
        //        gameGUI->show();
        //        this->close();
    }
}
