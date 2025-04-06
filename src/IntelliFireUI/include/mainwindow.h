#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QLabel>
#include <memory>

#include "sensorContainer.h"
#include "keyLogger.h"
#include "pumpControl.h"

/**
 * @class MainWindow
 * @brief Main UI window of the IntelliFire system.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    KeyLogger* getKeyLogger();  // Exposed for access

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private Q_SLOTS:
    void handleSpeedButton();
    void handleParamButton();
    void initializeADS1115(SensorContainer *c1, SensorContainer *c2,
                           SensorContainer *c3, SensorContainer *c4);
    void updateKeyDisplay(KeyEventInfo keyInfo);
    void updatePumpStatus(float pump_status);

private:
    void createSliders();

    QSlider *speedSlider = nullptr;
    QSlider *paramSlider = nullptr;

    QLabel *keyDisplayLabel = nullptr;
    QLabel *pumpStatusLabel = nullptr;

    std::unique_ptr<KeyLogger> keyLogger;
    std::unique_ptr<PumpControl> pump_control;
};

#endif // MAINWINDOW_H
