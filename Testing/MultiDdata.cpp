#include <iostream>

#include "..\Sources\Neural\Headers\MultiDdata.hpp"

using namespace std;

int main() {
	matrix1d<double> test1(10, 69);			
	matrix1d<double>* test1p = new matrix1d<double>(11, 69);
	
	matrix2d<double> test2(11, 12, 69);			
	matrix2d<double>* test2p = new matrix2d<double>(13, 14, 69);
	
	matrix3d<double> test3(3, 4, 5, 69);			
	matrix3d<double>* test3p = new matrix3d<double>(1, 2, 3, 69);

	cout << "matrix 1D\nSize: " << test1.Size() << "\nWidth: " << test1.Width() << "\nElements:\n";
	test1[2] = 73;
	for(int i = 0; i < test1.Width(); ++i) {
		cout << test1[i] << ' ';
	}
	cout << endl << endl;

	cout << "matrix 1D pointer\nSize: " << test1p->Size() << "\nWidth: " << test1p->Width() << "\nElements:\n";
	(*test1p)[2] = 73;
	for(int i = 0; i < test1p->Width(); ++i) {
		cout << (*test1p)[i] << ' ';
	}
	cout << endl << endl;

	cout << "matrix 2D\nSize: " << test2.Size() << "\nHeight: " << test2.Height() << "\nWidth: " << test2.Width() << "\nElements:\n";
	test2[2][2] = 73;
	for(int i = 0; i < test2.Height(); ++i) {
		for(int j = 0; j < test2.Width(); ++j) {
			cout << test2[i][j] << ' ';
		}
		cout << endl;
	}
	cout << endl;

	cout << "matrix 2D pointer\nSize: " << test2p->Size() << "\nHeight: " << test2p->Height() << "\nWidth: " << test2p->Width() << "\nElements:\n";
	(*test2p)[2][2] = 73;
	for(int i = 0; i < test2p->Height(); ++i) {
		for(int j = 0; j < test2p->Width(); ++j) {
			cout << (*test2p)[i][j] << ' ';
		}
		cout << endl;
	}
	cout << endl;

	cout << "matrix 3D\nSize: " << test3.Size() <<
		"\nDepth: " << test3.Depth() <<
		"\nHeight: " << test3.Height() <<
		"\nWidth: " << test3.Width() <<
		"\nElements:\n";
	test3[2][2][2] = 73;
	for(int i = 0; i < test3.Depth(); ++i) {
		for(int j = 0; j < test3.Height(); ++j) {
			for(int h = 0; h < test3.Width(); ++h) {
				cout << test3[i][j][h] << ' ';
			}
			cout << endl;
		}
		cout << "===============\n";
	}
	cout << endl << endl;

	cout << "matrix 3D pointer\nSize: " << test3p->Size() <<
		"\nDepth: " << test3p->Depth() <<
		"\nHeight: " << test3p->Height() <<
		"\nWidth: " << test3p->Width() <<
		"\nElements:\n";
	(*test3p)[0][0][0] = 73;
	for(int i = 0; i < test3p->Depth(); ++i) {
		for(int j = 0; j < test3p->Height(); ++j) {
			for(int h = 0; h < test3p->Width(); ++h) {
				cout << (*test3p)[i][j][h] << ' ';
			}
			cout << endl;
		}
		cout << "===============\n";
	}
	cout << endl << endl;
	return 0;
}
