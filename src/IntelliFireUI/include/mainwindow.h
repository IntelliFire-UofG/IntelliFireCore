#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include "sensorContainer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void handleSpeedButton();
    void handleParamButton();
    void initializeADS1115(SensorContainer *container_1, SensorContainer *container_2, SensorContainer *container_3, SensorContainer *container_4);
private:
    // void setupUI();
    void createSliders();
    // void createSensorGrid();
    
    QSlider *speedSlider;
    QSlider *paramSlider;
};

#endif // MAINWINDOW_H
