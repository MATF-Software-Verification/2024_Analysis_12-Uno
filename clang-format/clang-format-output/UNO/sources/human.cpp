#include "headers/human.h"

#include <QVector>

Human::Human() = default;

Human::Human(QString name, Game* game)
  : Player(name)
  , _game(game)
{
}

Human::~Human() = default;

auto
Human::isBot() const -> bool
{
    return false;
}

auto
Human::playCard(const int& cardIndex) -> bool
{
    Card* card = getCards().at(cardIndex);
    if (card->isPlayable(_game)) {
        removeCard(card);
        _game->playCard(card);
        return true;
    } else {
        return false;
    }
}

auto
Human::chooseColor() -> Card::COLOR
{
    return Card::BLUE;
}

auto
Human::play() -> QString
{
    return "";
}

auto
Human::game() const -> Game*
{
    return _game;
}

void
Human::setGame(Game* newGame)
{
    _game = newGame;
}

void
Human::drawCard(const int& howMany)
{
    for (int i = 0; i < howMany; i++) {
        Card* card = _game->drawCard();
        obtainCard(card);
    }
}

auto
Human::hasPlayableCards() -> bool
{
    bool result = false;
    for (auto card : hand) {
        if (card->isPlayable(_game))
            result = true;
    }
    return result;
}
