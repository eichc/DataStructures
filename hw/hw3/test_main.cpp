#include <iostream>
#include <fstream>
#include<cassert>
#include<cmath>
#include "Matrix.h"

int main() {
	Matrix m1(5,5,1);
	Matrix m2;
	m2 = m1;

	m2.set(2,0,0);
	m2.set(2,2,2);
	m2.set(1,2,-4);
	m2.set(3,3,7);
	m2.add(Matrix(5,5,3));
	std::cout << m2 << std::endl;

	

	std::cout << "DONE" << std::endl;
}