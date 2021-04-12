#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include "dataClass.h"
#include "serverStatus.h"

QT_BEGIN_NAMESPACE
namespace Ui { class server; }
QT_END_NAMESPACE

class Server : public QMainWindow
{
    Q_OBJECT

public:
    Server(QWidget *parent = nullptr);
    ~Server();

    //接受客户端连接请求
    QTcpServer *player_server;

    //用于开启服务器状态
    ServerStatus *server_status;

private slots:
    void on_btn_startServer_clicked();

    //关闭服务
    void closeServer();
private:
    Ui::server *ui;

};
#endif // SERVER_H
