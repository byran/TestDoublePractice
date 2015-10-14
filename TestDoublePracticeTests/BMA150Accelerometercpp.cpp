#include "BMA150Accelerometer.h"

void BMA150Accelerometer::I2CWrite(char data)
{
	if (!i2C->Write(data))
	{
		i2C->Stop();
		throw BMA150CommunicationFailure();
	}
}
BMA150Accelerometer::BMA150Accelerometer(I2CInterface* i2CPort) : i2C(i2CPort)
{
}

void BMA150Accelerometer::SelectXRegister()
{
	i2C->Start();
	I2CWrite(0x38 | 0x01);
	I2CWrite(0x02);
	i2C->Stop();
}

short BMA150Accelerometer::ReadXRegisters()
{
	i2C->Start();
	I2CWrite(0x38);
	char msb = i2C->Read(true);
	char lsb = i2C->Read(false);
	i2C->Stop();
	return ((msb << 8) + lsb);
}
