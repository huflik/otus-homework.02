#include "ip_filter.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <gtest/gtest.h>


int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS(); 
}
