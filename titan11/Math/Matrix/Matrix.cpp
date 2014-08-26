/*****************************************************************************
**
**	File:	  Matrix.cpp
**
**	Description: This matrix class defines majority of the matrix
**				 operations as overloaded operators or methods.
**
**	Created: 2003/03/10(Mon)	Updated: 2004/6/4(Fri)
**
**	Copyright (C) Tokyo Institute of Technology Hirose & Yoneda Lab.
*****************************************************************************/

#include "Matrix.h"

const double MATRIX_MACHINE_EPS = (1.0e-13);

#define __sgi

Matrix::Matrix()
{
	RowNum = 0;
	ColNum = 0;
}

Matrix::Matrix(int r, int c)	// Default is Vector
{
	RowNum = r;
	ColNum = c;	// Set dimension of this matrix

	Element = (double *)calloc((size_t)(RowNum)*(size_t)(ColNum), sizeof(double));
	if (Element == (double*)NULL)
	{
		fprintf(stderr, "Matrix::Matrix(): memory overflow\n");
		exit(1);
	}
}

Matrix::Matrix(const Matrix& src)
{
	RowNum = src.GetRow();
	ColNum = src.GetCol();    // Set dimension of this matrix

	Element = (double *)calloc((size_t)(RowNum)*(size_t)(ColNum), sizeof(double));
	
	if (Element == (double*)NULL)
	{
		fprintf(stderr, "Matrix::Matrix(): memory overflow\n");
		exit(1);
	}
	for (int i=1; i<=RowNum; i++)
		for (int j=1; j<=ColNum; j++)
			(*this)(i,j) = src(i,j);
}

int Matrix::SetSize(int r, int c)	// Default is Vector
{
	if( !(RowNum == 0 && ColNum == 0) )
	return -1;

	RowNum = r;
	ColNum = c;	// Set dimension of this matrix

	Element = (double *)calloc((size_t)(RowNum)*(size_t)(ColNum), sizeof(double));
	
	if (Element == (double*)NULL)
	{
		fprintf(stderr, "Matrix::Matrix(): memory overflow\n");
		exit(1);
	}
	return (r * c);
}

void Matrix::FreeMatrix()
{
	if (!(RowNum == 0 && ColNum == 0))
		free(Element);
	
	RowNum = 0;
	ColNum = 0;
}

Matrix::~Matrix()
{
	if (!(RowNum == 0 && ColNum == 0))
		free(Element);
}

double& Matrix::operator() (int r, int c) const
{
	if (!( (r>0) && (r<=this->GetRow()) && (c>0) && (c<=this->GetCol()) ))
	{
		fprintf(stderr, "Matrix::operator(): \n");
		exit(1);
	}

	return this->Element[(r-1)*this->GetCol() + (c-1)];
}

Matrix& Matrix::operator = (const Matrix& oper2)
{
	if ( ((*this).GetRow()!=oper2.GetRow()) || ((*this).GetCol()!=oper2.GetCol()) )
	{
		/*
		int a=(*this).GetRow();//debug
		int b=oper2.GetRow();//debug
		int c=(*this).GetCol();//debug
		int d=oper2.GetCol();//debug
		*/
		fprintf(stderr, "Matrix::operator=(): \n");
		exit(1);
	}

	for (int i=1; i<=this->GetRow(); i++)
		for (int j=1; j<=this->GetCol(); j++)
				(*this)(i,j) = oper2(i,j);
	
	return (*this);
}

bool Matrix::operator ==(const Matrix& oper2)
{
	if (  ( (*this).GetRow()!=oper2.GetRow() ) || ( (*this).GetCol()!=oper2.GetCol() ) )
	{
		return (false);
	}

	for (int i=1; i<=this->GetRow(); i++)
	{
		for (int j=1; j<=this->GetCol(); j++)
		{
			if ( (*this)(i,j) != oper2(i,j) )
				return (false);
		}
	}

	return (true);
}

bool Matrix::operator !=(const Matrix& oper2)
{
	return ( !( (*this) == oper2 ) );
}

Matrix& Matrix::operator+= (const Matrix& oper2)
{
	for (int i=1; i<=this->GetRow(); i++)
		for (int j=1; j<=this->GetCol(); j++)
		  (*this)(i,j) += oper2(i,j);
	
	return (*this);
}

Matrix& Matrix::operator-= (const Matrix& oper2)
{
	for (int i=1; i<=this->GetRow(); i++)
		for (int j=1; j<=this->GetCol(); j++)
			(*this)(i,j) -= oper2(i,j);
	
	return (*this);
}

Matrix& Matrix::operator*= (double s)
{
	for (int i=1; i<=this->GetRow(); i++)
		for (int j=1; j<=this->GetCol(); j++)
			(*this)(i,j) *= s;
	
	return (*this);
}

Matrix& Matrix::operator*= (int s)
{
	for (int i=1; i<=this->GetRow(); i++)
		for (int j=1; j<=this->GetCol(); j++)
			(*this)(i,j) *= (double)s;
	
	return (*this);
}

Matrix& Matrix::operator/= (double s)
{
	if (s==0.0)
	{
		fprintf(stderr, "Matrix::operator/=(): dived by zero\n");
		exit(1);
	}

	for (int i=1; i<=this->GetRow(); i++)
		for (int j=1; j<=this->GetCol(); j++)
			(*this)(i,j) /= s;
	
	return (*this);
}

Matrix& Matrix::operator/= (int s)
{
	if (s==0){
		fprintf(stderr, "Matrix::operator/=(): dived by zero\n");
		exit(1);
	}

	for (int i=1; i<=this->GetRow(); i++)
		for (int j=1; j<=this->GetCol(); j++)
			(*this)(i,j) /= (double)s;
	
	return (*this);
}

void Matrix::LoadMatrix(int i, int j, const Matrix& src, int p, int q, int r, int s)
{
	if ( (p<1) || (q<1) || (r<p) || (s<q) || ((r-p)>src.GetRow()) || ((s-q)>src.GetCol())
		|| (((*this).GetRow()-i)<(r-p)) || (((*this).GetCol()-j)<(s-q)) )
	{
		fprintf(stderr, "Matrix::LoadMatrix(): block copy\n");
		exit(1);
	}

	for (int y=0; y<(r-p+1); y++)
		for (int x=0; x<(s-q+1); x++)
			(*this)(i+y, j+x) = src(p+y, q+x);
}

void Matrix::LoadMatrix(const Matrix& src, int p, int q, int r, int s)
{
	this->LoadMatrix(1,1,src,p,q,r,s);
}

void Matrix::LoadMatrix(int i, int j, const Matrix& src)
{
	this->LoadMatrix(i,j,src,1,1,src.GetRow(),src.GetCol());
}

void Matrix::LoadMatrix(const Matrix& src, int i, int j)
{
	if ( ((*this).GetRow()>(src.GetRow()-i+1)) || ((*this).GetCol()>(src.GetCol()-j+1)) )
	{
		fprintf(stderr, "Matrix::LoadMatrix(): copy into sub matrix\n");
		exit(1);
	}
	
	for (int y=0; y<(*this).GetRow(); y++)
		for (int x=0; x<(*this).GetCol(); x++)
			(*this)(1+y, 1+x) = src(i+y, j+x);
}

void Matrix::LoadMatrix(const Matrix& src)
{
	int i=1, j=1;
	if ( ((*this).GetRow()>(src.GetRow()-i+1)) || ((*this).GetCol()>(src.GetCol()-j+1)) )
	{
		fprintf(stderr, "Matrix::LoadMatrix(): copy into sub matrix\n");
		exit(1);
	}

	for (int y=0; y<(*this).GetRow(); y++)
		for (int x=0; x<(*this).GetCol(); x++)
			(*this)(1+y, 1+x) = src(i+y, j+x);
}

void Matrix::LoadZero(void)
{
	for (int i=1; i<=this->GetRow(); i++)
		for (int j=1; j<=this->GetCol(); j++)
		(*this)(i,j) = 0.0;
}

void Matrix::LoadIdentity(void)
{
	if (!(this->IsSquare()))
	{
		fprintf(stderr, "Matrix::LoadIdentity(): \n");
		exit(1);
	}
	
	this->LoadZero();
	for (int i=1; i<=this->GetRow(); i++)
		(*this)(i,i) = 1.0;
}

void Matrix::Transpose(void)
{
	if (!this->IsSquare())
	{
		fprintf(stderr, "Matrix::transpose(): is not square matrix\n");
		exit(1);
	}

	double swap;
  
	for (int i=1; i<=this->GetRow(); i++)
	{
		for (int j=1; j<i; j++)
		{
			swap = (*this)(j,i);
			(*this)(j,i) = (*this)(i,j);
			(*this)(i,j) = swap;
		}
	}
}

void Matrix::Show(void) const
{
	for (int i=1; i<=this->GetRow(); i++)
	{
		for (int j=1; j<=this->GetCol(); j++)
		{
			if ((*this)(i,j) <0.0)
			{
				if ( (*this)(i,j)>-0.0001)
					fprintf(stderr, " -%.4f\\",-(*this)(i,j));
				else
					fprintf(stderr, " -%.4f ", -(*this)(i,j));
			}
			else if ((*this)(i,j) == 0.0)
				fprintf(stderr, "  %.4f ", 0.0);
			else
			{
				if ((*this)(i,j)<0.0001)
					fprintf(stderr,"  %.4f\\", (*this)(i,j));
				else
					fprintf(stderr,"  %.4f ", (*this)(i,j));
			}
		}

		fprintf(stderr,"\n");
	}
	
	fprintf(stderr,"\n");
}

void Matrix::ShowExp(void) const
{
	for (int i=1; i<=this->GetRow(); i++)
	{
		for (int j=1; j<=this->GetCol(); j++)
		{
			if ((*this)(i,j) < 0.0)
			{
				if ( (*this)(i,j) > -0.00000001)
					fprintf(stderr, " -%1.8e\\",-(*this)(i,j));
				else
					fprintf(stderr, " -%1.8e ", -(*this)(i,j));
			}
			else if ((*this)(i,j) == 0.0)
				fprintf(stderr, "  %1.8e ", 0.0);
			else
			{
				if ((*this)(i,j)<0.000000001)
					fprintf(stderr,"  %1.8e\\", (*this)(i,j));
				else
					fprintf(stderr,"  %1.8e ", (*this)(i,j));
			}
		}
		
		fprintf(stderr,"\n");
	}
	
	fprintf(stderr,"\n");
}

Matrix Matrix::Transposition(void) const
{
	Matrix tmp(this->GetCol(),this->GetRow());

	for (int i=1; i<=tmp.GetRow(); i++)
	{
		for (int j=1; j<=tmp.GetCol(); j++)
		{
			tmp(i,j) = (*this)(j,i);
		}
	}
	
	return tmp;
}

Matrix Matrix::Tilde(void) const
{
	if ( (this->GetRow() == 3) && (this->GetCol() == 1) )
	{
		Matrix tmp(3,3);

		tmp(1,1) = 0.0;		     tmp(1,2) =-(*this)(3,1); tmp(1,3) = (*this)(2,1);
		tmp(2,1) = (*this)(3,1); tmp(2,2) = 0.0;	      tmp(2,3) =-(*this)(1,1);
		tmp(3,1) =-(*this)(2,1); tmp(3,2) = (*this)(1,1); tmp(3,3) = 0.0;

    return tmp;
	}
	else if ( (this->GetRow() == 4) && (this->GetCol() == 1) )
	{
		Matrix tmp(4,4);

		tmp(1,1) = (*this)(4,1); tmp(1,2) =-(*this)(3,1); tmp(1,3) = (*this)(2,1); tmp(1,4) = (*this)(1,1);
		tmp(2,1) = (*this)(3,1); tmp(2,2) = (*this)(4,1); tmp(2,3) =-(*this)(1,1); tmp(2,4) = (*this)(2,1);
		tmp(3,1) =-(*this)(2,1); tmp(3,2) = (*this)(1,1); tmp(3,3) = (*this)(4,1); tmp(3,4) = (*this)(3,1);
		tmp(4,1) =-(*this)(1,1); tmp(4,2) =-(*this)(2,1); tmp(4,3) =-(*this)(3,1); tmp(4,4) = (*this)(4,1);
    
		return tmp;
	}
	else
	{
		fprintf(stderr, "Matrix::Tilde(): \n");
		exit(1);
	}

	Matrix dummy(3,3);
	return dummy;		// No meaning
}

double Matrix::Norm(void)
{
	if (!this->IsOneColumn())
	{
		fprintf(stderr, "Matrix::Norm(): \n");
		exit(1);
	}

	double abs, res, tmp;
	int i;
	
	for (i=1,abs=res=0.0; i<=this->GetRow(); i++)
	{
		/* underflow prevenstin */
		res  += (*this)(i) * (*this)(i);
		tmp   = abs;
		abs  += res;
		tmp   = abs - tmp;
		res   = res - tmp;
	}

	return sqrt(abs);
}

double Matrix::NormSquare(void)
{
	if (!this->IsOneColumn())
	{
		fprintf(stderr, "Matrix::NormSquare(): \n");
		exit(1);
	}

	double abs, res, tmp;
	int i;
	
	for (i=1,abs=res=0.0; i<=this->GetRow(); i++)
	{ 
		/* underflow prevenstin */
		
		res  += (*this)(i) * (*this)(i);
		tmp   = abs;
		abs  += res;
		tmp   = abs - tmp;
		res   = res - tmp;
	}

	return abs;
}

void Matrix::Normalize(void)
{
	(*this) /= this->Norm();
}

Matrix Matrix::Normal(void)
{
	Matrix result(*this);

	result.Normalize();
	return result;
}

double Matrix::Trace(void) const
{
	if (!this->IsSquare())
	{
		fprintf(stderr, "Matrix::Trace(): \n");
		exit(1);
	}
    
	double res,tmp,trace;
	int i;
	
	for (i=1,res=trace=0.0; i<=this->GetRow(); i++)
	{
		res   += (*this)(i,i);
		tmp    = trace;
		trace += res;
		tmp    = trace - tmp;
		res    = res   - tmp;
	}

	return trace;
}

/*****************************************************************************
 * LUdcmp : LU deoomposition 
 *
 * arg  : Matrix : a    : LU decomposited matrix
 *      : Matrix : b    : target matrix
 *      : Matrix : indx : column-row exchange infomation
 *      : double*: d    : odd-even infomation
 *
 * Reference : William H. Press, et al, NUMERICAL RECIPES in C, 
 *             Cambridge University Press, 1988
 *****************************************************************************/
int LUdcmp(Matrix& a, const Matrix& b, Matrix& indx, double *d)
{
	int i, imax = 0, j, k, n;
	double big, dum, sum, temp;

	a.LoadMatrix(b);
	n = b.GetRow();
	Matrix vv(n);
	*d = 1.0;

	for (i=1; i<=n; i++)
	{
		big = 0.0;
		
		for (j=1; j<=n; j++)
		{  
			if ((temp=fabs(a(i,j))) > big) 
				big=temp;
		}
		if (big == 0.0) 
		{
			fprintf(stderr, "Matrix::LUdcmp(): Singular matrix occurs\n");
			return 0;
		}
		
		vv(i) = 1.0/big;
	}

	for (j=1; j<=n; j++)
	{
		for (i=1; i<j; i++)
		{
			sum = a(i,j);
			
			for (k=1; k<i; k++)
			{
				sum -= a(i,k)*a(k,j);
			}
			
			a(i,j) = sum;
		}
		big=0.0;
    
		for (i=j; i<=n; i++)
		{
			sum = a(i,j);
			
			for (k=1; k<j; k++)
			{
				sum -= a(i,k)*a(k,j);
			}
      
			a(i,j) = sum;
      
			if ( (dum=vv(i)*fabs(sum)) >= big)
			{
				big = dum;
				imax = i;
			}
		}
    
		if (j != imax)
		{
			for (k=1; k<=n; k++)
			{
				dum = a(imax,k);
				a(imax,k) = a(j,k);
				a(j,k)=dum;
			}
			
			*d = -(*d);
			vv(imax) = vv(j);
		}
    
		indx(j,1) = imax;
		
		if (a(j,j)==0.0)
		{
			a(j,j) = MATRIX_MACHINE_EPS;
		}
    
		if (j != n)
		{
			dum=1.0/(a(j,j));
      
			for (i=j+1; i<=n; i++)
			{
				a(i,j) *= dum;
			}
		}
	}
  return 1;
}

/*****************************************************************************
 * LUbksb : linear equation solving using LU deoomposition 
 *
 * arg  : Matrix : x    : kernel
 *      : Matrix : a    : a x = b
 *      : Matrix : b    : 
 *      : Matrix : indx : colum-row exchaning information
 *
 * Reference : William H. Press, et al, NUMERICAL RECIPES in C, 
 *             Cambridge University Press, 1988
 *****************************************************************************/
void LUbksb(Matrix& x, const Matrix& a, const Matrix& b, Matrix& indx)
{
	int i, ii=0, ip, j;
	int n;
	double sum;

	n = a.GetRow();
	x = b;

	for (i=1; i<=n; i++)
	{
		ip  = (int)indx(i);
		sum = x(ip);
		x(ip) = x(i);
	
		if (ii)
		{
			for (j=ii; j<=i-1; j++)
			{
				sum -= a(i,j)*x(j);
			}
		}
		else if (sum)
		{
			ii = i;
		}
		
		x(i)=sum;
	}
  
	for (i=n; i>=1; i--)
	{
		sum = x(i);
		for (j=i+1; j<=n; j++)
		{
			sum -= a(i,j)*x(j);
		}
		x(i) = sum/a(i,i);
	}
}

/*****************************************************************************
 * Inversion : matrix inversion
 *
 * Reference : William H. Press, et al, NUMERICAL RECIPES in C, 
 *             Cambridge University Press, 1988
 *****************************************************************************/
Matrix Matrix::Inversion(void) const
{
	if (!this->IsSquare())
	{
		fprintf(stderr, "Matrix::Inversion():\n");
		exit(1);
	}

  /*
   * LU decomposition
   */
	Matrix   lu(this->GetRow(), this->GetCol());
	Matrix indx(this->GetRow());
	double d;

	LUdcmp(lu,(*this),indx,&d);

  /*
   * Get inverse matrix
   */
	Matrix  col(this->GetRow());
	Matrix col1(this->GetRow());
	Matrix result(this->GetRow(), this->GetCol());

	int i,j;
	for (j=1; j<=this->GetRow(); j++)
	{
		for (i=1; i<=this->GetRow(); i++)
		{
			col(i) = 0.0;
		}
		
		col(j) = 1.0;
		LUbksb(col1, lu, col, indx);

#ifdef __sgi
		for (i=1; i<=this->GetRow(); i++)
		{
#else
		for (int i=1; i<=this->GetRow(); i++)
		{
#endif /*__sgi */
			result(i,j) = col1(i);
		}
	}

  return result;
}

void Matrix::Inverse(void)
{
	if (!this->IsSquare())
	{
		fprintf(stderr, "Matrix::Inversion():\n");
		exit(1);
	}

  /*
   * LU decomposition
   */
	Matrix   lu(this->GetRow(), this->GetCol());
	Matrix indx(this->GetRow());
	double d;

	LUdcmp(lu,(*this),indx,&d);

  /*
   * Get inverse matrix
   */
	Matrix  col(this->GetRow());
	Matrix col1(this->GetRow());

	int i,j;
	for (j=1; j<=this->GetRow(); j++)
	{
		for (i=1; i<=this->GetRow(); i++)
		{
			col(i) = 0.0;
		}
		col(j) = 1.0;
		LUbksb(col1, lu, col, indx);

#ifdef __sgi
		for (i=1; i<=this->GetRow(); i++)
		{
#else
		for (int i=1; i<=this->GetRow(); i++)
		{
#endif /* __sgi */
			(*this)(i,j) = col1(i);
		}
	}
}



/*****************************************************************************
 * Det : matrix determinat
 *
 * Reference : William H. Press, et al, NUMERICAL RECIPES in C, 
 *             Cambridge University Press, 1988
 *****************************************************************************/
double Matrix::Det(void) const
{
	if (!this->IsSquare())
	{
		fprintf(stderr, "Matrix::det(): \n");
		exit(1);
	}

	Matrix   lu(this->GetRow(),this->GetRow());
	Matrix indx(this->GetRow());

	double det;
	if(LUdcmp(lu, (*this), indx, &det)==0)
		return 0.0;

	for (int j=1; j<=this->GetRow(); j++)
		det *= lu(j,j);

  return det;
}

/*****************************************************************************
 * Pythag : calcuarate root square of a*a + b*b 
 *
 * arg  : double : a
 *      : double : b
 * ret  : double : sqrt(a*a + b*b)
 *
 *
 * Reference : William H. Press, et al, NUMERICAL RECIPES in C, 
 *             Cambridge University Press, 1988
 *****************************************************************************/
inline double Pythag(double a, double b)
{
	double absa=fabs(a);
	double absb=fabs(b);

	double sqrarg;
	if (absa > absb)
	{
		sqrarg = absb/absa;
		return absa*sqrt(1.0+(sqrarg==0.0?0.0:sqrarg*sqrarg));
	}
	else
	{
		sqrarg = absa/absb;
		return (absb==0.0 ? 0.0 : absb*sqrt(1.0+(sqrarg==0.0?0.0:sqrarg*sqrarg)));
  }
}

/*****************************************************************************
 * SglValDcmp : Singular Value Decomposition
 *
 * arg  : Matrix : u   : 
 *      : Matrix : w   :
 *      : Matrix : v   :
 *      : Matrix : a   :    a = u * w * v.Transposition()
 *
 * Reference : William H. Press, et al, NUMERICAL RECIPES in C, 
 *             Cambridge University Press, 1988
 *****************************************************************************/
void Matrix::SglValDcmp(Matrix& uu, Matrix& w, Matrix& vv) const
{
	if (!(this->GetRow()==uu.GetRow() && this->GetCol() == uu.GetCol()))
	{
		fprintf(stderr, "Matrix::SglValDcmp(): \n");
		exit(1);
	}
	
	Matrix u(uu.GetRow(), uu.GetCol());
	Matrix v(vv.GetRow(), vv.GetCol());

	u.LoadMatrix(*this);

	int flag, i, its, j, jj, k, nm, l, n, m;
	double anorm,c,f,g,h,s,scale,x,y,z;
	double maxarg1, maxarg2;
	double iminarg1, iminarg2;

	m = u.GetRow();
	n = u.GetCol();
	Matrix rv1(n);

	g = scale = anorm = 0.0;

	for (i=1; i<=n; i++)
	{
		l=i+1;
		rv1(i) = scale*g;
		g = s = scale = 0.0;
		
		if (i <= m)
		{
			for (k=i; k<=m; k++)
				scale += fabs(u(k,i));
      
			if (scale)
			{
				for (k=i; k<=m; k++)
				{
					u(k,i) /= scale;
					s += u(k,i)*u(k,i);
				}
				
				f = u(i,i);
				g = - ( f>=0.0 ? fabs(sqrt(s)):-fabs(sqrt(s)) );
				h = f*g - s;
				u(i,i)=f-g;
	
				for (j=l; j<=n; j++)
				{
					for (s=0.0,k=i; k<=m; k++)
						s += u(k,i)*u(k,j);
	  
					f=s/h;
	  
					for (k=i; k<=m; k++)
						u(k,j) += f*u(k,i);
				}

				for (k=i; k<=m; k++)
					u(k,i) *= scale;
			}
		}

		w(i,i) = scale*g;
		g=s=scale=0.0;
    
		if (i<=m && i!=n)
		{
			for (k=l; k<=n; k++)
				scale += fabs(u(i,k));
      
			if (scale)
			{
				for (k=l; k<=n; k++)
				{
					u(i,k)/=scale;
					s += u(i,k)*u(i,k);
				}
	
				f = u(i,l);
				g = - ( f>=0.0 ? fabs(sqrt(s)):-fabs(sqrt(s)) );
				h = f*g-s;
				u(i,l) = f-g;
	
				for (k=l; k<=n; k++)
				{
					rv1(k) = u(i,k)/h;
				}
	
				for (j=l; j<=m; j++)
				{
					for (s=0.0, k=l; k<=n; k++)
						s += u(j,k)*u(i,k);
	  
					for (k=l; k<=n; k++)
						u(j,k) += s*rv1(k);
				}
	
				for (k=l; k<=n; k++)
					u(i,k) *= scale;
			}
		}
    
		maxarg1 = anorm;
		maxarg2 = fabs(w(i,i))+fabs(rv1(i));
		anorm = ( maxarg1>maxarg2 ? maxarg1 : maxarg2 );
	}

	for (i=n; i>=1; i--)
	{
		if (i<n)
		{
			if (g)
			{
				for (j=l; j<=n; j++)
					v(j,i) = (u(i,j)/u(i,l))/g;
	
				for (j=l; j<=n; j++)
				{
					for (s=0.0, k=l; k<=n; k++)
						s += u(i,k)*v(k,j);
	  
					for (k=l; k<=n; k++)
						v(k,j) += s*v(k,i);
				}
			}
      
			for (j=l; j<=n; j++)
				v(i,j) = v(j,i) = 0.0;
		}

		v(i,i) = 1.0;
		g = rv1(i);
		l = i;
	}
  
	iminarg1=m;
	iminarg2=n;
  
	for (i=(m<n ? m:n); i>=1; i--)
	{
		l=i+1;
		g=w(i,i);
    
		for (j=l; j<=n; j++)
			u(i,j) = 0.0;
    
		if (g)
		{
			g=1.0/g;

			for (j=l; j<=n; j++)
			{
				for (s=0.0,k=l; k<=m; k++)
	  				s += u(k,i)*u(k,j);
	
				f = (s/u(i,i))*g;
	
				for (k=i; k<=m; k++)
					u(k,j) += f*u(k,i);
			}
      
			for (j=i; j<=m; j++)
				u(j,i) *= g;
		}
		else
		{
			for (j=i; j<=m; j++)
				u(j,i) = 0.0;
		}
    
		u(i,i)+=1.0;
	}
	
	for (k=n; k>=1; k--)
	{
		for (its=1; its<=30; its++)
		{
			flag = 1;
      
			for (l=k; l>=1; l--)
			{
				nm = l - 1;
	
				if ( (fabs(rv1(l))+anorm) == anorm )
				{
					flag = 0;
					break;
				}
	
				if ( (fabs(w(nm,nm))+anorm) == anorm)
					break;
			}
      
			if (flag)
			{
				c = 0.0;
				s = 1.0;
	
				for (i=l; i<=k; i++)
				{
					f = s*rv1(i);
					rv1(i) = c*rv1(i);
	  
					if ( (fabs(f)+anorm) == anorm)
						break;
	  
					g = w(i,i);
					h = Pythag(f,g);
					w(i,i) = h;
					h = 1.0/h;
					c = g*h;
					s = -f*h;
	  
					for (j=1; j<=m; j++)
					{
						y = u(j,nm);
						z = u(j,i);
						u(j,nm) = y*c + z*s;
						u(j,i)  = z*c - y*s;
					}
				}
			}

			z = w(k,k);
      
			if (l == k)
			{
				if (z < 0.0)
				{
					w(k,k) = -z;
	  
					for (j=1; j<=n; j++)
						v(j,k) = -v(j,k);
				}
				break;
			}
      
			if (its == 30)
			{
				fprintf(stderr, "Matrix::SglValDcmp(): No convergence in 30 svdcmp iterations\n");
				exit(1);
			}

			x = w(l,l);
			nm = k - 1;
			y = w(nm,nm);
			g = rv1(nm);
			h = rv1(k);
			f = ((y-z)*(y+z)+(g-h)*(g+h))/(2.0*h*y);
			g = Pythag(f, 1.0);
			f = (x-z)*(x+z)+h*((y/(f+(f>=0.0?fabs(g):-fabs(g))))-h);
			f /= x;
			c = s = 1.0;
      
			for (j=l; j<=nm; j++)
			{
				i = j + 1;
				g = rv1(i);
				y = w(i,i);
				h = s*g;
				g = c*g;
				z = Pythag(f,h);
				rv1(j) = z;
				c = f/z;
				s = h/z;
				f = x*c + g*s;
				g = g*c - x*s;
				h = y*s;
				y *= c;
        
				for (jj=1; jj<=n; jj++)
				{
					x = v(jj,j);
					z = v(jj,i);
					v(jj,j) = x*c + z*s;
					v(jj,i) = z*c - x*s;
				}
        
				z = Pythag(f,h);
				w(j,j) = z;
        
				if (z)
				{
					z = 1.0/z;
					c = f*z;
					s = h*z;
				}
        
				f = c*g+s*y;
				x = c*y - s*g;
        
				for (jj=1; jj<=m; jj++)
				{
					y = u(jj,j);
					z = u(jj,i);
					u(jj,j) = y*c + z*s;
					u(jj,i) = z*c - y*s;
				}
			}
      
			rv1(l)  = 0.0;
			rv1(k)  = f;
			w(k,k) = x;
		}
	}

	Matrix no(this->GetCol());
	void QSort(Matrix& w, Matrix& no, int low, int high);
  
int num;
	for(num=1;num<=w.GetCol();num++)
		no(num) = (double)num;
  
	QSort(w, no, 1, w.GetCol());
  
	for(num=1; num<=w.GetCol(); num++)
	{
		for(i=1; i<=uu.GetRow(); i++)
			uu(i,num) = u(i,(int)no(num));
    
		for(j=1; j<=vv.GetRow(); j++)
			vv(j,num) = v(j,(int)no(num));
	}
}

void QSort(Matrix& w, Matrix& no, int low, int high)
{
	int mid;
	int Separate(Matrix& w, Matrix& no, int lp, int hp);
	
	if(low < high)
	{
		mid = Separate(w, no, low, high);
		QSort(w, no, low, mid-1);
		QSort(w, no, mid+1, high);
	}
}

int Separate(Matrix& w, Matrix& no, int lp, int hp)
{
	int upsw = 1;
	double w_tmp, no_tmp;

	while(lp<hp)
	{
		if(w(lp, lp) < w(hp, hp))
		{
			w_tmp = w(lp, lp); no_tmp = no(lp);
			w(lp, lp) = w(hp, hp); no(lp) = no(hp);
			w(hp, hp) = w_tmp; no(hp) = no_tmp;
      
			if(upsw)upsw = 0;
			else    upsw = 1;
		}
	
		if(upsw) hp--;
		else     lp++;
	}
  
	return (lp);
}




/*****************************************************************************
 * PseudoInverse : pseudo inverse matrix 
 *
 * arg  : Matrix : pa  : pseudo inverse matrix 
 *      : Matrix : a   : 
 *
 *
 * Reference : William H. Press, et al, NUMERICAL RECIPES in C, 
 *             Cambridge University Press, 1988
 *****************************************************************************/
void Matrix::PseudoInverse(void)
{
	if ( (!this->IsSquare()) || (this->GetRow()<this->GetCol()) )
	{
		fprintf(stderr, "Matrix:;PseudoInverse(): \n");
		exit(1);
	}

	int m,n;
	m = this->GetRow();
	n = this->GetCol();

	Matrix u(m,n);
	Matrix v(n,n);
	Matrix w(n,n);
	Matrix wi(n,n);

	(*this).SglValDcmp(u,w,v);

	double wmax,wmin;
	int i;
  
	for (i=1,wmax=0.0; i<=n; i++)
	{
		if (w(i,i)>wmax)
			wmax = w(i,i);
	}
  
	wmin = wmax*MATRIX_MACHINE_EPS;               /* defined at Inversion() */

	for (i=1; i<=n; i++)
	{
		if (w(i,i) < wmin)
			wi(i,i) = 0.0;
		else
			wi(i,i) = 1.0/w(i,i);
	}

	(*this) = (v * wi * u.Transposition());
}


Matrix Matrix::PseudoInversion(void) const
{
	if ( (!this->IsSquare()) || (this->GetRow()<this->GetCol()) )
	{
		fprintf(stderr, "Matrix:;PseudoInversion(): \n");
		exit(1);
	}

	int m,n;
	m = this->GetRow();
	n = this->GetCol();

	Matrix u(m,n);
	Matrix v(n,n);
	Matrix w(n,n);
	Matrix wi(n,n);

	(*this).SglValDcmp(u,w,v);

	double wmax,wmin;
	int i;
  
	for (i=1,wmax=0.0; i<=n; i++)
	{
		if (w(i,i)>wmax)
			wmax = w(i,i);
	}
  
	wmin = wmax*MATRIX_MACHINE_EPS;               /* defined at Inversion() */

	for (i=1; i<=n; i++)
	{
		if (w(i,i) < wmin)
			wi(i,i) = 0.0;
		else
			wi(i,i) = 1.0/w(i,i);
	}

	printf("--");
	v.Show();
	wi.Show();
	(u.Transposition()).Show();

	Matrix result = (v * wi * u.Transposition());
	return result;
}

/*****************************************************************************
 * ConditionNum : condition number
 *
 * arg  : Matrix : a : target matrix 
 * ret  : double : conditioning number 
 *****************************************************************************/
double Matrix::ConditionNum(void) const
{
	if (!this->IsSquare())
	{
		fprintf(stderr, "Matrix::ConditionNum(): \n");
		exit(1);
	}
  
	int m = this->GetRow(); int n = this->GetCol();
	Matrix u(m, n);
	Matrix v(n, n);
	Matrix w(n, n);

	(*this).SglValDcmp(u,w,v);

	double wmax,wmin;
	int i;
  
	for (wmin=wmax=fabs(w(1,1)),i=2; i<=this->GetCol(); i++)
	{
		if (fabs(w(i,i))>wmax)
			wmax = fabs(w(i,i));
		
		if (fabs(w(i,i))<wmin)
			wmin = fabs(w(i,i));
	}

	if (wmin<=0.0)
		return -1.0;

/*  return sqrt(wmax/wmin); */
	return log10(wmax/wmin); 
}
