#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_addMusicButton_clicked()
{
    QStringList windowName = QFileDialog::getOpenFileNames(this, "Выберите музыкальные файлы", QDir::homePath(), "Музыкальные файлы (*.mp3 *.wav)");

    // Копирование выбранных файлов в папку "musicfiles"
    QString destinationDir = "music";
    QDir().mkpath(destinationDir); // Создание папки, если она не существует

    foreach (QString fileName, windowName) {
        QFileInfo fileInfo(fileName);
        QString destinationFile = destinationDir + "/" + fileInfo.fileName();

        if (QFile::copy(fileName, destinationFile)) {
            ui->MusicList->addItem(fileInfo.fileName());
        } else {
            // Произошла ошибка при копировании файла
            // Обработайте ошибку по вашему усмотрению
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
}

void MainWindow::on_nextButton_clicked()
{
    if(songName!=nullptr)
    {
        playNextSong();
    }
}


