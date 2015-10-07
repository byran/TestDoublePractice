#include <gmock/gmock.h>

using namespace ::testing;

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
	void I2CWrite(char data)
	{
		if (!i2C->Write(data))
		{
			i2C->Stop();
			throw BMA150CommunicationFailure();
		}
	}
public:
	explicit BMA150Accelerometer(I2CInterface* i2CPort) : i2C(i2CPort) {  }
	void SelectXRegister()
	{
		i2C->Start();
		I2CWrite(0x38 | 0x01);
		I2CWrite(0x02);
		i2C->Stop();
	}

	short ReadXRegisters()
	{
		return 0;
	}

private:
	I2CInterface* i2C;
};

class I2CMock : public I2CInterface
{
public:
	MOCK_METHOD0(Start, void ());
	MOCK_METHOD1(Write, bool (char data));
	MOCK_METHOD1(Read, char (bool ack));
	MOCK_METHOD0(Stop, void ());
};

TEST(BMA150Accelerometer, Selecting_the_X_register_sends_the_correct_I2C_commands)
{
	//Given
	I2CMock i2CMock;
	BMA150Accelerometer accelerometer(&i2CMock);

	::testing::InSequence inSequence;
	EXPECT_CALL(i2CMock, Start());
	EXPECT_CALL(i2CMock, Write(0x38 | 0x01)).WillOnce(Return(true));
	EXPECT_CALL(i2CMock, Write(0x02)).WillOnce(Return(true));
	EXPECT_CALL(i2CMock, Stop());

	//When
	accelerometer.SelectXRegister();

	//Then
	
}

TEST(BMA150Accelerometer, When_selecting_the_X_register_and_the_chip_nacks_the_address_then_an_exception_is_thrown)
{
	//Given
	I2CMock i2CMock;
	BMA150Accelerometer accelerometer(&i2CMock);

	::testing::InSequence inSequence;
	EXPECT_CALL(i2CMock, Start());
	EXPECT_CALL(i2CMock, Write(0x38 | 0x01)).WillOnce(Return(false));
	EXPECT_CALL(i2CMock, Stop());

	//When
	ASSERT_THROW(accelerometer.SelectXRegister(), BMA150CommunicationFailure);

	//Then

}

TEST(BMA150Accelerometer, When_selecting_the_X_register_and_the_chip_nacks_the_register_address_then_an_exception_is_thrown)
{
	//Given
	I2CMock i2CMock;
	BMA150Accelerometer accelerometer(&i2CMock);

	::testing::InSequence inSequence;
	EXPECT_CALL(i2CMock, Start());
	EXPECT_CALL(i2CMock, Write(0x38 | 0x01)).WillOnce(Return(true));
	EXPECT_CALL(i2CMock, Write(0x02)).WillOnce(Return(false));
	EXPECT_CALL(i2CMock, Stop());


	//When
	ASSERT_THROW(accelerometer.SelectXRegister(), BMA150CommunicationFailure);

	//Then

}

TEST(BMA150Accelerometer, When_reading_the_X_register_the_correct_I2C_commands_are_sent)
{
	//Given
	I2CMock i2CMock;
	BMA150Accelerometer accelerometer(&i2CMock);

	::testing::InSequence inSequence;
	EXPECT_CALL(i2CMock, Start());
	EXPECT_CALL(i2CMock, Write(0x38 | 0x00)).WillOnce(Return(true));
	EXPECT_CALL(i2CMock, Read(true)).WillOnce(Return(0));
	EXPECT_CALL(i2CMock, Read(false)).WillOnce(Return(0));
	EXPECT_CALL(i2CMock, Stop());

	//When
	accelerometer.ReadXRegisters();

	//Then


	/*Start
		Write(address with write bit cleared)
		Read(…) x 6 with ack set on all but last Read operation
		Stop*/

}

