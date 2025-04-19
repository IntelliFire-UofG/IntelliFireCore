#include <QApplication>
#include "./include/mainwindow.h"
#include <QProcess>
#include <QDebug>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);



    QProcess cameraCheck;
    QScopedPointer<QProcess> mediaMtxProcess(new QProcess);
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


    // Launch MediaMTX server process
    // QScopedPointer<QProcess> mediaMtxProcess(new QProcess);


    mediaMtxProcess->setWorkingDirectory("../mediamtx_folder");
    mediaMtxProcess->start("./mediamtx", QStringList());

    if (!mediaMtxProcess->waitForStarted()) {
        qDebug() << "Failed to start MediaMTX!";
        QMessageBox::warning(nullptr, "Error", "Failed to start camera server.");
    
    } else {
        qDebug() << "MediaMTX started successfully.";
    }
    }

    // Global UI style
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

    int exitCode = app.exec();

    // Ensure MediaMTX is killed safely when app exits
    if (mediaMtxProcess->state() == QProcess::Running) {
        mediaMtxProcess->terminate();
        if (!mediaMtxProcess->waitForFinished(2000)) {
            mediaMtxProcess->kill();
        }
    }

    return exitCode;
}
