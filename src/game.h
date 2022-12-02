#ifndef GAME_H
#define GAME_H

#include <QRandomGenerator>
#include <QPainter>


namespace Direction {
    const QPoint
        Up    = QPoint( 0, -1),
        Down  = QPoint( 0,  1),
        Left  = QPoint(-1,  0),
        Right = QPoint( 1,  0);
}


enum State {
    Ready,
    Playing,
    Paused,
    GameOver
};



enum GameMode {
    Normal,
    Automatic
};



enum RulesSituation {
    StillAlive,
    AppleWasEaten,
    AppleWasNotEaten,
    Death
};



class Game
{

public:

    static const int fieldWidth = 50, fieldHeight = 40; // Field size in blocks
    static const int blocksMargin = 3;                  // Margins between blocks in px
    State state = State::Ready;
    GameMode gm = GameMode::Normal;

    void init();                                   // Initialize all game things (food, snake, state)
    void next();                                   // Calculate next step according to the game rules
    void draw(QSize frameSize, QPainter *painter); // Draw everything
    void turn(QPoint newDirection);                // Change direction; see namespace Direction
    inline int score() const { return snake.length(); }


private:

    inline void drawBlock(float x, float y, float w, float h, QPainter *painter)
    {
        painter->drawRoundRect(
            x*w + blocksMargin, y*h + blocksMargin,
            w - blocksMargin, h - blocksMargin,
            40, 40
        );
    }

    void drawField(float blockWidth, float blockHeight, QPainter *painter);

    QPoint food[3];
    inline int _genBounded(int bound) { return QRandomGenerator::global()->bounded(0, bound); }
    bool _checkCoord(QPoint p); // Check if p overlays the food or any snake segment
    void generateFood(int i);   // i - index of the food array
    void drawFood(float blockWidth, float blockHeight, QPainter *);

    QPoint direction = Direction::Up;
    QVector<QPoint> snake;
    void drawSnake(float blockWidth, float blockHeight, QPainter *);

    RulesSituation checkBump();
    RulesSituation checkFood();

    int nearestFoodX(); // X of food with Y < snake head Y


};



#endif // GAME_H
