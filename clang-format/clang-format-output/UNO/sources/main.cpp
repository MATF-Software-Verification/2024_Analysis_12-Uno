#include "headers/game.h"
#include "headers/gameGUI.h"
#include "headers/gameclient.h"
#include "headers/gameserver.h"
#include "headers/mainWindow.h"

#include "headers/gameParameters.h"
#include "headers/lobby.h"

#include <QApplication>

auto
main(int argc, char* argv[]) -> int
{
    QApplication a(argc, argv);
    MainWindow mw;
    mw.show();
    //	GameParameters *gamep = new GameParameters();
    //	Lobby l(nullptr, gamep);
    //	l.show();

    return a.exec();
    /*

    Game *serverSide = new Game();
    Game *clientSide = new Game();

    int x;
    std::cin >> x;
    if(x) //host mode
    {
        GameServer* server = new GameServer(serverSide);
        server->setNumberOfPlayers(4);

        std::thread server_wait([](GameServer* server) {
            server->wait();
        }, server);

        GameClient* client = new GameClient(clientSide, "player", true);

        client->connectToServer("127.0.0.1");
        std::cin >> x; //this should be a button
        server->stopWaiting();

        server_wait.join();

        std::thread server_start([](GameServer* server) {
            server->start();
        }, server);

        client->start();

        server_start.join();

        std::cout << "The winner is: " << serverSide->getWinner()->getName() <<
    std::endl;
    }
    else //client mode
    {
        GameClient* client = new GameClient(clientSide, "player1", false);
        client->connectToServer("127.0.0.1");

        client->start();

        std::cout << "The winner is: " << clientSide->getWinner()->getName() <<
    std::endl;
    }


    return 0;
            */
}
