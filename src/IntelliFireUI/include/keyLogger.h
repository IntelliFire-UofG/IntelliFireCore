#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#include <QWidget>
#include <QKeyEvent>
#include <functional>

struct KeyEventInfo {
    QString keyName;
    int keyCode;
    QString rawText;

    KeyEventInfo(QString name, int code, QString text)
        : keyName(name), keyCode(code), rawText(text) {}
};

class KeyLogger : public QWidget {
    Q_OBJECT

public:
    explicit KeyLogger(QWidget *parent = nullptr);
    void setKeyCallback(std::function<void(KeyEventInfo)> callback);

    void keyPressEvent(QKeyEvent *event) override;

private:
    std::function<void(KeyEventInfo)> keyCallback;
};

#endif // KEYLOGGER_H
