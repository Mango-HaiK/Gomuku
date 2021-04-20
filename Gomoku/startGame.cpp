#include "startGame.h"
#include "ui_startGame.h"

StartGame::StartGame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartGame)
{
    ui->setupUi(this);
}

StartGame::~StartGame()
{
    delete ui;
}

void StartGame::closeEvent(QCloseEvent *)
{
    QApplication *app;
    app->exit(0);
}

void StartGame::on_btn_PVE_clicked()
{
    emit actionPVEMode();

    hide();
}

void StartGame::on_btn_PVP_clicked()
{
    emit actionPVPMode();

    hide();
}
