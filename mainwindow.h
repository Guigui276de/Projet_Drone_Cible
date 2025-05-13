#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include <SDL2/SDL.h>
#include <QJsonObject>
#include <QApplication>
#include <QWebEngineView>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWebChannel>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void ComLora(); // Ouvre port série
    void WatchDog(); // Envoie toutets les X temps
    void EnvoiTrameManuel(); // Communication mode Mnauel
    void EnvoiTrameGPS();
    void AffichageGPS();
    void AffichageCAM();
    QSerialPort serial;
    void cleanupSDL();

public slots: // fonction publique cliquable
    void on_pb_Urgence_Stop_toggled(bool checked);
    void on_pb_mode_manuel_clicked();

private:
    Ui::MainWindow *ui;
    SDL_GameController *controller = nullptr;  // Initialisé à nullptr pour éviter les erreurs
    QJsonObject joystickData;
    QTimer *timerjoystick = nullptr;  // Ajout du pointeur QTimer pour bien le gérer
    QTimer *timerWatchdog = nullptr; // Timer pour le watchdog
    QWebEngineView *webView;
    QWebEngineView *gps;

};

#endif // MAINWINDOW_H
