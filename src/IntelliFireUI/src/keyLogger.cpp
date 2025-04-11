#include "keyLogger.h"
#include <QDebug>

KeyLogger::KeyLogger(QWidget *parent) : QWidget(parent) {
    try {
        setFocusPolicy(Qt::StrongFocus);
    } catch (const std::exception& ex) {
        qWarning() << "Exception setting focus policy:" << ex.what();
    } catch (...) {
        qWarning() << "Unknown exception in KeyLogger constructor.";
    }
}

void KeyLogger::setKeyCallback(std::function<void(KeyEventInfo)> callback) {
    keyCallback = callback;
}

void KeyLogger::keyPressEvent(QKeyEvent *event) {
    try {
        if (!event) {
            qWarning() << "Null event in keyPressEvent.";
            return;
        }

        QString keyPressed;

        switch (event->key()) {
            case Qt::Key_W: keyPressed = "Forward"; break;
            case Qt::Key_A: keyPressed = "Turning Left"; break;
            case Qt::Key_S: keyPressed = "Backward"; break;
            case Qt::Key_D: keyPressed = "Turning Right"; break;
            case Qt::Key_Up: keyPressed = "Forward"; break;
            case Qt::Key_Down: keyPressed = "Backward"; break;
            case Qt::Key_Left: keyPressed = "Turning Left"; break;
            case Qt::Key_Right: keyPressed = "Turning Right"; break;
            case Qt::Key_X: keyPressed = "Stop"; break;
            default: keyPressed = event->text();
        }

        KeyEventInfo keyInfo(keyPressed, event->key(), event->text());

        qDebug() << "Key Pressed:" << keyInfo.keyName
                 << "| KeyCode:" << keyInfo.keyCode
                 << "| Raw Text:" << keyInfo.rawText;

        if (keyCallback) {
            keyCallback(keyInfo);
        }

    } catch (const std::exception& ex) {
        qWarning() << "Exception in keyPressEvent:" << ex.what();
    } catch (...) {
        qWarning() << "Unknown exception in keyPressEvent.";
    }
}

void KeyLogger::keyReleaseEvent(QKeyEvent *event) {
    try {
        if (!event) {
            qWarning() << "Null event in keyReleaseEvent.";
            return;
        }

        QString keyReleased;

        switch (event->key()) {
            case Qt::Key_W: keyReleased = "Stopped Forward"; break;
            case Qt::Key_A: keyReleased = "Stopped Turning Left"; break;
            case Qt::Key_S: keyReleased = "Stopped Backward"; break;
            case Qt::Key_D: keyReleased = "Stopped Turning Right"; break;
            case Qt::Key_Up: keyReleased = "Stopped Forward"; break;
            case Qt::Key_Down: keyReleased = "Stopped Backward"; break;
            case Qt::Key_Left: keyReleased = "Stopped Turning Left"; break;
            case Qt::Key_Right: keyReleased = "Stopped Turning Right"; break;
            case Qt::Key_X: keyReleased = "Released Stop"; break;
            default: keyReleased = event->text();
        }

        KeyEventInfo keyInfo(keyReleased, event->key(), event->text());

        qDebug() << "Key Released:" << keyInfo.keyName
                 << "| KeyCode:" << keyInfo.keyCode
                 << "| Raw Text:" << keyInfo.rawText;

        if (keyCallback) {
            keyCallback(keyInfo);
        }

    } catch (const std::exception& ex) {
        qWarning() << "Exception in keyReleaseEvent:" << ex.what();
    } catch (...) {
        qWarning() << "Unknown exception in keyReleaseEvent.";
    }
}