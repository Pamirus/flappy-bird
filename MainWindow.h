#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>
#include <QLabel>
#include <QVBoxLayout>

class Game;

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
    void initialize();
    void drawBackground();
    void drawScoreOnWindow();

private slots:
    void gameLoop();
    void gameOver();
    void handleScoreChanged( unsigned int score );

private:
    Ui::MainWindow  * ui;
    QTimer          * timer            = nullptr;
    Game            * game             = nullptr;
    QLabel          * scoreLabel       = nullptr;
    QVBoxLayout     * scoreLabelLayout = nullptr;
    QWidget         * scoreLabelWidget = nullptr;
};

#endif // MAINWINDOW_H
