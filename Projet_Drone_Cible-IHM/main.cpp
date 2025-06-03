#include "mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QTimer>
#include <QSplashScreen>
#include <QPixmap>
#include <QLabel>
int main(int argc, char *argv[])

{

    QApplication a(argc, argv);

    QPixmap pixmap(":/ressources/image/logo.png");
    if (pixmap.isNull()) {
        qDebug() << "Erreur : logo.png introuvable ou non chargé";
    }

    QSplashScreen *splash = new QSplashScreen(pixmap.scaled(500, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    splash->setStyleSheet("background-color: #1e1e2f; color: white; font-size: 16px;");
    splash->showMessage("Chargement de l'application Drone Cible...", Qt::AlignBottom | Qt::AlignHCenter, Qt::white);
    splash->show();

    QApplication::processEvents();  // Force affichage du splash

    MainWindow w;

    // Affiche la fenêtre principale après 3 secondes et ferme le splash
    QTimer::singleShot(3000, splash, &QSplashScreen::close);
    QTimer::singleShot(3000, &w, &MainWindow::show);

    return a.exec();
}



