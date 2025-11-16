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
    
    
    EXPECT_EQ(ipPool[0][0], 1);
    EXPECT_EQ(ipPool[0][1], 1);
    EXPECT_EQ(ipPool[0][2], 1);
    EXPECT_EQ(ipPool[0][3], 1);
    
    EXPECT_EQ(ipPool[6][0], 192);
    EXPECT_EQ(ipPool[6][1], 168);
    EXPECT_EQ(ipPool[6][2], 1);
    EXPECT_EQ(ipPool[6][3], 1);
}

TEST_F(IPFilterTest, GetIPAddrFromNonExistentFile) {
    IPFilter ipFilter;
    
    EXPECT_THROW(ipFilter.getIPAddr("non_existent_file.tsv"), std::runtime_error);
}


TEST_F(IPFilterTest, ReverseSort) {
    IPFilter ipFilter;
    auto ipPool = ipFilter.getIPAddr(testFilename);
    
    ipFilter.reverseSort(ipPool);
    

    EXPECT_GE(ipPool[0][0], ipPool[1][0]);
    

    for (size_t i = 0; i < ipPool.size() - 1; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (ipPool[i][j] != ipPool[i + 1][j]) {
                EXPECT_GE(ipPool[i][j], ipPool[i + 1][j]);
                break;
            }
        }
    }
}


TEST_F(IPFilterTest, FilterByFirstOctet) {
    IPFilter ipFilter;
    auto ipPool = ipFilter.getIPAddr(testFilename);
    
    auto filtered = ipFilter.filter(ipPool, 1);
    

    EXPECT_EQ(filtered.size(), 2);
    
    for (const auto& ip : filtered) {
        EXPECT_EQ(ip[0], 1);
    }
}

TEST_F(IPFilterTest, FilterByFirstAndSecondOctet) {
    IPFilter ipFilter;
    auto ipPool = ipFilter.getIPAddr(testFilename);
    
    auto filtered = ipFilter.filter(ipPool, 46, 70);
    

    EXPECT_EQ(filtered.size(), 2);
    
    for (const auto& ip : filtered) {
        EXPECT_EQ(ip[0], 46);
        EXPECT_EQ(ip[1], 70);
    }
}


TEST_F(IPFilterTest, FilterAnyOctet) {
    IPFilter ipFilter;
    auto ipPool = ipFilter.getIPAddr(testFilename);
    
    auto filtered = ipFilter.filterAny(ipPool, 46);
    

    EXPECT_EQ(filtered.size(), 3);
    
    for (const auto& ip : filtered) {
        bool contains46 = false;
        for (int i = 0; i < 4; ++i) {
            if (ip[i] == 46) {
                contains46 = true;
                break;
            }
        }
        EXPECT_TRUE(contains46);
    }
}

TEST_F(IPFilterTest, FilterNonExistentOctet) {
    IPFilter ipFilter;
    auto ipPool = ipFilter.getIPAddr(testFilename);
    
    auto filtered = ipFilter.filterAny(ipPool, 255); 
    
    EXPECT_EQ(filtered.size(), 0);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS(); 
}