#ifndef MATH_CONSTANTS_HPP
#define MATH_CONSTANTS_HPP

#include <cmath>
#include <limits>

namespace simploce {

  /**
   * Holds various values for some mathematical constants.
   * V is the numerical value type (e.g. double).
   */
  template <typename V>
  struct MathConstants {
    
    /**
     * Value for PI.
     * @see <a href="http://en.wikipedia.org/wiki/Pi">PI and Wikipedia</a>
     */
    static const V PI;
    
    /**
     * Value for the number E (or e).
     * @see <a href="http://en.wikipedia.org/wiki/E_%28mathematical_constant%29">E at Wikipedia</a>
     */
    static const V E;
    
    /**
     * Value for the Euler constant.
     * @see <a href="http://en.wikipedia.org/wiki/Euler%E2%80%93Mascheroni_constant">
     *   Euler constant at Wikipedia
     * </a>
     */
    static const V GAMMA;
    
    /**
     * Very large value.
     */
    static const V VERY_LARGE;
    
    /**
     * Large value
     */
    static const V LARGE;
    
    /**
     * Very small value.
     */
    static const V VERY_SMALL;
    
    /**
     * Small value.
     */
    static const V SMALL;
    
  };
  
  template <typename V>
  const V MathConstants<V>::PI = std::acos(-1.L);
  
  template <typename V>
  const V MathConstants<V>::E = 2.71828182845904523536028747135266249775724709369995;
  
  template <typename V>
  const V MathConstants<V>::GAMMA = 0.5772156649015328606065120900824024310421;
  
  template <typename V>
  const V MathConstants<V>::VERY_LARGE = std::numeric_limits<V>::max();
  
  template <typename V>
  const V MathConstants<V>::LARGE = 1.0e+16;
  
  template <typename V>
  const V MathConstants<V>::VERY_SMALL = std::numeric_limits<V>::epsilon();
  
  template <typename V>
  const V MathConstants<V>::SMALL = 1.0e-16;
  
}

#endif
