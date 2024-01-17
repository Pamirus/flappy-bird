#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QMessageBox>

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent ), ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    initalize();

    timer = new QTimer( this );
    connect( timer, SIGNAL( timeout() ), this, SLOT( gameLoop() ) );
    timer->start( 16 ); // 60 FPS

    setFixedSize( 800, 600 );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initalize()
{
    birdY = 500;
    pipeX = 700;
    score = 0;
}

void MainWindow::keyPressEvent( QKeyEvent* event )
{
    if( birdY > 50 &&
        ( event->key() == Qt::Key_Space ||
         event->key() == Qt::Key_Up ))
    {
        birdY -= 100;
    }
}

void MainWindow::paintEvent( QPaintEvent* event )
{
    Q_UNUSED( event );

    QPainter painter( this );

    // Draw bird
    painter.fillRect( 150, birdY, 50, 50, Qt::red );

    // Draw pipe
    painter.fillRect( pipeX, topPipeYPos, pipeWidth, topPipeHeight, Qt::green );
    painter.fillRect( pipeX, bottomPipeYPos, pipeWidth, bottomPipeHeight, Qt::green );
}

void MainWindow::gameLoop()
{
    // Update bird position
    if( birdY < 550 )
    {
        birdY += 3;
    }

    // Update pipe position
    pipeX -= 5;
    if( pipeX < -50 )
    {
        pipeX = width();
    }

    // Check for collisions
    if( pipeX < 200 &&
        pipeX > 150 &&
        ( birdY <= topPipeYPos + topPipeHeight ||
          birdY >= bottomPipeYPos ))
    {
        gameOver();
    }
    else if( pipeX == 150 &&
             ( birdY >= topPipeYPos + topPipeHeight ||
               birdY <= bottomPipeYPos ))
    {
        score += 1;
    }

    // Repaint the window
    update();
}

void MainWindow::gameOver()
{
    timer->stop();

    QString infoText = "<font color = 'black'>Your score: " + QString::number(score) + "<br>Do you want to play again?</font>";
    QMessageBox gameOverBox(QMessageBox::Critical, "Game Over", infoText, QMessageBox::Yes | QMessageBox::No);
    int result = gameOverBox.exec();

    if (result == QMessageBox::Yes) {
        // Reset the game
        initalize();
        timer->start();
        update();
    } else {
        // User chose not to play again, close the application
        close();
    }
}
