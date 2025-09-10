#include "headers/leaderboards.h"

#include <QDir>
#include <QJsonObject>
#include <algorithm>

Leaderboards::Leaderboards()
  : _leaderboardsFileName(generateLeaderboardsFileName())
{
}

auto
Leaderboards::generateLeaderboardsFileName() -> QString
{
    QString absolutePath = QDir().absolutePath();
    QString workingDir =
      absolutePath.mid(0, absolutePath.lastIndexOf('/')) + "/UNO/";
    if (!QDir(workingDir + "Leaderboards").exists())
        QDir().mkdir(workingDir + "Leaderboards");
    return workingDir + "Leaderboards/leaderboards.json";
}

void
Leaderboards::insertGameScores(QVector<QPair<QString, int>> placements)
{
    QJsonDocument jsonDoc = loadJson(_leaderboardsFileName);
    QJsonObject jsonObj = jsonDoc.object();

    for (auto& placement : placements) {
        QString currPlayerName = placement.first;

        if (jsonObj.find(currPlayerName) == jsonObj.end())
            jsonObj.insert(currPlayerName, placement.second);
        else
            jsonObj.insert(currPlayerName,
                           jsonObj[currPlayerName].toInt() + placement.second);
    }

    jsonDoc.setObject(jsonObj);
    saveJson(jsonDoc, _leaderboardsFileName);
}

auto
Leaderboards::loadJson(QString fileName) -> QJsonDocument
{
    QFile jsonFile(fileName);
    if (!jsonFile.open(QFile::ReadOnly))
        jsonFile.open(QFile::ReadWrite);
    return QJsonDocument().fromJson(jsonFile.readAll());
}

void
Leaderboards::saveJson(QJsonDocument document, QString fileName)
{
    QFile jsonFile(fileName);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(document.toJson());
    jsonFile.close();
}

auto
compareByScore(const QPair<QString, int>& r1,
               const QPair<QString, int>& r2) -> bool
{
    return r1.second > r2.second;
}

auto
Leaderboards::returnSortedResults() -> QString
{
    QJsonDocument jsonDoc = loadJson(_leaderboardsFileName);
    QJsonObject jsonObj = jsonDoc.object();

    QVector<QPair<QString, int>> results;
    for (auto key : jsonObj.keys()) {
        QPair<QString, int> kv(key, jsonObj.value(key).toInt());
        results.push_back(kv);
    }

    std::sort(results.begin(), results.end(), compareByScore);

    QString retStr = " Rank\t\tName\t\tScore\n\n";
    int sz = results.size() > 15 ? 15 : results.size();
    for (int i = 0; i < sz; i++)
        if (results[i].second > 0)
            retStr.append(" " + QString::number(i + 1) + ".\t\t" +
                          results[i].first + "\t\t" +
                          QString::number(results[i].second) + "\n");
    return retStr;
}
