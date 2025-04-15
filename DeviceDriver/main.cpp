#include "gmock/gmock.h"
#include "device_driver.h"

using namespace testing;

class MockFlashMemoryDevice : public FlashMemoryDevice {
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

TEST(DeviceDriver, ReadFromHW) {
	MockFlashMemoryDevice MockHardware;
	DeviceDriver driver{ &MockHardware };

	EXPECT_CALL(MockHardware, read)
		.Times(5);

	int data = driver.read(0xFF);
}

TEST(DeviceDriver, ReadFromHWSuccess) {
	MockFlashMemoryDevice MockHardware;
	DeviceDriver driver{ &MockHardware };

	EXPECT_CALL(MockHardware, read(_))
		.WillRepeatedly(Return(1));

	int data = driver.read(0xFF);
	EXPECT_EQ(1, data);
}

TEST(DeviceDriver, ReadFromHWFail) {
	MockFlashMemoryDevice MockHardware;
	DeviceDriver driver{ &MockHardware };

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

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}