#include "dataClass.h"
#include <QMessageBox>

int DataClass::serverPort = 0;
int DataClass::currConnCount = 0;
int DataClass::maxConnLimit = 0;

bool DataClass::checkMsgBox(QString title, QString text)
{
    QMessageBox msg(title,text,QMessageBox::Question,
                       QMessageBox::Yes | QMessageBox::Default,
                       QMessageBox::No | QMessageBox::Escape,
                       QMessageBox::NoButton);

    return  msg.exec() == QMessageBox::No ? false : true;
}

void DataClass::sendMsg(CommRequestType type, QString context, QTcpSocket* tcpsocket)
{
    MsgRequestType mrt;
    mrt.request = type;
    mrt.data = context;

    //转换字节流
    QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out<<mrt.request<<mrt.data;

    //写入socket
    tcpsocket->write(buff);
}
