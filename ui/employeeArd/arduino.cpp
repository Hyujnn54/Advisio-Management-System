#include "arduino.h"
#include <QObject>
#include <QDebug>
Arduino::Arduino() : QObject(nullptr) {
    serialPort = new QSerialPort(this);
    connect(serialPort, &QSerialPort::readyRead, this, &Arduino::dataAvailable);
}

Arduino::~Arduino() {
    if (serialPort->isOpen()) {
        serialPort->close();
    }
    delete serialPort;
}

bool Arduino::connectArduino(const QString &portName) {
    serialPort->setPortName(portName);
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (serialPort->open(QIODevice::ReadWrite)) {
        return true;
    } else {
        qDebug() << "Failed to open serial port:" << serialPort->errorString();
        return false;
    }
}

QByteArray Arduino::readData() {
    return serialPort->readAll();
}
void Arduino::sendData(const QString &data) {
    if (serialPort->isOpen()) {
        serialPort->write(data.toUtf8() + "\n"); // Send data with a newline
        serialPort->flush(); // Ensure data is sent immediately
        qDebug() << "Sent to Arduino:" << data;
    } else {
        qDebug() << "Cannot send data: Serial port is not open";
    }
}
