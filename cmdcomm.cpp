#include "cmdcomm.h"

#include <QString>
#include <QProcess>
#include <QDebug>

#include "ui_mainwindow.h"
#include "declare.h"

CmdComm::CmdComm()
{

}


QString CmdComm::runcmd(const QString &command,const QString &dir)
{
    qDebug() << "start: run" << command << "in cmd";
    QProcess p(0);
    p.setWorkingDirectory(dir);
    p.start("cmd", QStringList() << "/c" << command);
    p.waitForStarted();
    p.waitForFinished();

    QString error = QString::fromLocal8Bit(p.readAllStandardError());
    QString output = QString::fromLocal8Bit(p.readAllStandardOutput());

    if(!error.isEmpty())
    {
        qDebug() << "warning: standarderror in cmd:" << error;
        return  error;
    }
    if(!output.isEmpty())
    {
        qDebug() << "cmd output:" << output;
        return output;
    }
    qDebug() << "warning: no match in runcmd";
    return 0;
}

void CmdComm::adbPath_init()
{
    adbPath = QDir::currentPath();
}
