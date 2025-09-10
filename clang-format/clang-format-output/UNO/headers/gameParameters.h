#ifndef GAMEPARAMETERS_H
#define GAMEPARAMETERS_H

#include <QString>

class GameParameters
{
  public:
    bool isOnline;
    bool isHost;
    int numOfPlayers;
    QString gameMode;
    QString mainPlayerName;
};

#endif // GAMEPARAMETERS_H
