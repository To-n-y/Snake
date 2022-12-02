#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    canvas = new Canvas(this);
    ui->layout->addWidget(canvas);

    canvas->setFocus();

    setMinimumSize(canvas->game->fieldWidth * 15, canvas->game->fieldHeight * 15);

    connect(ui->actionRules, SIGNAL(triggered()), this, SLOT(showRules()));

    connect(ui->actionControls, SIGNAL(triggered()), this, SLOT(showControls()));

    connect(ui->actionHigh_score, SIGNAL(triggered()), this, SLOT(showHighScore()));
    connect(canvas, SIGNAL(processScore(int)), this, SLOT(checkScore(int)));

    connect(ui->actionGame_mode, SIGNAL(triggered()), this, SLOT(toggleGameMode()));

    connect(ui->actionAbout_this_app, SIGNAL(triggered()), this, SLOT(showAbout()));
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), this, SLOT(showAboutQt()));

    connect(ui->actionEnglish, SIGNAL(triggered()), this, SLOT(changeEnglish()));
    connect(ui->actionRussian, SIGNAL(triggered()), this, SLOT(changeRussian()));
    connect(ui->actionUkrainian, SIGNAL(triggered()), this, SLOT(changeUkrainian()));

    scipher = new SmallCipher("Change the snake directon using arrows. Pause/continue using space.%%%%%%%%%%%%~");

    settings = new QSettings(QDir::currentPath() + "/settings.ini", QSettings::IniFormat);

    myTranslator = new QTranslator();
    qtBaseTranslator = new QTranslator();

    changeLanguage();

}



MainWindow::~MainWindow()
{
    delete settings; // Destructor must write all unsaved changes
    delete canvas;
    delete ui;
    delete myTranslator;
    delete qtBaseTranslator;
    delete scipher;
}


void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {

        event->accept();
        ui->retranslateUi(this);

    } else {
        event->ignore();
    }
}



void MainWindow::changeLanguage()
{
    settings->beginGroup("preferred-translation");
    QString my_tr = settings->value("my", "en_GB").toString();
    QString qt_tr = settings->value("qt", "en").toString();
    settings->endGroup();
    QString tr_dir = "translations";

    ui->actionEnglish->setChecked(my_tr == "en_GB");
    ui->actionRussian->setChecked(my_tr == "ru_RU");
    ui->actionUkrainian->setChecked(my_tr == "uk_UK");

    myTranslator->load(QString("my_") + my_tr, tr_dir);
    qtBaseTranslator->load(QString("qtbase_") + qt_tr, tr_dir);

    qApp->removeTranslator(myTranslator);
    qApp->removeTranslator(qtBaseTranslator);

    qApp->installTranslator(myTranslator);
    qApp->installTranslator(qtBaseTranslator);
}


void MainWindow::changeEnglish()
{
    settings->beginGroup("preferred-translation");
    settings->setValue("my", "en_GB");
    settings->setValue("qt", "en");
    settings->endGroup();

    changeLanguage();
}


void MainWindow::changeRussian()
{
    settings->beginGroup("preferred-translation");
    settings->setValue("my", "ru_RU");
    settings->setValue("qt", "ru");
    settings->endGroup();

    changeLanguage();
}


void MainWindow::changeUkrainian()
{
    settings->beginGroup("preferred-translation");
    settings->setValue("my", "uk_UK");
    settings->setValue("qt", "uk");
    settings->endGroup();

    changeLanguage();
}


void MainWindow::showRules()
{
    QMessageBox::information(this, tr("Snake rules"),
        tr("You have a board, divided into squares. Each square can be empty (white) or contain food (red) or a snake segment (green).\n"
        "You are playing for the snake. You should eat food using its head (dark green). You should not bump into the board border or yourself."));
}



void MainWindow::showControls()
{
    QMessageBox::information(this, tr("Snake controls"),
        tr("Change the snake directon using arrows. Pause/continue using space."));
}


void MainWindow::toggleGameMode()
{
    canvas->toggleGameMode();
}


void MainWindow::showAbout()
{
    QMessageBox::information(this, tr("Application Information"),
        tr("Author: Mark Lagodych  lgd.mrk@gmail.com\nBuilt on %1 at %2\nDescription: This program is an implementation of popular game \"Snake\"")
        .arg(__DATE__).arg(__TIME__));
}


void MainWindow::showAboutQt() {
    QMessageBox::aboutQt(this);
}



void MainWindow::showHighScore()
{
   int hs = loadHighScore();

    QMessageBox::information(this, tr("High score"),
        tr("Your high score: %1").arg(hs));
}


void MainWindow::checkScore(int score)
{

    int highScore = loadHighScore();

    if (score > highScore)
        saveHighScore(score);

}


int MainWindow::loadHighScore()
{

    QString cipher_base64;

    settings->beginGroup("high-score");

    if (settings->contains("hscore")) {
        cipher_base64 = settings->value("hscore").toString();
        settings->endGroup();
    } else {
        settings->endGroup();
        return 0;
    }



    QByteArray cipher = QByteArray::fromBase64(cipher_base64.toUtf8());

    return scipher->bArrayToInt(scipher->decrypt(cipher));

}

void MainWindow::saveHighScore(int score)
{
    QByteArray cipher = scipher->encrypt(scipher->intToBArray(score));
    settings->beginGroup("high-score");
    settings->setValue("hscore", QString(cipher.toBase64()));
    settings->endGroup();
}

