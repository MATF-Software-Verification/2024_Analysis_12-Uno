#ifndef LEADERBOARDS_H
#define LEADERBOARDS_H

#include <QJsonDocument>
#include <QPair>
#include <QString>
#include <QVector>

class Leaderboards
{
  public:
    Leaderboards();

    QString generateLeaderboardsFileName();

    QJsonDocument loadJson(QString fileName);

    void saveJson(QJsonDocument document, QString fileName);

    void insertGameScores(QVector<QPair<QString, int>> placements);

    QString returnSortedResults();

  private:
    QString _leaderboardsFileName;
};

#endif // LEADERBOARDS_H
