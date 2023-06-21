#include "playlist.h"
#include "qdebug.h"


Playlist::Playlist(const QString& playListName)
{
    playListDir.setPath("playlists");
    filePath = playListDir.absolutePath()+ "/" + playListName + ".m3u8";
    file.setFileName(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {

        QTextStream stream(&file);

        stream << "[playlists]\n";

        file.close();
    }
}

void Playlist::addSong(const QStringList& songNames, QString playlistName)
{
    QString filePath = playListDir.absolutePath() + "/" + playlistName;
    QFile file(filePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);
        for (const QString& song : songNames) {
            if (checkSongExists(playlistName, song))
                continue;
            else
                stream << song << "\n";
        }
        file.close();
    } else {
        qDebug() << "Failed to open file";
    }
}

bool Playlist::checkSongExists(const QString& playlistName, const QString& songName)
{
    QString filePath = "playlists/" + playlistName;
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            if (songName == line) {
                file.close();
                return true;
            }
        }
        file.close();
    }
    return false;
}





