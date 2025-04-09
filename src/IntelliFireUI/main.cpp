#include <QApplication>
#include "./include/mainwindow.h"
#include <QProcess>
#include <QDebug>
// #include "ads1115manager.h" // Waiting for sensor to implement this

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QProcess mediaMtxProcess;
    mediaMtxProcess.setWorkingDirectory("../mediamtx_folder");
    mediaMtxProcess.start("./mediamtx");

    if (!mediaMtxProcess.waitForStarted()) {
        qDebug() << "Failed to start MediaMTX!";
    } else {
        qDebug() << "MediaMTX started successfully.";
    }

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