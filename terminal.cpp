#include "terminal.h"

#include <QDir>
#include <QProcess>
#include <QDebug>

QProcess process;

terminal::terminal()
{
}

void terminal::processOutput()
{
    qDebug() << process.readAllStandardOutput();  // read normal output
    qDebug() << process.readAllStandardError();  // read error channel
}
