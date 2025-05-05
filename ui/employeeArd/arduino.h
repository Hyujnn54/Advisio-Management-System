#ifndef ARDUINO_H
#define ARDUINO_H

#include <QSerialPort>
#include <QSerialPortInfo>

class Arduino : public QObject {
    Q_OBJECT

public:
    Arduino();
    ~Arduino();
    bool connectArduino(const QString &portName);
    QByteArray readData();
    void sendData(const QString &data);

signals:
    void dataAvailable();

private:
    QSerialPort *serialPort;
};

#endif // ARDUINO_H
