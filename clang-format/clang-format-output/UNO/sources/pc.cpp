#include "headers/pc.h"
#include "headers/pile.h"
#include "headers/wildcard.h"
#include <random>

#include <iostream>

PC::PC() = default;

PC::PC(QString name, Game* game)
  : Player(name)
  , _game(game)
{
}

PC::~PC() = default;

auto
PC::isBot() const -> bool
{
    return true;
}

auto
PC::getCurrentGame() const -> Game*
{
    return _game;
}

void
PC::drawCard(const int& howMany)
{
    for (int i = 0; i < howMany; i++) {
        Card* card = _game->drawCard();
        obtainCard(card);
    }
}

auto
PC::playCard(const int& cardIndex) -> bool
{

    Card* card = getCards().at(cardIndex);

    if (card->isPlayable(_game)) {
        removeCard(card);
        _game->playCard(card);
        return true;
    }

    return false;
}

auto
PC::makeDecision() -> int
{

    QVector<Card*> cards = getPlayableCards();

    // if there are no playable cards return -1
    if (cards.isEmpty())
        return -1;

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, cards.size() - 1);
    int index = distribution(generator);

    Card* card = cards[index];

    int cardPlayed = getCards().indexOf(card);

    if (card->getCardType() == Card::CARD_TYPE::NUMERIC &&
        card->getCardNum() == 7 &&
        _game->gameMode == Game::GAME_MODE::ZEROSANDSEVENS) {
        int playerIndex = 0;
        QVector<Player*> players = _game->getPlayers();

        // pc is never index 0 so this goes through
        int min = players[0]->getTotalCards();

        for (int i = 1; i < players.size(); i++) {
            if (players[i] != this && players[i]->getTotalCards() < min) {
                min = players[i]->getTotalCards();
                playerIndex = i;
            }
        }
        _game->setChosenPlayer(playerIndex);
    }

    cards.removeAt(index);

    if (card->getColor() == Card::COLOR::BLACK) {
        int b = 0, r = 0, g = 0, y = 0, max = 0;
        Card::COLOR clr = Card::COLOR::BLUE;

        for (auto c : cards) {
            switch (c->getColor()) {
                case Card::COLOR::BLUE:
                    b++;
                    if (b > max) {
                        max = b;
                        clr = Card::COLOR::BLUE;
                    }
                    break;
                case Card::COLOR::RED:
                    r++;
                    if (r > max) {
                        max = r;
                        clr = Card::COLOR::RED;
                    }
                    break;
                case Card::COLOR::GREEN:
                    g++;
                    if (g > max) {
                        max = g;
                        clr = Card::COLOR::GREEN;
                    }
                    break;
                case Card::COLOR::YELLOW:
                    y++;
                    if (y > max) {
                        max = y;
                        clr = Card::COLOR::YELLOW;
                    }
                    break;
                default:
                    break;
            }
        }

        _game->setWildColor(clr);
    }

    return cardPlayed;
}

auto
PC::play() -> QString
{
    QString message = "";
    int cardPlayed = makeDecision();
    bool blackCard = false;
    bool seven = false;
    if (cardPlayed != -1) {
        message.push_back((char)(cardPlayed + 1));
        if (getCards().at(cardPlayed)->getColor() == Card::COLOR::BLACK) {
            blackCard = true;
        } else if (getCards().at(cardPlayed)->getCardNum() == 7) {
            seven = true;
        }
        increaseScore(getCards().at(cardPlayed));
        playCard(cardPlayed);
    } else {
        if (!_game->isDeckEmpty()) {
            message.append("d");
            drawCard();

            cardPlayed = makeDecision();
            if (cardPlayed != -1) {
                message.push_back((char)(cardPlayed + 1));
                if (getCards().at(cardPlayed)->getColor() ==
                    Card::COLOR::BLACK) {
                    blackCard = true;
                } else if (getCards().at(cardPlayed)->getCardNum() == 7) {
                    seven = true;
                }
                increaseScore(getCards().at(cardPlayed));
                playCard(cardPlayed);
            } else {
                message.append("e");
                if (_game->gameMode == Game::GAME_MODE::DEADLY) {
                    eliminate();
                    position = _game->notEliminated();
                }
            }
        } else {
            return "refill"; // refill the deck and call the bot again
        }
    }

    // always sends chosen color even if wild card isn't played
    if (blackCard) {
        message.push_back('y');
        message.push_back(Card::colorChar(_game->getWildColor()));
    } else if (seven && _game->gameMode == Game::GAME_MODE::ZEROSANDSEVENS) {
        message.push_back((char)(_game->getChosenPlayerIndex() + 1));

        swapCards(_game->getChosenPlayer());

        _game->emitRender();
    }

    if (_game->gameMode == Game::GAME_MODE::ELIMINATION && !hasCards() &&
        _game->notEliminated() > 2) {
        previousPlayer->eliminate();
        previousPlayer->position = _game->notEliminated();
        swapCards(previousPlayer);
    }

    if (getTotalCards() == 1) {
        message.push_back('u');
        sayUNO(true);
    }

    setIsMyTurn(false);

    return message;
}

auto
PC::chooseColor() -> Card::COLOR
{
    int red_count = 0;
    int blue_count = 0;
    int green_count = 0;
    int yellow_count = 0;

    for (const Card* card : getCards()) {
        switch (card->getColor()) {
            case Card::RED:
                red_count++;
                break;
            case Card::BLUE:
                blue_count++;
                break;
            case Card::GREEN:
                green_count++;
                break;
            case Card::YELLOW:
                yellow_count++;
                break;
            default:
                continue;
        }
    }
    int max_color_count =
      std::max({ red_count, blue_count, green_count, yellow_count });
    if (max_color_count == red_count)
        return Card::COLOR::RED;
    else if (max_color_count == green_count)
        return Card::COLOR::GREEN;
    else if (max_color_count == yellow_count)
        return Card::COLOR::YELLOW;
    else
        return Card::COLOR::BLUE;
}

auto
PC::hasPlayableCards() -> bool
{
    bool result = false;
    for (auto card : hand) {
        if (card->isPlayable(_game))
            result = true;
    }
    return result;
}

auto
PC::getPlayableCards() -> QVector<Card*>
{
    QVector<Card*> tmp;

    for (auto card : hand) {
        if (card->isPlayable(_game))
            tmp.push_back(card);
    }

    return tmp;
}
