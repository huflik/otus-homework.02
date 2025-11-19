#include "ip_filter.h"
#include "cin_redirect.h"

constexpr const char *defaultFilename = "./../ip_filter.tsv";


int main(int argc, char *argv[]) {
	
	std::string filename = argc == 1 ? defaultFilename : argv[1];

	try{
		
		IPFilter ipFilter;
		auto ipPool = ipFilter.getIPAddr(filename);
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
