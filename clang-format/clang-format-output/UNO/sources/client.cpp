#include "headers/client.h"
#include "headers/gameclient.h"
#include <headers/wildcard.h>

#include <sys/socket.h>
// #include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <utility>

Client::Client(QObject* parent)
  : QThread(parent)
{
}

Client::Client(QObject* parent, QString address, QString name, bool isHost)
  : QThread(parent)
  , _serverAddress(std::move(address))
  , _myName(std::move(name))
  , _isHost(isHost)
{
}

Client::~Client() = default;

auto
Client::isHost() -> bool
{
    return _isHost;
}

void
Client::run()
{
    sleep(1);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cout << "Failed to create socket." << std::endl;
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET,
                  _serverAddress.toStdString().c_str(),
                  &server_addr.sin_addr) <= 0) {
        std::cout << "Invalid address." << std::endl;
        exit(EXIT_FAILURE);
    }

    // maybe this needs a different implementation
    connect1(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    sendMessage(_myName);

    // first receive lobby info

    QString lobby_info = getMessage();

    if (lobby_info.toStdString()[0] == 'x') {
        emit signalClose();
        exit(EXIT_FAILURE);
    }

    int k = 0;

    char mode = (int)lobby_info[k].unicode();

    k++;

    switch ((int)mode) {
        case 1:
            _gameMode = Game::GAME_MODE::CLASSIC;
            break;
        case 2:
            _gameMode = Game::GAME_MODE::ZEROSANDSEVENS;
            break;
        case 3:
            _gameMode = Game::GAME_MODE::ELIMINATION;
            break;
        case 4:
            _gameMode = Game::GAME_MODE::DEADLY;
            break;
        default:
            break;
    }

    totalNumOfPlayers = (int)lobby_info[k].unicode();

    k++;

    int numOfPlayers = (int)lobby_info[k].unicode();

    k++;

    for (int i = 0; i < numOfPlayers; i++) {
        int nameLen = (int)lobby_info[k].unicode();
        k++;
        QString name =
          QString::fromStdString(lobby_info.toStdString().substr(k, nameLen));

        k += nameLen;

        names.push_back(name);
    }

    emit signalInitLobby();

    mainPlayerIndex = numOfPlayers - 1;

    // load new players as they come
    while (true) {
        lobby_info = getMessage();
        if (lobby_info.toStdString()[0] == 'x') {
            continue;
        } else if (lobby_info.toStdString()[0] == '/') {
            emit signalStartGame();
            break;
        } else {
            names.push_back(lobby_info);
            emit signalInitNames();
        }
    }
}

auto
Client::getMessage() -> QString
{
    char buffer[MAX_MESSAGE_LENGTH];
    int bytesRead = recv(sockfd, buffer, MAX_MESSAGE_LENGTH, 0);
    buffer[bytesRead] = '\0';
    QString s(buffer);
    return s;
}

void
Client::sendMessage(QString message)
{
    send(
      sockfd, message.toStdString().c_str(), message.toStdString().size(), 0);
}

void
connect1(int sockfd, struct sockaddr* sockaddr, socklen_t size)
{
    if (connect(sockfd, sockaddr, size) < 0) {
        std::cout << "Connection failed." << std::endl;
        exit(EXIT_FAILURE);
    }
}
