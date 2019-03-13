#include "simploce/util/file-store.hpp"
#include "simploce/util/data.hpp"
#include "simploce/util/util.hpp"

namespace simploce {

  FileStore::FileStore(const std::string& fileName, bool forOutput) :
    Store{}, forOutput_{forOutput}
    {
      if (forOutput_) {
	util::openOutputFile(ostream_, fileName);
      } else {
	util::openInputFile(istream_, fileName);
      }
    }
  
  FileStore::~FileStore()
  {
    if (forOutput_) {
      ostream_.close();
    } else {
      istream_.close();
    }
  }
  
  void FileStore::append(const std::shared_ptr<Data>& data)
  {
    data->append(ostream_);
    ostream_.flush();
  }
  
  void FileStore::append(const Data& data)
  {
    data.append(ostream_);
    ostream_.flush();
  }

  bool FileStore::readNext(std::shared_ptr<Data>& data)
  {
    istream_.peek();
    if ( !istream_.eof() ) {
      data->readNext(istream_);
      return !istream_.good();
    } else {
      return true;
    }
  }
  
  bool FileStore::readNext(Data& data)
  {
    istream_.peek();
    if ( !istream_.eof() ) {
      data.readNext(istream_);
      return !istream_.good();
    } else {
      return true;
    }
  }

  void FileStore::close()
  {
    if ( forOutput_ ) {
      ostream_.flush();
      ostream_.close();
    } else {
      istream_.close();
    }
  }
  
  std::shared_ptr<Store> FileStore::createForAppending(const std::string& fileName)
  {
    return std::shared_ptr<FileStore>(new FileStore{fileName, true});
  }

  std::shared_ptr<Store> FileStore::createForParsing(const std::string& fileName)
  {
    return std::shared_ptr<FileStore>(new FileStore{fileName, false});
  }  
  
}
