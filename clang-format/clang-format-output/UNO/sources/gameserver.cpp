#include "headers/gameserver.h"
#include "headers/human.h"
#include "headers/pc.h"
#include "headers/wildcard.h"

#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

GameServer::GameServer(QObject* parent)
  : QThread(parent)
{
}

GameServer::GameServer(QObject* parent, Game* game)
  : QThread(parent)
  , _game(game)
{
    _stopWaiting = false;
}

GameServer::~GameServer() = default;

void
GameServer::stopWaiting()
{
    _stopWaiting = true;
}

void
GameServer::setNumberOfPlayers(int number)
{
    _numberOfPlayers = number;
}

void
GameServer::run()
{
    sendInitial();

    _game->dealHands();

    while (!_game->isOver()) {
        if (isBotsTurn()) {
            msleep(2500);
            Player* currentPlayer = _game->getCurrentPlayer();
            std::string send = currentPlayer->play().toStdString();
            if (send == "refill") {
                refill();
                send = currentPlayer->play().toStdString();
            }
            if (!currentPlayer->hasCards()) {
                _game->setWinner(currentPlayer);
                _game->end();
            }
            sendMessage(send, -1);
        } else {
            int currentPlayer = _game->getCurrentPlayerIndex();
            std::string receive = getMessage();
            std::string send = processTurn(receive);

            sendMessage(send, currentPlayer);
        }
        _game->nextPlayer();
    }

    for (int connection : _connections)
        close(connection);

    close(sockfd);
}

auto
GameServer::isBotsTurn() const -> bool
{
    return _game->getCurrentPlayer()->isBot();
}

void
GameServer::sendInitial()
{
    std::string message = "";

    // append number of players
    message.push_back((char)_game->getNumberOfPlayers());

    // append length of name and then name for each player in order
    QVector<Player*> players = _game->getPlayers();

    for (Player* player : players) {
        message.push_back((char)player->getName().length());
        message.append(player->getName().toStdString());
    }

    // append deck order
    message.append(getDeckOrder());

    // append card on top of pile
    message.push_back((char)_game->getTopCard()->toHash());

    sendMessage(message, -1);
}

void
GameServer::refill()
{
    _game->reshuffle();
    std::string message = "z";
    message.append(getDeckOrder());
    sendMessage(message, -1);
}

auto
GameServer::getGame() const -> Game*
{
    return _game;
}

auto
GameServer::getDeckOrder() const -> std::string
{
    std::string deckOrder = "";
    // add deck size
    deckOrder.push_back((char)_game->getDeck()->getDeckSize());

    // add deck order
    QVector<int> hashVector = _game->getDeck()->toHash();

    for (int i : hashVector) {
        deckOrder.push_back((char)i);
    }

    return deckOrder;
}

// CURRENTLY PLAYERS HAVE THE SAME INDEX IN _game->getPlayers()
// AND IN _connections SO THIS IS IMPLEMENTED THIS WAY,
// IF WE DECIDE TO SHUFFLE PLAYERS BEFORE A GAME,
// IMPLEMENT MAP
void
GameServer::sendMessage(std::string message, int index)
{
    for (int i = 0; i < _connections.size(); i++) {
        if (i != index) {
            send(_connections[i], message.c_str(), message.size(), 0);
        }
    }
}

auto
GameServer::getMessage() -> std::string
{
    int currentPlayer = _game->getCurrentPlayerIndex();

    if (currentPlayer >= _connections.size()) {
        std::cout << "Out of range" << std::endl;
        exit(EXIT_FAILURE);
    }

    int connection = _connections[currentPlayer];

    char buffer[MAX_MESSAGE_LENGTH];

    int bytesRead = recv(connection, buffer, MAX_MESSAGE_LENGTH, 0);

    buffer[bytesRead] = '\0';

    std::string s(buffer);

    return s;
}

auto
GameServer::processTurn(std::string message) -> std::string
{
    if (message == "refill") {
        refill();
        message = getMessage();
    }

    _game->processTurn(QString::fromStdString(message));
    return message;
}
