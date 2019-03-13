#include "simploce/util/util.hpp"
#include <boost/lexical_cast.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <sstream>

namespace simploce {
  namespace util {

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
    
    bool isNonNegativeInteger(const std::string& s)
    {
      try {
	int n = boost::lexical_cast<int>(s);
	return n >= 0 ? true : false;
      } catch(std::exception exception) {
	// Not a number.
	return false;
      }    
    }

  }
}
