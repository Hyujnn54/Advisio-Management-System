#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class Arduino : public QObject
{
    Q_OBJECT

public:
    explicit Arduino(QObject *parent = nullptr);
    ~Arduino();

    int connectArduino();              // 0: Arduino connected, 1: not available, -1: error
    int closeArduino();
    QByteArray readFromArduino();
    bool writeToArduino(QByteArray data);
    QSerialPort* getSerial() const;

private:
    QSerialPort *serial;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 67;
    QString arduino_port_name;
    bool arduino_available;
};

#endif // ARDUINO_H
