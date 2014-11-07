/***************************************************************************
 Public methods:
 SEXP rowRanksWithTies(SEXP x, SEXP tiesMethod, SEXP byRow)

 Authors: Hector Corrada Bravo, Peter Langfelder and Henrik Bengtsson

 TO DO: Add support for missing values.
 **************************************************************************/
#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>


/* Template <col|row>Ranks_<Integer|Real>_ties<Min|Max|Average>() */

/*****************************************************************
 * ties.method = "min"
 *****************************************************************/
#define METHOD_NAME rowRanks_Real_tiesMin
#define MARGIN 'r'
#define X_TYPE 'r'
#define TIESMETHOD '0' /* min */
#include "rowRanksWithTies_TYPE_TIES-template.h"

#define METHOD_NAME rowRanks_Integer_tiesMin
#define MARGIN 'r'
#define X_TYPE 'i'
#define TIESMETHOD '0' /* min */
#include "rowRanksWithTies_TYPE_TIES-template.h"

#define METHOD_NAME colRanks_Real_tiesMin
#define MARGIN 'c'
#define X_TYPE 'r'
#define TIESMETHOD '0' /* min */
#include "rowRanksWithTies_TYPE_TIES-template.h"

#define METHOD_NAME colRanks_Integer_tiesMin
#define MARGIN 'c'
#define X_TYPE 'i'
#define TIESMETHOD '0' /* min */
#include "rowRanksWithTies_TYPE_TIES-template.h"


/*****************************************************************
 * ties.method = "max"
 *****************************************************************/
#define METHOD_NAME rowRanks_Real_tiesMax
#define MARGIN 'r'
#define X_TYPE 'r'
#define TIESMETHOD '1' /* max */
#include "rowRanksWithTies_TYPE_TIES-template.h"

#define METHOD_NAME rowRanks_Integer_tiesMax
#define MARGIN 'r'
#define X_TYPE 'i'
#define TIESMETHOD '1' /* max */
#include "rowRanksWithTies_TYPE_TIES-template.h"

#define METHOD_NAME colRanks_Real_tiesMax
#define MARGIN 'c'
#define X_TYPE 'r'
#define TIESMETHOD '1' /* max */
#include "rowRanksWithTies_TYPE_TIES-template.h"

#define METHOD_NAME colRanks_Integer_tiesMax
#define MARGIN 'c'
#define X_TYPE 'i'
#define TIESMETHOD '1' /* max */
#include "rowRanksWithTies_TYPE_TIES-template.h"


/*****************************************************************
 * ties.method = "average"
 *****************************************************************/
#define METHOD_NAME rowRanks_Real_tiesAverage
#define MARGIN 'r'
#define X_TYPE 'r'
#define TIESMETHOD 'a' /* average */
#include "rowRanksWithTies_TYPE_TIES-template.h"

#define METHOD_NAME rowRanks_Integer_tiesAverage
#define MARGIN 'r'
#define X_TYPE 'i'
#define TIESMETHOD 'a' /* average */
#include "rowRanksWithTies_TYPE_TIES-template.h"

#define METHOD_NAME colRanks_Real_tiesAverage
#define MARGIN 'c'
#define X_TYPE 'r'
#define TIESMETHOD 'a' /* average */
#include "rowRanksWithTies_TYPE_TIES-template.h"

#define METHOD_NAME colRanks_Integer_tiesAverage
#define MARGIN 'c'
#define X_TYPE 'i'
#define TIESMETHOD 'a' /* average */
#include "rowRanksWithTies_TYPE_TIES-template.h"



/* Peter Langfelder's modifications:
 * byrow: 0 => rank columns, !0 => rank rows
 * tiesMethod: 1: maximum, 2: average, 3:minimum
 * The returned rank is a REAL matrix to accomodate average ranks
 */
SEXP rowRanksWithTies(SEXP x, SEXP tiesMethod, SEXP byRow) {
  int tiesmethod, byrow;
  SEXP dim, ans = NILSXP;
  int nrow, ncol;

  /* Argument 'x': */
  if (!isMatrix(x))
    error("Argument 'x' must be a matrix.");

  /* Argument 'tiesMethod': */
  tiesmethod = INTEGER(tiesMethod)[0];
  if (tiesmethod < 1 || tiesmethod > 3) {
    error("Argument 'tiesMethod' is out of range [1,3]: %d", tiesmethod);
  }

  /* Argument 'byRow': */
  byrow = INTEGER(byRow)[0];


  /* Get dimensions for 'ans' (from 'x') */
  dim = getAttrib(x, R_DimSymbol);
  nrow = INTEGER(dim)[0];
  ncol = INTEGER(dim)[1];


  /* Double matrices are more common to use. */
  if (isReal(x)) {
    if (byrow) {
      switch (tiesmethod) {
        case 1:
          PROTECT(ans = allocMatrix(INTSXP, nrow, ncol));
          rowRanks_Real_tiesMax(REAL(x), nrow, ncol, 1, INTEGER(ans));
          UNPROTECT(1);
          break;
        case 2:
          PROTECT(ans = allocMatrix(REALSXP, nrow, ncol));
          rowRanks_Real_tiesAverage(REAL(x), nrow, ncol, 1, REAL(ans));
          UNPROTECT(1);
          break;
        case 3:
          PROTECT(ans = allocMatrix(INTSXP, nrow, ncol));
          rowRanks_Real_tiesMin(REAL(x), nrow, ncol, 1, INTEGER(ans));
          UNPROTECT(1);
          break;
      } /* switch */
    } else {
      switch (tiesmethod) {
        case 1:
          PROTECT(ans = allocMatrix(INTSXP, nrow, ncol));
          colRanks_Real_tiesMax(REAL(x), nrow, ncol, 0, INTEGER(ans));
          UNPROTECT(1);
          break;
        case 2:
          PROTECT(ans = allocMatrix(REALSXP, nrow, ncol));
          colRanks_Real_tiesAverage(REAL(x), nrow, ncol, 0, REAL(ans));
          UNPROTECT(1);
          break;
        case 3:
          PROTECT(ans = allocMatrix(INTSXP, nrow, ncol));
          colRanks_Real_tiesMin(REAL(x), nrow, ncol, 0, INTEGER(ans));
          UNPROTECT(1);
          break;
      } /* switch */
    }
  } else if (isInteger(x)) {
    if (byrow) {
      switch (tiesmethod) {
        case 1:
          PROTECT(ans = allocMatrix(INTSXP, nrow, ncol));
          rowRanks_Integer_tiesMax(INTEGER(x), nrow, ncol, 1, INTEGER(ans));
          UNPROTECT(1);
          break;
        case 2:
          PROTECT(ans = allocMatrix(REALSXP, nrow, ncol));
          rowRanks_Integer_tiesAverage(INTEGER(x), nrow, ncol, 1, REAL(ans));
          UNPROTECT(1);
          break;
        case 3:
          PROTECT(ans = allocMatrix(INTSXP, nrow, ncol));
          rowRanks_Integer_tiesMin(INTEGER(x), nrow, ncol, 1, INTEGER(ans));
          UNPROTECT(1);
          break;
      } /* switch */
    } else {
      switch (tiesmethod) {
        case 1:
          PROTECT(ans = allocMatrix(INTSXP, nrow, ncol));
          colRanks_Integer_tiesMax(INTEGER(x), nrow, ncol, 0, INTEGER(ans));
          UNPROTECT(1);
          break;
        case 2:
          PROTECT(ans = allocMatrix(REALSXP, nrow, ncol));
          colRanks_Integer_tiesAverage(INTEGER(x), nrow, ncol, 0, REAL(ans));
          UNPROTECT(1);
          break;
        case 3:
          PROTECT(ans = allocMatrix(INTSXP, nrow, ncol));
          colRanks_Integer_tiesMin(INTEGER(x), nrow, ncol, 0, INTEGER(ans));
          UNPROTECT(1);
          break;
      } /* switch */
    }
  } else {
    error("Argument 'x' must be numeric.");
  }

  return(ans);
} // rowRanksWithTies()


/***************************************************************************
 HISTORY:
 2013-01-13 [HB]
 o Added argument 'tiesMethod' to rowRanks().
 **************************************************************************/