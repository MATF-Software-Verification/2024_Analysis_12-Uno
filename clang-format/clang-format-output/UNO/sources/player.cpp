#include "headers/player.h"

#include <iostream>
#include <utility>

using namespace std;

Player::Player() = default;

Player::Player(QString name)
  : name(std::move(name))
  , _score(0)
{
    _saidUNO = false;
}

Player::~Player()
{
    for (Card* card : hand)
        delete card;
}

auto
Player::hasCard(const Card& card) const -> int
{
    for (int i = 0; i < hand.size(); i++) {
        Card* h = hand[i];
        if (h->getColor() == card.getColor() ||
            h->getCardNum() == card.getCardNum() ||
            h->getColor() == Card ::COLOR ::BLACK)
            return i;
    }

    return -1;
}

void
Player::increaseScore(Card* card)
{
    if (card->getColor() == Card::BLACK)
        _score += 50;
    else {
        if (card->getCardType() != Card::NUMERIC)
            _score += 20;
        else
            _score += card->getCardNum();
    }
}

auto
Player::getTotalCards() const -> int
{
    return hand.size();
}

void
Player::obtainCard(Card* card)
{
    hand.push_back(card);
}

// setters and getters below

auto
Player::drawnCard() const -> bool
{
    return _drawnCard;
}

auto
Player::getName() const -> QString
{
    return name;
}

auto
Player::getScore() const -> int
{
    return _score;
}

auto
Player::getCards() -> QVector<Card*>
{
    return hand;
}

void
Player::setScore(const int& s)
{
    _score = s;
}

void
Player::setDrawnCard(const bool& newDrawnCard)
{
    _drawnCard = newDrawnCard;
}

auto
Player::hasPlayed() const -> bool
{
    return _hasPlayed;
}

void
Player::setHasPlayed(const bool& newHasPlayed)
{
    _hasPlayed = newHasPlayed;
}

auto
Player::finishedTurn() const -> bool
{
    return _finishedTurn;
}

void
Player::setFinishedTurn(const bool& newFinishedTurn)
{
    _finishedTurn = newFinishedTurn;
}

auto
Player::isMyTurn() const -> bool
{
    return _isMyTurn;
}

void
Player::setIsMyTurn(const bool& newIsMyTurn)
{
    _isMyTurn = newIsMyTurn;
}

auto
Player::playedCard() const -> int
{
    return _playedCard;
}

void
Player::setPlayedCard(const int& newPlayedCard)
{
    _playedCard = newPlayedCard;
}

void
Player::removeCard(Card* card)
{
    hand.removeOne(card);
}

void
Player::sayUNO(const bool& say)
{
    _saidUNO = say;
}

auto
Player::hasCards() const -> bool
{
    return !hand.empty();
}

auto
Player::saidUNO() const -> bool
{
    return _saidUNO;
}

void
Player::setName(const QString& newName)
{
    name = newName;
}

void
Player::setHand(const QVector<Card*>& newHand)
{
    hand.clear();

    hand.append(newHand);
}

auto
Player::getPile() const -> Pile*
{
    return pile;
}

void
Player::setPile(Pile* newPile)
{
    pile = newPile;
}

void
Player::eliminate()
{
    _eliminated = true;
}

auto
Player::isEliminated() -> bool
{
    return _eliminated;
}

void
Player::swapCards(Player* player_)
{
    QVector<Card*> tmp;

    tmp.append(hand);

    takeCards(player_);

    player_->setHand(tmp);
}

void
Player::takeCards(Player* player_)
{
    setHand(player_->hand);
}
