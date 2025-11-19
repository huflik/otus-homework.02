#include "ip_filter.h"

constexpr const char *defaultFilename = "./../ip_filter.tsv";


int main(int argc, char *argv[]) {
	
	std::string filename = argc == 1 ? defaultFilename : argv[1];

	try{
		
		IPFilter ipFilter;
		std::ifstream file(filename);
		
        auto ipPool = ipFilter.getIPAddr(file ? file : std::cin);
		ipFilter.reverseSort(ipPool);
		ipFilter.out(ipPool);
		ipFilter.out(ipFilter.filter(ipPool,1));
		ipFilter.out(ipFilter.filter(ipPool,46, 70));
		ipFilter.out(ipFilter.filterAny(ipPool, 46));

	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	return 0; 
}
