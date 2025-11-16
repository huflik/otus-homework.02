#include "cin_redirect.h"

    CinRedirect::CinRedirect(const std::string& filename) : m_cin(nullptr), 
                                                            m_file(filename, std::ifstream::in){
              
        
        if (!m_file.fail()) {
            m_cin = std::cin.rdbuf(); 
            std::cin.rdbuf(m_file.rdbuf()); 
            
        //for(std::string line; std::getline(std::cin, line);)
		//{
		//	std::cout << "CinRedirect: " << line << std::endl;
		//}


        }else {
            throw std::runtime_error("Failed to open the file");
        }
    }

    
    CinRedirect::~CinRedirect() {
        if (m_cin) {
            std::cin.rdbuf(m_cin);
        }       
    }