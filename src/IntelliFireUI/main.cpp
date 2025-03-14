#include <QApplication>
#include "./include/mainwindow.h"
#include "pumpControl.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
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
    PumpControl pump_control;
    pump_control.start();
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
    
}