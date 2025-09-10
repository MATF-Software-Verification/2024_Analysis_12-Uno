#include "headers/card.h"
#include "headers/game.h"
#include "headers/wildcard.h"

#include <iostream>

#define NON_NUMERIC -1

Card::Card() = default;

Card::Card(COLOR color, CARD_TYPE cardType, int cardNum)
  : color(color)
  , cardType(cardType)
  , cardNum(cardNum)
{
    if ((color == Card::BLACK && cardType == Card::NUMERIC) ||
        (color == Card::BLACK && cardNum != NON_NUMERIC))
        throw "Black cards don't have numbers!";

    if (color == Card::BLACK && cardType != Card::WILD &&
        cardType != Card::WILD_DRAW_FOUR)
        throw "Black card type must either be WILD or WILD_DRAW_FOUR!";

    if ((color != Card::BLACK && cardType == Card::WILD) ||
        (color != Card::BLACK && cardType == Card::WILD_DRAW_FOUR))
        throw "Non black cards can't have WILD or WILD_DRAW_FOUR types!";

    if ((cardType == Card::NUMERIC && cardNum > 9) ||
        (cardType == Card::NUMERIC && cardNum < 0))
        throw "Numeric cards can't have numeric values greater then 9 or "
              "less "
              "then 0!";

    if (cardType != Card::NUMERIC && cardNum != NON_NUMERIC)
        throw "Non numeric cards mustn't have a number value!";
}

Card::Card(COLOR color, int cardNum)
  : color(color)
  , cardType(CARD_TYPE::NUMERIC)
  , cardNum(cardNum)
{
    if (color == Card::BLACK && cardNum != NON_NUMERIC)
        throw "Black cards don't have numbers!";

    if ((color != Card::BLACK && cardNum < 0) ||
        (color != Card::BLACK && cardNum > 9))
        throw "Numeric cards can't have numeric values greater then 9 or "
              "less "
              "then 0!";
}

Card::Card(COLOR color, CARD_TYPE cardType)
  : color(color)
  , cardType(cardType)
  , cardNum(NON_NUMERIC)
{
    if (color == Card::BLACK && cardType != Card::WILD &&
        cardType != Card::WILD_DRAW_FOUR)
        throw "Black card type must either be WILD or WILD_DRAW_FOUR!";

    if ((color != Card::BLACK && cardType == Card::WILD) ||
        (color != Card::BLACK && cardType == Card::WILD_DRAW_FOUR))
        throw "Non black cards can't have WILD or WILD_DRAW_FOUR types!";
}

Card::~Card() = default;

auto
Card::charColor(const char& c) -> Card::COLOR
{
    switch (c) {
        case 'r':
            return COLOR::RED;
        case 'y':
            return COLOR::YELLOW;
        case 'b':
            return COLOR::BLUE;
        default:
            return COLOR::GREEN;
    }
}

auto
Card::colorChar(const COLOR& clr) -> char
{
    switch (clr) {
        case COLOR::RED:
            return 'r';
        case COLOR::YELLOW:
            return 'y';
        case COLOR::BLUE:
            return 'b';
        default:
            return 'g';
    }
}

auto
Card::getColor() const -> Card::COLOR
{
    return color;
}

auto
Card::getCardType() const -> Card::CARD_TYPE
{
    return cardType;
}

auto
Card::getCardNum() const -> int
{
    return cardNum;
}

void
Card::setColor(COLOR newColor)
{
    color = newColor;
}

void
Card::setCardType(CARD_TYPE newCardType)
{
    cardType = newCardType;
}

void
Card::setCardNum(int newCardNum)
{
    cardNum = newCardNum;
}

auto
Card::operator==(const Card& rhs) const -> bool
{
    return color == rhs.color && cardType == rhs.cardType &&
           cardNum == rhs.cardNum;
}

auto
Card::operator!=(const Card& other) const -> bool
{
    return !(*this == other);
}

auto
Card::isPlayable(void* pointer) const -> bool
{

    Game* game = (Game*)pointer;

    if (game->getTopCard()->getColor() == COLOR::BLACK) {
        return color == game->getWildColor();
    }

    if (color == game->getTopCard()->getColor()) {
        return true;
    } else if (cardType == CARD_TYPE::NUMERIC) {
        return cardNum == game->getTopCard()->getCardNum();
    } else {
        return game->getTopCard()->getCardType() == cardType;
    }
}

void
Card::play(void* pointer)
{
    Game* game = (Game*)pointer;

    switch (cardType) {
        case CARD_TYPE::SKIP:
            game->nextPlayer();
            break;
        case CARD_TYPE::REVERSE:
            game->swapDirection();
            break;
        case CARD_TYPE::DRAW_TWO:
            game->getNextPlayer()->drawCard(2);
            game->nextPlayer();
            break;
        case CARD_TYPE::NUMERIC:
            if (game->gameMode == Game::GAME_MODE::ZEROSANDSEVENS) {
                if (cardNum == 0)
                    game->playZero();
            }
            break;
        default:
            break;
    }
}

// RED - 10..
// YELLOW - 30..
// BLUE - 50..
// GREEN - 70..
//
// NUMERIC - add number value
// SKIP - add 10
// REVERSE - add 11
// DRAW TWO - add 12

// wildcards are 1 and 2

auto
Card::toHash() -> int
{
    int base = 0, add = 0;

    switch (color) {
        case COLOR::RED:
            base = 10;
            break;
        case COLOR::YELLOW:
            base = 30;
            break;
        case COLOR::BLUE:
            base = 50;
            break;
        case COLOR::GREEN:
            base = 70;
            break;
        default:
            break;
    }
    switch (cardType) {
        case CARD_TYPE::NUMERIC:
            add = cardNum;
            break;
        case CARD_TYPE::SKIP:
            add = 10;
            break;
        case CARD_TYPE::REVERSE:
            add = 11;
            break;
        case CARD_TYPE::DRAW_TWO:
            add = 12;
            break;
        default:
            break;
    }

    return base + add;
}

auto
Card::hashToCard(const int& code) -> Card*
{
    COLOR clr = Card::BLUE;
    int base = 0;

    if (code < 10) // WILDCARD
    {
        if (code == 1) {
            return new WildCard(CARD_TYPE::WILD);
        } else if (code == 2) {
            return new WildCard(CARD_TYPE::WILD_DRAW_FOUR);
        }
    } else if (code < 30) // RED CARD
    {
        clr = COLOR::RED;
        base = 10;
    } else if (code < 50) // YELLOW CARD
    {
        clr = COLOR::YELLOW;
        base = 30;
    } else if (code < 70) // BLUE CARD
    {
        clr = COLOR::BLUE;
        base = 50;
    } else // GREEN CARD
    {
        clr = COLOR::GREEN;
        base = 70;
    }

    if (code - base == 11) // REVERSE
    {
        return new Card(clr, CARD_TYPE::REVERSE);
    } else if (code - base == 10) // SKIP
    {
        return new Card(clr, CARD_TYPE::SKIP);
    } else if (code - base == 12) // DRAW TWO
    {
        return new Card(clr, CARD_TYPE::DRAW_TWO);
    } else // NUMERIC
    {
        return new Card(clr, code - base);
    }
}

auto
Card::toString() const -> std::string
{
    std::string clr;

    switch (color) {
        case COLOR::RED:
            clr = "Red";
            break;
        case COLOR::YELLOW:
            clr = "Yellow";
            break;
        case COLOR::BLUE:
            clr = "Blue";
            break;
        case COLOR::GREEN:
            clr = "Green";
            break;
        default:
            clr = "Black";
    }

    std::string type;

    switch (cardType) {
        case CARD_TYPE::NUMERIC:
            type = std::to_string(cardNum);
            break;
        case CARD_TYPE::SKIP:
            type = "Skip";
            break;
        case CARD_TYPE::REVERSE:
            type = "Reverse";
            break;
        case CARD_TYPE::DRAW_TWO:
            type = "Draw two";
            break;
        case CARD_TYPE::WILD:
            type = "Wild card";
            break;
        default:
            type = "Wild draw four";
    }

    return "[ " + clr + ", " + type + " ]";
}

// rough first implementation just so it runs
auto
operator<<(std::ostream& os, const Card& cd) -> std::ostream&
{
    os << cd.toString();
    return os;
}
