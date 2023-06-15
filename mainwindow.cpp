#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->MusicList->addItems(getSongNamesFromFolder());

    timingLabel = findChild<QLabel *>("timing_label");
    timeLeftLabel = findChild<QLabel *>("timeleft_label");
    songnameLabel = findChild<QLabel *>("songname_label");

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateSliderPosition);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimingLabels);
    connect(timer, &QTimer::timeout, this, &MainWindow::autoPlay);
    isSliderPressed = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::autoPlay()
{
    if(player->position() == songDuration)
    {
        playNextSong();
    }
}

void MainWindow::playNextSong()
{
    previousSongs.append(songName);
    songName = queueNames.takeFirst();
    delete ui->QueueList->takeItem(0);
    playSong();
}

void MainWindow::playPrevSong()
{
    if(!previousSongs.empty())
    {
        ui->QueueList->insertItem(0, previousSongs.back());
        songName = previousSongs.back();
        previousSongs.pop_back();
        playSong();
    }
}

void MainWindow::updateTimingLabels()
{


    qint64 currentSongTiming = player->position();
    qint64 timeLeft = player->duration() - player->position();
    QTime currentTiming = QTime(0, 0, 0).addMSecs(currentSongTiming);  //format current time
    QString formattedCurrentTiming = currentTiming.toString("mm:ss");
    QTime timeLeft_ = QTime(0, 0, 0).addMSecs(timeLeft);               //format time left
    QString formattedTimeLeft = timeLeft_.toString("-mm:ss");

    timingLabel->setText(formattedCurrentTiming);
    timeLeftLabel->setText(formattedTimeLeft);
}

void MainWindow::setupPlayer()
{
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.5);
    ui->volumeSlider->setValue(audioOutput->volume()*100);
}



void MainWindow::playSong()
{
    isPlaying = true;
    player->setSource(QUrl::fromLocalFile("music\\" + songName));
    songnameLabel->setText(songName);
    player->play();
    timer->start(500);
}

QStringList MainWindow::getSongNamesFromFolder()
{

    QStringList fileNames;
    filters << "*.mp3" << "*.waw";
      //только файлы
    QDir musicDir("music");
    musicDir.setNameFilters(filters);
    musicDir.setFilter(QDir::Files);
    QFileInfoList fileInfoList = musicDir.entryInfoList();
        //получение названий
        foreach (const QFileInfo& fileInfo, fileInfoList)
        {
            fileNames.append(fileInfo.fileName());
        }
        return fileNames;
}



void MainWindow::generateQueue()
{
    for (const QString &fileName : getSongNamesFromFolder())
    {
        if(fileName != songName)
        {
            queueNames.append(fileName);
            ui->QueueList->addItem(fileName);
        }
    }
}

void MainWindow::updateSliderPosition()
{
    songDuration = player->duration();
    ui->TimingSlider->setMaximum(songDuration);

    if (!isSliderPressed && isPlaying) {
        qint64 position = player->position();
        ui->TimingSlider->setValue(position);
    }
}

void MainWindow::on_MusicList_itemDoubleClicked(QListWidgetItem *item)
{
    if(!isPlaying)
    {
        if (songName.isEmpty())

            setupPlayer();
        songName = item->text();
        generateQueue();
        playSong();
    }
    else
    {
        player->stop();
        timer->stop();
        songName = item->text();
        playSong();
    }
}

void MainWindow::on_TimingSlider_sliderPressed()
{
     isSliderPressed = true;
}


void MainWindow::on_TimingSlider_sliderReleased()
{
    isSliderPressed = false;
    if (isPlaying) {
        qint64 position = ui->TimingSlider->value();
        player->setPosition(position);
    }
}


void MainWindow::on_TimingSlider_sliderMoved(int position)
{
    if(!isPlaying && songName!=nullptr)
    {
        qint64 currentSongTiming = position;
        qint64 timeLeft = player->duration() - position;
        QTime currentTiming = QTime(0, 0, 0).addMSecs(currentSongTiming);  //format current time
        QString formattedCurrentTiming = currentTiming.toString("mm:ss");
        QTime timeLeft_ = QTime(0, 0, 0).addMSecs(timeLeft);               //format time left
        QString formattedTimeLeft = timeLeft_.toString("-mm:ss");

        timingLabel->setText(formattedCurrentTiming);
        timeLeftLabel->setText(formattedTimeLeft);
        player->setPosition(position);
    }
}


void MainWindow::on_volumeSlider_valueChanged(int value)
{
    float volume = value / 100.0;
    audioOutput->setVolume(volume);
}

