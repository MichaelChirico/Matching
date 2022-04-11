/* 
   DGEMM - perform one of the matrix-matrix operations 
   C := alpha*op( A )*op( B ) + beta*C
   
   * cblas_dgemm.c
   * This program is a C interface to dgemm.
   * Written by Keita Teranishi
   * 4/8/1998
   
   CBLAS provides a C interface to the BLAS routines, which were
   originally written in FORTRAN.  CBLAS wrappers are already provided
   on Windows and OS X, but not on other UNIX-like operating systems
   (such as Linux). For most platforms (particularly AMD chips), I
   recommend Kazushige Goto's High-Performance BLAS Library:

   http://www.cs.utexas.edu/users/flame/goto/
   
   For more information on BLAS (including function definitions) see:
   http://www.netlib.org/blas/
   
   Note that I have only included wrappers for the BLAS functions which
   the Matching package actually uses.

   Jas Sekhon
   <sekhon@berkeley.edu>
   http://sekhon.berkeley.edu
   August 1, 2007
*/

#define USE_FC_LEN_T
#include <R.h>
#include <R_ext/Applic.h> /* R blas declarations */

#include "cblas.h"
#ifndef FCONE
# define FCONE
#endif

void cblas_dgemm(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA,
                 const enum CBLAS_TRANSPOSE TransB, const int M, const int N,
                 const int K, const double alpha, const double  *A,
                 const int lda, const double  *B, const int ldb,
                 const double beta, double  *C, const int ldc)
{
   char TA, TB;   
#ifdef F77_CHAR
   F77_CHAR F77_TA, F77_TB;
#else
   #define F77_TA &TA  
   #define F77_TB &TB  
#endif

#ifdef F77_INT
   F77_INT F77_M=M, F77_N=N, F77_K=K, F77_lda=lda, F77_ldb=ldb;
   F77_INT F77_ldc=ldc;
#else
   #define F77_M M
   #define F77_N N
   #define F77_K K
   #define F77_lda lda
   #define F77_ldb ldb
   #define F77_ldc ldc
#endif

   if( Order == CblasColMajor )
   {
      if(TransA == CblasTrans) TA='T';
      else if ( TransA == CblasConjTrans ) TA='C';
      else if ( TransA == CblasNoTrans )   TA='N';
      else 
      {
	error("cblas_dgemm","Illegal TransA setting, %d\n", TransA);
	/* cblas_xerbla(2, "cblas_dgemm","Illegal TransA setting, %d\n", TransA); */
         return;
      }

      if(TransB == CblasTrans) TB='T';
      else if ( TransB == CblasConjTrans ) TB='C';
      else if ( TransB == CblasNoTrans )   TB='N';
      else 
      {
	error("cblas_dgemm","Illegal TransB setting, %d\n", TransB);
	/* cblas_xerbla(3, "cblas_dgemm","Illegal TransB setting, %d\n", TransB); */
         return;
      }

      #ifdef F77_CHAR
         F77_TA = C2F_CHAR(&TA);
         F77_TB = C2F_CHAR(&TB);
      #endif

	 F77_CALL(dgemm)(F77_TA, F77_TB, &F77_M, &F77_N, &F77_K, &alpha, A, &F77_lda, B, &F77_ldb, &beta, C, &F77_M FCONE FCONE);
   } else if (Order == CblasRowMajor)
   {
      if(TransA == CblasTrans) TB='T';
      else if ( TransA == CblasConjTrans ) TB='C';
      else if ( TransA == CblasNoTrans )   TB='N';
      else 
      {
	error("cblas_dgemm","Illegal TransA setting, %d\n", TransA);
	/* cblas_xerbla(2, "cblas_dgemm","Illegal TransA setting, %d\n", TransA); */
         return;
      }
      if(TransB == CblasTrans) TA='T';
      else if ( TransB == CblasConjTrans ) TA='C';
      else if ( TransB == CblasNoTrans )   TA='N';
      else 
      {
	error("cblas_dgemm","Illegal TransB setting, %d\n", TransB);
	/* cblas_xerbla(2, "cblas_dgemm","Illegal TransB setting, %d\n", TransB); */
         return;
      }
      #ifdef F77_CHAR
         F77_TA = C2F_CHAR(&TA);
         F77_TB = C2F_CHAR(&TB);
      #endif

	 F77_CALL(dgemm)(F77_TA, F77_TB, &F77_N, &F77_M, &F77_K, &alpha, B, &F77_ldb, A, &F77_lda, &beta, C, &F77_N FCONE FCONE);
   } 
   else  {
     error("cblas_dgemm", "Illegal Order setting, %d\n", Order);
     /* cblas_xerbla(1, "cblas_dgemm", "Illegal Order setting, %d\n", Order); */
   }
   return;
}


