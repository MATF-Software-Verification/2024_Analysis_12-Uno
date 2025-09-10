#include "headers/wildcard.h"
#include "headers/game.h"
#include <iostream>

WildCard::WildCard() = default;

WildCard::WildCard(CARD_TYPE cardType)
  : Card(COLOR::BLACK, cardType, -1)
{
    if (cardType != CARD_TYPE::WILD && cardType != CARD_TYPE::WILD_DRAW_FOUR)
        throw "Incorrect WildCard type!";
}

WildCard::WildCard(Card card)
  : WildCard(card.getCardType())
{
}

WildCard::~WildCard() = default;

void
WildCard::play(void* pointer)
{

    Game* game = (Game*)pointer;

    if (getCardType() == CARD_TYPE::WILD_DRAW_FOUR) {
        game->getNextPlayer()->drawCard(4);
        game->nextPlayer();
    }
}

auto
WildCard::isPlayable(void*) const -> bool
{
    return true;
}

void
WildCard::useWildColor(const COLOR& wildColor)
{
    chosenColor = wildColor;
}

auto
WildCard::getChosenWildColor() -> Card::COLOR
{
    return chosenColor;
}

// wildcards are 1 and 2
auto
WildCard::toHash() -> int
{
    if (getCardType() == CARD_TYPE::WILD)
        return 1;
    else
        return 2;
}
