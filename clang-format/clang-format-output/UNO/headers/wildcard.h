#ifndef WILDCARD_H
#define WILDCARD_H

#include "headers/card.h"

class WildCard : public Card
{
  public:
    WildCard();
    WildCard(CARD_TYPE cardType);
    WildCard(Card card);
    ~WildCard();

    virtual int toHash() override;

    void useWildColor(const COLOR& wildColor);
    bool isPlayable(void*) const override;
    void play(void*) override;

    Card::COLOR getChosenWildColor();

  private:
    COLOR chosenColor;
};

#endif // WILDCARD_H
