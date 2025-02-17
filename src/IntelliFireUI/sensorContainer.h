#ifndef SENSORCONTAINER_H
#define SENSORCONTAINER_H
#include <atomic>
#include <QLabel>
#include <QWidget>
#include <QTimer>

class SensorContainer : public QWidget
{
    Q_OBJECT
public:
    explicit SensorContainer(int containerNumber, QWidget *parent = nullptr);
private slots:
    void updateSensorValue();
    void updateUI(int value); 

signals:
    void sensorValueUpdated(int newValue);


private:
    QTimer *timer;  // Store QTimer in the class to prevent crashes
    QLabel *value; 
    int PLACEHOLDER ;




};

#endif // SENSORCONTAINER_H