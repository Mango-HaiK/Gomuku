#ifndef STARTGAME_H
#define STARTGAME_H

#include <QDialog>

namespace Ui {
class StartGame;
}

class StartGame : public QDialog
{
    Q_OBJECT

public:
    explicit StartGame(QWidget *parent = nullptr);
    ~StartGame();

    void closeEvent(QCloseEvent *) override;

signals:
    void actionPVEMode();

    void actionPVPMode();

private slots:
    void on_btn_PVE_clicked();

    void on_btn_PVP_clicked();

private:
    Ui::StartGame *ui;
};

#endif // STARTGAME_H
