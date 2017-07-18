#include "BMA150Accelerometer.h"
#include <gmock/gmock.h>

using namespace ::testing;

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

	//When, Then
	ASSERT_EQ(0, accelerometer.ReadXRegisters());

}
TEST(BMA150Accelerometer, When_reading_the_X_register_X_data_is_recieved)
{
	//Given
	I2CMock i2CMock;
	BMA150Accelerometer accelerometer(&i2CMock);

	::testing::InSequence inSequence;
	EXPECT_CALL(i2CMock, Start());
	EXPECT_CALL(i2CMock, Write(0x38 | 0x00)).WillOnce(Return(true));
	EXPECT_CALL(i2CMock, Read(true)).WillOnce(Return(1));
	EXPECT_CALL(i2CMock, Read(false)).WillOnce(Return(4));
	EXPECT_CALL(i2CMock, Stop());

	//When, Then
	ASSERT_EQ(260, accelerometer.ReadXRegisters());
	
}
