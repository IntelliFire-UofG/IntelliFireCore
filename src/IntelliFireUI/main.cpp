#include <QApplication>
#include "./include/mainwindow.h"
#include <QProcess>
#include <QDebug>
#include <QMessageBox>
// #include "ads1115manager.h" // Waiting for sensor to implement this

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);



    QProcess cameraCheck;
    cameraCheck.start("libcamera-hello", QStringList() << "--list-cameras");
    if (!cameraCheck.waitForStarted()) {
        QMessageBox::warning(nullptr, "Camera Error", "Failed to run camera check.");
    }
    cameraCheck.waitForFinished();
    QString result = cameraCheck.readAllStandardOutput();

    if (result.contains("No cameras available")) {
        QMessageBox::critical(nullptr, "Camera Error", "No camera detected!");

    }
    else{

    QProcess mediaMtxProcess;
    mediaMtxProcess.setWorkingDirectory("../mediamtx_folder");
    mediaMtxProcess.start("./mediamtx");

    if (!mediaMtxProcess.waitForStarted()) {
        qDebug() << "Failed to start MediaMTX!";
        QMessageBox::warning(nullptr, "Error", "Failed to start camera server.");
    
    } else {
        qDebug() << "MediaMTX started successfully.";
    }
    }
    ///////////////////////////////////////
    // QProcess mediaMtxProcess;
    // mediaMtxProcess.setWorkingDirectory("../mediamtx_folder");
    // mediaMtxProcess.start("./mediamtx");

    // if (!mediaMtxProcess.waitForStarted()) {
    //     qDebug() << "Failed to start MediaMTX!";
    //     QMessageBox::critical(nullptr, "Error", "Failed to start camera server.");
    
    // } else {
    //     qDebug() << "MediaMTX started successfully.";
    // }
    /////////////////////////////////////////////////

    // Set global styles
    app.setStyleSheet(R"(
        QWidget {
            background: #f5f5f5;
        }
        QPushButton {
            background: #0078d4;
            color: white;
            border: none;
            padding: 8px 16px;
            border-radius: 4px;
            font-family: Arial;
        }
        QPushButton:hover {
            background: #006cbd;
        }
    )");
    
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}