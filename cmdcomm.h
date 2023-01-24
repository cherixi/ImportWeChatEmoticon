#ifndef CMDCOMM_H
#define CMDCOMM_H

#include <QString>

extern QString adbPath;

class CmdComm
{
public:
    CmdComm();

    static QString runcmd(const QString &command,const QString &dir = adbPath);

    static void adbPath_init();


};

#endif // CMDCOMM_H
