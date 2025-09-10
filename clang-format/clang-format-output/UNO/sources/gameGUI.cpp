#include "headers/gameGUI.h"
#include "headers/human.h"
#include "headers/leaderboards.h"
#include "headers/mainWindow.h"
#include "ui_gameGUI.h"

#include <iostream>
#include <unistd.h>

// #include <QCloseEvent>
// #include <QGraphicsDropShadowEffect>
// #include <QHBoxLayout>
// #include <QMessageBox>
// #include <QTime>
// #include <QDebug>
// #include <QDir>
// #include <QJsonDocument>
// #include <QJsonObject>
// #include <QPair>

#include <QMessageBox>
#include <QScreen>
#include <QtCore>

// find a better way to do this, problem is that geometry (width,heigth)
// of main window dont update until constructor finishes
int _width = 1920;
int _height = 1080;

// for scaling when more players
int _frameWidth = 700;
int _frameHeight = 180;

// for scaling with 8 players
int _reducedWidth = 460;
int _reducedHeight = 130;

GameGUI::GameGUI(QWidget* parent, GameParameters* gameParams)
  : QWidget(parent)
  , ui(new Ui::TESTUI)
{

    if (!gameParams->isOnline) {

        game = new Game(this, gameParams);

        setupWidgets(gameParams->numOfPlayers);

        connectButtons();

        connectSignals();

        game->start();
    }
}

GameGUI::GameGUI(QWidget* parent,
                 GameParameters* gameParams,
                 Server* server_,
                 Client* client_)
  : QWidget(parent)
  , ui(new Ui::TESTUI)
{
    if (gameParams->isOnline && gameParams->isHost) {
        int num = gameParams->numOfPlayers;

        gameParams->numOfPlayers = 0;

        serverSide = server_->getGame();

        game = new Game(this, gameParams);

        game->gameMode = client_->_gameMode;

        gameParams->numOfPlayers = num;

        server = new GameServer(this, serverSide);
        client = new GameClient(this, game, gameParams->mainPlayerName, true);

        server->sockfd = server_->sockfd;

        server->_connections = QVector<int>(server_->_connections);

        init(client_);

        server->start();

        client->start();

        // emit displayWinner(game->getWinner()->getName());
    } else if (gameParams->isOnline && !gameParams->isHost) {
        gameParams->numOfPlayers = 0;

        switch (client_->_gameMode) {
            case Game::GAME_MODE::CLASSIC:
                gameParams->gameMode = "1";
                break;
            case Game::GAME_MODE::ZEROSANDSEVENS:
                gameParams->gameMode = "2";
                break;
            case Game::GAME_MODE::ELIMINATION:
                gameParams->gameMode = "3";
                break;
            case Game::GAME_MODE::DEADLY:
                gameParams->gameMode = "4";
                break;
            default:
                break;
        }

        game = new Game(this, gameParams);

        gameParams->numOfPlayers = client_->totalNumOfPlayers;

        client = new GameClient(this, game, gameParams->mainPlayerName, false);

        init(client_);

        client->start();
    }
}

void
GameGUI::init(Client* client_)
{
    client->sockfd = client_->sockfd;

    game->mainPlayerIndex = client_->mainPlayerIndex;

    setupWidgets(client_->totalNumOfPlayers);

    connectButtons();

    connectSignals();
}

GameGUI::~GameGUI()
{
    delete ui;
}

void
GameGUI::setupWidgets(int numOfPlayers)
{
    ui->setupUi(this);
    QWidget::showMaximized();
    this->setWindowTitle("UNO");

    mediaPlayer = new QMediaPlayer();
    audioOutput = new QAudioOutput();
    mediaPlayer->setAudioOutput(audioOutput);
    mediaPlayer->setSource(QUrl::fromUserInput("qrc:/Music/game.mp3"));
    mediaPlayer->play();

    ui->wildChoose_2->hide();
    ui->sayUNOButton->hide();
    ui->botSaidUNOlbl->hide();
    // add main player to list of frames (index 0)
    initMainPlayerFrame();
    // add bots (indexes 1-end)
    if (numOfPlayers == 2)
        init1BotFrame();
    else if (numOfPlayers == 4)
        init3BotFrames();
    else
        init7BotFrames();

    // init zeros and sevens indicators
    initZerosAndSevensButtons();
}

auto
GameGUI::generateFrame(QString position) -> QFrame*
{
    auto* frame = new QFrame(this);
    if (position == "down")
        frame->setGeometry(_width / 2 - _frameWidth / 2,
                           _height - _frameHeight * 2 + 30,
                           _frameWidth,
                           _frameHeight);
    else if (position == "up")
        frame->setGeometry(
          _width / 2 - _frameWidth / 2, 0, _frameWidth, _frameHeight);
    else if (position == "right")
        frame->setGeometry(_width - _frameWidth - 10,
                           _height / 2 - _frameHeight,
                           _frameWidth,
                           _frameHeight);
    else if (position == "left")
        frame->setGeometry(
          0, _height / 2 - _frameHeight, _frameWidth, _frameHeight);

    frame->setLayout(new QHBoxLayout());
    return frame;
}

auto
GameGUI::generateFrame8Players(QString position) -> QFrame*
{
    auto* frame = new QFrame(this);
    if (position == "down")
        frame->setGeometry(_width / 2 - _frameWidth / 2 - 50,
                           _height - _frameHeight * 2,
                           _frameWidth,
                           _frameHeight);
    else if (position == "up1")
        frame->setGeometry(
          _width / 2 - _reducedWidth / 2, 0, _reducedWidth, _reducedHeight);
    else if (position == "right1")
        frame->setGeometry(_width - _reducedWidth - 100,
                           _height - _frameHeight * 2 - _reducedHeight + 50,
                           _reducedWidth,
                           _reducedHeight);
    else if (position == "right2")
        frame->setGeometry((_width - _reducedWidth) - 100,
                           _height - _frameHeight * 2 - _reducedHeight * 3 + 50,
                           _reducedWidth,
                           _reducedHeight);
    else if (position == "right3")
        frame->setGeometry((_width - _reducedWidth) - 100,
                           _height - _frameHeight * 2 - _reducedHeight * 5 + 50,
                           _reducedWidth,
                           _reducedHeight);
    else if (position == "left1")
        frame->setGeometry(100,
                           _height - _frameHeight * 2 - _reducedHeight * 5 + 50,
                           _reducedWidth,
                           _reducedHeight);
    else if (position == "left2")
        frame->setGeometry(100,
                           _height - _frameHeight * 2 - _reducedHeight * 3 + 50,
                           _reducedWidth,
                           _reducedHeight);
    else if (position == "left3")
        frame->setGeometry(100,
                           _height - _frameHeight * 2 - _reducedHeight + 50,
                           _reducedWidth,
                           _reducedHeight);

    frame->setLayout(new QHBoxLayout());
    return frame;
}

auto
GameGUI::generateButton(QString position, QFrame* frame) -> QPushButton*
{
    auto* btn = new QPushButton(this);
    int frameXPos = frame->geometry().x();
    int frameYPos = frame->geometry().y();
    int btnSize = 35;
    if (position == "up")
        btn->setGeometry(frameXPos + _frameWidth + 50,
                         frameYPos + (_frameHeight / 2) - 10,
                         btnSize,
                         btnSize);
    else if (position == "down")
        btn->setGeometry(frameXPos + _frameWidth + 50,
                         frameYPos + (_frameHeight / 2) - 10,
                         btnSize,
                         btnSize);
    else if (position == "right" || position == "left")
        btn->setGeometry(frameXPos + _frameWidth / 2 - btnSize / 2,
                         frameYPos - btnSize,
                         btnSize,
                         btnSize);
    else if (position == "right1")
        btn->setGeometry(frameXPos - 50,
                         frameYPos + (_frameHeight / 2) - _reducedHeight + 120,
                         btnSize,
                         btnSize);
    else if (position == "right2")
        btn->setGeometry(frameXPos - 50,
                         frameYPos + (_frameHeight / 2) - _reducedHeight + 120,
                         btnSize,
                         btnSize);
    else if (position == "right3")
        btn->setGeometry(frameXPos - 50,
                         frameYPos + (_frameHeight / 2) - _reducedHeight + 120,
                         btnSize,
                         btnSize);
    else if (position == "left1")
        btn->setGeometry(frameXPos + _reducedWidth + 10,
                         frameYPos + (_frameHeight / 2) - _reducedHeight + 120,
                         btnSize,
                         btnSize);
    else if (position == "left2")
        btn->setGeometry(frameXPos + _reducedWidth + 10,
                         frameYPos + (_frameHeight / 2) - _reducedHeight + 120,
                         btnSize,
                         btnSize);
    else if (position == "left3")
        btn->setGeometry(frameXPos + _reducedWidth + 10,
                         frameYPos + (_frameHeight / 2) - _reducedHeight + 120,
                         btnSize,
                         btnSize);
    else if (position == "up1")
        btn->setGeometry(frameXPos + _reducedWidth + 10,
                         frameYPos + _frameHeight / 2 - 45,
                         btnSize,
                         btnSize);

    btn->setStyleSheet("QPushButton {background-color: red}"
                       "{border-radius: 15px}");
    return btn;
}

auto
GameGUI::generateLabel(QString position, QFrame* frame) -> QLabel*
{
    auto* lbl = new QLabel("", this);
    int frameXPos = frame->geometry().x();
    int frameYPos = frame->geometry().y();
    int lblSize = 100;
    if (position == "up")
        lbl->setGeometry(frameXPos - lblSize + 30,
                         frameYPos + _frameHeight / 2 - 15,
                         lblSize,
                         lblSize / 2);
    else if (position == "down")
        lbl->setGeometry(frameXPos - lblSize + 30,
                         frameYPos + _frameHeight / 2 - 10,
                         lblSize,
                         lblSize / 2);
    else if (position == "right" || position == "left")
        lbl->setGeometry(frameXPos + _frameWidth / 2 - lblSize / 4 - 5,
                         frameYPos + _frameHeight,
                         lblSize,
                         lblSize);
    else if (position == "right1" || position == "right3" ||
             position == "right2")
        lbl->setGeometry(frameXPos - 70,
                         frameYPos + (_frameHeight / 2) - _reducedHeight + 20,
                         lblSize,
                         lblSize);
    else if (position == "left1" || position == "left2" || position == "left3")
        lbl->setGeometry(frameXPos + _reducedWidth + 10,
                         frameYPos + (_frameHeight / 2) - _reducedHeight + 20,
                         lblSize,
                         lblSize);
    else if (position == "up1")
        lbl->setGeometry(frameXPos - lblSize + 30,
                         frameYPos + _reducedHeight / 2 - 50,
                         lblSize,
                         lblSize);
    else
        lbl->setGeometry(frameXPos - 70,
                         frameYPos + (_frameHeight / 2) - _reducedHeight + 10,
                         lblSize,
                         lblSize);
    return lbl;
}

void
GameGUI::initMainPlayerFrame()
{
    QFrame* frame = generateFrame("down");
    allFrames.push_back(frame);
    allTurnIndicators.push_back(generateButton("down", frame));
    allNames.push_back(generateLabel("down", frame));
}

void
GameGUI::init1BotFrame()
{
    QFrame* frame = generateFrame("up");
    allFrames.push_back(frame);
    allTurnIndicators.push_back(generateButton("up", frame));
    allNames.push_back(generateLabel("up", frame));
}

void
GameGUI::init3BotFrames()
{
    // right
    QFrame* frame1 = generateFrame("right");
    allFrames.push_back(frame1);
    allTurnIndicators.push_back(generateButton("right", frame1));
    allNames.push_back(generateLabel("right", frame1));

    // up
    QFrame* frame2 = generateFrame("up");
    allFrames.push_back(frame2);
    allTurnIndicators.push_back(generateButton("up", frame2));
    allNames.push_back(generateLabel("up", frame2));

    // left
    QFrame* frame3 = generateFrame("left");
    allFrames.push_back(frame3);
    allTurnIndicators.push_back(generateButton("left", frame3));
    allNames.push_back(generateLabel("left", frame3));
}

void
GameGUI::init7BotFrames()
{
    // right1
    QFrame* frame1 = generateFrame8Players("right1");
    allFrames.push_back(frame1);
    allTurnIndicators.push_back(generateButton("right1", frame1));
    allNames.push_back(generateLabel("right1", frame1));

    // right2
    QFrame* frame2 = generateFrame8Players("right2");
    allFrames.push_back(frame2);
    allTurnIndicators.push_back(generateButton("right2", frame2));
    allNames.push_back(generateLabel("rigth2", frame2));

    // right3
    QFrame* frame3 = generateFrame8Players("right3");
    allFrames.push_back(frame3);
    allTurnIndicators.push_back(generateButton("right3", frame3));
    allNames.push_back(generateLabel("right3", frame3));

    // up1
    QFrame* frame4 = generateFrame8Players("up1");
    allFrames.push_back(frame4);
    allTurnIndicators.push_back(generateButton("up1", frame4));
    allNames.push_back(generateLabel("up1", frame4));

    // left1
    QFrame* frame5 = generateFrame8Players("left1");
    allFrames.push_back(frame5);
    allTurnIndicators.push_back(generateButton("left1", frame5));
    allNames.push_back(generateLabel("left1", frame5));

    // left2
    QFrame* frame6 = generateFrame8Players("left2");
    allFrames.push_back(frame6);
    allTurnIndicators.push_back(generateButton("left2", frame6));
    allNames.push_back(generateLabel("left2", frame6));

    // left3
    QFrame* frame7 = generateFrame8Players("left3");
    allFrames.push_back(frame7);
    allTurnIndicators.push_back(generateButton("left3", frame7));
    allNames.push_back(generateLabel("left3", frame7));
}

void
GameGUI::initZerosAndSevensButtons()
{
    int mainPlayerIndex = game->mainPlayerIndex;

    for (int i = 1; i < allTurnIndicators.size(); i++) {
        auto* btn = new QPushButton(this);
        btn->setGeometry(allTurnIndicators[i]->geometry());
        btn->setStyleSheet("QPushButton {background-color: black;"
                           "border-radius: 15px;}");

        zerosAndSevensButtons.push_back(btn);
        //        QObject::connect(btn, &QPushButton::clicked, this,
        //                         [this, i]() { swapCards(i); });

        int index = (i + mainPlayerIndex) % allTurnIndicators.size();

        QObject::connect(btn, &QPushButton::clicked, this, [this, index]() {
            game->setChosenPlayer(index);
            game->mainPlayer->swapCards(game->getChosenPlayer());
            renderTable(game->getTopCard());
            endTurn();
        });
    }
}

void
GameGUI::debug()
{
}

void
GameGUI::delay(int mSec)
{
    QTime dieTime = QTime::currentTime().addMSecs(mSec);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void
GameGUI::connectButtons()
{
    // draw from deck button
    QObject::connect(
      ui->draw_card, &QPushButton::clicked, this, &GameGUI::onDrawCard);
    // choose wild color buttons
    QObject::connect(
      ui->wildBlue, &QPushButton::clicked, this, &GameGUI::setWildBlue);
    QObject::connect(
      ui->wildRed, &QPushButton::clicked, this, &GameGUI::setWildRed);
    QObject::connect(
      ui->wildGreen, &QPushButton::clicked, this, &GameGUI::setWildGreen);
    QObject::connect(
      ui->wildYellow, &QPushButton::clicked, this, &GameGUI::setWildYellow);

    // say uno button
    QObject::connect(
      ui->sayUNOButton, &QPushButton::clicked, this, &GameGUI::onSayUNO);

    // finish turn button
    QObject::connect(
      ui->finishTurnButton, &QPushButton::clicked, this, &GameGUI::endTurn);

    QObject::connect(mediaPlayer,
                     &QMediaPlayer::mediaStatusChanged,
                     mediaPlayer,
                     &QMediaPlayer::play);

    QObject::connect(
      ui->turnVolumeBtn, &QPushButton::clicked, this, &GameGUI::toggleVolume);
}

void
GameGUI::connectSignals()
{
    // game signals
    QObject::connect(
      game, &Game::signalRenderTable, this, &GameGUI::renderTable);
    QObject::connect(
      game, &Game::emitterChosenWildColor, this, &GameGUI::showChosenWildColor);
    QObject::connect(
      game, &Game::signalBotSaidUno, this, &GameGUI::displaySaidUnoMsg);
    QObject::connect(
      game, &Game::signalGameEnded, this, &GameGUI::displayWinner);
    QObject::connect(game, &Game::finished, game, &Game::deleteLater);
}

void
GameGUI::renderTable(/*QVector<Player*> players,*/ Card* cardOnPile)
{
    if (game->mainPlayer->getTotalCards() == 1)
        ui->sayUNOButton->show();

    renderCardOnPile(cardOnPile);

    QVector<Player*> tmpPlayers(game->getPlayers());
    int mainPlayerIndex = tmpPlayers.indexOf(game->mainPlayer);
    Player* mainPlayer = tmpPlayers.at(mainPlayerIndex);
    renderMainPlayerFrame(mainPlayer);

    renderBotFrames(tmpPlayers);
}

void
GameGUI::renderCardOnPile(Card* card)
{
    QString path = generateCardPath(card);
    ui->pileButton->setStyleSheet("QPushButton {border-image: url(" + path +
                                  "); border-radius: 15px;}");
    if (card->getColor() == Card::BLACK) {
        //        delay(1000);
        showChosenWildColor();
    }
}

void
GameGUI::renderMainPlayerFrame(Player* mainPlayer)
{

    renderPlayerCardsInHand(mainPlayer->getCards());

    allFrames[0]->show();

    if (mainPlayer->isMyTurn())
        allTurnIndicators[0]->setStyleSheet(
          "QPushButton {background-color: green}"
          "{border-radius: 15px}");
    else
        allTurnIndicators[0]->setStyleSheet(
          "QPushButton {background-color: red}"
          "border-radius: 15px");
    allTurnIndicators[0]->show();

    allNames[0]->setText(mainPlayer->getName());
    allNames[0]->show();

    if (mainPlayer->isMyTurn()) {
        if (mainPlayer->getTotalCards() == 1)
            ui->sayUNOButton->show();
        ui->finishTurnButton->show();
        ui->botSaidUNOlbl->hide();
    } else {
        ui->sayUNOButton->hide();
        ui->finishTurnButton->hide();
    }
}

void
GameGUI::renderPlayerCardsInHand(QVector<Card*> cards)
{
    auto* layout = qobject_cast<QHBoxLayout*>(allFrames[0]->layout());
    while (layout->count()) {
        QLayoutItem* item = layout->takeAt(0);
        delete item->widget();
        delete item;
    }

    if (!game->mainPlayer->isEliminated())
        for (int i = 0; i < cards.size(); i++)
            renderCardInHand(i);
}

void
GameGUI::onDrawCard()
{
    if (game->mainPlayer->isMyTurn() && !game->mainPlayer->drawnCard() &&
        !game->mainPlayer->hasPlayed()) {
        if (game->_isOnline && game->isDeckEmpty()) {
            client->sendMessage("refill");
            delay(1000);
            QString message = client->getMessage();
            game->processTurn(message);
        }
        game->mainPlayer->drawCard();
        game->mainPlayer->setDrawnCard(true);
        if (game->mainPlayer->getTotalCards() > 1)
            ui->sayUNOButton->hide();
        renderCardInHand(game->mainPlayer->getTotalCards() - 1);
    }
}

void
GameGUI::renderBotFrames(QVector<Player*> botPlayers)
{
    // indexing from 1 to skip allFrame[0] which is mainPlayer frame
    for (int i = 1; i < botPlayers.size(); i++) {
        auto* frameLayout = qobject_cast<QHBoxLayout*>(allFrames[i]->layout());

        while (frameLayout->count()) {
            QLayoutItem* item = frameLayout->takeAt(0);
            delete item->widget();
            delete item;
        }

        int mainPlayerIndex = botPlayers.indexOf(game->mainPlayer);

        int botIndex = (i + mainPlayerIndex) % botPlayers.size();

        for (int j = 0; j < botPlayers[botIndex]->getTotalCards(); j++) {

            if (!game->eliminationsOn() ||
                (game->eliminationsOn() &&
                 !botPlayers[botIndex]->isEliminated())) {
                QString buttonText = "";
                auto* button = new QPushButton(buttonText, allFrames[i]);
                if (game->getPlayers().size() == 8) {
                    button->setFixedSize(50, 84);
                    button->setStyleSheet(
                      "QPushButton {border-image: url(:/Cards/Cardback.jpg); "
                      "border-radius: 7px;}");
                } else {
                    button->setFixedSize(100, 168);
                    button->setStyleSheet(
                      "QPushButton {border-image: url(:/Cards/Cardback.jpg); "
                      "border-radius: 15px;}");
                }

                frameLayout->insertWidget(0, button);
            }
        }
        allFrames[i]->show();

        // TODO REIMPLEMENT WITH POSITION
        if (botPlayers[botIndex] == game->getCurrentPlayer())
            allTurnIndicators[i]->setStyleSheet(
              "QPushButton {background-color: green}");
        else
            allTurnIndicators[i]->setStyleSheet(
              "QPushButton {background-color: red}");
        allTurnIndicators[i]->show();

        allNames[i]->setText(botPlayers[botIndex]->getName());
        allNames[i]->show();
    }
}

void
GameGUI::renderCardInHand(int index)
{
    auto* layout = qobject_cast<QHBoxLayout*>(allFrames[0]->layout());

    QString buttonText = "";
    auto* button = new QPushButton(buttonText, allFrames[0]);
    button->setFixedSize(100, 170);

    Card* card = game->mainPlayer->getCards().at(index);
    QString path = generateCardPath(card);
    button->setStyleSheet("QPushButton {border-image: url(" + path +
                          "); border-radius: 15px;}");
    layout->insertWidget(0, button);

    QObject::connect(button, &QPushButton::clicked, this, [this, index]() {
        checkIfPlayable(index);
    });
}

void
GameGUI::checkIfPlayable(int index)
{
    if (game->mainPlayer->isMyTurn()) {
        Player* mainPlayer = game->mainPlayer;

        Card* card = mainPlayer->getCards().at(index);

        bool canPlayThisCard = card->isPlayable(game);

        if (canPlayThisCard && !mainPlayer->hasPlayed()) {
            auto* button = qobject_cast<QPushButton*>(sender());
            ui->pileButton->setStyleSheet(button->styleSheet());

            mainPlayer->setPlayedCard(index);
            mainPlayer->increaseScore(card);
            mainPlayer->playCard(index);

            if (game->gameMode == Game::GAME_MODE::ELIMINATION &&
                !mainPlayer->hasCards() && game->notEliminated() > 2) {
                mainPlayer->previousPlayer->eliminate();
                mainPlayer->previousPlayer->position = game->notEliminated();
                mainPlayer->swapCards(mainPlayer->previousPlayer);
            }

            // for zeros&sevens gamemode

            if (card->getCardNum() == 7 &&
                game->gameMode == Game::GAME_MODE::ZEROSANDSEVENS) {
                renderZerosAndSevensButtons();
            }

            if (card->getColor() == Card::BLACK) {
                ui->wildChoose_2->show();
            }

            delete button;

            if (mainPlayer->getTotalCards() == 1)
                ui->sayUNOButton->show();

            mainPlayer->setHasPlayed(true);
        }
    }
}

void
GameGUI::renderZerosAndSevensButtons()
{
    // hide all bot indicator buttons
    for (int i = 1; i < allTurnIndicators.size(); i++) {
        allTurnIndicators[i]->hide();
        zerosAndSevensButtons[i - 1]->show();
    }
}

void
GameGUI::showChosenWildColor()
{
    Card::COLOR clr = game->getWildColor();
    QString msg = returnCardColor(clr);
    if (msg == "Red")
        ui->pileButton->setStyleSheet("background-color: rgb(220, 25, 20);");
    else if (msg == "Green")
        ui->pileButton->setStyleSheet("background-color: rgb(130, 170, 90);");
    else if (msg == "Blue")
        ui->pileButton->setStyleSheet("background-color: rgb(100, 140, 190);");
    else
        ui->pileButton->setStyleSheet("background-color: rgb(237, 212, 0);");

    ui->pileButton->setStyleSheet(ui->pileButton->styleSheet().append(
      "font-size: 18pt;"
      "font-weight: bold;"
      "border-radius: 10px;"
      "border-left: 1px solid rgb(217, 197, 160);"
      "border-right: 1px solid rgb(217, 197, 160);"
      "border-bottom: 5px solid rgb(217, 197, 160);"));
}

auto
GameGUI::returnCardColor(Card::COLOR clr) -> QString
{
    switch (clr) {
        case Card::BLACK:
            return "Black";
        case Card::BLUE:
            return "Blue";
        case Card::RED:
            return "Red";
        case Card::YELLOW:
            return "Yellow";
        case Card::GREEN:
            return "Green";
        default:
            throw "Invalid card color!";
    }
}

auto
GameGUI::returnCardType(Card::CARD_TYPE type) -> QString
{
    switch (type) {
        case Card::NUMERIC:
            return "";
        case Card::SKIP:
            return "Skip";
        case Card::REVERSE:
            return "Reverse";
        case Card::DRAW_TWO:
            return "Drawtwo";
        case Card::WILD_DRAW_FOUR:
            return "DrawFour";
        case Card::WILD:
            return "Wild";
        default:
            throw "Invalid card type!";
    }
}

auto
GameGUI::generateCardPath(Card* card) -> QString
{
    QString color = returnCardColor(card->getColor());
    QString type = returnCardType(card->getCardType());
    if (color == "Black")
        return ":/Cards/Special/" + type + ".jpg";

    int number = card->getCardNum();
    Card::CARD_TYPE cardType = card->getCardType();
    if (cardType != Card::CARD_TYPE::NUMERIC)
        return ":/Cards/" + color + "/" + color + type + ".jpg";

    return ":/Cards/" + color + "/" + color + QString::number(number) + ".jpg";
}

void
GameGUI::showWildMenu()
{
    ui->wildChoose_2->show();
}

void
GameGUI::setWildBlue()
{
    //	emit emitWildColor(Card::BLUE);
    game->setWildColor(Card::BLUE);
    showChosenWildColor();
    ui->wildChoose_2->hide();
}

void
GameGUI::setWildRed()
{

    //	emit emitWildColor(Card::RED);
    game->setWildColor(Card::RED);
    showChosenWildColor();
    ui->wildChoose_2->hide();
}

void
GameGUI::setWildYellow()
{

    //	emit emitWildColor(Card::YELLOW);
    game->setWildColor(Card::YELLOW);
    showChosenWildColor();
    ui->wildChoose_2->hide();
}

void
GameGUI::setWildGreen()
{

    //	emit emitWildColor(Card::GREEN);
    game->setWildColor(Card::GREEN);
    showChosenWildColor();
    ui->wildChoose_2->hide();
}

void
GameGUI::onSayUNO()
{
    game->mainPlayer->sayUNO(true);
    ui->sayUNOButton->hide();
}

void
GameGUI::displaySaidUnoMsg(QString botName)
{
    ui->botSaidUNOlbl->setText(botName + " said UNO!");
    ui->botSaidUNOlbl->show();
}

void
GameGUI::endTurn()
{
    if ((game->mainPlayer->isMyTurn() && game->mainPlayer->drawnCard()) ||
        (game->mainPlayer->hasPlayed() && !ui->wildChoose_2->isVisible())) {
        game->mainPlayer->setFinishedTurn(true);
        game->mainPlayer->setIsMyTurn(false);

        if (ui->sayUNOButton->isVisible() &&
            game->mainPlayer->getTotalCards() > 1)
            ui->sayUNOButton->hide();

        if (game->gameMode == Game::GAME_MODE::ZEROSANDSEVENS) {
            for (int i = 0; i < zerosAndSevensButtons.size(); i++) {
                if (zerosAndSevensButtons[i]->isVisible())
                    zerosAndSevensButtons[i]->hide();
                allTurnIndicators[i]->show();
            }
            allTurnIndicators.last()->show();
        }
    }
}

auto
compareByScore(const Player* p1, const Player* p2) -> bool
{
    return p1->getScore() > p2->getScore();
}

auto
compareByPosition(const Player* p1, const Player* p2) -> bool
{
    return p1->position < p2->position;
}

void
GameGUI::displayWinner()
{
    //    if (game->eliminationsOn())
    //        game->getWinner()->position = 0;

    //    if (game->gameMode == Game::GAME_MODE::ELIMINATION)
    //        game->getWinner()->previousPlayer->position = 1;

    for (auto player : game->players) {
        if (!player->isEliminated() && !player->hasCards())
            player->position = 0;
        else if (!player->isEliminated() && player->hasCards())
            player->position = 1;
    }

    for (int i = 0; i < game->getPlayers().size(); i++)
        if (!game->getPlayers().at(i)->hasCards()) {
            game->getPlayers().at(i)->increaseScore(
              new Card(Card::BLACK, Card::WILD));
            game->getPlayers().at(i)->increaseScore(
              new Card(Card::BLACK, Card::WILD));
            break;
        }

    QVector<Player*> tmpPlayers(game->getPlayers());
    std::sort(tmpPlayers.begin(), tmpPlayers.end(), compareByScore);

    // TODO: improve with lambda?
    QVector<QPair<QString, int>> placements(tmpPlayers.size());
    for (int i = 0; i < tmpPlayers.size(); i++)
        placements[i] =
          std::make_pair(tmpPlayers[i]->getName(), tmpPlayers[i]->getScore());

    Leaderboards leaderboard;
    leaderboard.insertGameScores(placements);

    // TODO: improve string concatenation (+operator is slow)
    QMessageBox msgBox(QMessageBox::Information,
                       "GAME REPORT",
                       "Game ended!\n\nRank\t\tName\t\tScore\t\n");
    msgBox.setFont(QFont("URW Bookman", 14, QFont::Bold));
    msgBox.setStyleSheet("border-image: transparent");
    msgBox.setStyleSheet("background-color: rgb(46, 52, 54);"
                         "color: rgb(211, 215, 207);");

    for (int i = 0; i < tmpPlayers.size(); i++)
        msgBox.setInformativeText(
          msgBox.informativeText() + QString::number(i + 1) + ".\t\t" +
          tmpPlayers[i]->getName() + "\t\t" +
          QString::number(tmpPlayers[i]->getScore()) + "\t\n");

    QAbstractButton* pButtonMainMenu =
      msgBox.addButton(tr("Back to main menu"), QMessageBox::YesRole);
    pButtonMainMenu->setFont(QFont("URW Bookman", 13, QFont::Medium));
    pButtonMainMenu->setStyleSheet(
      "QAbstractButton {background-color: rgb(30, 40, 40);"
      "color: rgb(211, 215, 207);"
      "border-left: 1px solid rgb(10, 25, 25);"
      "border-right: 1px solid rgb(10, 25, 25);"
      "border-bottom: 3px solid rgb(10, 25, 25);"
      "border-radius: 10px;}"
      "QAbstractButton:hover {"
      "background-color: rgb(50, 55, 55);"
      "border-left: 1px solid rgb(10, 25, 25);"
      "border-right: 1px solid rgb(10, 25, 25);"
      "border-bottom: 3px solid rgb(10, 25, 25);}"
      "QAbstractButton:pressed {"
      "background-color: rgb(30, 40, 40);"
      "border-left: 1px solid rgb(10, 25, 25);"
      "border-right: 1px solid rgb(10, 25, 25);"
      "border-top: 3px solid rgb(10, 25, 25);"
      "border-bottom: none;}");

    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();

    this->close();
}

void
GameGUI::closeEvent(QCloseEvent* event)
{
    delete audioOutput;
    delete mediaPlayer;

    auto* mw = new MainWindow();
    mw->adjustSize();
    mw->move(QWidget::screen()->availableGeometry().center() -
             mw->rect().center());
    mw->show();

    this->close();
}

void
GameGUI::toggleVolume()
{
    if (_volumeInd) {
        audioOutput->setMuted(true);
        QPixmap pixmap(":/backgrounds/mute.png");
        QIcon buttonIcon(pixmap);
        ui->turnVolumeBtn->setIcon(buttonIcon);
        ui->turnVolumeBtn->setIconSize(QSize(40, 40));
    } else {
        audioOutput->setMuted(false);
        QPixmap pixmap(":/backgrounds/sound.png");
        QIcon buttonIcon(pixmap);
        ui->turnVolumeBtn->setIcon(buttonIcon);
        ui->turnVolumeBtn->setIconSize(QSize(40, 40));
    }
    _volumeInd = !_volumeInd;
}
