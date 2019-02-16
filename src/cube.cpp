#include "simploce/util/cube.hpp"
#include "simploce/conf.hpp"

namespace simploce {

  Cube::Cube() : Box{}
  {
  }

  Cube::Cube(length_t edgeLength) : Box{edgeLength, edgeLength, edgeLength}
  {
  }

  Cube::~Cube()
  {
  }

  length_t Cube::edgeLength() const
  {
    return this->lengthX();
  }

  void Cube::edgeLength_(length_t edgeLength)
  {
    this->lengths(edgeLength, edgeLength, edgeLength);
  }

  std::istream& operator >> (std::istream& stream, Cube &cube)
  {
    length_t edgeLength;
    stream >> edgeLength;
    cube.edgeLength_(edgeLength);
    return stream;
  }
  
  std::ostream& operator << (std::ostream& stream, const Cube &cube)
  {
    stream.precision(PRECISION);
    stream << std::setw(WIDTH) << cube.edgeLength();
    return stream;
  }
  
}
