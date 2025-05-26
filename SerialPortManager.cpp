#include "SerialPortManager.h"
#include <QDebug>

SerialPortManager::SerialPortManager(QObject* parent)
	: QObject(parent), serialPort(new QSerialPort(this)) {
	connect(serialPort, &QSerialPort::readyRead, this, &SerialPortManager::handleReadyRead);
}

SerialPortManager::~SerialPortManager() {
	closeSerialPort();
}

//开启串口
bool SerialPortManager::openSerialPort(const QString& portName, qint32 baudRate) {
	//串口名称
	serialPort->setPortName(portName);
	//波特率
	serialPort->setBaudRate(baudRate);
	//8比特单位数据
	serialPort->setDataBits(QSerialPort::Data8);
	serialPort->setParity(QSerialPort::NoParity);
	serialPort->setStopBits(QSerialPort::OneStop);
	//无校验控制
	serialPort->setFlowControl(QSerialPort::NoFlowControl);

	//以读写形式打开串口
	if (serialPort->open(QIODevice::ReadWrite)) {
		qDebug() << "Serial port opened successfully.";
		return true;
	}
	else {
		qDebug() << "Failed to open serial port:" << serialPort->errorString();
		return false;
	}
}

void SerialPortManager::closeSerialPort() {
	if (serialPort->isOpen()) {
		serialPort->close();
		qDebug() << "Serial port closed.";
	}
}

void SerialPortManager::writeData(const QByteArray& data) {
	if (serialPort->isOpen()) {
		serialPort->write(data);
	}
}

QStringList SerialPortManager::getAvailablePorts() const {
	QStringList portList;
	foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts()) {
		portList << info.portName();
	}
	return portList;
}

void SerialPortManager::Test()
{
	emit dataReceived("1234567891234\r");
}

void SerialPortManager::handleReadyRead() {
	//读出来的数据是字节流
	QByteArray data = serialPort->readAll();
	emit dataReceived(data);
}
