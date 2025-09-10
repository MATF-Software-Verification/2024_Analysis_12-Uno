#include "headers/deck.h"
#include "headers/pile.h"
#include "headers/wildcard.h"

#include <QDebug>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <random>

Deck::Deck()
{
    std::initializer_list<Card::COLOR> allClrs = {
        Card::RED, Card::BLUE, Card::YELLOW, Card::GREEN
    };
    std::initializer_list<Card::CARD_TYPE> allTypes = { Card::SKIP,
                                                        Card::DRAW_TWO,
                                                        Card::REVERSE };

    // iterate through all colors
    for (auto clr : allClrs) {
        // iterate through all numbers
        for (int i = 0; i < 10; i++) {
            // skip 1 zero
            if (i)
                deck.push_back(new Card(clr, Card::CARD_TYPE::NUMERIC, i));
            deck.push_back(new Card(clr, Card::CARD_TYPE::NUMERIC, i));
        }

        // iterate through all types
        for (auto type : allTypes) {
            deck.push_back(new Card(clr, type, -1));
            deck.push_back(new Card(clr, type, -1));
        }
    }

    // add wilds
    for (int i = 0; i < 4; i++) {
        deck.push_back(new WildCard(Card::WILD));
        deck.push_back(new WildCard(Card::WILD_DRAW_FOUR));
    }
}

Deck::~Deck()
{
    for (auto card : deck)
        delete card;
}

Deck::Deck(const QVector<int>& hashVector)
{
    hashToDeck(hashVector);
}

auto
Deck::toHash() -> QVector<int>
{
    QVector<int> hashVector(deck.size());

    for (int i = 0; i < deck.size(); i++)
        hashVector[i] = deck[i]->toHash();
    return hashVector;
}

void
Deck::hashToDeck(const QVector<int>& hashVector)
{
    QVector<Card*> cards(hashVector.size());

    for (int i = 0; i < hashVector.size(); i++)
        cards[i] = Card::hashToCard(hashVector[i]);
    deck = cards;
}

void
Deck::refill(Pile* pile)
{
    QVector<Card*> cards = pile->getPile();

    for (int i = 0; i < cards.size() - 1; i++)
        deck.append(cards[i]);
    Card* topCard = pile->lastCard();
    pile->clear();
    pile->pushCard(topCard);
    shuffle();
}

void
Deck::shuffle()
{
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(std::begin(deck), std::end(deck), rng);
}

auto
Deck::drawCard() -> Card*
{
    if (deck.size() == 0)
        throw "Cannot draw from empty deck!";

    Card* nextDraw = deck.back();
    deck.pop_back();
    return nextDraw;
}

auto
Deck::drawFirstCard() -> Card*
{
    auto it = deck.end() - 1;

    while ((*it)->getCardType() != Card::CARD_TYPE::NUMERIC &&
           it != deck.begin()) {
        it--;
    }

    Card* firstCard = *it;

    deck.erase(it);

    return firstCard;
}

auto
Deck::getDeckSize() const -> unsigned
{
    return deck.size();
}

auto
Deck::isEmpty() const -> bool
{
    return !getDeckSize();
}

void
Deck::spreadOut(QVector<Player*> players)
{
    for (int i = 0; i < drawHowMany; i++)
        for (Player* player : players)
            player->drawCard();
}
