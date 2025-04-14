#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QLabel>
#include <memory>
#include <mutex>

#include "sensorContainer.h"
#include "keyLogger.h"
#include "pumpControl.h"
#include "LN298MotorControlV3.h"

/**
 * @class MainWindow
 * @brief Main UI window of the IntelliFire system.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Rule of Five: Prevent copying
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow(MainWindow&&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;

    KeyLogger* getKeyLogger();  // Exposed for access

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private Q_SLOTS:
    void handleSpeedButton();
    void handleParamButton();
    void initializeADS1115(SensorContainer *c1, SensorContainer *c2,
                           SensorContainer *c3, SensorContainer *c4);
    void updateKeyEvent(KeyEventInfo keyInfo);
    void updatePumpStatus(float pump_status);

private:
    void createSliders();

    QSlider *speedSlider = nullptr;
    QSlider *paramSlider = nullptr;

    QLabel *keyDisplayLabel = nullptr;
    QLabel *pumpStatusLabel = nullptr;

    std::unique_ptr<KeyLogger> keyLogger;
    std::unique_ptr<PumpControl> pump_control;
    std::shared_ptr<Motor> motor_left;
    std::shared_ptr<Motor> motor_right;

    std::mutex ui_mutex;  // Optional: For future-proofing UI thread safety
};

#endif // MAINWINDOW_H
