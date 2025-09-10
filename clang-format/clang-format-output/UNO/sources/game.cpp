#include "headers/game.h"
#include "headers/human.h"
#include "headers/pc.h"
#include "headers/wildcard.h"

#include <QCoreApplication>
#include <QDebug>
#include <QTime>

#include <iostream>
#include <memory>
#include <string>

Game::Game(QObject* parent)
  : QThread(parent)
{
    throw "Error: no parameters given";
}

Game::Game(QObject* parent, GameParameters* gameParams)
  : QThread(parent)
{
    gameMode = GAME_MODE::CLASSIC;

    setupParams(gameParams);

    setupBoard();
}

void
Game::setupParams(GameParameters* gameParams)
{
    _isOnline = gameParams->isOnline;

    _numOfPlayers = gameParams->numOfPlayers;

    switch (gameParams->gameMode[0].unicode()) {
        case '1':
            gameMode = GAME_MODE::CLASSIC;
            break;
        case '2':
            gameMode = GAME_MODE::ZEROSANDSEVENS;
            break;
        case '3':
            gameMode = GAME_MODE::ELIMINATION;
            eliminations = true;
            break;
        case '4':
            gameMode = GAME_MODE::DEADLY;
            eliminations = true;
            break;
    }

    mainPlayer = new Human(gameParams->mainPlayerName, this);

    mainPlayerIndex = 0;

    gameDirection = 1;

    _currentPlayer = 0;

    _isOver = false;
}

void
Game::setupBoard()
{
    initDeck();
    initPile();
    if (_numOfPlayers > 0) {
        initPlayers();
        dealHands();
    }
}

Game::~Game()
{
    delete deck;
    delete pile;
}

void
Game::addPlayer(Player* player)
{
    players.push_back(player);
    _numOfPlayers = players.size();
}

void
Game::end()
{
    _isOver = true;
}

void
Game::initPlayers()
{
    players.push_back(mainPlayer);

    for (int i = 0; i < _numOfPlayers - 1; i++)
        players.push_back(new PC("BOT" + QString::number(i), this));
}

void
Game::setWinner(Player* player)
{
    _winner = player;
}

auto
Game::getWinner() -> Player*
{
    return _winner;
}

void
Game::dealHands()
{
    deck->spreadOut(players);
}

auto
Game::getDeck() -> Deck*
{
    return deck;
}

void
Game::initDeck()
{
    deck = new Deck();
    deck->shuffle();
}

void
Game::initPile()
{
    pile = new Pile();
    Card* card = deck->drawFirstCard();
    pile->pushCard(card);
}

auto
Game::getNumberOfPlayers() -> int
{
    return _numOfPlayers;
}

void
Game::delay(int mSec)
{
    QTime dieTime = QTime::currentTime().addMSecs(mSec);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void
Game::run()
{
    while (true) {
        if (deck->isEmpty())
            reshuffle();

        // sleep to see rendered cards

        msleep(1000);

        // render table
        emit signalRenderTable(/*players,*/ getTopCard());

        // display wild choose box
        if (getTopCard()->getColor() == Card::BLACK)
            emit emitterChosenWildColor();

        Player* currentPlayer = prepareCurrentPlayer();

        emit signalRenderTable(/*players,*/ getTopCard());

        // if its your turn sleep thread and wait for you to play
        while (currentPlayer == mainPlayer) {
            msleep(100);
            if (!currentPlayer->isMyTurn())
                break;
        }

        if (currentPlayer == mainPlayer) {
            if (currentPlayer->getTotalCards() == 1 &&
                !currentPlayer->saidUNO())
                currentPlayer->drawCard(2);

            if (gameMode == GAME_MODE::DEADLY && !currentPlayer->hasPlayed() &&
                currentPlayer->drawnCard() &&
                !currentPlayer->hasPlayableCards()) {
                currentPlayer->eliminate();
                currentPlayer->position = notEliminated();
            }
        }

        // if its bots turn let him play and have delay of 1s between each
        if (currentPlayer != mainPlayer) {
            msleep(1000);
            currentPlayer->play();
            if (currentPlayer->getTotalCards() == 1 && currentPlayer->saidUNO())
                emit signalBotSaidUno(currentPlayer->getName());
            if (!currentPlayer->hasCards())
                break;
        }

        if (!currentPlayer->hasCards()) {
            setWinner(currentPlayer);
            break;
        }
        if (gameMode == GAME_MODE::DEADLY && notEliminated() == 1) {
            for (auto player : players)
                if (!player->isEliminated())
                    setWinner(player);
            break;
        }

        emit signalRenderTable(/*players,*/ getTopCard());

        nextPlayer();
    }

    emit signalGameEnded();
}

auto
Game::drawCard() -> Card*
{
    if (deck->isEmpty()) {
        deck->refill(pile);
    }
    return deck->drawCard();
}

auto
Game::isOver() -> bool
{
    return _isOver;
}

auto
Game::prepareCurrentPlayer() -> Player*
{
    Player* currentPlayer = getCurrentPlayer();

    currentPlayer->setDrawnCard(false);
    currentPlayer->setFinishedTurn(false);
    currentPlayer->setHasPlayed(false);
    currentPlayer->setIsMyTurn(true);
    currentPlayer->sayUNO(false);

    return currentPlayer;
}

auto
Game::getPlayers() -> QVector<Player*>
{
    return players;
}

auto
Game::isDeckEmpty() -> bool
{
    return deck->isEmpty();
}

void
Game::reshuffle()
{
    if (isDeckEmpty())
        deck->refill(pile);
}

auto
Game::getTopCard() -> Card*
{
    return pile->lastCard();
}

auto
Game::getCurrentPlayerIndex() -> int
{
    return _currentPlayer;
}

auto
Game::getNextPlayerIndex() -> int
{
    if (eliminations) {
        int nextIndex =
          (_currentPlayer + gameDirection + players.size()) % players.size();
        while (players[nextIndex]->isEliminated()) {
            nextIndex =
              (nextIndex + gameDirection + players.size()) % players.size();
        }
        return nextIndex;
    } else
        return (_currentPlayer + gameDirection + players.size()) %
               players.size();
}

void
Game::nextPlayer()
{
    if (gameMode == GAME_MODE::ELIMINATION) {
        int nextIndex = getNextPlayerIndex();
        players[nextIndex]->previousPlayer = players[_currentPlayer];
        _currentPlayer = nextIndex;
    } else
        _currentPlayer = getNextPlayerIndex();
}

auto
Game::getNextPlayer() -> Player*
{
    return players[getNextPlayerIndex()];
}

auto
Game::getCurrentPlayer() -> Player*
{
    return players[_currentPlayer];
}

void
Game::playCard(Card* card)
{
    card->play(this);
    pile->pushCard(card);
}

void
Game::setGameMode(GAME_MODE mode)
{
    gameMode = mode;
}

void
Game::swapDirection()
{
    gameDirection *= -1;
}

void
Game::setWildColor(Card::COLOR clr)
{
    pile->setWildColor(clr);
}

auto
Game::getWildColor() -> Card::COLOR
{
    return pile->getWildColor();
}

auto
Game::getPlayerAtIndex(int index) -> Player*
{
    if (index > 0 && index < players.size()) {
        return players[index];
    }

    return nullptr;
}

auto
Game::getDeckSize() -> int
{
    return deck->getDeckSize();
}

// currently loading bots as humans in client mode,
// client does not need to see them as bots
void
Game::loadPlayers(QVector<QString> names)
{
    players.clear();
    for (const auto& name : names)
        players.push_back(new Human(name, this));
    _numOfPlayers = players.size();
}

void
Game::loadDeck(QVector<int> hashVector)
{
    if (deck)
        delete deck;
    deck = new Deck(hashVector);
}

void
Game::loadPile(int hash)
{
    if (pile)
        delete pile;
    pile = new Pile();
    pile->pushCard(Card::hashToCard(hash));
}

// d - drew a card
// e - ended their turn
// index + 1 - index of played card + 1 so it doesn't send 0 in a string
// y - played a black card so next character is the color
// u - if they were at 1 card and said uno
// n - if they were at 1 card and didn't say uno
void
Game::processTurn(QString message_)
{
    std::string message = message_.toStdString();

    int i = 0;

    if (message[i] == 'z') // reshuffle
    {
        i++;
        Card* topCard = pile->pop();
        delete pile;
        pile = new Pile();
        pile->pushCard(topCard);

        int deckSize = (int)message[i];
        i++;

        QVector<int> hashVector(deckSize);

        for (int j = 0; j < deckSize; j++) {
            hashVector[j] = (int)message[i];
            i++;
        }
        loadDeck(hashVector);
    } else {

        if (message[i] == 'd') // draw
        {
            getCurrentPlayer()->drawCard();
            i++;
        }
        if (message[i] != 'e') {
            Player* currentPlayer = getCurrentPlayer();
            int cardIndex = (int)message[i] - 1;
            i++;
            auto cards = currentPlayer->getCards();

            Card* card = cards[cardIndex];

            currentPlayer->playCard(cardIndex);

            if (card->getCardNum() == 7 &&
                gameMode == GAME_MODE::ZEROSANDSEVENS) {
                int chosenPlayer = (int)(message[i]) - 1;
                setChosenPlayer(chosenPlayer);

                currentPlayer->swapCards(getChosenPlayer());
                i++;
            }

            emitRender();

            if (message[i] == 'y') {
                i++;
                setWildColor(Card::charColor(message[i]));
                i++;
            }

            if (currentPlayer->getTotalCards() == 1 && message[i] == 'n') {
                currentPlayer->drawCard(2);
            }

            emitRender();

            if (currentPlayer->getTotalCards() == 1) {
                emit signalBotSaidUno(currentPlayer->getName());
            }

            if (getTopCard()->getColor() == Card::COLOR::BLACK) {
                msleep(750);
                emitWild();
                msleep(750);
            }

            if (gameMode == GAME_MODE::ELIMINATION &&
                !currentPlayer->hasCards() && notEliminated() > 2) {
                currentPlayer->previousPlayer->eliminate();
                currentPlayer->previousPlayer->position = notEliminated();
                currentPlayer->swapCards(currentPlayer->previousPlayer);
            } else if (!currentPlayer->hasCards()) {
                setWinner(currentPlayer);
                end();
            }
        } else {
            if (gameMode == GAME_MODE::DEADLY) {
                if (!getCurrentPlayer()->hasPlayableCards()) {
                    getCurrentPlayer()->eliminate();
                    getCurrentPlayer()->position = notEliminated();
                }
            }
        }
    }
}

void
Game::emitRender()
{
    emit signalRenderTable(getTopCard());
}

void
Game::emitWild()
{
    emit emitterChosenWildColor();
}

void
Game::emitWinner()
{
    emit signalGameEnded();
}

void
Game::playZero()
{
    if (gameDirection == 1) {
        QVector<Card*> tmpCards = players.back()->getCards();
        for (int i = players.size() - 1; i > 0; i--) {
            players[i]->takeCards(players[i - 1]);
        }
        players[0]->setHand(tmpCards);
    } else {
        QVector<Card*> tmpCards = players[0]->getCards();
        for (int i = 0; i < players.size() - 1; i++) {
            players[i]->takeCards(players[i + 1]);
        }
        players.back()->setHand(tmpCards);
    }
}

void
Game::setChosenPlayer(int index)
{
    _chosenPlayer = index;
}

auto
Game::getChosenPlayerIndex() -> int
{
    return _chosenPlayer;
}

auto
Game::getChosenPlayer() -> Player*
{
    return players[_chosenPlayer];
}

auto
Game::eliminationsOn() -> bool
{
    return eliminations;
}

auto
Game::notEliminated() -> int
{
    if (eliminations) {
        int i = 0;
        for (auto player : players)
            if (!player->isEliminated())
                i++;
        return i;
    } else
        return _numOfPlayers;
}
