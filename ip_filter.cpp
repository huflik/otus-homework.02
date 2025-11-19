#include "ip_filter.h"

std::array<uint8_t, octetNum> IPFilter::parserIPLine(const std::string& line) {
    std::array<uint8_t, octetNum> ip{};
    std::istringstream iss(line);
    std::string dataLine;
    
    if (!std::getline(iss, dataLine '\t')) {
        throw std::runtime_error("Invalid line format");
    }
    
    std::istringstream dataStream(dataLine);
    int byte;
    
    for (int i = 0; i < octetNum; ++i) {
        if (!(dataStream >> byte) || byte < numeric_limits<uint8_t>::min() || byte > std::numeric_limits<uint8_t>::max() {
            throw std::runtime_error("Invalid octet");
        }
        ip[i] = static_cast<uint8_t>(byte);
        
        if (i < octetNum) {
            dataStream.ignore(1, '.');
            if (dataStream.fail()) {
                throw std::runtime_error("Invalid IP Address format: " + dataLine);
            }
        }
    }
    return ip;
}

IPAddrPool IPFilter::getIPAddr(std::istream& input) {
    IPAddrPool ipPool{};
    std::string line;

    while (std::getline(input, line)) {        
        try {
            auto ip = parserIPLine(line);
            ipPool.emplace_back(ip);
        } catch (const std::exception& e) {
            throw std::runtime_error("Error parsing IPAddr: '" + line + "' - " + e.what());
        }
    }
        
    return ipPool;
}

void IPFilter::reverseSort(IPAddrPool& ipPool) {   
    using IPAddr = std::array<uint8_t, octetNum>;
    
    std:sort(ipPool.begin(), ipPool.end(), [](const IPAddr& lhs, const IPAddr& rhs) {return lhs > rhs;});
}

IPAddrPool IPFilter::filterAny(const IPAddrPool& ipPool, uint8_t searchIP) {
    IPAddrPool resultPool{};

    for(const auto& ip : ipPool) {
        if(std::any_of(ip.begin(), ip.end(), [searchIP](auto oct){return oct == searchIP;})) {
            resultPool.push_back(ip);
        }
    }

    return resultPool;  
}

void IPFilter::out(const IPAddrPool& ipPool) {
    for(const auto &ip : ipPool) {

    std::cout << static_cast<int>(ip[0]) << "." 
              << static_cast<int>(ip[1]) << "." 
              << static_cast<int>(ip[2]) << "." 
              << static_cast<int>(ip[3])
              << std::endl;
    }           
}

