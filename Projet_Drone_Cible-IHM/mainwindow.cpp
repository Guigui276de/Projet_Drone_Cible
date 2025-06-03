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
#include <QStackedWidget>
#include <QScreen>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), controller(nullptr), ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setWindowTitle("Drone cible");
    AffichageCAM();
    AffichageGPS();
    ComLora();
    WatchDog();
    // Optionnel : forcer une taille initiale (sinon taille par défaut du designer)
    // resize(800, 600);

    // Centrer la fenêtre
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->availableGeometry();
        int x = screenGeometry.x() + (screenGeometry.width() - this->width()) / 2;
        int y = screenGeometry.y() + (screenGeometry.height() - this->height()) / 2;
        this->move(x, y);
    }

    // Palette sombre et autres initialisations
    QPalette palette;
    palette.setColor(QPalette::Window, QColor("#1e1e2f"));
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Base, QColor("#2c3e50"));
    palette.setColor(QPalette::AlternateBase, QColor("#34495e"));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Button, QColor("#2c3e50"));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Highlight, QColor("#3498db"));
    palette.setColor(QPalette::HighlightedText, Qt::white);
    qApp->setPalette(palette);
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
    //gps->setMinimumWidth(400);  // ou plus, selon ta fenêtre
    gps->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    gps->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    gps->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    gps->setFixedSize(400,400); // si tu veux forcer la taille exacte
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
    webView->setUrl(QUrl("http://192.168.1.231:8080/stream_viewer?topic=/yolo_detections"));
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
   // Supprimer tous les <h1> de la page
    let headers = document.querySelectorAll("h1");
    headers.forEach(h => h.remove());

    // Masquer les débordements
    document.documentElement.style.overflow = 'hidden';
    document.body.style.overflow = 'hidden';

    // Adapter la vidéo plein écran
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
        static int time_value = 0;
        if (!timerWatchdog) {
            timerWatchdog = new QTimer(this);

            connect(timerWatchdog, &QTimer::timeout, this, [this]() {
                QString command = QString("AT+SEND=1,W%1,0,0\n").arg(time_value);
                time_value = time_value + 1;
                serial.write(command.toUtf8());
                ui->tb_transmet->append("WatchDog envoyé : " + command);
            });
        }
    } else {
        qCritical() << "Erreur: Port série non ouvert.";
    }
}


void MainWindow::on_pb_mode_manuel_toggled(bool checked)
{
    if(checked){
        timerWatchdog->stop();
        ui->pb_Urgence_Stop->setChecked(false);
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
                QString command = QString("AT+SEND=1,%1%2,0,3\n").arg(xi).arg((yi));
                //QString command = QString("AT+SEND=1,125,0,3\n");
                serial.write(command.toUtf8());
                ui->tb_transmet->append("Commande envoyée : " + command);


            }

        });

        timerjoystick->start(500); // Lecture toutes les 50 ms
    }
    if(!checked) {
        QString command = QString("AT+SEND=1,dmx0y0f,0,3\n");
        QString command2 = QString("AT+SEND=1,dmx0y0f,0,3\n");
        serial.write(command.toUtf8());
        serial.write(command2.toUtf8());
        ui->tb_transmet->append("Commande envoyée : " + command);
        ui->tb_transmet->append("Commande envoyée : " + command2);
        timerWatchdog->start(1000);
        timerjoystick->stop();
    }
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
        QString command = QString("AT+SEND=1,dmx0y0f,0,3\n");
        //QString command = QString("AT+SEND=1,125,0,3\n");
        serial.write(command.toUtf8());
        ui->tb_transmet->append("Commande envoyée : " + command);
        ui->tb_transmet->append("Arrêt d'urgence activé !");
        if (timerjoystick) timerjoystick->stop(); // sécurité
        ui->pb_mode_manuel->setChecked(false);    }
    timerWatchdog->start(1000);
}


void MainWindow::ReadSerial() {
    static QByteArray buffer;

    buffer.append(serial.readAll());

    if (buffer.contains('\n')) {
        QList<QByteArray> lines = buffer.split('\n');
        for (int i = 0; i < lines.size() - 1; ++i) {
            QString line = QString::fromUtf8(lines[i].trimmed());
            if (!line.isEmpty()) {
                QRegularExpression regex("DTL([-+]?\\d*\\.?\\d+)l([-+]?\\d*\\.?\\d+)f");
                QRegularExpressionMatch match = regex.match(line);
                if (match.hasMatch()) {
                    QString longitude = match.captured(1);
                    QString latitude = match.captured(2);

                    // Exemple : afficher dans la console et dans l'IHM
                    qDebug() << "Longitude :" << longitude << ", Latitude :" << latitude;
                    ui->tb_receive->append("Longitude : " + longitude + ", Latitude : " + latitude);
                } else {
                    ui->tb_receive->append("Trame reçue (non GPS) : " + line);
                }
            }
        }

        buffer = lines.last();
    }
}


void MainWindow::on_bt_debug_toggled(bool arg1)
{
    // Affiche ou cache les éléments en fonction de l’état de l'action
    ui->tb_transmet->setVisible(arg1);
    ui->tb_receive->setVisible(arg1);
    ui->bt_debug->setText(arg1 ? "Masquer Debug" : "Afficher Debug");
}



