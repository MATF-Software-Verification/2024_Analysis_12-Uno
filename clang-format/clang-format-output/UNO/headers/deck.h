#ifndef DECK_H
#define DECK_H

#include "headers/card.h"
#include "headers/player.h"
#include <QVector>

class Deck
{
  public:
    Deck();
    Deck(const QVector<int>& hashVector);
    ~Deck();

    QVector<int> toHash();
    void hashToDeck(const QVector<int>& hashVector);

    void shuffle();

    void spreadOut(QVector<Player*> players);
    Card* drawCard();
    Card* drawFirstCard();
    void refill(Pile* pile);

    unsigned getDeckSize() const;
    bool isEmpty() const;

  private:
    QVector<Card*> deck;
    int drawHowMany = 7;
};

#endif // DECK_H
