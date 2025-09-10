#include "../../UNO/headers/card.h"
#include "catch.hpp"

TEST_CASE("Testing correctness of methods in class Card", "[card]")
{
    SECTION("If a black card is created with a number specified, we expect an "
            "exception to be thrown")
    {
        // arrange + act + assert
        REQUIRE_THROWS(new Card(Card::BLACK, 9));
    }

    SECTION(
      "If a black card is created with a type different from both WILD and "
      "WILD_DRAW_FOUR, we expect an "
      "exception to be thrown")
    {
        // arrange + act + assert
        REQUIRE_THROWS(new Card(Card::BLACK, Card::SKIP));
    }

    SECTION("If a non black card is created with a type that is either WILD or "
            "WILD_DRAW_FOUR, we expect an "
            "exception to be thrown")
    {
        // arrange + act + assert
        CHECK_THROWS(new Card(Card::BLUE, Card::WILD_DRAW_FOUR));
        REQUIRE_THROWS(new Card(Card::RED, Card::WILD));
    }

    SECTION("If a numeric card is created with a numeric value greater than 9 "
            "or less then 0 we expect an "
            "exception to be thrown")
    {
        // arrange + act + assert
        CHECK_THROWS(new Card(Card::BLUE, Card::NUMERIC, 10));
        REQUIRE_THROWS(new Card(Card::BLUE, Card::NUMERIC, -3));
    }

    SECTION("If a non numeric card is created with a numer we expect an "
            "exception to be thrown")
    {
        // arrange + act + assert
        REQUIRE_THROWS(new Card(Card::RED, Card::SKIP, 5));
    }

    SECTION("If a card is created with a number specified, it's type should be "
            "NUMERIC")
    {
        // arrange
        Card* card = new Card(Card::BLUE, 9);

        // act + assert
        REQUIRE(card->getCardType() == Card::NUMERIC);

        // cleanup
        delete card;
    }

    SECTION("If a card has a card type other then numeric, it's number should "
            "be -1")
    {
        // arrange
        Card* card = new Card(Card::RED, Card::SKIP);

        // act + assert
        REQUIRE(card->getCardNum() == -1);

        // cleanup
        delete card;
    }

    SECTION("If two cards have different numbers then they must be different "
            "cards even if they "
            "have the same color")
    {
        // arrange
        Card* card1 = new Card(Card::RED, 5);
        Card* card2 = new Card(Card::RED, 7);

        // act + assert
        REQUIRE(*card1 != *card2);

        // cleanup
        delete card1;
        delete card2;
    }

    SECTION("If two cards have different colors then they must be different "
            "cards even if they "
            "have the same number")
    {
        // arrange
        Card* card1 = new Card(Card::BLUE, 5);
        Card* card2 = new Card(Card::RED, 5);

        // act + assert
        REQUIRE(*card1 != *card2);

        // cleanup
        delete card1;
        delete card2;
    }

    SECTION("If two cards are created with the same color and number then they "
            "must have the same string representation")
    {
        // arrange
        Card* card1 = new Card(Card::BLUE, 7);
        Card* card2 = new Card(Card::BLUE, 7);

        // act + assert
        REQUIRE(card1->toString() == card2->toString());

        // cleanup
        delete card1;
        delete card2;
    }

    SECTION("If two cards are created with the same color and type then they "
            "must have the same string representation")
    {
        // arrange
        Card* card1 = new Card(Card::BLUE, Card::REVERSE);
        Card* card2 = new Card(Card::BLUE, Card::REVERSE);

        // act + assert
        REQUIRE(card1->toString() == card2->toString());

        // cleanup
        delete card1;
        delete card2;
    }

    SECTION("If two cards have the same color and number then they must have "
            "the same hash numbers")
    {
        // arrange
        Card* card1 = new Card(Card::BLUE, 5);
        Card* card2 = new Card(Card::BLUE, 5);

        // act
        int hash1 = card1->toHash();
        int hash2 = card2->toHash();

        // assert
        REQUIRE(hash1 == hash2);

        // cleanup
        delete card1;
        delete card2;
    }

    SECTION("If two cards have the same color and type then they must have the "
            "same hash numbers")
    {
        // arrange
        Card* card1 = new Card(Card::RED, Card::SKIP);
        Card* card2 = new Card(Card::RED, Card::SKIP);

        // act
        int hash1 = card1->toHash();
        int hash2 = card2->toHash();

        // assert
        REQUIRE(hash1 == hash2);

        // cleanup
        delete card1;
        delete card2;
    }

    SECTION("If two cards have the same color but different number then they "
            "must have different hash numbers")
    {
        // arrange
        Card* card1 = new Card(Card::BLUE, 5);
        Card* card2 = new Card(Card::BLUE, 6);

        // act
        int hash1 = card1->toHash();
        int hash2 = card2->toHash();

        // assert
        REQUIRE(hash1 != hash2);

        // cleanup
        delete card1;
        delete card2;
    }

    SECTION("If two cards have the same number but different color then they "
            "must have different hash numbers")
    {
        // arrange
        Card* card1 = new Card(Card::BLUE, 5);
        Card* card2 = new Card(Card::RED, 5);

        // act
        int hash1 = card1->toHash();
        int hash2 = card2->toHash();

        // assert
        REQUIRE(hash1 != hash2);

        // cleanup
        delete card1;
        delete card2;
    }

    SECTION("If two cards have the same color but different type then they "
            "must have different hash numbers")
    {
        // arrange
        Card* card1 = new Card(Card::RED, Card::DRAW_TWO);
        Card* card2 = new Card(Card::RED, Card::SKIP);

        // act
        int hash1 = card1->toHash();
        int hash2 = card2->toHash();

        // assert
        REQUIRE(hash1 != hash2);

        // cleanup
        delete card1;
        delete card2;
    }

    SECTION("If two cards have different color but same type then they must "
            "have different hash numbers")
    {
        // arrange
        Card* card1 = new Card(Card::RED, Card::SKIP);
        Card* card2 = new Card(Card::BLUE, Card::SKIP);

        // act
        int hash1 = card1->toHash();
        int hash2 = card2->toHash();

        // assert
        REQUIRE(hash1 != hash2);

        // cleanup
        delete card1;
        delete card2;
    }

    SECTION("If two cards are created using the same hash number then they "
            "must be the same cards")
    {
        // arrange
        Card* card1 = Card::hashToCard(21);
        Card* card2 = Card::hashToCard(21);

        // act + assert
        REQUIRE(*card1 == *card2);

        // cleanup
        delete card1;
        delete card2;
    }

    SECTION("If two cards are created using the same hash number then they "
            "must have the same string representation")
    {
        // arrange
        Card* card1 = Card::hashToCard(33);
        Card* card2 = Card::hashToCard(33);

        // act + assert
        REQUIRE(card1->toString() == card2->toString());

        // cleanup
        delete card1;
        delete card2;
    }
}
