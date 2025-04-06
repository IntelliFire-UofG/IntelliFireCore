#include <QApplication>
#include "./include/mainwindow.h"
#include <QProcess>
#include <QDebug>
#include <memory>

int main(int argc, char *argv[]) {
    try {
        QApplication app(argc, argv);

        // Start MediaMTX
        auto mediaMtxProcess = std::make_unique<QProcess>();
        mediaMtxProcess->setWorkingDirectory("../mediamtx_folder");
        mediaMtxProcess->start("./mediamtx");

        if (!mediaMtxProcess->waitForStarted(3000)) {
            qWarning() << "⚠️ Failed to start MediaMTX!";
        } else {
            qDebug() << "✅ MediaMTX started successfully.";
        }

        // Optional: connect output for debug logging
        QObject::connect(mediaMtxProcess.get(), &QProcess::readyReadStandardOutput, [=]() {
            qDebug() << "[MediaMTX stdout]:" << mediaMtxProcess->readAllStandardOutput();
        });
        QObject::connect(mediaMtxProcess.get(), &QProcess::readyReadStandardError, [=]() {
            qWarning() << "[MediaMTX stderr]:" << mediaMtxProcess->readAllStandardError();
        });

        // Apply global stylesheet
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

        // Create and show main window
        MainWindow mainWindow;
        mainWindow.show();

        int result = app.exec();

        // Ensure MediaMTX is terminated
        if (mediaMtxProcess->state() != QProcess::NotRunning) {
            mediaMtxProcess->terminate();
            if (!mediaMtxProcess->waitForFinished(2000)) {
                mediaMtxProcess->kill();  // Force if necessary
            }
        }

        return result;

    } catch (const std::exception &e) {
        qCritical() << "❌ Fatal Qt exception:" << e.what();
        return EXIT_FAILURE;
    } catch (...) {
        qCritical() << "❌ Unknown fatal error.";
        return EXIT_FAILURE;
    }
}
