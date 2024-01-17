#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>
#include <QLabel>
#include <QVBoxLayout>

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
    void drawScoreOnWindow();

    bool isCollusionDetected();
    bool isPassedThroughThePipeGap();

private slots:
    void gameLoop();
    void gameOver();

private:
    Ui::MainWindow* ui;
    QTimer* timer;
    QLabel* scoreLabel;
    QVBoxLayout* scoreLabelLayout;
    QWidget* scoreLabelWidget;

    unsigned int score;

    int birdYPos;
    const unsigned int initialBirdYPos = 300;
    const unsigned int birdXPos = 150;
    const unsigned int birdSize = 50;
    const unsigned int birdRisePerWingbeat = 100;
    const unsigned int birdFallPerFrame = 4;

    const unsigned int groundPos = 550;

    int pipeX;
    const unsigned int initialPipeXPos = width();
    const unsigned int pipeMovementPerFrame = 3;
    const unsigned int pipeGap = 150;
    const unsigned int pipeWidth = 50;
    const int pipeOutOfScreenBorder = -50;
    const unsigned int topPipeHeight = 200;
    const unsigned int topPipeYPos = 0;
    const unsigned int bottomPipeHeight = height() - topPipeHeight + pipeGap;
    const unsigned int bottomPipeYPos = topPipeYPos + topPipeHeight + pipeGap;

    bool isBirdTouchToPipe = false;
    bool isBirdTouchToGround = false;
};

#endif // MAINWINDOW_H
