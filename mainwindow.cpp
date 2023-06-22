#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->MusicList->addItems(getSongNamesFromFolder());
    ui->MusicList->setContextMenuPolicy(Qt::CustomContextMenu);

    timingLabel = findChild<QLabel *>("timing_label");
    timeLeftLabel = findChild<QLabel *>("timeleft_label");
    songnameLabel = findChild<QLabel *>("songname_label");

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateSliderPosition);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimingLabels);
    connect(timer, &QTimer::timeout, this, &MainWindow::autoPlay);

    isSliderPressed = false;
    QString configPath = QCoreApplication::applicationDirPath() + "settings.ini";
    settings = new QSettings(configPath, QSettings::IniFormat, this);
    ui->songname_label->setText(settings->value("currentSong").toString());

    if(settings->value("musicDir").toString().isEmpty())
        pickPath = QDir::homePath();
    else pickPath = settings->value("musicDir").toString();

}

MainWindow::~MainWindow()
{
    saveSettings();
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
    if(!queueNames.isEmpty())
    {
        previousSongs.append(songName);
        songName = queueNames.takeFirst();
        delete ui->QueueList->takeItem(0);
        playSong();
    }
    else if(isLoop)
    {
        generateQueue();
        queueNames.append(songName);
        ui->QueueList->addItem(songName);
        if(isShuffle)
            shuffleQueue();
        playNextSong();
    }
}

void MainWindow::playPrevSong()
{
    if(!previousSongs.empty())
    {
        ui->QueueList->insertItem(0, songName);
        queueNames.push_front(songName);
        queueNames.push_front(previousSongs.back());
        songName = queueNames.takeFirst();
        previousSongs.pop_back();
        playSong();
    }
    else if (isLoop)
    {
        if(lastSongNames.isEmpty())
        {
            lastSongNames = getSongNames();
        }
        songName = lastSongNames.last();
        lastSongNames.pop_back();
        playSong();
    }
}

void MainWindow::shuffleQueue()
{
    shuffledQueueNames = getSongNames();
    shuffledQueueNames.contains(songName);
    QList<QString>::Iterator begin = shuffledQueueNames.begin();
    QList<QString>::Iterator end = shuffledQueueNames.end();
    ui->QueueList->clear();
    std::random_shuffle(begin, end);
    ui->QueueList->addItems(shuffledQueueNames);
    queueNames = shuffledQueueNames;
}

QStringList MainWindow::getSongNames()
{
    if(currentPlaylist.isEmpty())
        return getSongNamesFromFolder();
    else return getSongNamesFromPlaylist(currentPlaylist);
}

void MainWindow::updateTimingLabels()
{
    currentSongTiming = player->position();
    qint64 timeLeft = player->duration() - player->position();
    QTime currentTiming = QTime(0, 0, 0).addMSecs(currentSongTiming);  //format current time
    QString formattedCurrentTiming = currentTiming.toString("mm:ss");
    QTime timeLeft_ = QTime(0, 0, 0).addMSecs(timeLeft);               //format time left
    QString formattedTimeLeft = timeLeft_.toString("-mm:ss");
    if(!isSliderPressed)
    {
        timingLabel->setText(formattedCurrentTiming);
        timeLeftLabel->setText(formattedTimeLeft);
    }
}

void MainWindow::setupPlayer()
{
    setupSettings();
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(volume);
    ui->volumeSlider->setValue(audioOutput->volume()*100);
}

void MainWindow::setupSettings()
{
    volume = settings->value("volume").toReal();
    songName = settings->value("currentSong").toString();

}

void MainWindow::saveSettings()
{
    settings->setValue("volume",volume);
    settings->setValue("currentSong", songName);
    settings->setValue("musicDir", pickPath);
    settings->sync();
}

void MainWindow::playSong()
{
    isPlaying = true;
    player->setSource(QUrl::fromLocalFile("music\\" + songName));
    songnameLabel->setText(songName);
    player->play();
    timer->start(500);
}

void MainWindow::generateQueue()
{
    for (const QString &fileName : getSongNames())
    {
        if(fileName != songName)
        {
            queueNames.append(fileName);
            ui->QueueList->addItem(fileName);
        }
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
        if(queueNames.isEmpty())
            generateQueue();
        playSong();
    }
}

void MainWindow::on_QueueList_itemDoubleClicked(QListWidgetItem *item)
{
    int clickedIndex = ui->QueueList->row(item);
    previousSongs.append(songName);
    songName = item->text();

    for (int i = 0; i < clickedIndex; i++)
    {
        previousSongs.append(ui->QueueList->takeItem(0)->text());
        queueNames.removeFirst();
    }
    ui->QueueList->takeItem(0);
    queueNames.removeFirst();
    playSong();
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
    if(songName!=nullptr)
    {
        qint64 currentSongTiming = position;
        qint64 timeLeft = player->duration() - position;
        QTime currentTiming = QTime(0, 0, 0).addMSecs(currentSongTiming);  //format current time
        QString formattedCurrentTiming = currentTiming.toString("mm:ss");
        QTime timeLeft_ = QTime(0, 0, 0).addMSecs(timeLeft);               //format time left
        QString formattedTimeLeft = timeLeft_.toString("-mm:ss");

        timingLabel->setText(formattedCurrentTiming);
        timeLeftLabel->setText(formattedTimeLeft);
        if(!isPlaying)
            player->setPosition(position);
    }
}

void MainWindow::updateSliderPosition()
{
    songDuration = player->duration();
    ui->TimingSlider->setMaximum(songDuration);

    if (!isSliderPressed &&  isPlaying) {
        qint64 position = player->position();
        ui->TimingSlider->setValue(position);
    }
}

void MainWindow::on_volumeSlider_valueChanged(int value)
{
    volume = value / 100.0;
    audioOutput->setVolume(value / 100.0);

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

QStringList MainWindow::getSongNamesFromPlaylist(QString currentPlaylist)
{
    QString filePath = currentPlaylist;
    QStringList fileNames;
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        // Пропускаем первую строку
        if (!stream.atEnd()) {
            stream.readLine();
        }

        while (!stream.atEnd()) {
            QString line = stream.readLine();
            fileNames.append(line);
        }

        file.close();
    }
    return fileNames;
}

void MainWindow::on_MusicList_customContextMenuRequested(const QPoint& pos)
{
    playlists.clear();
    QListWidgetItem* selectedItem = ui->MusicList->itemAt(pos);

    QList<QListWidgetItem*> selectedItems = ui->MusicList->selectedItems();
       QStringList selectedSongs;

       foreach (QListWidgetItem* item, selectedItems) {
           selectedSongs << item->text();
       }

     if (selectedItem && selectedItem->listWidget() == ui->MusicList) {

        QMenu contextMenu(this);


        QAction* action1 = contextMenu.addAction("Add to queue");
        QAction* action2 = contextMenu.addAction("Delete");

        QMenu playlistMenu("Add to playlist", this);
        contextMenu.addMenu(&playlistMenu); // Добавление подменю в главное меню

        QDir playlistsDir("playlists");
        playlistsDir.setNameFilters(QStringList() << "*.m3u8");
        playlistsDir.setSorting(QDir::Name);

        QFileInfoList playlistFiles = playlistsDir.entryInfoList();
        for (const QFileInfo& playlistInfo : playlistFiles)
        {
            QAction* action = playlistMenu.addAction(playlistInfo.fileName()); // Добавление действия в подменю
            connect(action, &QAction::triggered, this, [this, action, selectedSongs]() {
                QString selectedPlaylistName = action->text();
                QString playlistName = QFileInfo(selectedPlaylistName).baseName();
                playlist = new Playlist(playlistName);
                playlist->addSong(selectedSongs, selectedPlaylistName);
            });
        }

        QAction* selectedAction = contextMenu.exec(ui->MusicList->viewport()->mapToGlobal(pos));

        if (selectedAction == action1) {
             ui->QueueList->insertItem(0, selectedItem->text());
             queueNames.push_front(selectedItem->text());

        }else if (selectedAction == action2) {
            QDir musicDir("music");
            for (QListWidgetItem *item : ui->MusicList->selectedItems())
            {
                if(songName != item->text())
                    musicDir.remove(item->text());
                else
                {
                    player->stop();
                    musicDir.remove(item->text());
                }
                ui->MusicList->takeItem(ui->MusicList->row(item));
                if (!queueNames.isEmpty() && queueNames.contains(item->text()))
                {
                    ui->QueueList->takeItem(queueNames.indexOf(item->text()));
                    queueNames.removeAt(queueNames.indexOf(item->text()));

                }
            }
         }

     }
}

void MainWindow::on_searchBar_textChanged(const QString &arg1)
{
    QStringList songNames = getSongNames();
    QStringList filteredSongNames = songNames.filter(arg1, Qt::CaseInsensitive);
    ui->MusicList->clear();
    ui->MusicList->addItems(filteredSongNames);
}

