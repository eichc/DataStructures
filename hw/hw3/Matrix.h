#ifndef __matrix_h_
#define __matrix_h_

#include<iostream>
#include<fstream>

class Matrix {
public:
	//constructors, assignment operator, and destructor
	Matrix();
	Matrix(const Matrix& m);
	Matrix(unsigned int r, unsigned int c, double f);
	Matrix& operator=(const Matrix& m);
	~Matrix();

	//accessors
	unsigned int num_rows() const { return rows; }
	unsigned int num_cols() const { return cols; }
	bool get(unsigned int r, unsigned int c, double& val) const;
	double* get_row(unsigned int r) const;
	double* get_col(unsigned int c) const;

	//modifiers
	void clear();
	bool set(unsigned int r, unsigned int c, double val);
	void multiply_by_coefficient(double coef);
	bool swap_row(unsigned int source, unsigned int target);
	void transpose();
	bool add(const Matrix& m);
	bool subtract(const Matrix& m);
	Matrix* quarter();
	void resize(unsigned int r, unsigned int c, double fill);


private:
	unsigned int rows;
	unsigned int cols;
	double** grid;

	void make_quarter(Matrix& m, unsigned int i1, unsigned int i2, unsigned int j1, 
		unsigned int j2); //helper function for quarter()

};

std::ostream& operator<< (std::ostream& out, const Matrix& m);
bool operator== (const Matrix& m1, const Matrix& m2);
bool operator!= (const Matrix& m1, const Matrix& m2);

#endif