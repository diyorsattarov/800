#include <iostream>

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <boost/version.hpp>
#include <gtest/gtest.h>

class BaseFixture : public ::testing::Test {
	protected:
	BaseFixture() {
		spdlog::info("Hello world. Welcome to spdlog");
		spdlog::info("Boost version: {}.{}.{}", BOOST_VERSION / 100000, (BOOST_VERSION / 100) % 1000, BOOST_VERSION % 100);
	}
};

class TestFixture : public BaseFixture {
};

TEST_F(TestFixture, FirsTest) {
	ASSERT_EQ(1,1);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
