#include "../../UNO/headers/pc.h"
#include "catch.hpp"

TEST_CASE("Testing correctness of methods in class PC", "[pc]")
{
    SECTION(
      "PC player is a bot player, so he must return true to isBot() method")
    {
        // Arrange
        PC* bot = new PC();

        // Act
        bool _isBot = true;

        // Assert
        REQUIRE(bot->isBot() == _isBot);

        // cleanup
        delete bot;
    }
    SECTION("If the bot has the most red cards,method chooseColor() should "
            "return RED card color")
    {
        // Arrange
        PC* bot = new PC();
        Card* card1 = new Card(Card::RED, 5);
        Card* card2 = new Card(Card::RED, 7);
        Card* card3 = new Card(Card::BLUE, 7);
        QVector<Card*> hand = { card1, card2, card3 };
        bot->setHand(hand);

        Card::COLOR expectedOutput = Card::COLOR::RED;

        // Act
        Card::COLOR actualOutput = bot->chooseColor();

        // Assert
        REQUIRE(expectedOutput == actualOutput);

        // Cleanup
        delete bot;
    }
    SECTION("If the bot has equally cards of all colors, method chooseColor "
            "should return color of the last card")
    {
        // Arrange
        PC* bot = new PC();
        Card* card1 = new Card(Card::BLUE, 5);
        Card* card2 = new Card(Card::GREEN, 7);
        Card* card3 = new Card(Card::RED, 7);
        QVector<Card*> hand = { card1, card2, card3 };
        bot->setHand(hand);

        Card::COLOR expectedOutput = Card::COLOR::RED;

        // Act
        Card::COLOR actualOutput = bot->chooseColor();

        // Assert
        REQUIRE(expectedOutput == actualOutput);

        // Cleanup
        delete bot;
    }
}
