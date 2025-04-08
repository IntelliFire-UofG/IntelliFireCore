#include <QApplication>
#include "./include/mainwindow.h"
#include <QProcess>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Launch MediaMTX server process
    QScopedPointer<QProcess> mediaMtxProcess(new QProcess);
    mediaMtxProcess->setWorkingDirectory("../mediamtx_folder");
    mediaMtxProcess->start("./mediamtx");

    if (!mediaMtxProcess->waitForStarted()) {
        qDebug() << "❌ Failed to start MediaMTX!";
    } else {
        qDebug() << "✅ MediaMTX started successfully.";
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
