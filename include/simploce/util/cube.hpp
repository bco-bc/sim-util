#ifndef CUBE_HPP
#define CUBE_HPP

#include "box.hpp"
#include <iostream>

namespace simploce {

  /**
   * A three-dimensional solid object bounded by six square faces, 
   * facets or sides, with three meeting at each vertex. The sides have equal length.
   * L is the length type, and V is the volume type.
   */
  template <typename L, typename V>
  class Cube<L,V> : public Box<L,V> {
  public:
    
    /**
     * Constructs cube with zero edge length.
     */
    Cube() : public Box<L,V>{};
    
    /**
     * Constructs box size with given edge length.
     * @param edgeLength Edge length.
     */
    Cube(length_t edgeLength) : Box<L,V>{edgeLength, edgeLength, edgeLength} {}
    
    virtual ~Cube();
    
    /**
     * Returns edge length.
     * @return Edge length.
     */
    L edgeLength() const { return this->lengthX(); }
    
  private:

    template <typename LL, typename VV>
    friend std::istream & operator >> (std::istream& stream, Cube<LL,VV> &cube);
    
    /**
     * Sets edge length.
     * @param length Edge length.
     */
    void edgeLength_(L edgeLength) { this->lengths(edgeLength, edgeLength, edgeLength); }
    
  };
  
  /**
   * Reads cube from input stream.
   * @param stream input stream. Should hold the cube's edge length.
   * @param cube Cube
   * @return Input stream.
   */
  template <typename L, typename V>
  std::istream& operator >> (std::istream& stream, Cube<L,V> &cube)
  {
    L edgeLength;
    stream >> edgeLength;
    cube.edgeLength_(edgeLength);
    return stream;
  }
  
  /**
   * Writes cube to output stream. Writes its edge length.
   * @param stream Output stream.
   * @param cube Cube
   * @return Output stream.
   */
  template <typename L, typename V>
  std::ostream& operator << (std::ostream& stream, const Cube<L,V> &cube)
  {
    stream.precision(5);
    stream << std::setw(13) << cube.edgeLength();
    return stream;
  }
  
}


#endif
