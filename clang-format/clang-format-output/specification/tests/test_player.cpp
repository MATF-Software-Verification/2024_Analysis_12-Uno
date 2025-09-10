#include "../../UNO/headers/card.h"
#include "../../UNO/headers/human.h"
#include "../../UNO/headers/player.h"
#include "../../UNO/headers/wildcard.h"

#include "catch.hpp"

TEST_CASE("Testing correctness of methods in class Player", "[player]")
{
    SECTION(
      "If player doesn't have a playable card, method hasCard should return -1")
    {
        // arrange
        Player* p = new Human();
        Card* card = new Card(Card::RED, 5);
        Card* card1 = new Card(Card::GREEN, 6);
        Card* card2 = new Card(Card::YELLOW, 7);
        Card* card3 = new Card(Card::BLUE, 8);

        const auto expectedOutput = -1;
        QVector<Card*> hand = { card1, card2, card3 };
        p->setHand(hand);

        // act
        const auto actualOutput = p->hasCard(*card);

        // assert
        REQUIRE(expectedOutput == actualOutput);

        // cleanup
        delete p;
    }

    SECTION("If player has card in hand with same color as forwarded card, "
            "method hasCard should return index card in hand")
    {
        // arrange
        Player* p = new Human();
        Card* card = new Card(Card::RED, 5);
        Card* card1 = new Card(Card::GREEN, 6);
        Card* card2 = new Card(Card::RED, 7);
        Card* card3 = new Card(Card::BLUE, 8);

        const auto expectedOutput = 1;
        QVector<Card*> hand = { card1, card2, card3 };
        p->setHand(hand);

        // act
        const auto actualOutput = p->hasCard(*card);

        // assert
        REQUIRE(expectedOutput == actualOutput);

        // cleanup
        delete p;
        delete card;
    }

    SECTION("If player has wildcard in hand, method hasCard() should return "
            "index wildcard in hand")
    {
        // arrange
        Player* p = new Human();
        Card* card = new Card(Card::RED, 5);
        Card* card1 = new Card(Card::GREEN, 6);
        Card* card2 = new WildCard(Card::WILD);
        Card* card3 = new Card(Card::BLUE, 8);

        const auto expectedOutput = 1;
        QVector<Card*> hand = { card1, card2, card3 };
        p->setHand(hand);

        // act
        const auto actualOutput = p->hasCard(*card);

        // assert
        REQUIRE(expectedOutput == actualOutput);

        // cleanup
        /* delete p;
         delete card;
         delete card1;
         delete card2;
         delete card3;*/
    }

    SECTION("If player has card in hand with same number as forwarded card, "
            "method hasCard should return index card in hand")
    {
        // arrange
        Player* p = new Human();
        Card* card = new Card(Card::RED, 5);
        Card* card1 = new Card(Card::GREEN, 6);
        Card* card2 = new Card(Card::YELLOW, 5);
        Card* card3 = new Card(Card::BLUE, 8);

        const auto expectedOutput = 1;
        QVector<Card*> hand = { card1, card2, card3 };
        p->setHand(hand);

        // act
        const auto actualOutput = p->hasCard(*card);

        // assert
        REQUIRE(expectedOutput == actualOutput);

        // cleanup
        delete p;
        delete card;
    }
    SECTION(
      "If player has cards in the hand, method hasCards() should return true")
    {
        // arrange

        Player* p = new Human();
        Card* card1 = new Card(Card::GREEN, 6);
        Card* card2 = new Card(Card::YELLOW, 5);
        Card* card3 = new Card(Card::BLUE, 8);
        QVector<Card*> hand = { card1, card2, card3 };
        p->setHand(hand);

        const auto expectedOutput = true;

        // act
        const auto actualOutput = p->hasCards();

        // assert
        REQUIRE(expectedOutput == actualOutput);

        // cleanup
        delete p;
    }
    SECTION(
      "If player hasn't cards in hand, method hasCards() should return false")
    {
        // arrange

        Player* p = new Human();

        QVector<Card*> hand = {};
        p->setHand(hand);

        const auto expectedOutput = false;

        // act
        const auto actualOutput = p->hasCards();

        // assert
        REQUIRE(expectedOutput == actualOutput);

        // cleanup
        delete p;
    }
    SECTION(
      "If the hand has n elements, method getTotalCards() should return n")
    {
        // arrange

        Player* p = new Human();
        Card* card1 = new Card(Card::GREEN, 6);
        Card* card2 = new Card(Card::YELLOW, 5);
        Card* card3 = new Card(Card::BLUE, 8);
        QVector<Card*> hand = { card1, card2, card3 };
        p->setHand(hand);

        const auto expectedOutput = hand.size();

        // act
        const auto actualOutput = p->getTotalCards();

        // assert
        REQUIRE(expectedOutput == actualOutput);

        // cleanup
        delete p;
    }

    SECTION("If the hand is empty, method getTotalCards() should return 0")
    {
        // arrange

        Player* p = new Human();

        QVector<Card*> hand = {};
        p->setHand(hand);

        const auto expectedOutput = 0;

        // act
        const auto actualOutput = p->getTotalCards();

        // assert
        REQUIRE(expectedOutput == actualOutput);
        // cleanup
        delete p;
    }

    SECTION("Method getScore() should return player's real score")
    {
        // arrange

        Player* p = new Human();
        int score = 4;

        p->setScore(score);
        const auto expectedOutput = score;

        // act
        const auto actualOutput = p->getScore();

        // assert
        REQUIRE(expectedOutput == actualOutput);

        // cleanup
        delete p;
    }
    SECTION(
      "If hasPlayed is set to true, method hasPlayed() should return true")
    {
        // arrange

        Player* p = new Human();
        bool hasPlayed = true;
        p->setHasPlayed(hasPlayed);

        const auto expectedOutput = hasPlayed;

        // act
        const auto actualOutput = p->hasPlayed();

        // assert
        REQUIRE(expectedOutput == actualOutput);

        // cleanup
        delete p;
    }

    SECTION(
      "If hasPlayed is set to false, method hasPlayed() should return true")
    {
        // arrange

        Player* p = new Human();
        bool hasPlayed = false;
        p->setHasPlayed(hasPlayed);

        const auto expectedOutput = hasPlayed;

        // act
        const auto actualOutput = p->hasPlayed();

        // assert
        REQUIRE(expectedOutput == actualOutput);

        // cleanup
        delete p;
    }
    SECTION("Method getCards() should return all the cards the player has in "
            "his hand")
    {
        // arrange

        Player* p = new Human();
        Card* card1 = new Card(Card::GREEN, 6);
        Card* card2 = new Card(Card::YELLOW, 5);
        Card* card3 = new Card(Card::BLUE, 8);
        QVector<Card*> hand = { card1, card2, card3 };
        p->setHand(hand);

        const auto expectedOutput = hand;

        // act
        const auto actualOutput = p->getCards();

        // assert
        REQUIRE(expectedOutput == actualOutput);

        // cleanup
        delete p;
    }
    SECTION("If hand is empty, method getCards() should return empty vector")
    {
        // arrange

        Player* p = new Human();

        QVector<Card*> hand = {};
        p->setHand(hand);

        const auto expectedOutput = hand;

        // act
        const auto actualOutput = p->getCards();

        // assert
        REQUIRE(expectedOutput == actualOutput);

        // cleanup
        delete p;
    }

    SECTION("Method getPile() sholud return pile")
    {
        // arrange

        Player* p = new Human();
        Pile* pile = new Pile();

        Card* card1 = new Card(Card::GREEN, 6);
        Card* card2 = new Card(Card::YELLOW, 5);
        Card* card3 = new Card(Card::BLUE, 8);
        pile->pushCard(card1);
        pile->pushCard(card2);
        pile->pushCard(card3);

        p->setPile(pile);

        const auto expectedOutput = pile;

        // act
        const auto actualOutput = p->getPile();

        // assert
        REQUIRE(expectedOutput == actualOutput);

        // cleanup
        delete p;
    }

    SECTION("After method swapCards(), player number one cards(hand1) become "
            "player number two cards(hand2)")
    {
        // arrange

        Player* p1 = new Human();
        Card* card1 = new Card(Card::GREEN, 6);
        Card* card2 = new Card(Card::YELLOW, 5);
        Card* card3 = new Card(Card::BLUE, 8);
        QVector<Card*> hand1 = { card1, card2, card3 };
        p1->setHand(hand1);

        Player* p2 = new Human();
        Card* card4 = new Card(Card::YELLOW, 8);
        Card* card5 = new Card(Card::YELLOW, 9);
        Card* card6 = new Card(Card::RED, 8);
        QVector<Card*> hand2 = { card4, card5, card6 };
        p2->setHand(hand2);

        p1->swapCards(p2);

        const auto expectedOutput = hand2;

        // act
        const auto actualOutput = p1->getCards();

        // assert
        REQUIRE(expectedOutput == actualOutput);

        // cleanup
        delete p1;
        delete p2;
    }
    SECTION("After method swapCards(), player number two cards(hand2) become "
            "player number one cards(hand1)")
    {
        // arrange

        Player* p1 = new Human();
        Card* card1 = new Card(Card::GREEN, 6);
        Card* card2 = new Card(Card::YELLOW, 5);
        Card* card3 = new Card(Card::BLUE, 8);
        QVector<Card*> hand1 = { card1, card2, card3 };
        p1->setHand(hand1);

        Player* p2 = new Human();
        Card* card4 = new Card(Card::YELLOW, 8);
        Card* card5 = new Card(Card::YELLOW, 9);
        Card* card6 = new Card(Card::RED, 8);
        QVector<Card*> hand2 = { card4, card5, card6 };
        p2->setHand(hand2);

        p1->swapCards(p2);

        const auto expectedOutput = hand1;

        // act
        const auto actualOutput = p2->getCards();

        // assert
        REQUIRE(expectedOutput == actualOutput);

        // cleanup
        delete p1;
        delete p2;
    }
    SECTION("If we remove card from the hand, method getTotalCards() should "
            "return hand.size()-1")
    {
        // arrange

        Player* p = new Human();
        Card* card1 = new Card(Card::GREEN, 6);
        Card* card2 = new Card(Card::YELLOW, 5);
        Card* card3 = new Card(Card::BLUE, 8);
        QVector<Card*> hand = { card1, card2, card3 };
        p->setHand(hand);
        p->removeCard(card1);

        const auto expectedOutput = hand.size() - 1;

        // act
        const auto actualOutPut = p->getTotalCards();

        // assert
        REQUIRE(expectedOutput == actualOutPut);

        // cleanup
        delete p;
    }
    SECTION("If we obtain card from the deck, method getTotalCards() should "
            "return hand.size()+1")
    {
        // arrange

        Player* p = new Human();
        Card* card1 = new Card(Card::GREEN, 6);
        Card* card2 = new Card(Card::YELLOW, 5);
        Card* card3 = new Card(Card::BLUE, 8);
        Card* card4 = new Card(Card::BLUE, 8);
        QVector<Card*> hand = { card1, card2, card3 };
        p->setHand(hand);
        p->obtainCard(card4);

        const auto expectedOutput = hand.size() + 1;

        // act
        const auto actualOutPut = p->getTotalCards();

        // assert
        REQUIRE(expectedOutput == actualOutPut);

        // cleanup
        delete p;
    }

    SECTION("If we obtain card from the deck, card should be added to the hand")
    {
        // arrange

        Player* p = new Human();
        Card* card1 = new Card(Card::GREEN, 6);
        Card* card2 = new Card(Card::YELLOW, 5);
        Card* card3 = new Card(Card::BLUE, 8);
        Card* card4 = new Card(Card::BLUE, 8);
        QVector<Card*> hand = { card1, card2, card3 };
        p->setHand(hand);
        p->obtainCard(card4);

        QVector<Card*> newHand = { card1, card2, card3, card4 };
        const auto expectedOutput = newHand;

        // act
        const auto actualOutPut = p->hand;

        // assert
        REQUIRE(expectedOutput == actualOutPut);

        // cleanup
        delete p;
    }
}
