#pragma once

#include <iostream>
#include <fstream>

class CinRedirect {
    private:
        std::streambuf* m_cin;
        std::ifstream m_file;
    
    public:

        CinRedirect(const std::string& filename); 
    
        ~CinRedirect();
};