#ifndef CARD_H
#define CARD_H

#include <QString>
#include <ostream>

class Card
{
  public:
    enum COLOR
    {
        RED,
        YELLOW,
        BLUE,
        GREEN,
        BLACK
    };
    enum CARD_TYPE
    {
        NUMERIC,
        SKIP,
        REVERSE,
        DRAW_TWO,
        WILD,
        WILD_DRAW_FOUR
    };

    Card();
    Card(COLOR color, CARD_TYPE cardType, int cardNum);
    Card(COLOR color, int cardNum);
    Card(COLOR color, CARD_TYPE cardType);

    virtual ~Card();

    // returns char representation for color (only non wild cards)
    static char colorChar(const COLOR& clr);

    static COLOR charColor(const char& c);

    // returns integer hash for card
    virtual int toHash();

    // returns created card from hash
    static Card* hashToCard(const int& code);

    virtual bool isPlayable(void*) const;
    virtual void play(void*);

    // getters
    COLOR getColor() const;
    CARD_TYPE getCardType() const;
    int getCardNum() const;

    // setters
    void setColor(COLOR newColor);
    void setCardType(CARD_TYPE newCardType);
    void setCardNum(int newCardNum);

    // comparators
    bool operator==(const Card& rhs) const;
    bool operator!=(const Card& rhs) const;

    std::string toString() const;

    friend std::ostream& operator<<(std::ostream& os, const Card& cd);

  private:
    COLOR color;
    CARD_TYPE cardType;
    int cardNum;
};

#endif // CARD_H
