#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#include <QWidget>
#include <QKeyEvent>
#include <functional>

/**
 * @struct KeyEventInfo
 * @brief Contains details about a keyboard event.
 */
struct KeyEventInfo {
    QString keyName;
    int keyCode;
    QString rawText;

    KeyEventInfo(const QString& name, int code, const QString& text)
        : keyName(name), keyCode(code), rawText(text) {}
};

/**
 * @class KeyLogger
 * @brief Captures key press and release events and sends them via a callback.
 */
class KeyLogger : public QWidget {
    Q_OBJECT

public:
    explicit KeyLogger(QWidget *parent = nullptr);
    virtual ~KeyLogger() override = default;

    /**
     * @brief Set a callback function to handle key events.
     * @param callback The function to be executed when a key event occurs.
     */
    void setKeyCallback(const std::function<void(const KeyEventInfo&)>& callback);

    /**
     * @brief Handles key press events.
     * @param event The key event data.
     */
    void keyPressEvent(QKeyEvent *event) override;

    /**
     * @brief Handles key release events.
     * @param event The key event data.
     */
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    std::function<void(const KeyEventInfo&)> keyCallback;
};

#endif // KEYLOGGER_H
