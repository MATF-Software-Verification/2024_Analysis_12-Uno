#include "../../UNO/headers/pile.h"
#include "catch.hpp"

#define DECK_SIZE 108

TEST_CASE("Testing correctness of methods in class Pile", "[pile]")
{
    SECTION("Newly created pile must be empty")
    {
        // Arrange
        Pile* new_pile = new Pile();

        // Act + Assert
        REQUIRE(new_pile->getSize() == 0);

        // Cleanup
        delete new_pile;
    }

    SECTION("Size of the pile cannot exceed the size of the deck")
    {
        // Arrange
        Pile* newPile = new Pile();

        // Act + Assert
        REQUIRE(newPile->getSize() <= DECK_SIZE);

        // Cleanup
        delete newPile;
    }

    SECTION("If the pile is full, method pushCard() should"
            "throw an exception")
    {
        // Arrange
        Pile* newPile = new Pile();
        Card* card = new Card();

        // Act
        for (int i = 0; i < DECK_SIZE; i++)
            newPile->pushCard(card);

        // Assert
        REQUIRE_THROWS(newPile->pushCard(card));
    }

    SECTION("If the pile is not full, method pushCard() should add"
            "card to the pile")
    {
        // Arrange
        Pile* newPile = new Pile();
        Card* card = new Card(Card::GREEN, Card::NUMERIC, 7);
        const auto expectedSize = 1;

        // Act
        newPile->pushCard(card);
        const auto actualSize = newPile->getSize();

        // Assert
        REQUIRE(actualSize == expectedSize);

        // Cleanup
        delete newPile;
    }

    SECTION("First card on the pile must have a numeric type")
    {
        // Arrange
        Card* card = new Card();

        // Act + Assert
        REQUIRE(card->getCardType() == Card::NUMERIC);

        // Cleanup
        delete card;
    }

    SECTION("If the pile is empty, method getSize() should return 0")
    {
        // Arrange
        Pile* newPile = new Pile();
        const auto expectedSize = 0;

        // Act
        const auto actualSize = newPile->getSize();

        // Assert
        REQUIRE(actualSize == expectedSize);

        // Cleanup
        delete newPile;
    }

    SECTION("If the pile has n elements, method getSize() should return n")
    {
        // Arrange
        Pile* newPile = new Pile();
        Card* card1 = new Card(Card::YELLOW, Card::NUMERIC, 5);
        Card* card2 = new Card(Card::BLUE, Card::NUMERIC, 2);
        newPile->pushCard(card1);
        newPile->pushCard(card2);

        const auto expectedSize = 2;

        // Act
        const auto actualSize = newPile->getSize();

        // Assert
        REQUIRE(actualSize == expectedSize);

        // Cleanup
        delete newPile;
    }

    SECTION(
      "If the pile is empty, method lastCard() should return a blank card")
    {
        // Arrange
        Pile* newPile = new Pile();
        Card* card = new Card();
        const auto expectedSize = 0;

        // Act
        Card* lastCard = newPile->lastCard();
        const auto actualSize = newPile->getSize();

        // Assert
        REQUIRE(actualSize == expectedSize);
        REQUIRE(lastCard->getCardType() == card->getCardType());

        // Cleanup
        delete card;
        delete newPile;
    }

    SECTION("If the pile has n cards, method lastCard() returns the last card")
    {
        // Arrange
        Pile* newPile = new Pile();
        Card* card1 = new Card(Card::YELLOW, Card::NUMERIC, 5);
        newPile->pushCard(card1);

        // Act
        Card* lastCard = newPile->lastCard();

        // Assert
        REQUIRE(lastCard == card1);

        // Cleanup
        delete newPile;
    }

    SECTION("Method getWildColor() should return the current wild color")
    {
        // Arrange
        Pile* newPile = new Pile();
        newPile->setWildColor(Card::COLOR::RED);
        const auto expectedColor = Card::COLOR::RED;

        // Act
        const auto actualColor = newPile->getWildColor();

        // Assert
        REQUIRE(actualColor == expectedColor);

        // Cleanup
        delete newPile;
    }

    SECTION("Method setWildColor() should change the wild color"
            "to the given color")
    {
        // Arrange
        Pile* newPile = new Pile();

        // Act
        newPile->setWildColor(Card::COLOR::BLUE);

        // Assert
        REQUIRE(newPile->getWildColor() == Card::COLOR::BLUE);

        // Cleanup
        delete newPile;
    }

    SECTION("If the pile is empty, method isEmpty() should return true")
    {
        // Arrange
        Pile* newPile = new Pile();
        bool expectedValue = true;

        // Act
        bool actualValue = newPile->isEmpty();

        // Assert
        REQUIRE(actualValue == expectedValue);

        // Cleanup
        delete newPile;
    }

    SECTION("If the pile is not empty, method isEmpty() should return false")
    {
        // Arrange
        Pile* newPile = new Pile();
        Card* card = new Card(Card::YELLOW, Card::NUMERIC, 5);
        newPile->pushCard(card);
        bool expectedValue = false;

        // Act
        bool actualValue = newPile->isEmpty();

        // Assert
        REQUIRE(actualValue == expectedValue);

        // Cleanup
        delete newPile;
    }

    SECTION("If the pile has n cards, method clear() should remove all except"
            "the last card from the pile")
    {
        // Arrange
        Pile* newPile = new Pile();
        Card* card1 = new Card(Card::YELLOW, Card::NUMERIC, 5);
        Card* card2 = new Card(Card::GREEN, Card::NUMERIC, 0);
        newPile->pushCard(card1);
        newPile->pushCard(card2);

        const auto expectedSize = 1;

        // Act
        newPile->clear();
        const auto actualSize = newPile->getSize();

        // Assert
        REQUIRE(actualSize == expectedSize);
        REQUIRE(card2 == newPile->lastCard());

        // Cleanup
        delete newPile;
    }

    SECTION(
      "If the pile is empty, we expect method pop() to throw an exception")
    {
        // Arrange
        Pile* newPile = new Pile();

        // Act + Assert
        REQUIRE_THROWS(newPile->pop());

        // Cleanup
        delete newPile;
    }

    SECTION(
      "If the pile is not empty, method pop() removes the last card from the"
      "pile")
    {
        // Arrange
        Pile* newPile = new Pile();
        Card* card1 = new Card(Card::YELLOW, Card::NUMERIC, 5);
        Card* card2 = new Card(Card::GREEN, Card::NUMERIC, 0);
        newPile->pushCard(card1);
        newPile->pushCard(card2);
        const auto expectedSize = 1;

        // Act
        Card* card = newPile->pop();
        const auto actualSize = newPile->getSize();

        // Assert
        REQUIRE(actualSize == expectedSize);
        REQUIRE(card == card2);

        // Cleanup
        delete newPile;
        delete card;
    }
}
