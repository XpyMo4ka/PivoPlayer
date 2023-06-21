#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_addMusicButton_clicked()
{

    QStringList windowName = QFileDialog::getOpenFileNames(this, "Выберите музыкальные файлы", pickPath, "Музыкальные файлы (*.mp3 *.wav)");

    // Копирование выбранных файлов в папку "musicfiles"
    QString destinationDir = "music";
    QDir().mkpath(destinationDir); // Создание папки, если она не существует

    foreach (QString fileName, windowName) {
        QFileInfo fileInfo(fileName);
        QString destinationFile = destinationDir + "/" + fileInfo.fileName();
        pickPath = fileInfo.absoluteFilePath();
        if (QFile::copy(fileName, destinationFile)) {
            ui->MusicList->addItem(fileInfo.fileName());
        } else {
            // Произошла ошибка при копировании файла
        }
    }
}

void MainWindow::on_repeatButton_toggled(bool checked)
{
    isLoop = checked;
}

void MainWindow::on_shuffleButton_toggled(bool checked)
{
    isShuffle = checked;
    if(!queueNames.isEmpty())
        shuffleQueue();
    if(!checked)
    {
        ui->QueueList->clear();
        queueNames.clear();
        generateQueue();
    }
}

void MainWindow::on_prevButton_clicked()
{
    if(songName!=nullptr)
    {
        playPrevSong();
    }
}

void MainWindow::on_pauseButton_clicked()
{
    if(songName != nullptr)
    {
        if(isPlaying){
            player->pause();
            timer->stop();
            isPlaying = false;
        }
        else
        {
            player->play();
            timer->start(500);
            isPlaying = true;
        }
    }
    else if (!queueNames.empty())
    {
        setupPlayer();
        songName = queueNames.first();
        generateQueue();
        queueNames.pop_front();
        delete ui->QueueList->takeItem(0);
        playSong();
    }
    else
    {
        setupPlayer();
        if(songName != nullptr)
        {
            generateQueue();
            playSong();
        }
    }
}

void MainWindow::on_nextButton_clicked()
{
    if(songName!=nullptr)
    {
        playNextSong();
    }
}

void MainWindow::on_addPlaylistButton_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, "Create playlist", "Enter playlist name:", QLineEdit::Normal, "", &ok);

     if (ok && !text.isEmpty())
     {
         playlist = new Playlist(text);
         playlists.append(text);
     }
}

void MainWindow::on_pickPlaylistButton_clicked()
{
    QFileDialog dialog(this, "Pick a Playlist", QDir::currentPath(), "Playlist Files (*.m3u8)");

    dialog.setFileMode(QFileDialog::ExistingFile);

    QString playlistsDirPath = QDir::currentPath() + "/playlists";
    dialog.setDirectory(playlistsDirPath);

    if (dialog.exec())
    {
        QString filePath = dialog.selectedFiles().first();
        currentPlaylist = filePath;
        ui->MusicList->clear();
        queueNames.clear();
        ui->QueueList->clear();
        ui->MusicList->addItems(getSongNamesFromPlaylist(currentPlaylist));
    }
}

void MainWindow::on_setDefaultButton_clicked()
{
    ui->MusicList->clear();
    currentPlaylist.clear();
    ui->MusicList->addItems(getSongNames());
}


