#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <charconv>
#include <limits>
#include <algorithm>


constexpr static const uint8_t octetNum = 4;

using IPAddrPool = std::vector<std::array<uint8_t, octetNum>>;

class IPFilter{

    public:
       
        IPFilter() = default;
        ~IPFilter() = default;
        IPAddrPool getIPAddr(std::istream& input);
        void reverseSort(IPAddrPool& ipPool);
        void out(const IPAddrPool& ipPool);
        IPAddrPool filterAny(const IPAddrPool& ipPool, uint8_t searchIP);

        template<typename... Args>
        IPAddrPool filter(const IPAddrPool& ipPool, Args... args) {
            
            if(sizeof...(args) <= 0 && sizeof...(args) > octetNum) {
                throw std::invalid_argument ("Count of argumens out of range");
            }
   
            IPAddrPool resultPool{};
            std::array<uint8_t, sizeof...(args)> filterIP = {static_cast<uint8_t>(args)...};
    
            for (const auto& ip : ipPool) {
                
                bool matches = true;
        
                for (size_t i = 0; i < filterIP.size(); ++i) {
                    
                    if (ip[i] != filterIP[i]) {
                        matches = false;              
                        break;
                }
            }
        
            if (matches) {
                resultPool.push_back(ip);
            }
        }
    
        return resultPool;
    }
        
    private: 

        std::array<uint8_t, octetNum> parserIPLine(const std::string& line);
    
};
