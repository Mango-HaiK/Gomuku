#ifndef HELPDIGLOG_H
#define HELPDIGLOG_H

#include <QDialog>

namespace Ui {
class helpDiglog;
}

class helpDiglog : public QDialog
{
    Q_OBJECT

public:
    explicit helpDiglog(QWidget *parent = nullptr);
    ~helpDiglog();

private slots:
    void on_btn_yes_clicked();

private:
    Ui::helpDiglog *ui;
};

#endif // HELPDIGLOG_H
