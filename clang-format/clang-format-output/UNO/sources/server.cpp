#include "headers/server.h"
#include "headers/human.h"
#include "headers/pc.h"
#include "headers/wildcard.h"

#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

Server::Server(QObject* parent)
  : QThread(parent)
{
}

Server::Server(QObject* parent, Game* game)
  : QThread(parent)
  , _game(game)
{
    _stopWaiting = false;
}

Server::~Server() = default;

void
Server::stopWaiting()
{
    _stopWaiting = true;
}

void
Server::setNumberOfPlayers(int number)
{
    _numberOfPlayers = number;
}

void
Server::run()
{
    bool fail = false;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cout << "Failed to create socket." << std::endl;
        exit(EXIT_FAILURE);
    }

    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
        std::cout << "Failed to bind." << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, _numberOfPlayers) < 0) {
        std::cout << "Failed to listen on socket." << std::endl;
        exit(EXIT_FAILURE);
    }
    auto addrlen = sizeof(sockaddr);

    // now accept up to _numberOfPlayers connections
    QVector<QString> names;

    char gameMode;

    switch (_game->gameMode) {
        case Game::GAME_MODE::CLASSIC:
            gameMode = (char)1;
            break;
        case Game::GAME_MODE::ZEROSANDSEVENS:
            gameMode = (char)2;
            break;
        case Game::GAME_MODE::ELIMINATION:
            gameMode = (char)3;
            break;
        case Game::GAME_MODE::DEADLY:
            gameMode = (char)4;
            break;
        default:
            break;
    }

    while (!_stopWaiting && _connections.size() < _numberOfPlayers) {
        struct timeval maxTime;
        maxTime.tv_sec = MAX_WAIT_IN_LOOP;

        fd_set sock;

        FD_ZERO(&sock);
        FD_SET(sockfd, &sock);

        int check = select(sockfd + 1, &sock, nullptr, nullptr, &maxTime);

        if (check > 0) {
            int connection =
              accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
            if (connection < 0) {
                std::cout << "Failed to connect." << std::endl;
                exit(EXIT_FAILURE);
            }

            _connections.push_back(connection);

            char buffer[MAX_NAME_LENGTH]; // name

            int bytesRead = recv(connection, buffer, 25, 0);

            buffer[bytesRead] = '\0';

            QString name(buffer);

            if (names.contains(name)) {
                std::cout << "Error: inputted same name" << std::endl;
                sendMessage("x", -1);
                close(_connections.back());
                _connections.pop_back();
                continue;
            }

            names.push_back(name);

            Player* human = new Human(name, _game);
            _game->addPlayer(human);

            std::string message = "";

            message.push_back(gameMode);

            message.push_back((char)_numberOfPlayers);

            message.push_back((char)names.size());
            for (const QString& name : names) {
                message.push_back((char)name.size());
                message.append(name.toStdString());
            }
            send(connection, message.c_str(), message.size(), 0);
            sendMessage(name.toStdString(), _connections.size() - 1);
        }
    }

    int numberOfHumans = _game->getNumberOfPlayers();

    for (int i = 1; i + numberOfHumans <= _numberOfPlayers; i++) {
        std::string s = "PC" + std::to_string(i);
        Player* pc = new PC(QString::fromStdString(s), _game);
        _game->addPlayer(pc);
    }
    sleep(1);

    sendMessage("/", -1);
}

auto
Server::getGame() const -> Game*
{
    return _game;
}

// CURRENTLY PLAYERS HAVE THE SAME INDEX IN _game->getPlayers()
// AND IN _connections SO THIS IS IMPLEMENTED THIS WAY,
// IF WE DECIDE TO SHUFFLE PLAYERS BEFORE A GAME,
// IMPLEMENT MAP
void
Server::sendMessage(std::string message, int index)
{

    for (int i = 0; i < _connections.size(); i++) {
        if (i != index) {
            send(_connections[i], message.c_str(), message.size(), 0);
        }
    }
}

auto
Server::getMessage() -> std::string
{
    int currentPlayer = _game->getCurrentPlayerIndex();

    if (currentPlayer >= _connections.size()) {
        std::cout << "Out of range" << std::endl;
        exit(EXIT_FAILURE);
    }

    int connection = _connections[currentPlayer];

    char buffer[MAX_MESSAGE_LENGTH];

    int bytesRead = recv(connection, buffer, MAX_MESSAGE_LENGTH, 0);

    buffer[bytesRead] = '\0';

    std::string s(buffer);

    return s;
}
