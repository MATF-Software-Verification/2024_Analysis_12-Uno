#include "headers/pile.h"

Pile::Pile() = default;

Pile::~Pile()
{
    for (Card* card : pile) {
        delete card;
    }
}

void
Pile::pushCard(Card* c)
{
    if (pile.size() < DECK_SIZE) {
        pile.push_back(c);
    } else
        throw "Can't push card on a full pile";
}

auto
Pile::lastCard() const -> Card*
{
    if (!isEmpty()) {
        return pile.back();
    } else {
        return new Card();
    }
}

auto
Pile::getSize() const -> int
{
    return pile.size();
}

auto
Pile::isEmpty() const -> bool
{
    return !getSize();
}

void
Pile::clear()
{
    Card* tmp = pile.takeLast();
    pile.clear();
    pile.push_back(tmp);
}

auto
Pile::getPile() -> QVector<Card*>
{
    return pile;
}

auto
Pile::getWildColor() -> Card::COLOR
{
    return wildColor;
}

void
Pile::setWildColor(Card::COLOR clr)
{
    wildColor = clr;
}

auto
Pile::pop() -> Card*
{
    if (!pile.size())
        throw "Can't remove cards from an empty pile";

    Card* card = lastCard();
    pile.pop_back();
    return card;
}
