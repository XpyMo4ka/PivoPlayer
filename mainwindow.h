#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qlabel.h"
#include "qlistwidget.h"
#include "QList"
#include "QSettings"
#include "playlist.h"
#include "QMessageBox"
#include "QInputDialog"
#include "QMenu"
#include <QMainWindow>
//#include "SoundVisualisationWidget.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QStringList>
#include <QFileDialog>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QLabel *timingLabel;
    QLabel *timeLeftLabel;
    QLabel *songnameLabel;
    QStringList playlists;
private slots:
    void on_MusicList_itemDoubleClicked(QListWidgetItem *item);
    void on_MusicList_customContextMenuRequested(const QPoint& pos);

    void on_repeatButton_toggled(bool checked);
    void on_shuffleButton_toggled(bool checked);
    void on_prevButton_clicked();
    void on_pauseButton_clicked();
    void on_nextButton_clicked();
    void on_addMusicButton_clicked();

    void on_TimingSlider_sliderPressed();
    void on_TimingSlider_sliderReleased();
    void on_TimingSlider_sliderMoved(int position);

    void on_volumeSlider_valueChanged(int value);
    void on_QueueList_itemDoubleClicked(QListWidgetItem *item);

    void on_addPlaylistButton_clicked();


    void on_pickPlaylistButton_clicked();

    void on_setDefaultButton_clicked();

    void on_searchBar_textChanged(const QString &arg1);

private:
    QTimer *timer;
    void updateTimingLabels();
    void updateSliderPosition();

    qint64 songDuration;

    Ui::MainWindow *ui;

    QStringList queueNames;
    QStringList shuffledQueueNames;
    QStringList previousSongs;
    void generateQueue();
    void shuffleQueue();
    QListWidgetItem* firstQueueItem;
    QStringList lastSongNames;

    QDir musicDir;
    QStringList getSongNamesFromFolder();
    QStringList getSongNamesFromPlaylist(QString currentPlaylist);
    QStringList filters;

    QString songName;
    QStringList getSongNames();

    void playPrevSong();
    void playNextSong();
    void autoPlay();
    void playSong();

    void setupPlayer();
    void setupSettings();
    void saveSettings();
    QSettings *settings;
    float volume;
    qint64 currentSongTiming;
    QString pickPath;

    QMediaPlayer *player;
    QAudioOutput *audioOutput;

    bool isPlaying = false;
    bool isShuffle = false;
    bool isLoop = false;
    bool isSliderPressed;

    Playlist *playlist;
    QString currentPlaylist;
    void getPlaylistsPath();
    QStringList playlistNames;


};
#endif // MAINWINDOW_H
