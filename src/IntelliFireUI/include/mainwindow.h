#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QKeyEvent>
#include "sensorContainer.h"
#include "keyLogger.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    KeyLogger *getKeyLogger();  // Make KeyLogger accessible
    void keyPressEvent(QKeyEvent *event) override;


private slots:
    void handleSpeedButton();
    void handleParamButton();
    void updateKeyDisplay(KeyEventInfo keyInfo);

private:
    // void setupUI();
    void createSliders();
    // void createSensorGrid();
    
    QSlider *speedSlider;
    QSlider *paramSlider;

    QLabel *keyDisplayLabel;
    KeyLogger *keyLogger;

    void updateKeyDisplay(QString key);

};

#endif // MAINWINDOW_H