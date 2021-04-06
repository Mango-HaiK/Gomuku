/********************************************************************************
** Form generated from reading UI file 'serverStatus.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVERSTATUS_H
#define UI_SERVERSTATUS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_ServerStatus
{
public:
    QTableWidget *tbw_lobby_info;
    QPushButton *btn_create_room;
    QPushButton *btn_quit_lobby;
    QPushButton *btn_login_server;

    void setupUi(QDialog *ServerStatus)
    {
        if (ServerStatus->objectName().isEmpty())
            ServerStatus->setObjectName(QString::fromUtf8("ServerStatus"));
        ServerStatus->resize(462, 367);
        tbw_lobby_info = new QTableWidget(ServerStatus);
        if (tbw_lobby_info->columnCount() < 3)
            tbw_lobby_info->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tbw_lobby_info->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tbw_lobby_info->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tbw_lobby_info->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tbw_lobby_info->setObjectName(QString::fromUtf8("tbw_lobby_info"));
        tbw_lobby_info->setGeometry(QRect(10, 10, 441, 311));
        btn_create_room = new QPushButton(ServerStatus);
        btn_create_room->setObjectName(QString::fromUtf8("btn_create_room"));
        btn_create_room->setGeometry(QRect(290, 330, 75, 23));
        btn_quit_lobby = new QPushButton(ServerStatus);
        btn_quit_lobby->setObjectName(QString::fromUtf8("btn_quit_lobby"));
        btn_quit_lobby->setGeometry(QRect(370, 330, 75, 23));
        btn_login_server = new QPushButton(ServerStatus);
        btn_login_server->setObjectName(QString::fromUtf8("btn_login_server"));
        btn_login_server->setGeometry(QRect(210, 330, 75, 23));

        retranslateUi(ServerStatus);

        QMetaObject::connectSlotsByName(ServerStatus);
    } // setupUi

    void retranslateUi(QDialog *ServerStatus)
    {
        ServerStatus->setWindowTitle(QCoreApplication::translate("ServerStatus", "Dialog", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tbw_lobby_info->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("ServerStatus", "\344\270\273\346\234\272", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tbw_lobby_info->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("ServerStatus", "\346\226\260\345\273\272\345\210\227", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tbw_lobby_info->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("ServerStatus", "\347\212\266\346\200\201", nullptr));
        btn_create_room->setText(QCoreApplication::translate("ServerStatus", "\345\210\233\345\273\272\346\210\277\351\227\264", nullptr));
        btn_quit_lobby->setText(QCoreApplication::translate("ServerStatus", "\351\200\200\345\207\272", nullptr));
        btn_login_server->setText(QCoreApplication::translate("ServerStatus", "\350\277\236\346\216\245\345\244\247\345\216\205", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ServerStatus: public Ui_ServerStatus {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERSTATUS_H
