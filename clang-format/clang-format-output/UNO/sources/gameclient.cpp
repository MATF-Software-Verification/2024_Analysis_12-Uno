#include "headers/gameclient.h"
#include <headers/wildcard.h>

#include <sys/socket.h>
// #include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <utility>

GameClient::GameClient(QObject* parent)
  : QThread(parent)
{
}

GameClient::GameClient(QObject* parent, Game* game, QString name, bool isHost)
  : QThread(parent)
  , _game(game)
  , _myName(std::move(name))
  , _isHost(isHost)
{
}

GameClient::~GameClient() = default;

auto
GameClient::isHost() -> bool
{
    return _isHost;
}

void
GameClient::run()
{
    loadGame();

    while (!_game->isOver()) {
        _game->emitRender();
        if (_game->getTopCard()->getColor() == Card::COLOR::BLACK) {
            _game->emitWild();
        }

        if (isMyTurn()) {
            Player* me = _game->prepareCurrentPlayer();

            while (true) {
                msleep(50);
                if (me->finishedTurn())
                    break;
            }

            // creating the message
            QString message = "";

            if (me->drawnCard()) {
                message.push_back('d');
            }

            if (me->hasPlayed()) {
                int cardIndex = me->playedCard();

                message.push_back((char)(cardIndex + 1));

                // ugly way to fetch card, but works for now

                Card* card = _game->getTopCard();

                if (card->getColor() == Card::COLOR::BLACK) {
                    message.push_back('y');
                    message.push_back(Card::colorChar(_game->getWildColor()));
                } else if (card->getCardNum() == 7 &&
                           _game->gameMode == Game::GAME_MODE::ZEROSANDSEVENS) {
                    message.push_back(
                      (char)(_game->getChosenPlayerIndex() + 1));
                }

                if (me->getTotalCards() == 1) {
                    if (me->saidUNO()) {
                        message.push_back('u');
                    } else {
                        message.push_back('n');
                    }
                }
            } else {
                message.append("e");
                if (_game->gameMode == Game::GAME_MODE::DEADLY &&
                    !me->hasPlayableCards()) {
                    me->eliminate();
                    me->position = _game->notEliminated();
                }
            }

            if (me->getTotalCards() == 1 && !me->saidUNO())
                me->drawCard(2);

            if (!me->hasCards()) {
                _game->setWinner(me);
                _game->end();
            }

            sendMessage(message);
        } else {
            QString receive = getMessage();
            _game->processTurn(receive);
        }

        _game->nextPlayer();
        msleep(750);
    }
    _game->emitWinner();
    close(sockfd);
}

auto
GameClient::isMyTurn() -> bool
{
    return _game->getCurrentPlayerIndex() == _myIndex;
}

auto
GameClient::getMessage() -> QString
{
    char buffer[MAX_MESSAGE_LENGTH];
    int bytesRead = recv(sockfd, buffer, MAX_MESSAGE_LENGTH, 0);
    buffer[bytesRead] = '\0';
    QString s = QString::fromStdString(std::string(buffer));
    return s;
}

void
GameClient::loadGame()
{
    // get initial message from server
    QString message = getMessage();

    int k = 0;

    // load number of players
    int numOfPlayers = (int)message[k].unicode();
    k++;

    // for each player load the length of their name and then their name
    // if the name matches yours, give index value to _myIndex

    QVector<QString> players;

    for (int i = 0; i < numOfPlayers; i++) {
        int length = (int)message[k].unicode();
        k++;

        QString name = message.mid(k, length);

        k += length;

        players.push_back(name);

        if (name == _myName) {
            _myIndex = i;
        }
    }

    _game->loadPlayers(players);

    _game->mainPlayer = _game->players[_myIndex];

    _game->mainPlayerIndex = _myIndex;

    // load deck size

    int deckSize = (int)message[k].unicode();
    k++;

    // load deck

    QVector<int> hashVector(deckSize);

    for (int i = 0; i < deckSize; i++) {
        hashVector[i] = (int)message[k].unicode();
        k++;
    }

    _game->loadDeck(hashVector);

    // load card on top of pile
    int hash = (int)message[k].unicode();

    _game->loadPile(hash);

    _game->dealHands();
}

void
GameClient::sendMessage(QString message)
{
    send(
      sockfd, message.toStdString().c_str(), message.toStdString().size(), 0);
}

auto
GameClient::getGame() -> Game*
{
    return _game;
}
