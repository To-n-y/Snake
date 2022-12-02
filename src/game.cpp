#include "game.h"


void Game::init()
{
    state = State::Ready;

    direction = Direction::Up;

    if (!snake.isEmpty())
        snake.clear();

    snake.push_back(QPoint(fieldWidth/2, fieldHeight/2));
    snake.push_back(QPoint(fieldWidth/2, fieldHeight/2+1));
    snake.push_back(QPoint(fieldWidth/2, fieldHeight/2+2));

    generateFood(0);
    generateFood(1);
    generateFood(2);
}


void Game::next()
{
    if (state == State::Playing) {

        if (gm == GameMode::Automatic) {

            QPoint head = snake.front();
            int maxX = fieldWidth-1, maxY = fieldHeight-1;

            int nearest = nearestFoodX();

            // Note: the board is divided into the right corridor (the right column)
            // and the main space. The snake should eat the food inside the main space,
            // moving from the bottom to the top and then return by the right corridor.

            // ================= Do not bump into the border ==================

            // Do not bump into the board ceil - go to the right corridor
            if (head.y() == 0 && direction == Direction::Up) {
                direction = Direction::Right;

            // Return by the right corridor
            } else if (head.x() == maxX) {
                if (direction == Direction::Right)
                    direction = Direction::Down;

                else if (head.y() == maxY && direction == Direction::Down)
                    direction = Direction::Left;

            // ==================== Eat food ===========================

            // Handle the main space
            } else if (head.x() < maxX && head.y() != 0) {
                if (nearest == -1 && direction != Direction::Up) {
                    direction = Direction::Up;

                } else if (nearest != -1) {

                    if (food[nearest].x() > head.x()) {
                        if (direction == Direction::Left)
                            direction = Direction::Up;
                        else
                            direction = Direction::Right;

                    } else if (food[nearest].x() < head.x()) {
                        if (direction == Direction::Right)
                            direction = Direction::Up;
                        else
                            direction = Direction::Left;

                    } else {
                        direction = Direction::Up;
                    }
                }
            }

        } // if (gm == GameMode::Automatic)

        RulesSituation sit = checkBump();

        if (sit == RulesSituation::Death) {
            state = State::GameOver;
            return;
        }

        // Make a step
        snake.push_front(snake.front() + direction);

        sit = checkFood();

        if (sit == RulesSituation::AppleWasNotEaten)
            snake.pop_back();

   }
}


/*! Finds the x coord. of food with the least positive distance to the snake's head
 * @return -1 if no such food, else its X coord.
*/
int Game::nearestFoodX()
{
    QVector<int> suitable;

    QPoint head = snake.front();

    // Find suitable food
    for (int i = 0; i < 3; i++) {
        if (food[i].y() <= head.y() && food[i].x() < fieldWidth-1)
            suitable.push_back(i);
    }

    if (suitable.isEmpty()) {
        return -1;

    } else {
        // Find the nearest food

        int nearest = 0;
        for (int i = 0; i < suitable.length(); i++)
            if (food[suitable[i]].y() > food[suitable[nearest]].y())
                nearest = i;

        // If there are >1 food pieces in one row, return the left one
        for (int i = 0; i < suitable.length(); i++)
            if (food[suitable[nearest]].y() == food[suitable[i]].y()
                && food[suitable[nearest]].x() > food[suitable[i]].x())
                nearest = i;

        return suitable[nearest];
    }
}



void Game::turn(QPoint newDirection) {

    // Prevent momental fail
    // e.g. if direction was Up and player pressed Down
    if (newDirection + direction == QPoint(0, 0)) return;

    if (gm == GameMode::Normal && state == State::Playing)
        direction = newDirection;
}



RulesSituation Game::checkFood()
{
    QPoint head = snake.front();

    for (int i = 0; i < 3; i++)
        if (head == food[i]) {
            generateFood(i);
            return RulesSituation::AppleWasEaten;
        }

    return RulesSituation::AppleWasNotEaten;

}



RulesSituation Game::checkBump()
{
    QPoint head = snake.front();
    QPoint next = head + direction;

    if (next.x() < 0 || next.x() > fieldWidth-1)
        return RulesSituation::Death;

    if (next.y() < 0 || next.y() > fieldHeight-1)
        return RulesSituation::Death;

    for (int i = 0; i < snake.length()-1; i++) // -1 -- exclude the snake's tail
        if (next == snake[i]) return RulesSituation::Death;

    return RulesSituation::StillAlive;
}



void Game::draw(QSize frameSize, QPainter *painter)
{
    float blockWidth = (float) frameSize.width() / fieldWidth;
    float blockHeight = (float) frameSize.height() / fieldHeight;

    drawFood (blockWidth, blockHeight, painter);
    drawSnake(blockWidth, blockHeight, painter);
    drawField(blockWidth, blockHeight, painter);
}


/*! Returns true if p overlays food or any snake segment  */
bool Game::_checkCoord(QPoint p)
{
    for (QPoint f : food)
        if (f == p) return true;

    for (QPoint s : snake)
        if (s == p) return true;

    return false;
}


void Game::generateFood(int i)
{
    QPoint point;

    do {
        point.setX(_genBounded(fieldWidth));
        point.setY(_genBounded(fieldHeight));
    } while (_checkCoord(point));

    food[i] = point;
}



void Game::drawField(float blockWidth, float blockHeight, QPainter *painter)
{
    painter->setPen(Qt::lightGray);
    painter->setBrush(Qt::transparent);

    for (int y = 0; y < fieldHeight; y++)
        for (int x = 0; x < fieldWidth; x++)
            drawBlock(x, y, blockWidth, blockHeight, painter);
}



void Game::drawFood(float blockWidth, float blockHeight, QPainter *painter)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(255, 100, 100));

    for (QPoint f : food)
        drawBlock(f.x(), f.y(), blockWidth, blockHeight, painter);
}




void Game::drawSnake(float blockWidth, float blockHeight, QPainter *painter)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(70, 255, 70));

    for (QPoint s : snake)
        drawBlock(s.x(), s.y(), blockWidth, blockHeight, painter);

    painter->setBrush(QColor(0, 170, 0));
    drawBlock(snake.front().x(), snake.front().y(), blockWidth, blockHeight, painter);

}
