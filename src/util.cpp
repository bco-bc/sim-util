#include "simploce/util/util.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <sstream>

namespace simploce {

  std::ifstream& openInputFile(std::ifstream& stream, const std::string& fileName)
  {
    stream.open(fileName, std::ios_base::in);
    if ( !stream.good() ) {
      stream.close();
      throw std::runtime_error(fileName + ": Cannot open this file.");
    }
    return stream;
  }
  
  std::ofstream& openOutputFile(std::ofstream& stream, const std::string& fileName)
  {
    stream.open(fileName, std::ios_base::out);
    if ( !stream.good() ) {
      stream.close();
      throw std::runtime_error(fileName + ": Cannot open this file.");
    }
    return stream;
  }

}
