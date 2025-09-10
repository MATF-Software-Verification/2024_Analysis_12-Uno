#include "../../UNO/headers/wildcard.h"
#include "catch.hpp"

TEST_CASE("Testing correctness of methods in class WildCard", "[wildcard]")
{
    SECTION("If a WildCard is created with the card type being 'draw two', we "
            "expect an exception to be thrown")
    {
        // Arrange + Act + Assert
        REQUIRE_THROWS(new WildCard(Card::CARD_TYPE::DRAW_TWO));
    }

    SECTION("If a WildCard is created with the card type being 'skip', we "
            "expect an exception to be thrown")
    {
        // Arrange + Act + Assert
        REQUIRE_THROWS(new WildCard(Card::CARD_TYPE::SKIP));
    }

    SECTION("If a WildCard is created with the card type being 'reverse', we "
            "expect an exception to be thrown")
    {
        // Arrange + Act + Assert
        REQUIRE_THROWS(new WildCard(Card::CARD_TYPE::REVERSE));
    }

    SECTION("If a WildCard is created with the card type being 'numeric', we "
            "expect an exception to be thrown")
    {
        // Arrange + Act + Assert
        REQUIRE_THROWS(new WildCard(Card::CARD_TYPE::NUMERIC));
    }

    SECTION("isPlayable is expected to always return true")
    {
        // Arrange
        WildCard* card = new WildCard(Card::CARD_TYPE::WILD);

        // Act + Assert
        REQUIRE(card->isPlayable(nullptr));

        // Cleanup
        delete card;
    }

    SECTION("toHash of WildCard with type 'wild' will return 1")
    {
        // Arrange
        WildCard* card = new WildCard(Card::CARD_TYPE::WILD);

        // Act + Assert
        REQUIRE(card->toHash() == 1);

        // Cleanup
        delete card;
    }

    SECTION("toHash of WildCard with type 'wild draw four' will return 2")
    {
        // Arrange
        WildCard* card = new WildCard(Card::CARD_TYPE::WILD_DRAW_FOUR);

        // Act + Assert
        REQUIRE(card->toHash() == 2);

        // Cleanup
        delete card;
    }
}
