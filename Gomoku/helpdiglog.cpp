#include "helpdiglog.h"
#include "ui_helpdiglog.h"

helpDiglog::helpDiglog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::helpDiglog)
{
    ui->setupUi(this);
}

helpDiglog::~helpDiglog()
{
    delete ui;
}

void helpDiglog::on_btn_yes_clicked()
{
    this->close();
}
