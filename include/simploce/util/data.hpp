#ifndef DATA_HPP
#define DATA_HPP

#include <iostream>

namespace simploce {

  /**
   * Holds data.
   */
  struct Data {
    
    virtual ~Data() {}
    
    /**
     * Appends this data to output stream.
     * @param stream Output stream.
     */
    virtual void append(std::ostream& stream) const = 0;
    
    /**
     * Reads (copies) next data entry from input stream.
     * @param stream Input stream.
     */
    virtual void readNext(std::istream& stream) = 0;
    
  };
  
}

#endif
