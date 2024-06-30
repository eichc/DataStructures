#include<algorithm>
#include<cmath>
#include "Matrix.h"

//Default constructor
Matrix::Matrix() {
	rows = 0;
	cols = 0;
	grid = NULL;
}

//Copy constructor
Matrix::Matrix(const Matrix& m) {
	rows = m.num_rows();
	cols = m.num_cols();

	double temp = 0;
	grid = new double*[rows];
	for (unsigned int i = 0; i < rows; i++) {
		grid[i] = new double[cols];
		for (unsigned int j = 0; j < cols; j++) {
			m.get(i,j,temp);
			grid[i][j] = temp;
		}
	}
}

//Constructor with all 3 arguments
Matrix::Matrix(unsigned int r, unsigned int c, double f) {
	if (r == 0 || c == 0) {
		rows = 0;
		cols = 0;
		grid = NULL;
		return;
	}
	rows = r;
	cols = c;
	grid = new double*[rows];
	for (unsigned int i = 0; i < rows; i++) {
		grid[i] = new double[cols];
		for (unsigned int j = 0; j < cols; j++) {
			grid[i][j] = f;
		}
	}
}

//Allows use of asignment operator to create a copy of a matrix
Matrix& Matrix::operator=(const Matrix& m) {
	//delete all data in the matrix before assigning new data
	if (rows > 0) {
		for (unsigned int i = 0; i < rows; i++) {
			delete [] grid[i];
		}
	}
	delete [] grid;

	//same functionality as copy constructor
	rows = m.num_rows();
	cols = m.num_cols();

	double temp = 0;
	grid = new double*[rows];
	for (unsigned int i = 0; i < rows; i++) {
		grid[i] = new double[cols];
		for (unsigned int j = 0; j < cols; j++) {
			m.get(i,j,temp);
			grid[i][j] = temp;
		}
	}
	return *this;
}

//Destructor
Matrix::~Matrix() {
	if (rows > 0) {
		for (unsigned int i = 0; i < rows; i++) {
			delete [] grid[i];
		}
	}
	delete [] grid;
}

//Set val to the value stored at [r,c]
bool Matrix::get(unsigned int r, unsigned int c, double& val) const {
	if (r >= 0 && c >= 0 && r < rows && c < cols) {
		val = grid[r][c];
		return true;
	}
	return false;
}

//Create an array with the values in the desired row, and return a pointer to that array
double* Matrix::get_row(unsigned int r) const {
	double* m_row;
	if (r < 0 || r >= rows) {
		m_row = NULL;
		return m_row;
	}

	m_row = new double[cols];
	for (unsigned int i = 0; i < cols; i++) {
		m_row[i] = grid[r][i];
	}

	return m_row;
}

//Create an array with the values in the desired column, and return a pointer to that array
double* Matrix::get_col(unsigned int c) const {
	double* m_col;
	if (c < 0 || c >= cols) {
		m_col = NULL;
		return m_col;
	}

	m_col = new double[rows];
	for (unsigned int i = 0; i < rows; i++) {
		m_col[i] = grid[i][c];
	}

	return m_col;
}

//Delete all data in the matrix
void Matrix::clear() {
	if (rows > 0) {
		for (unsigned int i = 0; i < rows; i++) {
			delete [] grid[i];
		}
	}
	delete [] grid;
	rows = 0;
	cols = 0;	
}

//Set the element at [r,c] to the value stored in val
bool Matrix::set(unsigned int r, unsigned int c, double val) {
	if (r >= 0 && c >= 0 && r < rows && c < cols) {
		grid[r][c] = val;
		return true;
	}
	return false;
}

//Multiply every element in the matrix by c
void Matrix::multiply_by_coefficient(double coef) {
	if (rows == 0) {
		return;
	}
	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < cols; j++) {
			grid[i][j] = grid[i][j] * coef;
		}
	}
}

//Swap the two given rows, as long as both rows exist in the matrix
bool Matrix::swap_row(unsigned int source, unsigned int target) {
	if (source >= 0 && target >= 0 && source < rows && target < rows) {
		double* temp;
		temp = grid[source];
		grid[source] = grid[target];
		grid[target] = temp;

		return true;
	}
	return false;
}

//Flip the matrix, turning it from (rows x cols) to (cols x rows)
void Matrix::transpose() {
	if (rows == 0) {
		return;
	}

	//create a new grid that has the swapped values
	unsigned int new_rows = cols;
	unsigned int new_cols = rows;
	double** new_grid;
	new_grid = new double*[new_rows];
	for (unsigned int i = 0; i < new_rows; i++) {
		new_grid[i] = new double[new_cols];
		for (unsigned int j = 0; j < new_cols; j++) {
			new_grid[i][j] = grid[j][i];
		}
	}

	//clean up memory
	clear();
	rows = new_rows;
	cols = new_cols;
	grid = new_grid;
}

//Add each element in matrix m to its corresponding element in the current matrix
bool Matrix::add(const Matrix& m) {
	if (m.num_rows() != rows || m.num_cols() != cols) {
		return false;
	}

	double temp;
	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < cols; j++) {
			m.get(i,j,temp);
			grid[i][j] += temp;
		}
	}

	return true;
}

//Subtract each element in matrix m from its corresponding element in the current matrix
bool Matrix::subtract(const Matrix& m) {
	if (m.num_rows() != rows || m.num_cols() != cols) {
		return false;
	}

	double temp;
	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < cols; j++) {
			m.get(i,j,temp);
			grid[i][j] -= temp;
		}
	}

	return true;
}

/*Make matrix m into a quarter of the original matrix. All other parameters are starting
and ending indices that make up the dimensions of the quarter.*/
void Matrix::make_quarter(Matrix& m, unsigned int i1, unsigned int i2, unsigned int j1, 
	unsigned int j2) {
	for (unsigned int i = i1; i < i2; i++) {
		for (unsigned int j = j1; j < j2; j++) {
			m.set(i-i1,j-j1,grid[i][j]);
		}
	}
}

//Split the matrix into four quadrants and return a Matrix* containing them.
Matrix* Matrix::quarter() {
	Matrix* quarters = new Matrix[4];

	//number of rows and columns per quarter
	unsigned int q_rows = std::ceil(rows/2.0);
	unsigned int q_cols = std::ceil(cols/2.0);

	//create each quarter with all values initialized to 0
	for (unsigned int i = 0; i < 4; i++) {
		quarters[i] = Matrix(q_rows, q_cols, 0);
	}

	//assign correct values to quarters
	make_quarter(quarters[0], 0, q_rows, 0, q_cols);
	make_quarter(quarters[1], 0, q_rows, cols/2, cols);
	make_quarter(quarters[2], rows/2, rows, 0, q_cols);
	make_quarter(quarters[3], rows/2, rows, cols/2, cols);

	return quarters;
}

/*Change the dimensions of the matrix to the given number of rows and columns. Keep all
values from the original matrix, and any new elements should have the given fill value. */
void Matrix::resize(unsigned int r, unsigned int c, double fill) {
	//create a grid with the new size, all values initialized to 0
	double** new_grid;
	new_grid = new double*[r];
	for (unsigned int i = 0; i < r; i++) {
		new_grid[i] = new double[c];
		for (unsigned int j = 0; j < c; j++) {
			new_grid[i][j] = fill;
		}
	}

	//transfer old values to the new grid
	for (unsigned int i = 0; i < std::min(rows,r); i++) {
		for (unsigned int j = 0; j < std::min(cols,c); j++) {
			new_grid[i][j] = grid[i][j];
		}

	}

	//clean up memory
	clear();
	rows = r;
	cols = c;
	grid = new_grid;
}

//Allows the matrix to be output using the << operator
std::ostream& operator<< (std::ostream& out, const Matrix& m) {
	out << std::endl << m.num_rows() << " x " << m.num_cols() << " matrix:" 
	<< std::endl << '[';
	double temp; //to pass into get()
	for (unsigned int i = 0; i < m.num_rows(); i++) {
		if (i != 0) {
			out << ' '; //to line up each row after the opening bracket
		}
		for (unsigned int j = 0; j < m.num_cols(); j++) {
			m.get(i,j,temp);
			out << ' ' << temp;
		}
		if (i == m.num_rows()-1) {
			out << " ]";
		}
		out << std::endl;
	}
	//account for the above loop not running for an empty matrix
	if (m.num_rows() == 0) {
		out << " ]" << std::endl;
	}

	return out;
}

//Allows comparison of two matrices using ==
bool operator== (const Matrix& m1, const Matrix& m2) {
	if (m1.num_rows() != m2.num_rows() || m1.num_cols() != m2.num_cols()) {
		return false;
	}
	double val1, val2;
	for (unsigned int i = 0; i < m1.num_rows(); i++) {
		for (unsigned int j = 0; j < m1.num_cols(); j++) {
			m1.get(i,j,val1);
			m2.get(i,j,val2);
			if (val1 != val2) {
				return false;
			}
		}
	}
	return true;
}

//Allows use of != to compare matrices
bool operator!= (const Matrix& m1, const Matrix& m2) {
	return !(m1 == m2);
}