#ifndef HUMAN_H
#define HUMAN_H

#include "headers/game.h"

class Human : public Player
{
  public:
    Human();
    Human(QString name, Game* game);
    virtual ~Human();

    Game* game() const;
    void setGame(Game* newGame);

    void drawCard(const int& howMany = 1) override;

    bool isBot() const override;

    // done by GUI logic
    bool playCard(const int& cardIndex) override;
    Card::COLOR chooseColor() override;
    QString play() override;
    bool hasPlayableCards() override;

  private:
    Game* _game;
};

#endif // HUMAN_H
