#ifndef BOX_HPP
#define BOX_HPP

#include "../types.hpp"
#include <cassert>
#include <vector>

namespace simploce {

  /**
   * 3D Box with arbitrary box side lengths. L is the length type, V is the volume type
   */
  template <typename T = double, typename V = double>
  class Box {
  public:
    
    /**
     * Default constructible. Box with zero edge lengths.
     */
    Box() : lengths_{0.0, 0.0, 0.0} {}
    
    /**
     * Box with arbitrary edge lengths.
     * @param lx Dimension in x-direction.
     * @param ly Dimension in y-direction.
     * @param lz Dimension in z-direction.
     */
    Box(L lx, L ly, L lz) : lengths_{lx, ly, lz} {}

    virtual ~Box();

    /**
     * Returns box dimension (edge length) in x-direction.
     * return Box size.
     */
    L lengthX() const { return lengths_[0]; }
    
    /**
     * Returns box dimension (edge length) in y-direction.
     * return Box size.
     */
    L lengthY() const { return lengths_[1]; }
    
    /**
     * Returns box dimension (edge length) in z-direction.
     * return Box size.
     */
    L lengthZ() const { return lengths_[2]; }
    
    /**
     * Returns box dimension (edge length) for given dimension.
     * @param k Dimension (k < 3, k = 0 -> x, k = 1 -> y, k = 2 -> z).
     * @return Length.
     */
    L operator [] (std::size_t k) const { assert(k < 3); return lengths_[k]; }
    
    /**
     * Returns box edge length for given dimension.
     * @param k Dimension (k < 3, k = 0 -> x, k = 1 -> y, k = 2 -> z).
     * @return Length.
     */
    L operator () (std::size_t k) const { assert(k < 3); return lengths_[k]; }
    
    /**
     * Returns size of the box. This is the largest edge lengths.
     * @return Length.
     */
    L size() const;
    
    /**
     * Returns volume.
     * @return Volume.
     */
    virtual V volume() const { lengths_[0] * lengths_[1] * lengths_[2]; }
    
  protected:
    
    /**
     * Sets edge lengths of the box.
     * @param lx Dimension in x-direction.
     * @param ly Dimension in y-direction.
     * @param lz Dimension in z-direction.
     */
    void lengths(L lx, L ly, L lz) { lengths_ = std::vector<L>{lx, ly, lz}; }
    
  private:

    template <typename LL, typename VV>
    friend std::istream & operator >> (std::istream& stream, Box<LL,VV> &box);
    
    std::vector<L> lengths_;
  };

  /**
   * Writes box to output stream.
   * @param stream Output stream.
   * @param box Box.
   * @return Output stream.
   */
  template <typename L, typename V>
  std::ostream & operator << (std::ostream &stream, const Box<L,V> &box)
  {
    stream.precision(5);
    stream << std::setw(13) << box[0];
    stream << " " << std::setw(13) << box[1];
    stream << " " << std::setw(13) << box[2];
    return stream;
  }
  
    /**
     * Reads box from input stream.
     * @param stream Input stream.
     * @param box Box.
     * @return Input stream.
     */
  template <typename L, typename V>
  std::istream & operator >> (std::istream& stream, Box<L,V> &box)
  {
    L lengthX, lengthY, lengthZ;
    stream >> lengthX >> lengthY >> lengthZ;
    box.lengths(lengthX, lengthY, lengthZ);
    return stream;
  }
    
}


#endif
