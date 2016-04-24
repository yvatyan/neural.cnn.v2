#include <iostream>

#include "..\Sources\Neural\Headers\NeuralData.h"

using namespace std;

int main() {
	matrix1d<double> test1(27);
	matrix2d<double> test2(8, 7);
	matrix3d<double> test3(3, 5, 7);

	for(int i = 0; i < test1.Size(); ++i) {
		test1[i] = i;
	}
	cout << "test1 filled\n";
	for(int i = 0; i < test2.Height(); ++i) {
		for(int j = 0; j < test2.Width(); ++j) {
			test2[i][j] = 100*i + j;
		}
	}
	cout << "test2 filled\n";
	for(int i = 0; i < test3.Depth(); ++i) {
		for(int j = 0; j < test3.Height(); ++j) {
			for(int h = 0; h < test3.Width(); ++h) {
				test3[i][j][h] = 10000*i + 100*j + h;
			}
		}
	}
	cout << "test3 filled\n\n";

	Buffer testFor1d(&test1);
	testFor1d.SetPseudo2Dwidth(9);
	testFor1d.SetPseudo3Dwidth(3);
	testFor1d.SetPseudo3Dheight(3);

	cout << "Information (contains 1D member)"
		"\n\nDepth3D:  " << testFor1d.Depth3D() <<
		"\nHeight2D: " << testFor1d.Height2D() <<
		"\nHeight3D: " << testFor1d.Height3D() <<
		"\nWidth1D:  " << testFor1d.Width1D() <<
		"\nWidth2D:  " << testFor1d.Width2D() <<
		"\nWidth3D:  " << testFor1d.Width3D() <<
		"\n\nElements in 1D mode:\n";

	for(int i = 0; i < testFor1d.Width1D(); ++i) {
		cout << testFor1d.ElementAt(i) << ' ';
	}
	cout << "\n\nElements in 2D mode:\n";
	for(int i = 0; i < testFor1d.Height2D(); ++i) {
		for(int j = 0; j < testFor1d.Width2D(); ++j) {
			cout << testFor1d.ElementAt(i, j) << ' ';
		}
		cout << endl;
	}
	cout << "\n\nElements in 3D mode:\n";
	for(int i = 0; i < testFor1d.Depth3D(); ++i) {
		for(int j = 0; j < testFor1d.Height3D(); ++j) {
			for(int h = 0; h < testFor1d.Width3D(); ++h) {
				cout << testFor1d.ElementAt(i, j, h) << ' ';
			}
			cout << endl;
		}
		cout << "=====================\n";
	}
	cout << endl;
	system("pause");
	system("cls");

	Buffer testFor2d(&test2);
	testFor2d.SetPseudo2Dwidth(17);				// success, OK	(for 2D member pseudo2d width in NOT checked)
	testFor2d.SetPseudo3Dwidth(7);
	testFor2d.SetPseudo3Dheight(4);

	cout << "Information (contains 2D member)"
		"\n\nDepth3D:  " << testFor2d.Depth3D() <<
		"\nHeight2D: " << testFor2d.Height2D() <<
		"\nHeight3D: " << testFor2d.Height3D() <<
		"\nWidth1D:  " << testFor2d.Width1D() <<
		"\nWidth2D:  " << testFor2d.Width2D() <<
		"\nWidth3D:  " << testFor2d.Width3D() <<
		"\n\nElements in 1D mode:\n";

	for(int i = 0; i < testFor2d.Width1D(); ++i) {
		cout << testFor2d.ElementAt(i) << ' ';
	}
	cout << "\n\nElements in 2D mode:\n";
	for(int i = 0; i < testFor2d.Height2D(); ++i) {
		for(int j = 0; j < testFor2d.Width2D(); ++j) {
			cout << testFor2d.ElementAt(i, j) << ' ';
		}
		cout << endl;
	}
	cout << "\n\nElements in 3D mode:\n";
	for(int i = 0; i < testFor2d.Depth3D(); ++i) {
		for(int j = 0; j < testFor2d.Height3D(); ++j) {
			for(int h = 0; h < testFor2d.Width3D(); ++h) {
				cout << testFor2d.ElementAt(i, j, h) << ' ';
			}
			cout << endl;
		}
		cout << "=====================\n";
	}
	cout << endl;
	system("pause");
	system("cls");
	
	Buffer testFor3d(&test3);
	testFor3d.SetPseudo2Dwidth(15);
	testFor3d.SetPseudo3Dheight(63);			// success, OK	(for 3D member pseudo3d height in NOT checked)
	testFor3d.SetPseudo3Dwidth(13);				// success, OK	(for 3D member pseudo3d width in NOT checked)

	cout << "Information (contains 3D member)"
		"\n\nDepth3D:  " << testFor3d.Depth3D() <<
		"\nHeight2D: " << testFor3d.Height2D() <<
		"\nHeight3D: " << testFor3d.Height3D() <<
		"\nWidth1D:  " << testFor3d.Width1D() <<
		"\nWidth2D:  " << testFor3d.Width2D() <<
		"\nWidth3D:  " << testFor3d.Width3D() <<
		"\n\nElements in 1D mode:\n";

	for(int i = 0; i < testFor3d.Width1D(); ++i) {
		cout << testFor3d.ElementAt(i) << ' ';
	}
	cout << "\n\nElements in 2D mode:\n";
	for(int i = 0; i < testFor3d.Height2D(); ++i) {
		for(int j = 0; j < testFor3d.Width2D(); ++j) {
			cout << testFor3d.ElementAt(i, j) << ' ';
		}
		cout << endl;
	}
	cout << "\n\nElements in 3D mode:\n";
	for(int i = 0; i < testFor3d.Depth3D(); ++i) {
		for(int j = 0; j < testFor3d.Height3D(); ++j) {
			for(int h = 0; h < testFor3d.Width3D(); ++h) {
				cout << testFor3d.ElementAt(i, j, h) << ' ';
			}
			cout << endl;
		}
		cout << "=====================\n";
	}
	cout << endl;
	system("pause");
	system("cls");


	// test out of range index assert

//	double t1 = testFor1d.ElementAt(300);			// abort, OK
//	double t2 = testFor1d.ElementAt(300, 400);		// abort, OK
//	double t21 = testFor1d.ElementAt(0, 300);		// abort, OK
//	double t3 = testFor1d.ElementAt(300, 400, 500);		// abort, OK
//	double t31 = testFor1d.ElementAt(0, 400, 500);		// abort, OK
//	double t32 = testFor1d.ElementAt(0, 0, 500);		// abort, OK
	
	// test zero pseudo sizes assert

//	testFor1d.SetPseudo2Dwidth(0);
//	testFor1d.SetPseudo3Dwidth(0);
//	testFor1d.SetPseudo3Dheight(0);

//	double p1 = testFor1d.ElementAt(0);			// success, OK
//	double p2 = testFor1d.ElementAt(0, 0);			// abort, OK 
//	double p3 = testFor1d.ElementAt(0, 0, 0);		// abort, OK

	// test float sizes assert
	
//	testFor1d.SetPseudo2Dwidth(5); 				// abort, OK	( 27 % 5 != 0 )
//	testFor1d.SetPseudo3Dwidth(5);				// abort, OK	( 27 / 3 % 5 != 0)
//	testFor1d.SetPseudo3Dheight(3);				// success, OK	( 27 % 3 == 0 )

//	testFor1d.SetPseudo3Dwidth(3);				// success, OK	( 27 % 3 == 0)
//	testFor1d.SetPseudo3Dwidth(5);				// abort, OK	( 27 / 3 % 5 != 0)

//	double f2 = testFor1d.ElementAt(0, 0);
//	double f3 = testFor1d.ElementAt(0, 0, 0);

	return 0;
}
