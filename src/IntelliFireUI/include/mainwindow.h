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

private:
    // void setupUI();
    void createSliders();
    // void createSensorGrid();
    
    QSlider *speedSlider;
    QSlider *paramSlider;
};

#endif // MAINWINDOW_H