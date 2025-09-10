#include "../../UNO/headers/game.h"
#include "catch.hpp"

TEST_CASE("Testing the correctness of methods in class Game", "[game]")
{
    SECTION("If a game is created with no parameters, we expect an"
            "exception to be thrown")
    {
        // Arrange + Act + Assert
        REQUIRE_THROWS(new Game());
    }

    SECTION("If the deck is empty, method drawCard() reffils it from the pile,"
            "then returns the last card in the deck")
    {
        // Arrange
        Pile* newPile = new Pile();
        Card* card1 = new Card(Card::YELLOW, Card::NUMERIC, 5);
        Card* card2 = new Card(Card::GREEN, Card::NUMERIC, 0);
        newPile->pushCard(card1);
        newPile->pushCard(card2);
        unsigned expectedSize = newPile->getSize() - 1;

        Deck* newDeck = new Deck();
        unsigned deckSize = newDeck->getDeckSize();

        // Act
        for (unsigned i = 0; i < deckSize; i++)
            card1 = newDeck->drawCard();

        newDeck->refill(newPile);
        unsigned actualSize = newDeck->getDeckSize();

        // Assert
        REQUIRE(actualSize == expectedSize);

        // Cleanup
        delete newDeck;
    }

    SECTION("If the deck is not empty, method drawCard() removes one card from "
            "the deck")
    {
        // Arrange
        Deck* newDeck = new Deck();
        unsigned originalSize = newDeck->getDeckSize();
        Card* card = newDeck->drawCard();

        // Act
        unsigned reducedSize = newDeck->getDeckSize();

        // Assert
        REQUIRE(reducedSize == (originalSize - 1));

        // Cleanup
        delete card;
        delete newDeck;
    }

    SECTION(
      "If the deck is not empty, method isDeckEmpty() should return false")
    {
        // Arrange
        Deck* newDeck = new Deck();
        const auto expected = false;

        // Act
        const auto actual = newDeck->isEmpty();

        // Assert
        REQUIRE(actual == expected);

        // Cleanup
        delete newDeck;
    }

    SECTION("If the deck is empty, method isDeckEmpty() should return true")
    {
        // Arrange
        Deck* newDeck = new Deck();
        const auto expected = true;
        Card* card = new Card();
        unsigned deckSize = newDeck->getDeckSize();

        // Act
        for (unsigned i = 0; i < deckSize; i++)
            card = newDeck->drawCard();

        const auto actual = newDeck->isEmpty();

        // Assert
        REQUIRE(actual == expected);

        // Cleanup
        delete card;
        delete newDeck;
    }
}
