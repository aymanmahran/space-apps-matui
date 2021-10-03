#include "foo.h"
#include "mainwindow.h"
#include "dialog.h"

#include <QApplication>
#include <QFileInfo>
#include <QProgressDialog>
#include <iostream>
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <thread>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Dialog d;

    if(QFileInfo::exists("log.txt")){
        qDebug() << "Here";
        w.show();
    }
    else{
        //d.show();
        QProcess process;
        process.start("/bin/sh", QStringList() << "-c" << "pkexec " + QDir::currentPath()+"/install_deb.sh");
        process.waitForFinished();
        QString output = process.readAllStandardOutput();
        process.waitForFinished();
        //qDebug() << output;
        QString err = process.readAllStandardError();
        qDebug() << err;

        if(!err.contains("Error", Qt::CaseInsensitive)){
            QString filename = "log.txt";
            QFile file(filename);
            if (file.open(QIODevice::ReadWrite)) {
                QTextStream stream(&file);
                stream << ".";
            }
        }
        d.hide();
    }
    return a.exec();
}
