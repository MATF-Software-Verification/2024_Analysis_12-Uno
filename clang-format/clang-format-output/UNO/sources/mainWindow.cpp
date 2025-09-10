#include "headers/mainWindow.h"
#include "ui_mainWindow.h"

#include <QCursor>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
    this->setFixedSize(800, 600);
    ui->setupUi(this);
    QWidget::showNormal();
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    ui->stackedWidget->setCurrentIndex(0);
    this->setWindowTitle("Main Menu");

    gameParams = new GameParameters();

    leaderboard = new Leaderboards();
    ui->leaderboardsText->setText(leaderboard->returnSortedResults());
    ui->leaderboardsText->setReadOnly(true);

    mediaPlayer = new QMediaPlayer();
    audioOutput = new QAudioOutput();
    mediaPlayer->setAudioOutput(audioOutput);
    mediaPlayer->setSource(QUrl::fromUserInput("qrc:/Music/mainWindow.mp3"));
    mediaPlayer->play();

    connectButtons();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::connectButtons()
{
    // page 0 = main menu
    QObject::connect(ui->playVSAIBtn,
                     &QPushButton::clicked,
                     this,
                     &MainWindow::playVSAIBtn_clicked);
    QObject::connect(ui->playOnlineBtn,
                     &QPushButton::clicked,
                     this,
                     &MainWindow::playOnlineBtn_clicked);
    QObject::connect(ui->leaderboardsBtn,
                     &QPushButton::clicked,
                     this,
                     &MainWindow::leaderboardsBtn_clicked);
    QObject::connect(ui->settingsBtn,
                     &QPushButton::clicked,
                     this,
                     &MainWindow::settingsButton_clicked);

    // page 1 = gamemode
    QObject::connect(ui->gamemode1Btn,
                     &QPushButton::clicked,
                     this,
                     &MainWindow::gamemode1Btn_clicked);
    QObject::connect(ui->gamemode2Btn,
                     &QPushButton::clicked,
                     this,
                     &MainWindow::gamemode2Btn_clicked);
    QObject::connect(ui->gamemode3Btn,
                     &QPushButton::clicked,
                     this,
                     &MainWindow::gamemode3Btn_clicked);
    QObject::connect(ui->gamemode4Btn,
                     &QPushButton::clicked,
                     this,
                     &MainWindow::gamemode4Btn_clicked);
    QObject::connect(ui->backToMainBtn,
                     &QPushButton::clicked,
                     this,
                     &MainWindow::backToMainBtn_clicked);

    // page 2 = num of players
    QObject::connect(ui->player2Btn,
                     &QPushButton::clicked,
                     this,
                     &MainWindow::player2Btn_clicked);
    QObject::connect(ui->player4Btn,
                     &QPushButton::clicked,
                     this,
                     &MainWindow::player4Btn_clicked);
    QObject::connect(ui->player8Btn,
                     &QPushButton::clicked,
                     this,
                     &MainWindow::player8Btn_clicked);
    QObject::connect(ui->backToGameModeBtn,
                     &QPushButton::clicked,
                     this,
                     &MainWindow::backToGameModeBtn_clicked);

    // page 3 = start game and enter name
    QObject::connect(ui->startGameBtn,
                     &QPushButton::clicked,
                     this,
                     &MainWindow::startGameBtn_clicked);
    QObject::connect(ui->backToPlayerNumBtn,
                     &QPushButton::clicked,
                     this,
                     &MainWindow::backToPlayerNumBtn_clicked);

    // page 4 = leaderboards
    QObject::connect(ui->refreshLeaderboardsBtn,
                     &QPushButton::clicked,
                     this,
                     &MainWindow::refreshLeaderboardsBtn_clicked);
    QObject::connect(ui->backToMainBtn_2,
                     &QPushButton::clicked,
                     this,
                     &MainWindow::backToMainBtn_2_clicked);

    // page 5 = host/join
    QObject::connect(
      ui->hostBtn, &QPushButton::clicked, this, &MainWindow::hostBtn_clicked);
    QObject::connect(
      ui->joinBtn, &QPushButton::clicked, this, &MainWindow::joinBtn_clicked);
    QObject::connect(ui->backToMainBtn_3,
                     &QPushButton::clicked,
                     this,
                     &MainWindow::backToMainBtn_3_clicked);

    // page 6 = mediaPlayer
    QObject::connect(mediaPlayer,
                     &QMediaPlayer::positionChanged,
                     this,
                     &MainWindow::positionChanged);
    QObject::connect(mediaPlayer,
                     &QMediaPlayer::durationChanged,
                     this,
                     &MainWindow::durationChanged);
    QObject::connect(mediaPlayer,
                     &QMediaPlayer::mediaStatusChanged,
                     mediaPlayer,
                     &QMediaPlayer::play);
    QObject::connect(ui->rewindSlider,
                     &QSlider::sliderMoved,
                     this,
                     &MainWindow::rewindSlider_sliderMoved);
    QObject::connect(ui->volumeSlider,
                     &QSlider::sliderMoved,
                     this,
                     &MainWindow::volumeSlider_sliderMoved);
    QObject::connect(ui->playMediaButton,
                     &QPushButton::clicked,
                     this,
                     &MainWindow::playMediaButton_clicked);
    QObject::connect(ui->muteMediaButton,
                     &QPushButton::clicked,
                     this,
                     &MainWindow::muteMediaButton_clicked);
    QObject::connect(ui->backToMainBtn_4,
                     &QPushButton::clicked,
                     this,
                     &MainWindow::backToMainBtn_4_clicked);
}

void
MainWindow::playVSAIBtn_clicked()
{
    gameParams->isOnline = false;
    ui->stackedWidget->setCurrentIndex(1);
    this->setWindowTitle("Choose Gamemode");
}

void
MainWindow::playOnlineBtn_clicked()
{
    gameParams->isOnline = true;
    ui->stackedWidget->setCurrentIndex(5);
    this->setWindowTitle("Choose Host or Join");
}

void
MainWindow::hostBtn_clicked()
{
    gameParams->isHost = true;
    ui->stackedWidget->setCurrentIndex(1);
    this->setWindowTitle("Choose Gamemode");
}

void
MainWindow::joinBtn_clicked()
{
    gameParams->isHost = false;
    ui->stackedWidget->setCurrentIndex(3);
    this->setWindowTitle("Enter Name");
}

void
MainWindow::gamemode1Btn_clicked()
{
    gameParams->gameMode = "1";
    ui->stackedWidget->setCurrentIndex(2);
    this->setWindowTitle("Choose number of players");
}

void
MainWindow::gamemode2Btn_clicked()
{
    gameParams->gameMode = "2";
    ui->stackedWidget->setCurrentIndex(2);
    this->setWindowTitle("Choose number of players");
}

void
MainWindow::gamemode3Btn_clicked()
{
    gameParams->gameMode = "3";
    ui->stackedWidget->setCurrentIndex(2);
}

void
MainWindow::gamemode4Btn_clicked()
{
    gameParams->gameMode = "4";
    ui->stackedWidget->setCurrentIndex(2);
}

void
MainWindow::player2Btn_clicked()
{
    gameParams->numOfPlayers = 2;
    ui->stackedWidget->setCurrentIndex(3);
    this->setWindowTitle("Enter Name");
}

void
MainWindow::player4Btn_clicked()
{
    gameParams->numOfPlayers = 4;
    ui->stackedWidget->setCurrentIndex(3);
    this->setWindowTitle("Enter Name");
}

void
MainWindow::player8Btn_clicked()
{
    gameParams->numOfPlayers = 8;
    ui->stackedWidget->setCurrentIndex(3);
    this->setWindowTitle("Enter Name");
}

void
MainWindow::startGameBtn_clicked()
{
    gameParams->mainPlayerName = ui->enterNameText->toPlainText();
    if (gameParams->mainPlayerName != "")
        startGameGUI(gameParams);
}

void
MainWindow::startGameGUI(GameParameters* gameParams)
{
    if (!gameParams->isOnline) {
        gameGUI = new GameGUI(nullptr, gameParams);
        gameGUI->show();
    } else {
        lobby = new Lobby(nullptr, gameParams);
        lobby->show();
    }
    delete audioOutput;
    delete mediaPlayer;
    this->close();
}

void
MainWindow::backToMainBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    this->setWindowTitle("Main Menu");
}

void
MainWindow::backToGameModeBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    this->setWindowTitle("Choose Gamemode");
}

void
MainWindow::backToPlayerNumBtn_clicked()
{
    gameParams->mainPlayerName = "";
    ui->enterNameText->setText("");
    if (gameParams->isOnline && !gameParams->isHost)
        ui->stackedWidget->setCurrentIndex(0);
    else
        ui->stackedWidget->setCurrentIndex(2);
    this->setWindowTitle("Choose number of players");
}

void
MainWindow::leaderboardsBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    this->setWindowTitle("Leaderboards");
}

void
MainWindow::backToMainBtn_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    this->setWindowTitle("Main Menu");
}

void
MainWindow::backToMainBtn_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    this->setWindowTitle("Main Menu");
}

void
MainWindow::refreshLeaderboardsBtn_clicked()
{
    ui->leaderboardsText->setText(leaderboard->returnSortedResults());
}

void
MainWindow::rewindSlider_sliderMoved(int position)
{
    mediaPlayer->setPosition(position);
}

void
MainWindow::volumeSlider_sliderMoved(int position)
{
    qreal linearVolume = QAudio::convertVolume(position / qreal(100),
                                               QAudio::LogarithmicVolumeScale,
                                               QAudio::LinearVolumeScale);
    audioOutput->setVolume(linearVolume);
}

void
MainWindow::playMediaButton_clicked()
{
    audioOutput->setMuted(false);
}

void
MainWindow::muteMediaButton_clicked()
{
    audioOutput->setMuted(true);
}

void
MainWindow::backToMainBtn_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    this->setWindowTitle("Main Menu");
}

void
MainWindow::positionChanged(int position)
{
    ui->rewindSlider->setValue(position);
}

void
MainWindow::durationChanged(int position)
{
    ui->rewindSlider->setMaximum(position);
}

void
MainWindow::settingsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    this->setWindowTitle("Settings");
}

void
MainWindow::on_pushButton_2_clicked()
{
    instructions = new InstructionsWidget();
    instructions->show();
    delete audioOutput;
    delete mediaPlayer;
    this->close();
}
