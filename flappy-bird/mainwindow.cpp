#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QMessageBox>
#include <QLabel>
#include <QVBoxLayout>

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent ), ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    initalize();

    timer = new QTimer( this );
    connect( timer, SIGNAL( timeout() ), this, SLOT( gameLoop() ));
    timer->start( 16 ); // 60 FPS

    setFixedSize( 400, 600 );
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scoreLabel;
    delete scoreLabelLayout;
    delete scoreLabelWidget;
}

void MainWindow::initalize()
{
    scoreLabel = new QLabel;
    scoreLabelLayout = new QVBoxLayout;
    scoreLabelWidget = new QWidget;

    isBirdTouchToPipe = false;
    isBirdTouchToGround = false;
    birdYPos = initialBirdYPos;
    pipeX = initialPipeXPos;

    score = 0;
    scoreLabel->setText( QString::number( score ));
    drawScoreOnWindow();
}

void MainWindow::keyPressEvent( QKeyEvent* event )
{
    if( event->key() == Qt::Key_Space ||
        event->key() == Qt::Key_Up )
    {
        birdYPos -= birdRisePerWingbeat;
    }
}

void MainWindow::paintEvent( QPaintEvent* event )
{
    Q_UNUSED( event );

    QPainter painter( this );

    // Draw bird
    painter.fillRect( birdXPos, birdYPos, birdSize, birdSize, Qt::red );

    // Draw pipe
    painter.fillRect( pipeX, topPipeYPos, pipeWidth, topPipeHeight, Qt::green );
    painter.fillRect( pipeX, bottomPipeYPos, pipeWidth, bottomPipeHeight, Qt::green );
}

void MainWindow::drawScoreOnWindow()
{
    scoreLabel->setStyleSheet( "QLabel { color: white; font-family: 'Roboto'; font-weight: bold; font-size: 72px; }" );
    setCentralWidget( scoreLabel );

    scoreLabelLayout->addWidget(scoreLabel, 0, Qt::AlignTop | Qt::AlignHCenter);
    scoreLabelWidget->setLayout(scoreLabelLayout);
    setCentralWidget(scoreLabelWidget);
}

void MainWindow::gameLoop()
{
    // Update bird position
    birdYPos += birdFallPerFrame;

    // Update pipe position
    pipeX -= pipeMovementPerFrame;
    if( pipeX < pipeOutOfScreenBorder )
    {
        pipeX = width();
    }

    // Check for collisions
    if( isCollusionDetected() )
    {
        gameOver();
    }
    else if( isPassedThroughThePipeGap() )
    {
        score += 1;
        scoreLabel->setText(QString::number(score));
    }

    // Repaint the window
    update();
}

bool MainWindow::isCollusionDetected()
{
    isBirdTouchToPipe = pipeX < birdXPos + birdSize &&
                        pipeX > birdXPos &&
                        ( birdYPos <= topPipeYPos + topPipeHeight ||
                          birdYPos >= bottomPipeYPos);

    if( birdYPos >= groundPos )
    {
        isBirdTouchToGround = true;
    }

    return ( isBirdTouchToPipe || isBirdTouchToGround );
}

bool MainWindow::isPassedThroughThePipeGap()
{
    return pipeX == birdXPos + 1 &&
           ( birdYPos >= topPipeYPos + topPipeHeight ||
             birdYPos <= bottomPipeYPos );
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
