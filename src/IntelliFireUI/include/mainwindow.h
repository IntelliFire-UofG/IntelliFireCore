#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QKeyEvent>
#include "sensorContainer.h"
#include "keyLogger.h"

#include "pumpControl.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    
    explicit MainWindow(QWidget *parent = nullptr);
    

    KeyLogger *getKeyLogger();  // Make KeyLogger accessible
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    


private Q_SLOTS:
    void handleSpeedButton();
    void handleParamButton();
    // void initializeADS1115(SensorContainer *container_1, SensorContainer *container_2, SensorContainer *container_3, SensorContainer *container_4);
    // void updateKeyDisplay(KeyEventInfo keyInfo);
    void updatePumpStatus(float pump_status);

private:
    // void setupUI();
    void createSliders();
    // void createSensorGrid();
    
    QSlider *speedSlider;
    QSlider *paramSlider;

    QLabel *keyDisplayLabel;
    KeyLogger *keyLogger;
    void updateKeyDisplay(QString key);

    QLabel *pumpStatusLabel;
    
    PumpControl *pump_control;

};

#endif // MAINWINDOW_H
