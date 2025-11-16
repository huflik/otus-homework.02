#include "ip_filter.h"
#include "cin_redirect.h"

std::vector<std::string> IPFilter::split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

IPAddrPool IPFilter::getIPAddr(const std::string& filename) {

    IPAddrPool ipPool{};

    CinRedirect redirect(filename);

    for(std::string line; std::getline(std::cin, line);)
    {
        auto dataLine = split(line, '\t');
        if(dataLine.size() == 0) {
            throw std::runtime_error("Invalid line format: " + line);
        }

        auto strOctet = split(dataLine.at(0), '.');
        if(strOctet.size() != octetNum) {
            throw std::runtime_error("Invalid IP Address format: " + dataLine.at(0));
        }


        std::array<uint8_t, octetNum> bOctet{};
        for(size_t i = 0; i < octetNum; ++i) {
          
            int octet;

            auto[ptr, ec] = std::from_chars(strOctet[i].data(), strOctet[i].data() + strOctet[i].size(), octet);

            if(ec == std::errc::invalid_argument) {
                throw std::invalid_argument("Invalid octet format");
            }

            if(octet < std::numeric_limits<uint8_t>::min() || octet > std::numeric_limits<uint8_t>::max()) {
                throw std::invalid_argument("Octet out of range");
            }

            bOctet[i] = static_cast<uint8_t>(octet);           
               
        }

        ipPool.emplace_back(bOctet);

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

