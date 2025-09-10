#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gameGUI.h"
#include "gameParameters.h"
#include "instructionsWidget.h"
#include "leaderboards.h"
#include "lobby.h"

#include <QAudioOutput>
#include <QMainWindow>
#include <QMediaPlayer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

  private slots:
    void playVSAIBtn_clicked();
    void playOnlineBtn_clicked();
    void leaderboardsBtn_clicked();
    void settingsButton_clicked();

    void gamemode1Btn_clicked();
    void gamemode2Btn_clicked();
    void gamemode3Btn_clicked();
    void gamemode4Btn_clicked();
    void backToMainBtn_clicked();

    void player2Btn_clicked();
    void player4Btn_clicked();
    void player8Btn_clicked();
    void backToGameModeBtn_clicked();

    void startGameBtn_clicked();
    void backToPlayerNumBtn_clicked();

    void refreshLeaderboardsBtn_clicked();
    void backToMainBtn_2_clicked();
    void backToMainBtn_3_clicked();

    void hostBtn_clicked();
    void joinBtn_clicked();

    void rewindSlider_sliderMoved(int position);
    void volumeSlider_sliderMoved(int position);
    void playMediaButton_clicked();
    void muteMediaButton_clicked();
    void backToMainBtn_4_clicked();

    void positionChanged(int position);
    void durationChanged(int position);

    void connectButtons();

    void on_pushButton_2_clicked();

  private:
    Ui::MainWindow* ui;
    bool isOnline;
    int numOfPlayers;
    GameGUI* gameGUI;
    QString gameMode;
    GameParameters* gameParams;
    Leaderboards* leaderboard;
    Lobby* lobby;
    QWidget* instructions;

    void startGameGUI(GameParameters* gameParams);

    QMediaPlayer* mediaPlayer;
    QAudioOutput* audioOutput;
};
#endif // MAINWINDOW_H
