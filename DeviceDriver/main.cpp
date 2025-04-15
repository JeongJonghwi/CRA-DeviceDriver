#include "gmock/gmock.h"
#include "device_driver.h"

using namespace testing;

class MockFlashMemoryDevice : public FlashMemoryDevice {
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

class DeviceDriverFixture : public Test {
public:
	MockFlashMemoryDevice MockHardware;
	DeviceDriver driver{ &MockHardware };
};

TEST_F(DeviceDriverFixture, ReadMustOccurFiveTimes) {
	EXPECT_CALL(MockHardware, read)
		.Times(5);

	int data = driver.read(0xFF);
}

TEST_F(DeviceDriverFixture, ReadSuccess) {
	EXPECT_CALL(MockHardware, read(_))
		.WillRepeatedly(Return(1));

	int data = driver.read(0xFF);
	EXPECT_EQ(1, data);
}

TEST_F(DeviceDriverFixture, ReadFailWithException) {
	EXPECT_CALL(MockHardware, read(_))
		.WillOnce(Return(2))
		.WillRepeatedly(Return(1));

	try {
		int data = driver.read(0xFF);
		FAIL();
	}
	catch (std::exception& e) {
		EXPECT_EQ(string{ e.what() }, string{ "read fail exception" });
	}
}

TEST_F(DeviceDriverFixture, WriteMustOccurOneRead) {
	EXPECT_CALL(MockHardware, read(_))
		.Times(1)
		.WillRepeatedly(Return(0xFF));

	EXPECT_CALL(MockHardware, write(_, _));

	driver.write(0xFF, 0xAB);
}

TEST_F(DeviceDriverFixture, WriteSuccess) {
	EXPECT_CALL(MockHardware, read(_))
		.WillRepeatedly(Return(0xFF));

	EXPECT_CALL(MockHardware, write(_, _));

	driver.write(0xFF, 0xAB);
}

TEST_F(DeviceDriverFixture, WriteFailWithException) {
	EXPECT_CALL(MockHardware, read(_))
		.WillRepeatedly(Return(0xAB));

	try {
		driver.write(0xFF, 0xAB);
		FAIL();
	}
	catch (std::exception& e) {
		EXPECT_EQ(string{ e.what() }, string{ "write fail exception" });
	}
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}