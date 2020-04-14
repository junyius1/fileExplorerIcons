#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFolder(bool addFolder) // функция открытия директории и вывода на таблоы
{
    if(addFolder)
        longPath << cPath + "/";
    // перезапись path
    wholePath = "";
    for(int i = 0; i < longPath.size(); i++)
        wholePath += longPath[i];
    // если это не первая директория, то добавляем ячейку для возврата наверх
    if(longPath[0] != cPath + "/" && longPath.size() != 1) {
        QTreeWidgetItem* itmA = new QTreeWidgetItem(ui->explorer);
        itmA->setText(0, ".");
        itmA->setIcon(0, QIcon(":/back.ico"));
    }
    QDir dir(wholePath);
    QFileInfoList dirList = dir.entryInfoList();
    if(dirList.size() == 2)
        return;
    int is = -1;
    if(wholePath.size() == 3)
        is = 0;
    else
        is = 2;
    for(int i = is; i < dirList.size(); i++) // присвоение иконок к файлам
    {
        QTreeWidgetItem *itmC = new QTreeWidgetItem(ui->explorer);
        QString txtC = dirList.at(i).fileName();
        if(!txtC.contains("."))
            itmC->setIcon(0, QIcon(":/folder.ico"));
        else if(txtC.contains(".txt"))
            itmC->setIcon(0, QIcon(":/text.ico"));
        else if(txtC.contains(".cpp"))
            itmC->setIcon(0, QIcon(":/cpp.ico"));
        else if(txtC.contains(".mp3") || txtC.contains(".aac"))
            itmC->setIcon(0, QIcon(":/music.ico"));
        else if(txtC.contains(".mp4") || txtC.contains(".avi") || txtC.contains(".vga"))
            itmC->setIcon(0, QIcon(":/movie.ico"));
        else if(txtC.contains(".jpg") || txtC.contains(".jpeg") || txtC.contains(".png") || txtC.contains(".raw"))
            itmC->setIcon(0, QIcon(":/photo.ico"));
        else if(txtC.contains(".dll"))
            itmC->setIcon(0, QIcon(":/dll.ico"));
        else if(txtC.contains(".doc") || txtC.contains(".docx"))
            itmC->setIcon(0, QIcon(":/word.ico"));
        else if(txtC.contains(".xls") || txtC.contains(".xlsx"))
            itmC->setIcon(0, QIcon(":/excel.ico"));
        else if(txtC.contains(".ppt") || txtC.contains(".pptx"))
            itmC->setIcon(0, QIcon(":/powerpoint.ico"));
        else if(txtC.contains(".exe") || txtC.contains(".bat"))
            itmC->setIcon(0, QIcon(":/exe.ico"));
        else
            itmC->setIcon(0, QIcon(":/file.ico"));
        itmC->setText(0, txtC);
    }
    ui->statuslbl->setText("Current directory: " + wholePath); // вывод статус бара
}

void MainWindow::on_pushButton_clicked() // открытие директории
{
    ui->explorer->clear();
    QString path = ui->lineEdit->text(); // получение вводных данных
    //
    if(!path.contains(":") || path.at(path.size() - 1) == '/' || path.contains(".") || path.at(0) == ':')
    {
        //
        QMessageBox u;
        u.setText("Invalid Format, see example!");
        u.exec();
        ui->lineEdit->clear();
        return;
    }
    cPath = path;
    //
    for(int i = 0; i < longPath.size(); i++)
        longPath.pop_back();
    openFolder(true);
}

void MainWindow::updateTable()
{
    ui->explorer->clear(); // обновляем папку
    openFolder(false);
}

void MainWindow::on_explorer_doubleClicked(const QModelIndex &index)
{
    cPath = index.data().toString();
    if(cPath == ".") {
        longPath.pop_back();
        updateTable();
        return;
    }
    if(cPath.contains(".")) {
        QDesktopServices::openUrl(wholePath + cPath);
        return;
    }
    ui->explorer->clear();
    openFolder(true);
}
