#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QSharedPointer>

class GamePrivate;
class QPainter;

/*!
 * \class Game
 * \brief Represents the core game logic and state of the Flappy Bird clone.
 */
class Game : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief Constructs a new Game instance.
     * \param parent Parent QObject for this game object.
     */
    explicit Game( QObject * parent = 0 );
    virtual ~Game();

    /*!
     * \brief Resets the game state to its initial values.
     */
    void reset();

    /*!
     * \brief Updates the game state for a single frame.
     */
    void update();

    /*!
     * \brief Handles a key press that affects the bird.
     * \param key Qt key code that has been pressed.
     */
    void handleKey( int key );

    /*!
     * \brief Renders the game scene using the given painter.
     * \param painter Painter used for drawing the scene.
     */
    void paint( QPainter& painter );

    /*!
     * \brief Returns the current score.
     * \return Current score value.
     */
    unsigned int score() const;

signals:
    /*!
     * \brief Emitted when the game is over due to a collision.
     */
    void gameOver();

    /*!
     * \brief Emitted when the score value changes.
     * \param score New score value.
     */
    void scoreChanged( unsigned int score );

private:
    QSharedDataPointer<GamePrivate> d;
    friend class GamePrivate;
};

#endif
