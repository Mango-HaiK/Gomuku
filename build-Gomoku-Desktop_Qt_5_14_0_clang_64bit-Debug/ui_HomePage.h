/********************************************************************************
** Form generated from reading UI file 'HomePage.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOMEPAGE_H
#define UI_HOMEPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HomePage
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_4;
    QLabel *label_9;
    QWidget *horizontalLayoutWidget_5;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_10;
    QLineEdit *edit_playerA_name;
    QWidget *horizontalLayoutWidget_6;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_11;
    QLCDNumber *lcd_PlayerA_record;
    QGroupBox *groupBox;
    QLabel *label_4;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QLineEdit *edit_playerB_name;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_5;
    QLCDNumber *lcd_PlayerB_record;
    QGroupBox *groupBox_3;
    QTextEdit *text_edit_chat_info;
    QWidget *horizontalLayoutWidget_7;
    QHBoxLayout *horizontalLayout_9;
    QLineEdit *edit_send_msg;
    QPushButton *btn_send_char_msg;
    QGridLayout *gridLayout_3;
    QPushButton *btn_ready;
    QPushButton *btn_currender;
    QPushButton *btn_undo;
    QPushButton *btn_setting;

    void setupUi(QWidget *HomePage)
    {
        if (HomePage->objectName().isEmpty())
            HomePage->setObjectName(QString::fromUtf8("HomePage"));
        HomePage->resize(910, 660);
        gridLayoutWidget = new QWidget(HomePage);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(720, 10, 4, 4));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayoutWidget = new QWidget(HomePage);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(640, 10, 261, 648));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        groupBox_4 = new QGroupBox(verticalLayoutWidget);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setMinimumSize(QSize(0, 120));
        groupBox_4->setMaximumSize(QSize(16777215, 100));
        label_9 = new QLabel(groupBox_4);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(20, 30, 61, 61));
        horizontalLayoutWidget_5 = new QWidget(groupBox_4);
        horizontalLayoutWidget_5->setObjectName(QString::fromUtf8("horizontalLayoutWidget_5"));
        horizontalLayoutWidget_5->setGeometry(QRect(100, 20, 151, 31));
        horizontalLayout_5 = new QHBoxLayout(horizontalLayoutWidget_5);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_10 = new QLabel(horizontalLayoutWidget_5);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_5->addWidget(label_10);

        edit_playerA_name = new QLineEdit(horizontalLayoutWidget_5);
        edit_playerA_name->setObjectName(QString::fromUtf8("edit_playerA_name"));

        horizontalLayout_5->addWidget(edit_playerA_name);

        horizontalLayoutWidget_6 = new QWidget(groupBox_4);
        horizontalLayoutWidget_6->setObjectName(QString::fromUtf8("horizontalLayoutWidget_6"));
        horizontalLayoutWidget_6->setGeometry(QRect(100, 70, 151, 31));
        horizontalLayout_8 = new QHBoxLayout(horizontalLayoutWidget_6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        label_11 = new QLabel(horizontalLayoutWidget_6);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_8->addWidget(label_11);

        lcd_PlayerA_record = new QLCDNumber(horizontalLayoutWidget_6);
        lcd_PlayerA_record->setObjectName(QString::fromUtf8("lcd_PlayerA_record"));

        horizontalLayout_8->addWidget(lcd_PlayerA_record);


        verticalLayout_2->addWidget(groupBox_4);

        groupBox = new QGroupBox(verticalLayoutWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(0, 120));
        groupBox->setMaximumSize(QSize(16777215, 100));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 30, 61, 61));
        horizontalLayoutWidget = new QWidget(groupBox);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(100, 20, 151, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(horizontalLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout->addWidget(label_3);

        edit_playerB_name = new QLineEdit(horizontalLayoutWidget);
        edit_playerB_name->setObjectName(QString::fromUtf8("edit_playerB_name"));

        horizontalLayout->addWidget(edit_playerB_name);

        horizontalLayoutWidget_2 = new QWidget(groupBox);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(100, 70, 151, 31));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(horizontalLayoutWidget_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_2->addWidget(label_5);

        lcd_PlayerB_record = new QLCDNumber(horizontalLayoutWidget_2);
        lcd_PlayerB_record->setObjectName(QString::fromUtf8("lcd_PlayerB_record"));

        horizontalLayout_2->addWidget(lcd_PlayerB_record);


        verticalLayout_2->addWidget(groupBox);

        groupBox_3 = new QGroupBox(verticalLayoutWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setMinimumSize(QSize(0, 302));
        groupBox_3->setMaximumSize(QSize(16777215, 305));
        text_edit_chat_info = new QTextEdit(groupBox_3);
        text_edit_chat_info->setObjectName(QString::fromUtf8("text_edit_chat_info"));
        text_edit_chat_info->setGeometry(QRect(10, 20, 241, 246));
        horizontalLayoutWidget_7 = new QWidget(groupBox_3);
        horizontalLayoutWidget_7->setObjectName(QString::fromUtf8("horizontalLayoutWidget_7"));
        horizontalLayoutWidget_7->setGeometry(QRect(0, 270, 259, 33));
        horizontalLayout_9 = new QHBoxLayout(horizontalLayoutWidget_7);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(0, 0, 0, 0);
        edit_send_msg = new QLineEdit(horizontalLayoutWidget_7);
        edit_send_msg->setObjectName(QString::fromUtf8("edit_send_msg"));
        edit_send_msg->setMinimumSize(QSize(12, 0));
        edit_send_msg->setMaximumSize(QSize(500, 16777215));

        horizontalLayout_9->addWidget(edit_send_msg);

        btn_send_char_msg = new QPushButton(horizontalLayoutWidget_7);
        btn_send_char_msg->setObjectName(QString::fromUtf8("btn_send_char_msg"));
        btn_send_char_msg->setMaximumSize(QSize(67, 16777215));

        horizontalLayout_9->addWidget(btn_send_char_msg);


        verticalLayout_2->addWidget(groupBox_3);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        btn_ready = new QPushButton(verticalLayoutWidget);
        btn_ready->setObjectName(QString::fromUtf8("btn_ready"));

        gridLayout_3->addWidget(btn_ready, 0, 0, 1, 1);

        btn_currender = new QPushButton(verticalLayoutWidget);
        btn_currender->setObjectName(QString::fromUtf8("btn_currender"));

        gridLayout_3->addWidget(btn_currender, 0, 1, 1, 1);

        btn_undo = new QPushButton(verticalLayoutWidget);
        btn_undo->setObjectName(QString::fromUtf8("btn_undo"));

        gridLayout_3->addWidget(btn_undo, 1, 0, 1, 1);

        btn_setting = new QPushButton(verticalLayoutWidget);
        btn_setting->setObjectName(QString::fromUtf8("btn_setting"));

        gridLayout_3->addWidget(btn_setting, 1, 1, 1, 1);


        verticalLayout_2->addLayout(gridLayout_3);


        retranslateUi(HomePage);

        QMetaObject::connectSlotsByName(HomePage);
    } // setupUi

    void retranslateUi(QWidget *HomePage)
    {
        HomePage->setWindowTitle(QCoreApplication::translate("HomePage", "Form", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("HomePage", "\350\207\252\345\267\261", nullptr));
        label_9->setText(QCoreApplication::translate("HomePage", "TextLabel", nullptr));
        label_10->setText(QCoreApplication::translate("HomePage", "\346\230\265\347\247\260:", nullptr));
        label_11->setText(QCoreApplication::translate("HomePage", "\350\203\234\345\234\272\357\274\232", nullptr));
        groupBox->setTitle(QCoreApplication::translate("HomePage", "\344\275\240\347\232\204\345\257\271\346\211\213", nullptr));
        label_4->setText(QCoreApplication::translate("HomePage", "TextLabel", nullptr));
        label_3->setText(QCoreApplication::translate("HomePage", "\346\230\265\347\247\260:", nullptr));
        label_5->setText(QCoreApplication::translate("HomePage", "\350\203\234\345\234\272", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("HomePage", "GroupBox", nullptr));
        text_edit_chat_info->setHtml(QCoreApplication::translate("HomePage", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'.AppleSystemUIFont'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'.AppleSystemUIFont';\"><br /></p></body></html>", nullptr));
        btn_send_char_msg->setText(QCoreApplication::translate("HomePage", "\345\217\221\351\200\201", nullptr));
        btn_ready->setText(QCoreApplication::translate("HomePage", "\345\207\206\345\244\207", nullptr));
        btn_currender->setText(QCoreApplication::translate("HomePage", "\350\256\244\350\276\223", nullptr));
        btn_undo->setText(QCoreApplication::translate("HomePage", "\346\202\224\346\243\213", nullptr));
        btn_setting->setText(QCoreApplication::translate("HomePage", "\350\256\276\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HomePage: public Ui_HomePage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOMEPAGE_H
