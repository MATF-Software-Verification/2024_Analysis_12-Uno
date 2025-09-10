#ifndef PILE_H
#define PILE_H

#include "headers/card.h"

#include <QVector>

#define DECK_SIZE 108

class Pile
{
  public:
    Pile();

    ~Pile();

    QVector<Card*> getPile();
    void pushCard(Card*);
    Card* lastCard() const;
    int getSize() const;
    bool isEmpty() const;
    void clear();
    Card::COLOR getWildColor();
    void setWildColor(Card::COLOR clr);
    Card* pop();

  private:
    Card::COLOR wildColor;
    QVector<Card*> pile;
};

#endif // PILE_H
