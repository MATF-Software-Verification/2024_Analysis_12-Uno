#include "../../UNO/headers/deck.h"
#include "../../UNO/headers/game.h"
#include "catch.hpp"

TEST_CASE("Testing correctness of methods in class Deck", "[deck]")
{
    SECTION("Newly created deck must have 108 cards")
    {
        // Arrange
        Deck* deck = new Deck();

        // Act + Assert
        REQUIRE(deck->getDeckSize() == 108);

        // Cleanup
        delete deck;
    }

    SECTION("After drawing a card, the deck will have 1 card less")
    {
        // Arrange
        Deck* deck = new Deck();

        // Act
        int before = deck->getDeckSize();
        Card* card = deck->drawCard();
        int after = deck->getDeckSize();

        // Assert
        REQUIRE(before - 1 == after);

        // Cleanup
        delete deck;
        delete card;
    }

    SECTION("The first card drawn will not be a black card")
    {
        // Arrange
        Deck* deck = new Deck();

        // Act
        Card* card = deck->drawFirstCard();

        // Assert
        REQUIRE(card->getColor() != Card::COLOR::BLACK);

        // Cleanup
        delete deck;
        delete card;
    }

    SECTION("The first card drawn will not be a 'draw two' card")
    {
        // Arrange
        Deck* deck = new Deck();

        // Act
        Card* card = deck->drawFirstCard();

        // Assert
        REQUIRE(card->getCardType() != Card::CARD_TYPE::DRAW_TWO);

        // Cleanup
        delete deck;
        delete card;
    }

    SECTION("The first card drawn will not be a 'skip' card")
    {
        // Arrange
        Deck* deck = new Deck();

        // Act
        Card* card = deck->drawFirstCard();

        // Assert
        REQUIRE(card->getCardType() != Card::CARD_TYPE::SKIP);

        // Cleanup
        delete deck;
        delete card;
    }

    SECTION("The first card drawn will not be a 'reverse' card")
    {
        // Arrange
        Deck* deck = new Deck();

        // Act
        Card* card = deck->drawFirstCard();

        // Assert
        REQUIRE(card->getCardType() != Card::CARD_TYPE::REVERSE);

        // Cleanup
        delete deck;
        delete card;
    }

    SECTION("The first card drawn will be a 'numeric' card")
    {
        // Arrange
        Deck* deck = new Deck();

        // Act
        Card* card = deck->drawFirstCard();

        // Assert
        REQUIRE(card->getCardType() == Card::CARD_TYPE::NUMERIC);

        // Cleanup
        delete deck;
        delete card;
    }

    SECTION("The hash vector of a deck will have the same size as the deck")
    {
        // Arrange
        Deck* deck = new Deck();

        // Act
        QVector<int> hashVector = deck->toHash();

        // Assert
        REQUIRE(deck->getDeckSize() == hashVector.size());

        // Cleanup
        delete deck;
    }

    SECTION("The deck will have the same size as the hash vector of size n")
    {
        // Arrange
        Deck* deck = new Deck();
        QVector<int> hashVector;

        // Act
        hashVector.push_back(1);
        hashVector.push_back(11);
        hashVector.push_back(33);
        hashVector.push_back(60);
        deck->hashToDeck(hashVector);

        // Assert
        REQUIRE(deck->getDeckSize() == hashVector.size());

        // Cleanup
        delete deck;
    }

    SECTION("After dealing cards when initialized by Game, the deck will have "
            "107 - 7 * numOfPlayers cards")
    {
        // Arrange
        GameParameters* gameParams = new GameParameters();
        gameParams->mainPlayerName = "Player1";
        gameParams->numOfPlayers = 4;
        gameParams->isHost = false;
        gameParams->gameMode = "1";

        // Act
        Game* game = new Game(nullptr, gameParams);

        // Assert
        REQUIRE(game->getDeckSize() == 107 - 7 * game->getNumberOfPlayers());

        // Cleanup
        delete game;
    }

    SECTION("IsEmpty will return false for newly created deck")
    {
        // Arrange
        Deck* deck = new Deck();

        // Act + Assert
        REQUIRE(!deck->isEmpty());

        // Cleanup
        delete deck;
    }

    SECTION("IsEmpty will return true if the deck is empty")
    {
        // Arrange
        Deck* deck = new Deck();
        int deckSize = deck->getDeckSize();

        // Act
        for (int i = 0; i < deckSize; i++) {
            // delete cards so we don't need to store them
            delete deck->drawCard();
        }

        // Assert
        REQUIRE(deck->isEmpty());

        // Cleanup
        delete deck;
    }

    SECTION("The deck will be empty after hashToDeck with an empty hashVector")
    {
        // Arrange
        Deck* deck = new Deck();
        QVector<int> hashVector;

        // Act
        deck->hashToDeck(hashVector);

        // Assert
        REQUIRE(deck->isEmpty());

        // Cleanup
        delete deck;
    }

    SECTION("The deck will not change the card through hashToDeck")
    {
        // Arrange
        Deck* deck = new Deck();
        QVector<int> hashVector;
        Card* card = new Card(Card::COLOR::BLUE, 6);

        // Act
        hashVector.push_back(card->toHash());
        deck->hashToDeck(hashVector);
        Card* newCard = deck->drawCard();

        // Assert
        REQUIRE(*newCard == *card);

        // Cleanup
        delete deck;
        delete card;
        delete newCard;
    }
}
