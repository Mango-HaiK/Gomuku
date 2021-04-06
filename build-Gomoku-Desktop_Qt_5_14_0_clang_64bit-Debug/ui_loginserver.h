/********************************************************************************
** Form generated from reading UI file 'loginserver.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINSERVER_H
#define UI_LOGINSERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginServer
{
public:
    QPushButton *btn_join;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *edit_username;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *edit_ipadds;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QLineEdit *edit_port;

    void setupUi(QWidget *LoginServer)
    {
        if (LoginServer->objectName().isEmpty())
            LoginServer->setObjectName(QString::fromUtf8("LoginServer"));
        LoginServer->resize(485, 399);
        btn_join = new QPushButton(LoginServer);
        btn_join->setObjectName(QString::fromUtf8("btn_join"));
        btn_join->setGeometry(QRect(180, 310, 121, 31));
        verticalLayoutWidget = new QWidget(LoginServer);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(120, 110, 251, 161));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        edit_username = new QLineEdit(verticalLayoutWidget);
        edit_username->setObjectName(QString::fromUtf8("edit_username"));

        horizontalLayout->addWidget(edit_username);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        edit_ipadds = new QLineEdit(verticalLayoutWidget);
        edit_ipadds->setObjectName(QString::fromUtf8("edit_ipadds"));

        horizontalLayout_2->addWidget(edit_ipadds);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_5 = new QLabel(verticalLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_5->addWidget(label_5);

        edit_port = new QLineEdit(verticalLayoutWidget);
        edit_port->setObjectName(QString::fromUtf8("edit_port"));

        horizontalLayout_5->addWidget(edit_port);


        verticalLayout->addLayout(horizontalLayout_5);


        retranslateUi(LoginServer);

        QMetaObject::connectSlotsByName(LoginServer);
    } // setupUi

    void retranslateUi(QWidget *LoginServer)
    {
        LoginServer->setWindowTitle(QCoreApplication::translate("LoginServer", "Form", nullptr));
        btn_join->setText(QCoreApplication::translate("LoginServer", "OK", nullptr));
        label->setText(QCoreApplication::translate("LoginServer", "  \347\216\251\345\256\266\347\224\250\346\210\267\345\220\215 \357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("LoginServer", "\346\234\215\345\212\241\345\231\250IP\345\234\260\345\235\200\357\274\232", nullptr));
        label_5->setText(QCoreApplication::translate("LoginServer", "\346\234\215\345\212\241\345\231\250\347\253\257\345\217\243\345\217\267\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginServer: public Ui_LoginServer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINSERVER_H
