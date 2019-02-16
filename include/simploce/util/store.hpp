#ifndef STORE_HPP
#define STORE_HPP

#include <memory>

namespace simploce {

  class Data;

  /**
   * Data store interface.
   */
  struct Store {
    
    virtual ~Store() {}
    
    /**
     * Appends data to this store.
     * @param data Data.
     */
    virtual void append(const std::shared_ptr<Data>& data) = 0;

    /**
     * Appends data to store.
     * @param data Data.
     */
    virtual void append(const Data& data) = 0;
    
    /**
     * Reads (or copies) next data "entry" from this store.
     * @param data Data. Holds read data upon return.
     * @return False if the end of store is reached, otherwise true.
     */
    virtual bool readNext(std::shared_ptr<Data>& data) = 0;

    /**
     * Reads (or copies) next data "entry" from this store.
     * @param data Data. Holds read data upon return.
     * @return False if the end of store is reached, otherwise true.
     */
    virtual bool readNext(Data& data) = 0;

    /**
     * Closes the store.
     */
    virtual void close() = 0;
    
  };

}

#endif
