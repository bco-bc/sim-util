#ifndef SINGLETON_HPP
#define	SINGLETON_HPP

#include <boost/utility.hpp>
#include <memory>
#include <cstddef>

namespace simploce {

  /**
   * Singleton pattern. Use it as follows:
   * <pre>
   * class Foo : public Singleton<Foo> {
   * public:
   *   ...
   * private:
   *   friend class Singleton<Foo>; 
   *   Foo();
   *   ...
   * };
   *
   * Foo & foo = Foo::instance();
   * </pre>
   * Foo must be default constructible, its 
   * default constructor should be private.
   * This singleton implementation is based on the 
   * Double Checked Locking Pattern (DCLP).
   * @see <a href="http://preshing.com/20130930/double-checked-locking-is-fixed-in-cpp11/">
   * Double-Checked Locking is Fixed In C++11
   * </a>
   */
  template <typename T>
  class Singleton : private boost::noncopyable {
  public:

    /**
     * Returns pointer. 
     * @return Shared pointer. Never throws.
     */
    static std::shared_ptr<T> ptr()
    {
      if ( !instance_ ) {
	Singleton<T>::init_();
      }
      return instance_;
    }

    /**
     * Returns a reference.
     * @return Reference. Never throws.
     */
    static T& reference()
    {
      // First check.
      if ( instance_.get() == nullptr ) {
	Singleton<T>::init_();
      }
      return *instance_;
    }

  protected:

    Singleton() {}
    
    virtual ~Singleton() {}
    
  private:
    
    static void init_()
    {
      // Second check.
      if ( instance_.get() == nullptr )
	Singleton<T>::instance_.reset(new T());
    }
    
    //static std::unique_ptr<T> instance_;
    static std::shared_ptr<T> instance_;
  };
  
  //  template <typename T>
  //std::unique_ptr<T> Singleton<T> :: instance_ = std::unique_ptr<T>();

  template <typename T>
  std::shared_ptr<T> Singleton<T> :: instance_ = std::shared_ptr<T>();
  
}

#endif
