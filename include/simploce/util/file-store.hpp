#ifndef FILE_STORE_HPP
#define FILE_STORE_HPP

#include "store.hpp"
#include <memory>
#include <string>
#include <fstream>

namespace simploce {
  
  /**
   * A file on the file system acting as a store for data.
   */
  class FileStore : public Store {
  public:
    
    ~FileStore();
    
    void append(const std::shared_ptr<Data>& data) override;

    void append(const Data& data) override;
    
    bool readNext(std::shared_ptr<Data>& data) override;

    bool readNext(Data& data) override;

    void close();
    
    /**
     * Creates new file store for appending data.
     * @param fileName File name.
     * @return File store.
     */
    static std::shared_ptr<Store> createForAppending(const std::string& fileName);

    /**
     * Creates a file store for parsing (reading) it.
     * @param fileName File name.
     * @return File store.
     */
    static std::shared_ptr<Store> createForParsing(const std::string& fileName);

  private:
    
    FileStore(const std::string& fileName, bool forOutput);
    
    std::ofstream ostream_{};
    std::ifstream istream_{};
    bool forOutput_;
    
  };

}

#endif
