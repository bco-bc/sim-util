#ifndef ENTITY_RANGE_HPP
#define ENTITY_RANGE_HPP

#include <vector>
#include <utility>
#include <thread>
#include <future>

namespace simploce {

  /**
   * Defines a range (or subset) of entities in a larger collection of entities of type T. 
   * Each range consists of a start point and an end point in the entity collection, and 
   * may include the number of possible pairs for the range. 
   * Assuming each entity is indexed, all entities in the range may form pairs with all 
   * entities in the set that have a higher index. 
   * The template parameter serves as a discriminator.
   */
  template <typename T>
  class EntityRange {
  public:

    /**
     * Constructor.
     * @param start Start point.
     * @param end End point.
     * @param npairs Number of possible pairs.
     * @return Range.
     */
    EntityRange(std::size_t start, std::size_t end, std::size_t npairs);

    /**
     * Returns start point.
     * @return Index poiting to an element of an entity collection.
     */
    std::size_t start() const { return start_; }

    /**
     * Returns end point.
     * @return Index poiting to an element of an entity collection.
     */
    std::size_t end() const { return end_; }

    /**
     * Returns numbers of pairs possible.
     * @return Number of pairs.
     */
    std::size_t numberOfPairs() const { return npairs_; }

  private:

    std::size_t start_;
    std::size_t end_;
    std::size_t npairs_;
  };

  template <typename T>
  EntityRange<T>::EntityRange(std::size_t start, std::size_t end, std::size_t npairs) :
    start_{start}, end_{end}, npairs_{npairs}
  {
    if ( start_ >= end_) {
      std::string message = "EntityRange: The start point (index) of an entity range ";
      message += "must be smaller than the end point (index).";
      throw std::domain_error(message);
    }
  }

  /**
   * Creates ranges for an entity collection of type T. This is based on the number of 
   * available threads 
   * @param entities Entities.
   * @return Ranges.
   */
  template <typename T>
  std::vector<EntityRange<T>> createRanges(const std::vector<T>& entities)
  {
    using entity_range_t = EntityRange<T>;
    static const std::size_t NMAX = 1000;
    
    // Determine number of pairs per range. Assumes at least four hardware threads.
    std::size_t nentities = entities.size();
    std::size_t npairs = nentities * ( nentities - 1) / 2;
    std::size_t hardware_concurrency = std::thread::hardware_concurrency();
    std::size_t nthreads = ( hardware_concurrency < 4 ? 4 : hardware_concurrency );
    std::size_t npairsPerRange = npairs / nthreads;

    // Prevent concurrency for a small set of particles.
    npairsPerRange = ( nentities < NMAX ? npairs : npairsPerRange);
    std::clog << "TOTAL number of pairs: " << npairs << std::endl;
    std::clog << "Number of pairs per range: " << npairsPerRange << std::endl;
    
    // Set up the ranges.
    std::vector<entity_range_t> ranges;
    std::size_t counter = 0;
    while (counter != nentities) {
      std::size_t start = counter;  // Start of current range.
      std::size_t npairs = 0;
      while (npairs < npairsPerRange && counter != nentities) {
	npairs += nentities - (counter + 1);    // Number of pairs for one entities.     
	counter += 1;                           // Next entity.
      }
      std::size_t end = counter;    // End of current range.
      entity_range_t range{start, end, npairs};
      if ( range.numberOfPairs() > 0 ) {
	ranges.push_back(range);
      }
    }

    // Validate...
    if ( counter != nentities) {
      throw std::domain_error("Concurrent: Some entities are excluded from ranges.");
    }

    return ranges;
    
  }

}

#endif
