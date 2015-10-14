#pragma once

class BMA150CommunicationFailure
{

};

class I2CInterface
{
public:
	virtual ~I2CInterface(){}
	virtual void Start() = 0;
	virtual bool Write(char data) = 0;
	virtual char Read(bool ack) = 0;
	virtual void Stop() = 0;
};

class BMA150Accelerometer
{
private:
	void I2CWrite(char data);
public:
	explicit BMA150Accelerometer(I2CInterface* i2CPort);
	void SelectXRegister();
	short ReadXRegisters();

private:
	I2CInterface* i2C;
};
