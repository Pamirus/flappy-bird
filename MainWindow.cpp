#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Game.h"
#include <QPainter>
#include <QMessageBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QFontDatabase>
#include <QPixmap>

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent ), ui( new Ui::MainWindow )
{
    QIcon icon( "./resources/icon/favicon.ico" );
    setWindowIcon( icon );

    ui->setupUi( this );
    QFontDatabase::addApplicationFont( "./resources/font/PressStart2P-Regular.ttf" );
    drawBackground();

    scoreLabel = new QLabel;
    scoreLabelLayout = new QVBoxLayout;
    scoreLabelWidget = new QWidget;
    drawScoreOnWindow();

    game = new Game( this );
    connect( game, SIGNAL( scoreChanged( unsigned int ) ), this, SLOT( handleScoreChanged( unsigned int ) ) );
    connect( game, SIGNAL( gameOver() ), this, SLOT( gameOver() ) );

    initialize();

    timer = new QTimer( this );
    connect( timer, SIGNAL( timeout() ), this, SLOT( gameLoop() ));
    timer->start( 16 );

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
    game->reset();
    scoreLabel->setText( QString::number( game->score() ));
}

void MainWindow::keyPressEvent( QKeyEvent* event )
{
    game->handleKey( event->key() );
}

void MainWindow::paintEvent( QPaintEvent* event )
{
    Q_UNUSED( event );

    QPainter painter( this );
    game->paint( painter );
}

void MainWindow::drawBackground()
{
    QPixmap backgroundPixmap( "./resources/world/background.png" );
    QPalette palette;
    palette.setBrush( QPalette::Window, backgroundPixmap );
    this->setPalette( palette );
}

void MainWindow::drawScoreOnWindow()
{
    scoreLabel->setStyleSheet( "QLabel { color: white; font-weight: bold; }" );
    scoreLabel->setFont( QFont( "Press Start 2P", 36 ));

    scoreLabelLayout->addWidget( scoreLabel, 0, Qt::AlignTop | Qt::AlignHCenter );
    scoreLabelWidget->setLayout( scoreLabelLayout );

    setCentralWidget( scoreLabelWidget );
}

void MainWindow::gameLoop()
{
    game->update();
    update();
}

void MainWindow::gameOver()
{
    timer->stop();

    QString infoText = "<font color = 'black'>Your score: " + QString::number( game->score() ) + "<br>Do you want to play again?</font>";
    QMessageBox gameOverBox( QMessageBox::Critical, "Game Over", infoText, QMessageBox::Yes | QMessageBox::No );
    int result = gameOverBox.exec();

    if( result == QMessageBox::Yes )
    {
        initialize();
        timer->start();
        update();
    }
    else
    {
        close();
    }
}

void MainWindow::handleScoreChanged( unsigned int score )
{
    scoreLabel->setText( QString::number( score ));
}
