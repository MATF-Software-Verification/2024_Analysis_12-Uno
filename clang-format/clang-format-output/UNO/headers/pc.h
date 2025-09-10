#ifndef PC_H
#define PC_H

#include "headers/card.h"
#include "headers/game.h"
#include "headers/player.h"

#include <QString>
#include <QVector>

class PC : public Player
{
  public:
    PC();
    PC(QString name, Game* game);
    virtual ~PC();

    Game* getCurrentGame() const;

    bool playCard(const int& cardIndex) override;

    // if there's a playable card, returns index of decided card, else returns
    // -1
    int makeDecision();

    void drawCard(const int& howMany = 1) override;

    //    Player *choosePlayer() override;

    //    int chooseCardIndex() override;

    // PC plays out their turn
    QString play() override;

    Card::COLOR chooseColor() override;

    bool isBot() const override;

    bool hasPlayableCards() override;

    QVector<Card*> getPlayableCards();

  private:
    Game* _game;
};

#endif // PC_H
