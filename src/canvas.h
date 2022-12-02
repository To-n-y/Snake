#ifndef CANVAS_H
#define CANVAS_H

#include <QPropertyAnimation>
#include <QMessageBox>
#include <QSizePolicy>
#include <QBasicTimer>
#include <QKeyEvent>
#include <QLabel>
#include <QFrame>
#include "game.h"



class Canvas : public QFrame
{
    Q_OBJECT

private:

    static const int interval_ms = 100; // Milliseconds
    QBasicTimer *timer;
    bool canTurn = true; // If the snake has not turned yet,
                         // but the player presses new button,
                         // canTurn will prevent from errors

    QLabel *label;
    QPropertyAnimation *anim;
    QSize lsize; // Target animation size
    void showString(QString s);
    void hideString();

    void timerEvent(QTimerEvent *) override;
    void paintEvent(QPaintEvent *) override;
    void keyPressEvent(QKeyEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void changeEvent(QEvent *) override;


public:   

    Canvas(QWidget *parent);
    ~Canvas();    

    void toggleGameMode();
    int getHighScore();

    Game *game;

signals:
    void processScore(int score);

};



#endif // CANVAS_H
