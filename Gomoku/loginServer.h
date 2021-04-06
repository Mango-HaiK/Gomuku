#ifndef LOGINSERVER_H
#define LOGINSERVER_H

#include <QWidget>
#include <QTcpSocket>
#include "dataClass.h"

namespace Ui {
class LoginServer;
}
/**
 * @brief The LoginServer class
 * 负责设置IP地址、端口号和用户名信息
 * 连接服务器并返回一个连接服务器的socket
 */
class LoginServer : public QWidget
{
    Q_OBJECT

public:
    explicit LoginServer(QWidget *parent = nullptr);
    ~LoginServer();

    //返回连接服务器socket
    QTcpSocket *getConnServerSocket();

signals:
    void connectYes();

private slots:
    //服务器连接成功
    void connServerYes();

    //服务器连接失败
    void connServerNo();

    //断开连接
    void disConn();

    void on_btn_join_clicked();

private:
    Ui::LoginServer *ui;

    //服务器连接的socket
    QTcpSocket *conn_Server_Socket;


    //初始化控件
    void initWidget();
};

#endif // LOGINSERVER_H
