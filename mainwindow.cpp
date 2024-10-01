#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QMessageBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QFontDatabase>
#include <QPixmap>

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent ), ui( new Ui::MainWindow )
{
    QIcon icon("./resources/icon/favicon.ico");
    setWindowIcon(icon);

    ui->setupUi( this );
    QFontDatabase::addApplicationFont("./resources/font/PressStart2P-Regular.ttf");
    drawBackground();

    initialize();

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

void MainWindow::initialize()
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

    drawGround( painter );
    drawBird( painter );
    drawPipe( painter );
}

void MainWindow::drawBird(QPainter& painter)
{
    QPixmap birdPixmap( "./resources/world/bird.png" );
    painter.drawPixmap( birdXPos, birdYPos, birdPixmap );
}

void MainWindow::drawPipe(QPainter& painter)
{
    QPixmap pipePixmap( "./resources/world/pipe.png" );
    QPixmap topPipePixmap = pipePixmap.transformed(QTransform().scale(1, -1));

    painter.drawPixmap( pipeX, width() - bottomPipeYPos - pipeGap, topPipePixmap );
    painter.drawPixmap( pipeX, bottomPipeYPos, pipePixmap );
}

void MainWindow::drawGround(QPainter& painter)
{
    QPixmap groundPixmap( "./resources/world/ground.png" );
    painter.drawPixmap( groundXPos, groundYPos, groundPixmap.width(), groundPixmap.height(), groundPixmap );
}

void MainWindow::drawBackground()
{
    QPixmap backgroundPixmap( "./resources/world/background.png" );
    QPalette palette;
    palette.setBrush(QPalette::Window, backgroundPixmap);
    this->setPalette(palette);
}

void MainWindow::drawScoreOnWindow()
{
    scoreLabel->setStyleSheet( "QLabel { color: white; font-weight: bold; }" );
    scoreLabel->setFont(QFont("Press Start 2P", 36));

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
                        pipeX > birdXPos - birdSize &&
                        ( birdYPos <= topPipeYPos + topPipeHeight ||
                          birdYPos >= bottomPipeYPos - birdSize );

    if( birdYPos >= groundYPos - birdSize )
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
        initialize();
        timer->start();
        update();
    } else {
        // User chose not to play again, close the application
        close();
    }
}
