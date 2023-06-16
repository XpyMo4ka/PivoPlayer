#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::SetPivo(QObject* object, const QString& imagePath)
{
    if (QAbstractButton* button = qobject_cast<QAbstractButton*>(object)) {
        QIcon icon((QPixmap(imagePath)));
        button->setIcon(icon);
        button->setIconSize(button->size());
    } else if (QListWidget* listWidget = qobject_cast<QListWidget*>(object)) {
        listWidget->setStyleSheet(QString("QListWidget { background-image: url(%1); }").arg(imagePath));
        QString styleSheet = QString("QListWidget { background-image: url(%1); background-repeat: no-repeat; background-position: center}").arg(imagePath);
        listWidget->setStyleSheet(styleSheet);
    } else if (QWidget* widget = qobject_cast<QWidget*>(object)) {
        QPalette palette;
        palette.setBrush(widget->backgroundRole(), QBrush(QPixmap(imagePath)));
        widget->setPalette(palette);
    }
}

void MainWindow::on_pushButton_clicked()
{
    SetPivo(ui->MusicList, ":/pivo/taras.png");
    SetPivo(ui->QueueList, ":/pivo/6kg.png");
    SetPivo(this, ":/pivo/pivo.png");
}
