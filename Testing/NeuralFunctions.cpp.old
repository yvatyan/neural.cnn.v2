#include <iostream>
#include <iomanip>

#include "..\Sources\Neural\Headers\NeuralFunctions.h"

using namespace std;
using namespace neural;

int main() {

	// Test activation functions

	cout << "==== Test activation functions ====\n";
	FunctionCollection collection;

	for(int i = 1; i < 11; ++i) {
		collection.Accumulate(i);
	}
	cout << setprecision(8);
	cout << "SoftMax(55, 7) = " << collection.Activation().SoftMax(7) << endl;
	cout << "SoftStep(55, 1) = " << collection.Activation().SoftStep(1) << endl;
	cout << "SoftPlus(55) = " << collection.Activation().SoftPlus() << endl;
	cout << "SoftSign(55) = " << collection.Activation().SoftSign() << endl;
	cout << "BinaryStep() = " << collection.Activation().BinaryStep() << endl;
	cout << "TanH(55) = " << collection.Activation().TanH() << endl;
	cout << "ArcTan(55) = " << collection.Activation().ArcTan() << endl;
	cout << "Identity(55) = " << collection.Activation().Identity() << endl;
	cout << "BentIdentity(55) = " << collection.Activation().BentIdentity() << endl;
	cout << "Gaussian(55) = " << collection.Activation().Gaussian() << endl;

	// Test combination functions
	
	cout << "\n==== Test combination functions ====\n";
	collection.ClearStorage();

	for(int i = 11; i < 22; ++i) {
		collection.Accumulate(i);
	}

	cout << "Maximum() = " << collection.Combination().Maximum() << endl; 
	cout << "Minimum() = " << collection.Combination().Minimum() << endl; 
	cout << "Mean() = " << collection.Combination().Mean() << endl; 
	cout << "MaxQtyBoolean() = " << collection.Combination().MaxQtyBoolean() << endl; 
	cout << "MinQtyBoolean() = " << collection.Combination().MinQtyBoolean() << endl; 
	cout << "GreyscaleHDTV(207, 32, 149) = " << collection.Combination().GreyscaleHDTV(207, 32,149) << endl; 
	cout << "GreyscaleYUV(207, 32, 149) = " << collection.Combination().GreyscaleYUV(207, 32, 149) << endl;
	cout << "Sum() = " << collection.Combination().Sum() << endl;
	cout << "BooleanSum() = " << collection.Combination().BooleanSum() << endl;
	cout << "Normalize(176, 54) = " << collection.Combination().Normalize(54) << endl;

	// Test names and types
	cout << "Name: " << FunctionCollection::FunctionName(FunctionCollection::SoftMax) << endl;
	cout << "Type (a): " << FunctionCollection::FunctionType(FunctionCollection::SoftMax) << endl;
	cout << "Type (c): " << FunctionCollection::FunctionType(FunctionCollection::Maximum) << endl;

	return 0;
}
