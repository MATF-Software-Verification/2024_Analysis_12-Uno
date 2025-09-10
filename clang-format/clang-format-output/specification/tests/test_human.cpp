#include "../../UNO/headers/human.h"
#include "catch.hpp"

TEST_CASE("Testing correctness of methods in class Human", "[human]")
{
    SECTION("Human player is not a bot player, so method isBot() should"
            "return false")
    {
        // Arrange
        Human* human = new Human();

        // Act
        const auto expected = false;

        // Assert
        REQUIRE(human->isBot() == expected);

        // Cleanup
        delete human;
    }

    SECTION("Method chooseColor() should return BLUE card color")
    {
        // Arrange
        Human* human = new Human();

        // Act
        Card::COLOR expected = Card::COLOR::BLUE;

        // Assert
        REQUIRE(human->chooseColor() == expected);

        // Cleanup
        delete human;
    }
}
