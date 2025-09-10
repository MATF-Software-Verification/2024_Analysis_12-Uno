#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QThread>
#include <QVector>

#include "card.h"
#include "deck.h"
#include "gameParameters.h"
#include "pile.h"
#include "player.h"

#define PORT (8000)

// TODO: Consider making separate classes for each game mode
// TODO: Refactor methods

class Game : public QThread
{
    Q_OBJECT
  public:
    enum GAME_MODE
    {
        CLASSIC,
        ZEROSANDSEVENS,
        DEADLY,
        ELIMINATION
    };

    Game(QObject* parent = nullptr);

    Game(QObject* parent, GameParameters* gameParams);

    virtual ~Game();

    // begins the game
    void run() override;

    QVector<Player*> getPlayers();

    // probably not needed
    void removePlayedCard(Card playedCard);

    void addPlayer(Player* player);

    void end();

    void setWinner(Player* player);

    Player* getWinner();

    Deck* getDeck();

    int getNumberOfPlayers();

    Card* drawCard();

    void switchTurn();
    // response whose turn it is
    void whoseTurn();
    // check if deck is empty
    bool isDeckEmpty();
    // reshuffle the new deck
    void reshuffle();
    // get card from top of the pile
    Card* getTopCard();
    // set game mode
    void setGameMode(GAME_MODE);
    // swap direction
    void swapDirection();
    // play card
    void playCard(Card*);
    // returns index of current player
    int getCurrentPlayerIndex();
    // returns index of next player
    int getNextPlayerIndex();
    // returns pointer to current player
    Player* getCurrentPlayer();
    // returns pointer to next player in order
    Player* getNextPlayer();
    // go to next player
    void nextPlayer();
    // set wild color when wild card is played
    void setWildColor(Card::COLOR clr);
    // get wild color
    Card::COLOR getWildColor();
    // returns pointer to player at the desired index
    Player* getPlayerAtIndex(int index);

    void setChosenPlayer(int index);

    int getChosenPlayerIndex();

    Player* getChosenPlayer();

    void dealHands();

    bool isOver();

    // loads player names in order
    void loadPlayers(QVector<QString>);

    // loads deck from message
    void loadDeck(QVector<int>);

    // loads card on top of pile
    void loadPile(int);

    // processes what happened in the previous turn from a message
    virtual void processTurn(QString);

    int getDeckSize();

    Player* prepareCurrentPlayer();

    void emitRender();

    void emitWild();

    void emitWinner();

    void playZero();

    bool eliminationsOn();

    // returns the number of players not yet eliminated
    int notEliminated();

    QVector<Player*> players;
    bool _isOver;
    GAME_MODE gameMode;
    int _currentPlayer;
    Deck* deck;
    Pile* pile;
    int gameDirection;
    Player* mainPlayer;
    bool _isOnline;
    int _numOfPlayers;
    Player* _winner;
    int mainPlayerIndex;

  signals:
    void emitterRenderPileTopCard(Card* card);

    // void emitterRenderPlayerCards(QVector<Card*> cards);

    void emitterChosenWildColor();

    void signalGameEnded();

    void signalRenderBotCards(Player* player, int index);

    void signalRenderTable(/*QVector<Player*> players,*/ Card* cardOnPile);

    void signalBotSaidUno(QString botName);

  private:
    void initPlayers();
    void initDeck();
    void initPile();

    int _chosenPlayer;

    bool eliminations = false;

    void delay(int mSec);

    void setupParams(GameParameters* gameParams);

    void setupBoard();
};

#endif // GAME_H
