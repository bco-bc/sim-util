#ifndef LU_HPP
#define LU_HPP

#include <stdexcept>
#include <cmath>

namespace simploce {

  /**
   * Performs LU-decomposition.
   * @param a - Matrix. a(i,j) refers to an element value with i is the row index, and j the 
   * column index.
   * @param ndim - Dimension of a. 
   * @param indx - Upon return, holds pivot information.
   */
  template <typename T, template <typename...> class MATRIX, template <typename...> class VECTOR>
  void luDecomposition(MATRIX<T> &a, std::size_t ndim, VECTOR<std::size_t> &indx)
  {
    const static T SMALL = 1.0e-07;

    VECTOR<T> vv(ndim), p{};
    indx = VECTOR<std::size_t>(ndim);
    T aamax{0.0}, sum{0.0};
    std::size_t imax{0};
    
    // Find scale factor for each equation.
    for (std::size_t i = 0; i < ndim; ++i) {
      aamax = 0.0;
      for (std::size_t j = 0; j < ndim; j++) {
	aamax = (std::fabs(a(i,j)) > aamax) ?  std::fabs(a(i,j)) : aamax;
      }
      if (aamax <= SMALL) {
	throw std::domain_error("Matrix LU-decomposition: Singular matrix.");
      }
      vv[i] = 1.0 / aamax;
    }
    // Loop over COLUMNS starts.
    for (std::size_t j = 0; j < ndim; j++) {
      
      // Compute betas above diagonal.
      for (std::size_t i = 0; i < j; ++i) {
	sum = a(i,j);
	for (std::size_t k = 0; k < i; ++k) {
	  sum -= a(i,k) * a(k,j);
	}
	a(i,j) = sum;
      }
      
      // Start search for pivot.
      aamax = 0.0;
      for (std::size_t i = j; i < ndim; i++) {	
	// Now compute diagonal betas and alpha below diagonal.
	// The diagonal betas can be got this way because no pivoting
	// is done until the largest element is found.
	sum = a(i,j);
	for (std::size_t k = 0; k < j; k++) {
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
	// Swap rows
	for (std::size_t k=0; k != ndim; ++k) {
	  p[k] = a(imax, k);
	}
	//p = a(imax);
	for (std::size_t k=0; k != ndim; ++k) {
	  a(imax, k) = a(j, k);
	}
	//a(imax) = a(j);
	for (std::size_t k=0; k != ndim; ++k) {
	  a(j, k) = p[k];
	}
	//a(j) = p;
	vv[imax] = vv[j];    // Swap scale factors too.
      }
      
      // Save permutation of the rows.
      indx[j] = imax;
      a(j,j) = a(j,j) <= 0.0 ? SMALL : a(j,j);
      if (j != ndim - 1) {
	// Divide sub-diagonal alphas by pivot element.
	T dum = 1.0 / a(j,j);
	for (std::size_t i = j + 1; i < ndim; ++i) {
	  a(i,j) *= dum;
	}
      }
    }
  }


  /*
   * Backsubstitution. May be used to solve the matrix equation Ax=b. Needed is the
   * LU-decomposed matrix of A. The unknown vector x is returned in b.
   * @param a - LU-decomposed matrix.
   * @param ndim - dimension of matrix a.
   * @param indx - Pivot information (delivered by `luDecomposition').
   * @param b - The right hand side of the matrix equation Ax=b.
   * @param x - Solution of Ax=b.
   */
  template <typename T, template <typename...> class MATRIX, template <typename...> class VECTOR>
  void backSubstitution(const MATRIX<T>& a,
			std::size_t ndim,
			const VECTOR<std::size_t>& indx,
			const VECTOR<T> &b,
			VECTOR<T> &x)
  {
    // Make copy.
    x = VECTOR<T>{b};
    
    // Permute the equations.
    for (std::size_t i=0; i<ndim; ++i) {
      std::size_t l = indx[i];
      T sum = x[l];
      x[l] = x[i];
      x[i]  = sum;
    }
    
    for (std::size_t i=0; i<ndim; ++i) {
      T sum = x[i];
      for (std::size_t j=0; j<i; ++j) {
	sum -= a(i,j) * x[j];
      }
      x[i] = sum;
    }
    
    // Forward substitution ends, y[*] determined.        
    for (std::size_t i=ndim-1; i>0; --i) {
      T sum = x[i];  
      for (std::size_t j=i+1; j<ndim; --j) {
	sum -= a(i,j) * x[j];
      }
      x[i] = sum/a(i,i);
    }
  }

  /**
   * Matrix inversion using LU-decoposed matrix.
   * @param a - LU decomposed matrix.
   * @param ndim - Dimension of a.
   * @param indx - Pivot information (delivered by `luDecomposition').
   * 
   */
  template <typename T, template <typename...> class MATRIX, template <typename...> class VECTOR>
  void inverseNoLU(MATRIX<T> &a, std::size_t ndim, const VECTOR<std::size_t> &indx)
  {
    VECTOR<T> col(ndim);
    MATRIX<T> inv_a(ndim, ndim);
    
    for (std::size_t j=0; j<ndim; ++j) {
      for (std::size_t i=0; i<ndim; ++i)
	col[i] = 0.0;
      col[j] = 1.0;
      backSubstitution(a, ndim, indx, col);
      for (std::size_t i=0; i<ndim; ++i)
	inv_a(i,j) = col[i];
    }
    for (std::size_t i=0; i<ndim; ++i) {
      for (std::size_t j=0; j<ndim; j++) {
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
    inverseNoLU(a, indx, ndim);
  }

/*

template <class TYPE>
BOOLEAN Inverse(MATRIX<TYPE> &a, int ndim, int recoverLU)
{
    int i, j;
    VECTOR<int> indx(ndim);
    MATH_VECTOR<double> col(ndim);
    MATRIX<TYPE> inv_a(ndim, ndim);
    BOOLEAN error_status;

    if (recoverLU)

// Recover LU-decomposition.
         a.ReadElements("LU.matrix", ndim, ndim);     // Read backup.

    else {
         error_status=LuDcmp(a, ndim, indx);
         a.WriteElements("LU.matrix", ndim, ndim);     // Write backup.
    }

// Calculate inverse.
    if (!error_status) {
         cout << "Inverse matrix: LU-decomposition succesfull\n";
         cout.flush();
         for (j=0; j<ndim; j++) {
              for (i=0; i<ndim; i++)
                   col[i]=0.0;
              col[j]=1.0;
              BkSub(a, ndim, indx, col);
              for (i=0; i<ndim; i++)
                   inv_a(i,j)=col[i];
         }
         for (i=0; i<ndim; i++)
              for (j=0; j<ndim; j++)
                   a(i,j)=inv_a(i,j);
    }

    return error_status;
}

*/

}

#endif
