#include "ip_filter.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <gtest/gtest.h>

class IPFilterTest : public ::testing::Test {
protected:
    IPFilter ipFilter;
    
    void SetUp() override {
        // Создаем тестовые данные
        testData = {
            "1.1.1.1\t111\t0",
            "1.2.3.4\t222\t0", 
            "10.20.30.40\t333\t0",
            "46.70.1.1\t444\t0",
            "46.70.2.2\t555\t0",
            "70.46.1.1\t666\t0",
            "192.168.1.1\t777\t0",
            "255.255.255.255\t888\t0",
            "0.0.0.0\t999\t0"
        };
    }

    std::vector<std::string> testData;
};

TEST_F(IPFilterTest, GetIPAddrFromValidStream) {
    std::stringstream ss;
    for (const auto& line : testData) {
        ss << line << "\n";
    }
    
    auto ipPool = ipFilter.getIPAddr(ss);
    EXPECT_EQ(ipPool.size(), testData.size());
    
    std::array<uint8_t, 4> firstIP = {1, 1, 1, 1};
    EXPECT_EQ(ipPool[0], firstIP);
}

TEST_F(IPFilterTest, GetIPAddrWithInvalidData) {
    std::stringstream ss;
    ss << "1.1.1.1\t111\t0\n";
    ss << "invalid_data\n";
    ss << "2.2.2.2\t222\t0\n";
    
    EXPECT_THROW(ipFilter.getIPAddr(ss), std::runtime_error);
}


TEST_F(IPFilterTest, ReverseSort) {
    std::stringstream ss;
    ss << "1.1.1.1\t111\t0\n";
    ss << "2.2.2.2\t222\t0\n";
    ss << "3.3.3.3\t333\t0\n";
    
    auto ipPool = ipFilter.getIPAddr(ss);
    ipFilter.reverseSort(ipPool);
    
    std::array<uint8_t, 4> firstIP = {3, 3, 3, 3};
    std::array<uint8_t, 4> lastIP = {1, 1, 1, 1};
    
    EXPECT_EQ(ipPool[0], firstIP);
    EXPECT_EQ(ipPool[2], lastIP);
}


TEST_F(IPFilterTest, FilterSingleOctet) {
    std::stringstream ss;
    for (const auto& line : testData) {
        ss << line << "\n";
    }
    
    auto ipPool = ipFilter.getIPAddr(ss);
    auto filtered = ipFilter.filter(ipPool, 46);
    

    EXPECT_EQ(filtered.size(), 2);
    
    for (const auto& ip : filtered) {
        EXPECT_EQ(ip[0], 46);
    }
}

TEST_F(IPFilterTest, FilterTwoOctets) {
    std::stringstream ss;
    for (const auto& line : testData) {
        ss << line << "\n";
    }
    
    auto ipPool = ipFilter.getIPAddr(ss);
    auto filtered = ipFilter.filter(ipPool, 46, 70);
    
    EXPECT_EQ(filtered.size(), 2);
    
    for (const auto& ip : filtered) {
        EXPECT_EQ(ip[0], 46);
        EXPECT_EQ(ip[1], 70);
    }
}


TEST_F(IPFilterTest, FilterAny) {
    std::stringstream ss;
    for (const auto& line : testData) {
        ss << line << "\n";
    }
    
    auto ipPool = ipFilter.getIPAddr(ss);
    auto filtered = ipFilter.filterAny(ipPool, 46);
    
    EXPECT_EQ(filtered.size(), 3); // 46.70.1.1, 46.70.2.2, 70.46.1.1
    
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

TEST_F(IPFilterTest, IntegrationTestWithRealFile) {

    std::string filename = "test_ip_data.tsv";
    std::ofstream file(filename);
    
    for (const auto& line : testData) {
        file << line << "\n";
    }
    file.close();
    
    std::ifstream inputFile(filename);
    auto ipPool = ipFilter.getIPAddr(inputFile);
    
    EXPECT_EQ(ipPool.size(), testData.size());
    

    ipFilter.reverseSort(ipPool);
    
    auto filtered46 = ipFilter.filter(ipPool, 46);
    auto filteredAny46 = ipFilter.filterAny(ipPool, 46);
    
    EXPECT_GE(filtered46.size(), 0);
    EXPECT_GE(filteredAny46.size(), 0);

    std::remove(filename.c_str());
}

TEST_F(IPFilterTest, BoundaryValues) {
    std::stringstream ss;
    ss << "0.0.0.0\t111\t0\n";
    ss << "255.255.255.255\t222\t0\n";
    ss << "127.0.0.1\t333\t0\n";
    
    auto ipPool = ipFilter.getIPAddr(ss);
    
    EXPECT_EQ(ipPool.size(), 3);
    
    std::array<uint8_t, 4> minIP = {0, 0, 0, 0};
    EXPECT_EQ(ipPool[0], minIP);
    
    std::array<uint8_t, 4> maxIP = {255, 255, 255, 255};
    EXPECT_EQ(ipPool[1], maxIP);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}