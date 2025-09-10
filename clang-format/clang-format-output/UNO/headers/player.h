#ifndef PLAYER_H
#define PLAYER_H

#include "headers/card.h"
#include "headers/pile.h"
#include <QVector>

class Player
{
  public:
    Player();
    Player(QString name);
    virtual ~Player();

    // getters
    QString getName() const;
    int getScore() const;
    QVector<Card*> getCards();
    int getTotalCards() const;
    bool hasCards() const;
    bool saidUNO() const;
    Pile* getPile() const;

    bool drawnCard() const;
    bool hasPlayed() const;
    bool finishedTurn() const;
    bool isMyTurn() const;
    int playedCard() const;

    // setters
    void setName(const QString& newName);
    void setHand(const QVector<Card*>& newHand);
    void setPile(Pile* newPile);
    void setScore(const int& newScore);
    void sayUNO(const bool& say);

    void setDrawnCard(const bool& newDrawnCard);
    void setHasPlayed(const bool& newHasPlayed);
    void setFinishedTurn(const bool& newFinishedTurn);
    void setIsMyTurn(const bool& newIsMyTurn);
    void setPlayedCard(const int& newPlayedCard);

    // increase score for played card
    void increaseScore(Card* card);
    // checks if player has card and returns its index, else returns -1
    int hasCard(const Card& card) const;
    void removeCard(Card* card);
    void obtainCard(Card* card);

    virtual bool hasPlayableCards() = 0;
    virtual bool playCard(const int& cardIndex) = 0;
    virtual void drawCard(const int& howMany = 1) = 0;
    virtual Card::COLOR chooseColor() = 0;
    virtual QString play() = 0;
    virtual bool isBot() const = 0;

    void eliminate();

    bool isEliminated();

    void takeCards(Player*);
    void swapCards(Player*);

    QVector<Card*> hand;

    int position = 0;

    Player* previousPlayer;

  private:
    QString name;

    int _score;
    Pile* pile;
    bool _saidUNO;

    bool _eliminated = false;

    bool _drawnCard = false;
    bool _hasPlayed = false;
    bool _finishedTurn = false;
    bool _isMyTurn = false;

    int _playedCard;
};

#endif // PLAYER_H
