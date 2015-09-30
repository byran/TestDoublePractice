#include <gmock/gmock.h>

using namespace ::testing;

class I2CInterface
{
public:
	virtual ~I2CInterface(){}
	virtual void Start() = 0;
	virtual bool Write(char data) = 0;
	virtual void Stop() = 0;
};

class BMA150Accelerometer
{
public:
	explicit BMA150Accelerometer(I2CInterface*){  }
	void SelectXRegister(){}
};

class I2CDouble : public I2CInterface
{
public:
	MOCK_METHOD0(Start,void ());
	MOCK_METHOD1(Write, bool (char data));
	MOCK_METHOD0(Stop, void ());
};

TEST(BMA150Accelerometer, Selecting_the_X_register_sends_the_correct_I2C_commands)
{
	//Given
	I2CDouble i2CDouble;
	BMA150Accelerometer accelerometer(&i2CDouble);

	EXPECT_CALL(i2CDouble, Start());
	EXPECT_CALL(i2CDouble, Write(0x38 | 0x01)).WillOnce(Return(true));
	EXPECT_CALL(i2CDouble, Write(0x02)).WillOnce(Return(true));
	EXPECT_CALL(i2CDouble, Stop());

	//When
	accelerometer.SelectXRegister();

	//Then
	
}