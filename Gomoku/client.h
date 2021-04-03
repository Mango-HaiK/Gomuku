#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include "dataClass.h"

namespace Ui {
class Client;
}

/**
 * @brief The Client class
 * 用于客户端连接服务器
 */
class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QDialog *parent = nullptr);
    ~Client();

private slots:
    //服务器连接成功
    void connServerYes();

    //服务器连接失败
    void connServerNo();

    //断开连接
    void disConn();

    void on_btn_join_clicked();

private:
    Ui::Client *ui;

    //服务器连接的socket
    QTcpSocket *conn_Server_Socket;

    //服务器状态窗口
    //TODO

    //初始化控件
    void initWidget();
};

#endif // CLIENT_H
