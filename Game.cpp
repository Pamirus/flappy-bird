#include "Game.h"

#include <QPainter>
#include <QTransform>
#include <QRandomGenerator>

class GamePrivate : public QSharedData {
    Game * q = nullptr;

public:
    GamePrivate( Game * q_ ) : q( q_ ) { Q_UNUSED( q ); }
    virtual ~GamePrivate()             {}

    bool isCollisionDetected();
    bool isPassedThroughThePipeGap();
    void randomizePipePosition();

public:
    unsigned int              currentScore          = 0;
    int                       birdYPos              = 0;
    int                       pipeX                 = 0;
    bool                      isBirdTouchToPipe     = false;
    bool                      isBirdTouchToGround   = false;
    int                       pipeYOffset           = 0;
    static const unsigned int sceneWidth            = 400;
    static const unsigned int sceneHeight           = 600;
    static const unsigned int initialBirdYPos       = 300;
    static const unsigned int birdXPos              = 150;
    static const unsigned int birdSize              = 40;
    static const unsigned int birdRisePerWingbeat   = 75;
    static const unsigned int birdFallPerFrame      = 3;
    static const unsigned int groundXPos            = 0;
    static const unsigned int groundYPos            = 575;
    static const unsigned int pipeMovementPerFrame  = 3;
    static const unsigned int pipeGap               = 130;
    static const unsigned int pipeWidth             = 50;
    static const int          pipeOutOfScreenBorder = -50;
    static const unsigned int topPipeHeight         = 200;
    static const unsigned int topPipeYPos           = 0;
    static const unsigned int bottomPipeYPos        = topPipeYPos + topPipeHeight + pipeGap;
};

Game::Game( QObject* parent )
    : QObject( parent )
    , d( new GamePrivate( this ))
{
    reset();
}

Game::~Game()
{
}

void Game::reset()
{
    d->isBirdTouchToPipe   = false;
    d->isBirdTouchToGround = false;
    d->birdYPos            = ( int )d->initialBirdYPos;
    d->pipeX               = ( int )d->sceneWidth;
    d->currentScore        = 0;
    d->randomizePipePosition();
}

void Game::update()
{
    d->birdYPos += ( int )d->birdFallPerFrame;
    d->pipeX    -= ( int )d->pipeMovementPerFrame;
    if( d->pipeX < d->pipeOutOfScreenBorder ) {
        d->pipeX = ( int )d->sceneWidth;
        d->randomizePipePosition();
    }

    if( d->isCollisionDetected() )
        emit gameOver();
    else if( d->isPassedThroughThePipeGap() ) {
        d->currentScore += 1;
        emit scoreChanged( d->currentScore );
    }
}

void Game::handleKey( int key )
{
    if( key == Qt::Key_Space || key == Qt::Key_Up )
        d->birdYPos -= ( int )d->birdRisePerWingbeat;
}

void Game::paint( QPainter& painter )
{
    QPixmap groundPixmap( "./resources/world/ground.png" );
    painter.drawPixmap( d->groundXPos, d->groundYPos, groundPixmap.width(), groundPixmap.height(), groundPixmap );

    QPixmap birdPixmap( "./resources/world/bird.png" );
    painter.drawPixmap( d->birdXPos, d->birdYPos, birdPixmap );

    QPixmap pipePixmap( "./resources/world/pipe.png" );
    QPixmap topPipePixmap = pipePixmap.transformed( QTransform().scale( 1, -1 ) );

    int topPipeYPos    = ( int )GamePrivate::topPipeYPos + d->pipeYOffset + ( int )GamePrivate::topPipeHeight - topPipePixmap.height();
    int bottomPipeYPos = ( int )GamePrivate::bottomPipeYPos + d->pipeYOffset;

    painter.drawPixmap( d->pipeX, topPipeYPos, topPipePixmap );
    painter.drawPixmap( d->pipeX, bottomPipeYPos, pipePixmap );
}

unsigned int Game::score() const
{
    return d->currentScore;
}

bool GamePrivate::isCollisionDetected()
{
    isBirdTouchToPipe = pipeX       <  (int)birdXPos        + (int)birdSize         &&
                        pipeX       >  (int)birdXPos        - (int)birdSize         &&
                        ( birdYPos  <= (int)topPipeYPos     + pipeYOffset + (int)topPipeHeight    ||
                          birdYPos  >= (int)bottomPipeYPos  + pipeYOffset - (int)birdSize );
    if( birdYPos >= (int)groundYPos - (int)birdSize )
        isBirdTouchToGround = true;
    return ( isBirdTouchToPipe || isBirdTouchToGround );
}

bool GamePrivate::isPassedThroughThePipeGap()
{
    return pipeX        == ( int )birdXPos      + 1                                   &&
           ( birdYPos   >= ( int )topPipeYPos   + pipeYOffset + ( int )topPipeHeight  ||
            birdYPos    <= ( int )bottomPipeYPos + pipeYOffset );
}

void GamePrivate::randomizePipePosition()
{
    int baseHeight            = ( int )topPipeHeight;
    int minVisiblePipeHeight  = 125;
    int minOffset             = minVisiblePipeHeight - baseHeight;
    int maxOffset             = baseHeight - minVisiblePipeHeight;

    if( maxOffset < minOffset )
        pipeYOffset = 0;
    else
        pipeYOffset = ( int )QRandomGenerator::global()->bounded( minOffset, maxOffset + 1 );
}
