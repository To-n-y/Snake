#include "canvas.h"

Canvas::Canvas(QWidget *parent)
    : QFrame(parent)
{
    label = new QLabel(this);
    label->setStyleSheet("font: 72px Gabriola");
    label->setAlignment(Qt::AlignCenter);
    label->hide();

    game = new Game();
    game->init();

    timer = new QBasicTimer();
    timer->start(interval_ms, this);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    anim = new QPropertyAnimation(label, "geometry");
    anim->setDuration(900);
}



Canvas::~Canvas()
{
    timer->stop();
    delete timer;

    delete game;
    delete anim;
}


void Canvas::timerEvent(QTimerEvent *event)
{
    event->accept();

    if (game->state == State::Ready && !label->isVisible())
        showString(tr("Press Space"));

    game->next();

    if (game->state == State::GameOver) {
        timer->stop();
        showString(tr("Score: %1").arg(game->score()));
        if (game->gm == GameMode::Normal)
            emit processScore(game->score());
    }

    update();

    canTurn = true;

}



void Canvas::paintEvent(QPaintEvent *event)
{
    event->accept();

    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    game->draw(size(), &painter);
    painter.end();
}


void Canvas::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {

    case Qt::Key_Up:
        if (canTurn)  { game->turn(Direction::Up); canTurn = false; }
        break;

    case Qt::Key_Down:
        if (canTurn)  { game->turn(Direction::Down); canTurn = false; }
        break;

    case Qt::Key_Left:
        if (canTurn)  { game->turn(Direction::Left); canTurn = false; }
        break;

    case Qt::Key_Right:
        if (canTurn)  { game->turn(Direction::Right); canTurn = false; }
        break;

    case Qt::Key_Space:

        switch (game->state) {

        case State::Ready:
            hideString();
            game->state = State::Playing;
            break;

        case State::Playing:
            timer->stop();
            showString(tr("Paused"));
            game->state = State::Paused;
            break;

        case State::Paused:
            timer->start(interval_ms, this);
            hideString();
            game->state = State::Playing;
            break;

        case State::GameOver:
            if (!label->isVisible()) {
                showString(QString(tr("Score: %1")).arg(game->score()));
            } else {
                QMessageBox mb(this);
                mb.setWindowTitle(tr("Start new game"));
                mb.setText(tr("Do you want to start new game?"));
                mb.setIcon(QMessageBox::Question);
                mb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

                if (mb.exec() == QMessageBox::Yes) {
                    hideString();
                    game->init();
                    timer->start(interval_ms, this);
                }
            }
            break;

        default:
            break;

        } // switch (state)

        break;

    default:
        event->ignore();
        return;

    } // switch (event->key())

    event->accept();
}



void Canvas::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        event->accept();

        switch (game->state) {
        case State::Ready:
            showString(tr("Press Space"));
            break;

        case State::Paused:
            showString(tr("Paused"));
            break;

        case State::Playing:
            break;

        case State::GameOver:
            showString(tr("Score: %1").arg(game->score()));
            break;
        }

    } else {
        event->ignore();
    }
}



void Canvas::resizeEvent(QResizeEvent *event)
{
    event->accept();

    if (anim->state() == QPropertyAnimation::Running)
        anim->stop();

    if (label->isVisible()) {
        int w = event->size().width();
        int h = event->size().height();
        label->move(w/2 - lsize.width()/2, h/2 - lsize.height());
        label->resize(lsize.width(), lsize.height());
    }
}



void Canvas::toggleGameMode()
{
    if (game->gm == GameMode::Normal)
        game->gm = GameMode::Automatic;
    else
        game->gm = GameMode::Normal;

    hideString();
    game->init();
    if (!timer->isActive())
        timer->start(interval_ms, this);
}


int Canvas::getHighScore()
{
    return 0;
}




void Canvas::showString(QString s)
{
    if (!label) return;
    if (anim->state() == QPropertyAnimation::Running) anim->stop();

    label->show();
    label->setText(s);
    label->adjustSize();
    lsize = label->size();
    anim->setStartValue(QRect(width()/2, 0, 0, 0));
    anim->setEndValue(QRect(width()/2 - lsize.width()/2, height()/2 - lsize.height(), lsize.width(), lsize.height()));
    anim->start();
}


void Canvas::hideString()
{
    if (label) label->hide();
}

