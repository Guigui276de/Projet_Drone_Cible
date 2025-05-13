/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionMode_Manuel;
    QAction *actionMode_Leurre;
    QAction *actionMode_Attaque;
    QAction *actionAfficher_Logs;
    QAction *actionImporter_Map;
    QAction *actionmode_autonome;
    QAction *actionmode_leure;
    QAction *actionmode_attaque;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QSlider *horizontalSlider;
    QPushButton *pb_mode_manuel;
    QLabel *label_Attaque;
    QTextEdit *tb_transmet;
    QPushButton *pb_Straight_Line;
    QGridLayout *layout_cam;
    QPushButton *pb_Zig_Zag;
    QLCDNumber *lcd_speed_num;
    QPushButton *pb_Urgence_Stop;
    QHBoxLayout *layout_gps;
    QMenuBar *menubar;
    QMenu *menumode;
    QMenu *menulogs;
    QMenu *menuimporter_maps;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1214, 600);
        actionMode_Manuel = new QAction(MainWindow);
        actionMode_Manuel->setObjectName("actionMode_Manuel");
        actionMode_Leurre = new QAction(MainWindow);
        actionMode_Leurre->setObjectName("actionMode_Leurre");
        actionMode_Attaque = new QAction(MainWindow);
        actionMode_Attaque->setObjectName("actionMode_Attaque");
        actionAfficher_Logs = new QAction(MainWindow);
        actionAfficher_Logs->setObjectName("actionAfficher_Logs");
        actionImporter_Map = new QAction(MainWindow);
        actionImporter_Map->setObjectName("actionImporter_Map");
        actionmode_autonome = new QAction(MainWindow);
        actionmode_autonome->setObjectName("actionmode_autonome");
        actionmode_autonome->setCheckable(true);
        actionmode_leure = new QAction(MainWindow);
        actionmode_leure->setObjectName("actionmode_leure");
        actionmode_leure->setCheckable(true);
        actionmode_attaque = new QAction(MainWindow);
        actionmode_attaque->setObjectName("actionmode_attaque");
        actionmode_attaque->setCheckable(true);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalSlider = new QSlider(centralwidget);
        horizontalSlider->setObjectName("horizontalSlider");
        horizontalSlider->setOrientation(Qt::Orientation::Horizontal);

        verticalLayout->addWidget(horizontalSlider);


        gridLayout->addLayout(verticalLayout, 4, 2, 1, 1);

        pb_mode_manuel = new QPushButton(centralwidget);
        pb_mode_manuel->setObjectName("pb_mode_manuel");

        gridLayout->addWidget(pb_mode_manuel, 5, 2, 1, 1);

        label_Attaque = new QLabel(centralwidget);
        label_Attaque->setObjectName("label_Attaque");

        gridLayout->addWidget(label_Attaque, 2, 0, 1, 1);

        tb_transmet = new QTextEdit(centralwidget);
        tb_transmet->setObjectName("tb_transmet");

        gridLayout->addWidget(tb_transmet, 1, 3, 1, 1);

        pb_Straight_Line = new QPushButton(centralwidget);
        pb_Straight_Line->setObjectName("pb_Straight_Line");

        gridLayout->addWidget(pb_Straight_Line, 3, 0, 1, 1);

        layout_cam = new QGridLayout();
        layout_cam->setObjectName("layout_cam");

        gridLayout->addLayout(layout_cam, 1, 2, 1, 1);

        pb_Zig_Zag = new QPushButton(centralwidget);
        pb_Zig_Zag->setObjectName("pb_Zig_Zag");

        gridLayout->addWidget(pb_Zig_Zag, 4, 0, 1, 1);

        lcd_speed_num = new QLCDNumber(centralwidget);
        lcd_speed_num->setObjectName("lcd_speed_num");

        gridLayout->addWidget(lcd_speed_num, 3, 2, 1, 1);

        pb_Urgence_Stop = new QPushButton(centralwidget);
        pb_Urgence_Stop->setObjectName("pb_Urgence_Stop");
        pb_Urgence_Stop->setCheckable(true);

        gridLayout->addWidget(pb_Urgence_Stop, 2, 2, 1, 1);

        layout_gps = new QHBoxLayout();
        layout_gps->setObjectName("layout_gps");

        gridLayout->addLayout(layout_gps, 1, 0, 1, 2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1214, 22));
        menumode = new QMenu(menubar);
        menumode->setObjectName("menumode");
        menulogs = new QMenu(menubar);
        menulogs->setObjectName("menulogs");
        menuimporter_maps = new QMenu(menubar);
        menuimporter_maps->setObjectName("menuimporter_maps");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menumode->menuAction());
        menubar->addAction(menulogs->menuAction());
        menubar->addAction(menuimporter_maps->menuAction());
        menumode->addAction(actionmode_autonome);
        menumode->addAction(actionmode_leure);
        menumode->addAction(actionmode_attaque);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionMode_Manuel->setText(QCoreApplication::translate("MainWindow", "Mode Manuel", nullptr));
        actionMode_Leurre->setText(QCoreApplication::translate("MainWindow", "Mode Leurre", nullptr));
        actionMode_Attaque->setText(QCoreApplication::translate("MainWindow", "Mode Attaque", nullptr));
        actionAfficher_Logs->setText(QCoreApplication::translate("MainWindow", "Afficher Logs", nullptr));
        actionImporter_Map->setText(QCoreApplication::translate("MainWindow", "Importer Map", nullptr));
        actionmode_autonome->setText(QCoreApplication::translate("MainWindow", "mode_autonome", nullptr));
        actionmode_leure->setText(QCoreApplication::translate("MainWindow", "mode_leure", nullptr));
        actionmode_attaque->setText(QCoreApplication::translate("MainWindow", "mode_attaque", nullptr));
        pb_mode_manuel->setText(QCoreApplication::translate("MainWindow", "mode manuel", nullptr));
        label_Attaque->setText(QCoreApplication::translate("MainWindow", "Attaque Mode", nullptr));
        pb_Straight_Line->setText(QCoreApplication::translate("MainWindow", "Ligne droite", nullptr));
        pb_Zig_Zag->setText(QCoreApplication::translate("MainWindow", "Serpentin", nullptr));
        pb_Urgence_Stop->setText(QCoreApplication::translate("MainWindow", "Arr\303\252t d'urgence", nullptr));
        menumode->setTitle(QCoreApplication::translate("MainWindow", "mode ", nullptr));
        menulogs->setTitle(QCoreApplication::translate("MainWindow", "logs", nullptr));
        menuimporter_maps->setTitle(QCoreApplication::translate("MainWindow", "importer maps", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
