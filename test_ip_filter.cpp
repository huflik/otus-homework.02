#include "ip_filter.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <gtest/gtest.h>


class IPFilterTest : public ::testing::Test {
protected:
    void SetUp() override {
        createTestFile();
    }

    void TearDown() override {
        if (std::filesystem::exists(testFilename)) {
            std::filesystem::remove(testFilename);
        }
    }

    void createTestFile() {

        std::ofstream file(testFilename);

        file << "1.1.1.1\t111\t0\n";
        file << "2.2.2.2\t222\t0\n";
        file << "1.2.3.4\t333\t0\n";
        file << "46.70.225.39\t444\t0\n";
        file << "46.70.113.73\t555\t0\n";
        file << "186.46.222.194\t666\t0\n";
        file << "192.168.1.1\t777\t0\n";
        file.close();
    }

    const std::string testFilename = "test_ip_filter.tsv";
};

TEST_F(IPFilterTest, GetIPAddrFromFile) {
    IPFilter ipFilter;
    
    auto ipPool = ipFilter.getIPAddr(testFilename);
    
    EXPECT_EQ(ipPool.size(), 7);
    
    // Проверяем первый IP
    EXPECT_EQ(ipPool[0][0], 1);
    EXPECT_EQ(ipPool[0][1], 1);
    EXPECT_EQ(ipPool[0][2], 1);
    EXPECT_EQ(ipPool[0][3], 1);
    
    // Проверяем последний IP
    EXPECT_EQ(ipPool[6][0], 192);
    EXPECT_EQ(ipPool[6][1], 168);
    EXPECT_EQ(ipPool[6][2], 1);
    EXPECT_EQ(ipPool[6][3], 1);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS(); 
}