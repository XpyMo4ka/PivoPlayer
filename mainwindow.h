#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qlabel.h"
#include "qlistwidget.h"
#include <QMainWindow>
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
private slots:


    void on_MusicList_itemDoubleClicked(QListWidgetItem *item);

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

private:
    QTimer *timer;
    void updateTimingLabels();
    bool isSliderPressed;
    void updateSliderPosition();
    qint64 songDuration;

    Ui::MainWindow *ui;

    QStringList queueNames;
    void generateQueue();

    QString songName;
    QStringList previousSongs;
    void playPrevSong();
    void autoPlay();
    void playNextSong();
    void setupPlayer();
    void playSong();
    QStringList getSongNamesFromFolder();
    QDir musicDir;
    QStringList filters;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QListWidgetItem* firstQueueItem;
    bool isPlaying = false;
    bool isShuffle = false;
    bool isLoop = false;
};
#endif // MAINWINDOW_H
