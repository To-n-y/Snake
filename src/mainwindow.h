#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTranslator>
#include <QMessageBox>
#include <QMainWindow>
#include <QByteArray>
#include <QSettings>
#include <QDir>
#include "canvas.h"
#include "smallcipher.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE





class MainWindow : public QMainWindow
{
    Q_OBJECT


public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:

    QSettings *settings;
    Ui::MainWindow *ui;
    Canvas *canvas;
    QTranslator *myTranslator, *qtBaseTranslator;
    SmallCipher *scipher;

    void changeEvent(QEvent *) override;
    void changeLanguage();

    int loadHighScore();
    void saveHighScore(int score);


private slots:

    void showRules();
    void showControls();
    void toggleGameMode();
    void showAbout();
    void showAboutQt();
    void showHighScore();

    void changeEnglish();
    void changeRussian();
    void changeUkrainian();

    void checkScore(int score);

};



#endif // MAINWINDOW_H
