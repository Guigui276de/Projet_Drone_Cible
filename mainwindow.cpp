#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCoreApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include <QJsonObject>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <SDL2/SDL.h>
#include <algorithm> // Pour std::clamp
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), controller(nullptr), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Drone cible");
    AffichageCAM();
    AffichageGPS();
    ComLora();
    //WatchDog();
}

MainWindow::~MainWindow() {
    cleanupSDL();
}

void MainWindow::ComLora() {

    // Configuration du port série
    serial.setPortName("/dev/ttyUSB0");  // Remplacez par votre port
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    if (!serial.open(QIODevice::ReadWrite)) {
        ui->tb_transmet->append("Erreur lors de l'ouverture du port série.");
    } else {
        ui->tb_transmet->append("Port série ouvert.");
    }
}

void MainWindow::AffichageGPS(){

    gps = new QWebEngineView(this);
    gps->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    gps->setMinimumWidth(400);  // ou plus, selon ta fenêtre

    gps->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    gps->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);

    // Charger la carte GPS
    //gps->setUrl(QUrl("ressources/map.html"));
    QString path = QCoreApplication::applicationDirPath() + "/ressources/map.html";
    qDebug() << "Chemin complet vers la carte : " << path;
    if (!QFile::exists(path)) {
        ui->tb_transmet->append("Erreur : map.html introuvable à ce chemin !");
    }

    gps->setUrl(QUrl::fromLocalFile(path));

    ui->layout_gps->addWidget(gps);
}

void MainWindow::AffichageCAM(){

    // Création des WebViews
    webView = new QWebEngineView(this);

    // Charger le flux vidéo
    webView->setUrl(QUrl("http://10.42.0.77:8080/stream_viewer?topic=/yolo_detections"));
    // Ajout des WebViews dans l'interface
    ui->layout_cam->addWidget(webView);

    // Exécuter le JavaScript uniequement après le chargement complet du webView
    connect(webView, &QWebEngineView::loadFinished, this, [this](bool ok) {
        if (ok) {
            webView->page()->runJavaScript(R"(
                document.documentElement.style.overflow = 'hidden';
                document.body.style.overflow = 'hidden';
            )");
            webView->page()->runJavaScript(R"(
                let video = document.querySelector("video");
                if (video) {
                    video.style.position = "fixed";
                    video.style.top = "0";
                    video.style.left = "0";
                    video.style.width = "100vw";
                    video.style.height = "100vh";
                    video.style.objectFit = "cover";
                }
            )");
        } else {
            ui->tb_transmet->append("Le flux vidéo n'a pas pu être chargé !");
        }
    });


}

void MainWindow::WatchDog(){
    if (serial.isOpen()) {
        // Création du timer s'il n'existe pas
        if (!timerWatchdog) {
            timerWatchdog = new QTimer(this);
            connect(timerWatchdog, &QTimer::timeout, this, [this]() {
                QString command = QString("AT+SEND=1,1,0,0\n");
                serial.write(command.toUtf8());
                ui->tb_transmet->append("WatchDog envoyé : " + command);
            });
        }

        // Démarrage à 50 ms
        timerWatchdog->start(500);
        ui->tb_transmet->append("WatchDog activé (toutes les 500ms).");
    } else {
        qCritical() << "Erreur: Port série non ouvert.";
    }
}


void MainWindow::on_pb_mode_manuel_clicked()
{
    QString command1 = QString("AT+SEND=1,dmxMyMf");
    serial.write(command1.toUtf8());
    ui->tb_transmet->append("Commande envoyée : " + command1);

    // Initialise SDL avec uniquement le sous-système contrôleur de jeu
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
        qCritical() << "Erreur SDL_Init:" << SDL_GetError();
        return;
    }

    // Vérifie s'il y a au moins une manette connectée et compatible
    if (SDL_NumJoysticks() > 0 && SDL_IsGameController(0)) {
        controller = SDL_GameControllerOpen(0);
        if (!controller) {
            ui->tb_transmet->append("Impossible d'ouvrir le contrôleur SDL.");
            cleanupSDL();
            return;
        }
    } else {
        ui->tb_transmet->append("Aucune manette détectée.");
        cleanupSDL();
        return;
    }

    // Création et gestion du timer pour lire les entrées toutes les 500ms
    timerjoystick = new QTimer(this);
    connect(timerjoystick, &QTimer::timeout, this, [this]() {
        SDL_PumpEvents(); // Met à jour l'état du joystick

        if (controller) {
            int y = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
            int x = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);

            // Conversion de -32768 à 32767 vers -255 à 255
            int converted_X = std::clamp<int>(static_cast<int>((x / 32768.0) * 255.0), -255, 255);
            int converted_Y = std::clamp<int>(static_cast<int>((y / 32768.0) * 255.0), -255, 255);

            QString xi = QString("dmx%1y").arg(-converted_X);
            QString yi = QString("%1f").arg(converted_Y);
            QPushButton *modeUrgence = findChild<QPushButton*>("pb_Urgence_Stop");
                // Vérifie si le mode urgence est activé
                if (modeUrgence && modeUrgence->isChecked()) {
                    ui->tb_transmet->append("Arret du drone en mode URGENCE !");
                } else {
                    //QString command = QString("AT+SEND=1,%1%2,0,3\n").arg(xi).arg((yi));
                    QString command = QString("AT+SEND=1,125,0,3\n");
                    serial.write(command.toUtf8());
                    ui->tb_transmet->append("Commande envoyée : " + command);
                }
            }
    });

    timerjoystick->start(100); // Lecture toutes les 50 ms

    // Assurer le nettoyage de SDL lors de la fermeture de l'application
    connect(qApp, &QCoreApplication::aboutToQuit, this, &MainWindow::cleanupSDL);
}

void MainWindow::cleanupSDL()
{
    if (timerjoystick) {
        timerjoystick->stop();
        delete timerjoystick;
        timerjoystick = nullptr;
    }

    if (controller) {
        SDL_GameControllerClose(controller);
        controller = nullptr;
    }

    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
    SDL_Quit();
}

void MainWindow::on_pb_Urgence_Stop_toggled(bool checked)
{
    if (checked) {
        ui->tb_transmet->append("Arrêt d'urgence activé !");
        if (timerjoystick) timerjoystick->stop(); // sécurité
        if (timerWatchdog) timerWatchdog->stop(); // évite le crash
    } else {
        ui->tb_transmet->append("Arrêt d'urgence désactivé.");
        if (timerjoystick) timerjoystick->start(500);
        if (timerWatchdog) timerWatchdog->start(500);
        // Réactiver les commandes du joystick si l'urgence est désactivée
    }
}
