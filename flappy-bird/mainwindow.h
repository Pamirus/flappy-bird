#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);
    void initalize();

private slots:
    void gameLoop();
    void gameOver();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    int birdY;
    int pipeX;
    int score;

    const unsigned int pipeGap = 50;
    const unsigned int topPipeHeight = height() - pipeGap;
    const unsigned int pipeWidth = 50;
    const unsigned int topPipeYPos = 0;
    const unsigned int bottomPipeHeight = 50;
    const unsigned int bottomPipeYPos = 550;
};

#endif // MAINWINDOW_H
