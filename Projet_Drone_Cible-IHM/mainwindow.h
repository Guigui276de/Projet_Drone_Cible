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
#include <QRegularExpression>
#include <QSplashScreen>
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
    void ReadSerial();
public slots: // fonction publique cliquable
    void on_pb_Urgence_Stop_toggled(bool checked);
    void on_bt_debug_toggled(bool arg1);
    void on_pb_mode_manuel_toggled(bool checked);

private:

    QSplashScreen *splash = new QSplashScreen(QPixmap("image/logo.png").scaled(500, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    Ui::MainWindow *ui;
    SDL_GameController *controller = nullptr;  // Initialisé à nullptr pour éviter les erreurs
    QJsonObject joystickData;
    QTimer *timerjoystick = nullptr;  // Ajout du pointeur QTimer pour bien le gérer
    QTimer *timerWatchdog = nullptr; // Timer pour le watchdog
    QWebEngineView *webView;
    QWebEngineView *gps;
    QRegularExpression regex;


};

#endif // MAINWINDOW_H
