#ifndef GAMEGUI_H
#define GAMEGUI_H

#include "card.h"
#include "client.h"
#include "deck.h"
#include "game.h"
#include "gameParameters.h"
#include "gameclient.h"
#include "gameserver.h"
#include "human.h"
#include "pc.h"
#include "player.h"
#include "server.h"

#include <QAudioOutput>
#include <QFrame>
#include <QLabel>
#include <QMediaPlayer>
#include <QPushButton>
#include <QWidget>

namespace Ui {
class TESTUI;
}

class GameGUI : public QWidget
{
    Q_OBJECT

  public:
    GameGUI(QWidget* parent, GameParameters* gameParams);
    GameGUI(QWidget* parent,
            GameParameters* gameParams,
            Server* server_,
            Client* client_);
    ~GameGUI();

    void renderCardInHand(int index);

  private slots:
    // draw card from deck and render it
    void onDrawCard();

    // checks if the card is playable and places it on top of pile
    void checkIfPlayable(int index /*Card* card*/);

    // renders the visuals of the cards in players hand
    void renderPlayerCardsInHand(QVector<Card*> cards);

    // renders the visuals of the card on top of pile
    void renderCardOnPile(Card* card);

    void showChosenWildColor();

    // show menu to choose wild color
    void showWildMenu();
    void setWildBlue();
    void setWildRed();
    void setWildGreen();
    void setWildYellow();

    void endTurn();

    // say UNO button
    void onSayUNO();

    // display winner of the game
    void displayWinner();

    void renderBotFrames(QVector<Player*> botPlayers);

    void renderMainPlayerFrame(Player* mainPlayer);

    void renderTable(/*QVector<Player*> players,*/ Card* cardOnPile);

    void renderZerosAndSevensButtons();

    void displaySaidUnoMsg(QString botName);

    void toggleVolume();

  private:
    Ui::TESTUI* ui;

    Game* game;
    GameServer* server;
    Game* serverSide;
    GameClient* client;
    Game* clientSide;

    Card* clickedCard;
    QVector<QPushButton*> zerosAndSevensButtons;

    QFrame* generateFrame(QString position);
    QFrame* generateFrame8Players(QString position);
    QPushButton* generateButton(QString position, QFrame* frame);
    QLabel* generateLabel(QString position, QFrame* frame);

    QString returnCardColor(Card::COLOR clr);
    QString returnCardType(Card::CARD_TYPE type);
    QString generateCardPath(Card* card);

    void setupWidgets(int numOfPlayers);
    void connectButtons();
    void connectSignals();

    QVector<QFrame*> allFrames;
    QVector<QPushButton*> allTurnIndicators;
    QVector<QLabel*> allNames;

    void initMainPlayerFrame();
    void init1BotFrame();
    void init3BotFrames();
    void init7BotFrames();

    void initZerosAndSevensButtons();

    void debug();
    void delay(int sec);

    void init(Client* client_);

    void closeEvent(QCloseEvent* event);

    QMediaPlayer* mediaPlayer;
    QAudioOutput* audioOutput;

    bool _volumeInd = true;
};

#endif // GAMEGUI_H
