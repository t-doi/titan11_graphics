/*****************************************************************************
**
**	File:	  Matrix.h
**
**	Description: This matrix class defines majority of the matrix
**				 operations as overloaded operators or methods.
**
**	Created: 2003/03/10(Mon)	Updated: 2005/01/24(Mon)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#ifndef	__Matrix_h__	// Avoid double include
#define __Matrix_h__	// Prevent double include

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*******************************************************************************
**  Matrix class
**
**  Member defined
**		Matrix(int i, int j)	// Default constructor
**		Matrix(Matrix)			// Copy constructor
**
**		GetRow()				// Get row
**		Getcol()				// Get column
**
**		Transposition()			// Return transposed matrix
**		Tilde()					// Return skew synmetric matrix
**		Inversion()				// Return inverse matrix
**		PseudoInv()				// Return pseudo inverse matrix
**		Normal()				// Return normalized matrix
**		Unity()					//
**		Zero()					//
**		Trace()					// Return trace
**		Norm()					// Return norm
**		NormSquare()			// Return squeared norm
**		Normalize()				// Normalize
**		Transpose()				// Transpose
**		Inverse()				// Inverse
**		PseudoInverse()			// Psudo inverse
**		LoadMatrix()			// Copy submatirx
**		LoadZero()				// zero matrix
**		LoadIdentity()			// identity matrix
**	
**  Operator member functions
**				m -- a matrix object
**				s -- a scalar (double)
**		m = m;
**		m += m;
**		m += s;
**		m -= m;
**		m -= s;
**		m /= m;
**		m /= s;
**		m *= m;
**		m *= s;
**
**		m++
**		++m
**		m--
**		--m
**
**	Arithmetic operators defined
**		m = m + m;
**		m = s + m;
**		m = m + s;
**		m = m - m;
**		m = s - m;
**		m = m - s; 
**		m = m * m;
**		m = m * s;
**		m = s * m;
**		m = m / m;
**		m = m / s;
**		m = s / m;
**		-m
**		+m
*******************************************************************************/

class Matrix
{
private:
	int RowNum;				// Row dimension
	int ColNum;				// Column dimension

public:
	double *Element;              // matrix element

/***********************************************************************
**  Constructor and Destructor
***********************************************************************/
	Matrix();                     // Constructor
	Matrix(int r, int c=1);       // Default is Vector
	Matrix(const Matrix& src);    // Copy constructor
	virtual ~Matrix();            // Desturctor

/***********************************************************************
**	Functions to return parts of the matrix
***********************************************************************/
	int SetSize(int r, int c=1);	// Default is Vector
	void FreeMatrix();

	int GetRow(void) const     {return (RowNum);}
	int GetCol(void) const     {return (ColNum);}
	int IsSquare(void) const   {return (this->GetRow()==this->GetCol());}
	int IsOneColumn(void) const{return (this->GetCol() == 1);} 

/***********************************************************************
**	Functions to set parts of a number
***********************************************************************/
	virtual double& operator() (int r, int c=1) const;
	virtual Matrix& operator = (const Matrix& oper2);
	virtual bool	operator== (const Matrix& oper2);
	virtual bool	operator!= (const Matrix& oper2);
	virtual Matrix& operator+= (const Matrix& oper2);
	virtual Matrix& operator-= (const Matrix& oper2);
	virtual Matrix& operator*= (double s);
	virtual Matrix& operator*= (int s);
	virtual Matrix& operator/= (double s);
	virtual Matrix& operator/= (int s);

/***********************************************************************
**	Block copy
***********************************************************************/
	virtual void LoadMatrix(int i, int j, const Matrix& src, int p, int q, int r, int s);
	virtual void LoadMatrix(const Matrix& src, int p, int q, int r, int s);
	virtual void LoadMatrix(int i, int j, const Matrix& src);

/***********************************************************************
**	Sub Matrix copy
***********************************************************************/
	virtual void LoadMatrix(const Matrix& src, int i, int j);
	virtual void LoadMatrix(const Matrix& src);

	virtual void LoadZero(void);
	virtual void LoadIdentity(void);

/***********************************************************************
**	Majority operations
***********************************************************************/
	void   Transpose(void);				// Turn into transpose matrix of it
	Matrix Transposition(void) const;	// Return its transposed matrix
	Matrix Tilde(void) const;			// Return its skew synmetric matrix

	void   Inverse(void);				// Turn into inverse matrix of it
	Matrix Inversion(void) const;		// Return its inverse matrix
	double Det(void) const;				// Return its determinant

	void   SglValDcmp(Matrix&, Matrix&, Matrix&) const;
	void   PseudoInverse(void);
	Matrix PseudoInversion(void) const;
	double ConditionNum(void) const;

	virtual void Show(void) const;		// Print the all elements
	virtual void ShowExp(void) const;	// Print the all elements in Exponential form

	virtual double Norm(void);
	virtual double NormSquare(void);
	virtual void   Normalize(void);
	virtual Matrix Normal(void);

	double Trace(void) const;
};

void LUbksb(Matrix& x, const Matrix& a, const Matrix& b, Matrix& indx);
int LUdcmp(Matrix& a, const Matrix& b, Matrix& indx, double *d);

/*****************************************************************************
 * IsSameDimension
 *
 * Description : Check if the dimension is same between two matrix
 *****************************************************************************/
inline int IsSameDimension(const Matrix& oper1, const Matrix& oper2)
{
  return (oper1.GetRow()==oper2.GetRow()) && (oper1.GetCol()==oper2.GetCol());
}

/*****************************************************************************
 * IsValidMultiplication
 *
 * Description : Check the possibility of multiplication between two matrix
 *****************************************************************************/
inline int IsValidMultiplication(const Matrix& oper1, const Matrix& oper2)
{
  return (oper1.GetCol()==oper2.GetRow());
}

inline Matrix operator + (const Matrix& oper1, const Matrix& oper2)
{
	if (!IsSameDimension(oper1, oper2))
	{
		fprintf(stderr, "Matrix::operator+(): \n");
		exit(1);
	}

	Matrix result(oper1);
	return result += oper2;
}

inline Matrix operator - (const Matrix& oper1, const Matrix& oper2)
{
	if (!IsSameDimension(oper1, oper2))
	{
		fprintf(stderr, "Matrix::operator-(): \n");
		exit(1);
	}

	Matrix result(oper1);
	return result -= oper2;
}

inline Matrix operator * (const Matrix& oper1, const Matrix& oper2)
{
	if (!IsValidMultiplication(oper1, oper2))
	{
		fprintf(stderr, "Matrix::operator*n(): \n");
		exit(1);
	}

	Matrix result(oper1.GetRow(),oper2.GetCol());

	double tmp,sum,res;
	int i,j,k,l;
	Matrix index(oper2.GetRow()+1,oper2.GetCol());
  
/* 1st row */
	for (i=1,j=1; j<=result.GetCol(); j++)
	{
		for (sum=res=0.0,k=1,l=1; k<=oper1.GetCol(); k++)
		{
			if (oper2(k,j) != 0.0)
			{
				index(l,j) = k;
				l++;
				/* Uderflow prevention */
				res  += oper1(i,k)*oper2(k,j);
				tmp   = sum;
				sum  += res;
				tmp   = sum - tmp;
				res   = res - tmp;
			}
		}

		result(i, j)   = sum;
		index(oper2.GetRow()+1,j) = l-1;
	}

/* the other rows */
	for (i=2; i<=result.GetRow(); i++)
	{
		for (j=1; j<=result.GetCol(); j++)
		{
			for (sum=res=0.0,l=1; l<=index(oper2.GetRow()+1,j); l++)
			{
				// Null multiplication  prevention
				res  += oper1(i, (int)index(l,j)) * oper2((int)index(l,j), j);
				// Uderflow prevention
				tmp   = sum;
				sum  += res;
				tmp   = sum - tmp;
				res   = res - tmp;
			}
			
			result(i, j) = sum;
		}
	} 

	return result;
}

inline Matrix operator * (const double s, const Matrix& oper2)
{
	Matrix result(oper2.GetRow(),oper2.GetCol());
	
	for (int i=1; i<=oper2.GetRow(); i++)
	{
		for (int j=1; j<=oper2.GetCol(); j++)
		{
			result(i,j) = s*oper2(i,j);
		}
	}
	
	return result;
}

inline Matrix operator * (const Matrix& oper1, const double s)
{
	Matrix result(oper1.GetRow(),oper1.GetCol());

	for (int i=1; i<=oper1.GetRow(); i++)
	{
		for (int j=1; j<=oper1.GetCol(); j++)
		{
			result(i,j) = s*oper1(i,j);
		}
	}
  
	return result;
}

inline Matrix operator * (const int s, const Matrix& oper2)
{
	Matrix result(oper2.GetRow(),oper2.GetCol());

	for (int i=1; i<=oper2.GetRow(); i++)
	{
		for (int j=1; j<=oper2.GetCol(); j++)
		{
			result(i,j) = oper2(i,j)*(double)s;
		}
	}
	
	return result;
}

inline Matrix operator * (const Matrix& oper1, const int s)
{
	Matrix result(oper1.GetRow(),oper1.GetCol());

	for (int i=1; i<=oper1.GetRow(); i++)
	{
		for (int j=1; j<=oper1.GetCol(); j++)
		{
			result(i,j) = oper1(i,j)*(double)s;
		}
	}
  
	return result;
}

inline Matrix operator / (const Matrix& b, const Matrix& a)
{
	if ((!a.IsSquare()) || (a.GetCol()!=b.GetRow()))
	{
		fprintf(stderr, "Matrix::operator/(): \n");
		exit(1);
	}

	return (b * a.Inversion());
}

inline Matrix operator / (const Matrix& oper1, const double s)
{
	Matrix result(oper1.GetRow(),oper1.GetCol());

	for (int i=1; i<=oper1.GetRow(); i++)
	{
		for (int j=1; j<=oper1.GetCol(); j++)
		{
			result(i,j) = oper1(i,j)/s;
		}
	}
	
	return result;
}

inline Matrix operator / (const Matrix& oper1, const int s)
{
	Matrix result(oper1.GetRow(),oper1.GetCol());

	for (int i=1; i<=oper1.GetRow(); i++)
	{
		for (int j=1; j<=oper1.GetCol(); j++)
		{
			result(i,j) = oper1(i,j)/(double)s;
		}
	}
	
	return result;
}

inline Matrix operator | (const Matrix& oper1, const Matrix& oper2)
{
	if ( !(oper1.IsOneColumn() && oper2.IsOneColumn()) )
	{
		fprintf(stderr, "Matrix::operator|(): \n");
		exit(1);
	}

  return (oper1 * oper2.Transposition());
}   

inline Matrix operator - (const Matrix& oper1)
{
	return (-1.0 * oper1);
}

inline Matrix operator + (const Matrix& oper1)
{
	return Matrix(oper1);
}

inline Matrix operator ~ (const Matrix& oper1)
{
	return oper1.Tilde();
}
#endif // __Matrix_h__
