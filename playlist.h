#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "QString"
#include "QFile"
#include "QDir"
#include "QTextStream"

class Playlist
{
public:
    Playlist(const QString& playListName);
    void addSong(const QStringList& songNames, QString playlistName);
    bool checkSongExists(const QString& playlistName, const QString& songName);
private:
    QFile file;
    QDir playListDir;
    QString filePath;
};

#endif // PLAYLIST_H
