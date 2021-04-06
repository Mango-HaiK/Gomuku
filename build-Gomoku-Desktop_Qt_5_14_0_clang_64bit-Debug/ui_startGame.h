/********************************************************************************
** Form generated from reading UI file 'startGame.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STARTGAME_H
#define UI_STARTGAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_StartGame
{
public:
    QPushButton *btn_PVP;
    QPushButton *btn_PVE;

    void setupUi(QDialog *StartGame)
    {
        if (StartGame->objectName().isEmpty())
            StartGame->setObjectName(QString::fromUtf8("StartGame"));
        StartGame->resize(400, 300);
        btn_PVP = new QPushButton(StartGame);
        btn_PVP->setObjectName(QString::fromUtf8("btn_PVP"));
        btn_PVP->setGeometry(QRect(30, 190, 141, 91));
        btn_PVE = new QPushButton(StartGame);
        btn_PVE->setObjectName(QString::fromUtf8("btn_PVE"));
        btn_PVE->setGeometry(QRect(230, 190, 141, 91));

        retranslateUi(StartGame);

        QMetaObject::connectSlotsByName(StartGame);
    } // setupUi

    void retranslateUi(QDialog *StartGame)
    {
        StartGame->setWindowTitle(QCoreApplication::translate("StartGame", "Dialog", nullptr));
        btn_PVP->setText(QCoreApplication::translate("StartGame", "PVP Mode", nullptr));
        btn_PVE->setText(QCoreApplication::translate("StartGame", "PVE Mode", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StartGame: public Ui_StartGame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTGAME_H
