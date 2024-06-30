#include<iostream>

void compute_squares(unsigned int a[], unsigned int b[], const unsigned int n) {
	unsigned int *pa;
	unsigned int *pb;
	pa = a;
	for (pb = b; pb<b+n; pb++) {
		*pb = (*pa)*(*pa);
		pa++;
	}
}

int main() {
	const unsigned int n = 4;
	unsigned int a[n] = {2, 3, 4, 5};
	unsigned int b[n];

	compute_squares(a, b, n);

	std::cout << "a: ";
	for (unsigned int i = 0; i < n; i++) {
		std::cout << a[i] << ' ';
	}
	std::cout << std::endl << "b: ";
	for (unsigned int i = 0; i < n; i++) {
		std::cout << b[i] << ' ';
	}
	std::cout << std::endl;
}