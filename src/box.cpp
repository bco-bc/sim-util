#include "simploce/util/box.hpp"
#include "simploce/conf.hpp"

namespace simploce {
  
  Box::Box() : lengths_{0.0, 0.0, 0.0}
  {
  }

  Box::Box(length_t lx, length_t ly, length_t lz) : lengths_{lx, ly, lz}
  {
  }

  Box::~Box()
  {
  }
  
  void Box::lengths(length_t lx, length_t ly, length_t lz)
  {
    lengths_[0] = lx;
    lengths_[1] = ly;
    lengths_[2] = lz;
  }
  
  length_t Box::size() const
  {
    length_t max{0.0};
    for (std::size_t k = 0; k != 3; ++k) {
      if ( lengths_[k] > max ) {
	max = lengths_[k];
      }
    }
    return max;
  }

  volume_t Box::volume() const
  {
    return lengths_[0] * lengths_[1] * lengths_[2];
  }
  
  std::ostream & operator << (std::ostream &stream, const Box &box)
  {
    stream.precision(PRECISION);
    stream << std::setw(WIDTH) << box[0];
    stream << SPACE << std::setw(WIDTH) << box[1];
    stream << SPACE << std::setw(WIDTH) << box[2];
    return stream;
  }
  
  std::istream & operator >> (std::istream& stream, Box &box)
  {
    length_t lengthX, lengthY, lengthZ;
    stream >> lengthX >> lengthY >> lengthZ;
    box.lengths(lengthX, lengthY, lengthZ);
    return stream;
  }
  
}
