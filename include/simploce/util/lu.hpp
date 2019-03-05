#ifndef LU_HPP
#define LU_HPP

#include <iostream>
#include <stdexcept>
#include <cmath>
#include <limits>

namespace simploce {

  /**
   * See Press et al, "Numerical recipes in C++. The art of scientific computing", 2ed, 
   * Cambridge University Press, Cambridge, 2002.
   */

  /**
   * Performs LU-decomposition.
   * @param a - Matrix. a(i,j) refers to an element value with i is the row index, and j the 
   * column index.
   * @param ndim - Dimension of a. 
   * @param indx - Upon return, holds pivot information.
   * @returns -1 or +1, depending on whether the number of rows interchanges was even or odd, 
   * respectively.
   */
  template <typename T, template <typename...> class MATRIX, template <typename...> class VECTOR>
  std::size_t luDecomposition(MATRIX<T> &a, std::size_t ndim, VECTOR<std::size_t> &indx)
  {
    std::size_t n = ndim/100.0 * 10.0;  // 10%
    const static T SMALL = std::numeric_limits<T>::epsilon();

    VECTOR<T> vv(ndim);
    indx = VECTOR<std::size_t>(ndim);
    T d{1.0};
    
    // Find scale factor for each equation.
    for (std::size_t i = 0; i != ndim; ++i) {
      T aamax = 0.0;
      for (std::size_t j = 0; j != ndim; ++j) {
	T dum = std::fabs(a(i,j));
	aamax = ( dum > aamax ? dum : aamax );
      }
      if (aamax <= SMALL) {
	throw std::domain_error("Matrix LU-decomposition: Singular matrix.");
      }
      vv[i] = 1.0 / aamax;
    }
    
    // Loop over COLUMNS starts.
    for (std::size_t j = 0; j != ndim; ++j) {

      if ( j % n == 0) {
	double real_j = j;
	double real_ndim = ndim;
	std::clog << real_j/real_ndim * 100.0 << "% LU completed." << std::endl;
      }
      
      // Compute betas above diagonal.
      for (std::size_t i = 0; i != j; ++i) {
	T sum = a(i,j);
	for (std::size_t k = 0; k != i; ++k) {
	  sum -= a(i,k) * a(k,j);
	}
	a(i,j) = sum;
      }
      
      // Start search for pivot.
      T aamax = 0.0;
      std::size_t imax = 0;
      for (std::size_t i = j; i != ndim; ++i) {	
	// Now compute diagonal betas and alpha below diagonal.
	// The diagonal betas can be got this way because no pivoting
	// is done until the largest element is found.
	T sum = a(i,j);
	for (std::size_t k = 0; k != j; ++k) {
	  sum -= a(i,k) * a(k,j);
	}
	a(i,j) = sum;
	T dum = vv[i] * std::fabs(sum); //  figure of merit for the pivot
	if (dum >= aamax) {
	  imax = i;
	  aamax = dum;
	}
      }
      
      if (j != imax) {
	// Interchange rows.
	for (std::size_t k = 0; k != ndim; ++k) {
	  T dum = a(imax, k);
	  a(imax, k) = a(j, k);
	  a(j, k) = dum;
	}
	vv(imax) = vv(j);    // Swap scale factors too.
	d = -d;              // Change parity.	
      }
      
      // Save permutation of the rows.
      indx[j] = imax;
      a(j,j) = ( std::fabs(a(j,j)) <= SMALL ? SMALL : a(j,j) );
      if (j != ndim - 1) {
	// Divide sub-diagonal alphas by pivot element.
	T dum = 1.0 / a(j,j);
	for (std::size_t i = j + 1; i != ndim; ++i) {
	  a(i,j) *= dum;
	}
      }
    }

    return d;
  }


  /*
   * Backsubstitution. May be used to solve the matrix equation Ax=b. Needed is the
   * LU-decomposed matrix of A. The unknown vector x is returned in b.
   * @param a - LU-decomposed matrix.
   * @param ndim - dimension of matrix a.
   * @param indx - Pivot information (delivered by `luDecomposition').
   * @param b - The right hand side of the matrix equation Ax=b. Upon return, this holds 
   * the solution x.
   */
  template <typename T, template <typename...> class MATRIX, template <typename...> class VECTOR>
  void backSubstitution(const MATRIX<T>& a,
			std::size_t ndim,
			const VECTOR<std::size_t>& indx,
			VECTOR<T> &b)
  {
    const static T SMALL = std::numeric_limits<T>::epsilon();
    std::size_t ii{0};
    
    // Permute the equations.
    for (std::size_t i = 0; i != ndim; ++i) {
      std::size_t l = indx[i];
      T sum = b[l];
      b[l] = b[i];
      if ( ii != 0 ) {
	for (std::size_t j = ii - 1; j != i; ++j)
	  sum -= a(i, j) * b(j);
      } else {
	if (std::fabs(sum) > SMALL ) {
	  ii = i + 1;
	}
      }
      b[i]  = sum;
    }
    
    // Backward substitution. Keep 'int' type!
    for (int i = ndim - 1; i >= 0; --i) {
      T sum = b(i);  
      for (int j = i + 1; j != int(ndim); ++j) {
	sum -= a(i, j) * b(j);
      }
      b[i] = sum / a(i, i);
    }
  }

  /**
   * Matrix inversion using LU-decoposed matrix.
   * @param a - LU decomposed matrix. Upon return, this holds the inverse.
   * @param ndim - Dimension of a.
   * @param indx - Pivot information (delivered by `luDecomposition').
   * 
   */
  template <typename T, template <typename...> class MATRIX, template <typename...> class VECTOR>
  void inverseNoLU(MATRIX<T> &a, std::size_t ndim, const VECTOR<std::size_t> &indx)
  {
    VECTOR<T> col(ndim);
    MATRIX<T> inv_a(ndim, ndim);
    
    for (std::size_t j = 0; j != ndim; ++j) {
      for (std::size_t i=0; i != ndim; ++i) {
	col[i] = 0.0;
      }
      col[j] = 1.0;
      backSubstitution<T, MATRIX, VECTOR>(a, ndim, indx, col);
      for (std::size_t i = 0; i != ndim; ++i)
	inv_a(i,j) = col[i];
    }
    for (std::size_t i = 0; i != ndim; ++i) {
      for (std::size_t j = 0; j != ndim; ++j) {
	a(i,j) = inv_a(i,j);
      }
    }
  }


  /**
   * Matrix inversion of given matrix.
   * @param a - Matrix.
   * @param ndim - Dimension of a.
   */
  template <typename T, template <typename...> class MATRIX, template <typename...> class VECTOR>
  void inverse(MATRIX<T> &a, std::size_t ndim)
  {
    VECTOR<std::size_t> indx(ndim);

    // Perform LU-decomposition.
    luDecomposition(a, ndim, indx);
    
    // Calculate inverse.
    inverseNoLU(a, ndim, indx);
  }

}

#endif
