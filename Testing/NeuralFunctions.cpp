#include <iostream>

#include "..\Sources\Neural\Headers\NeuralFunctions.h"

using namespace std;

int main() {

	// Test activation functions
	
	FunctionCollection collection;
	collection.Accumulate(91);

	cout << "SoftMax(91, 8) = " << collection.Activation().SoftMax(8) << endl;
	cout << "SoftStep(91, 8) = " << collection.Activation().SoftStep(8) << endl;
	cout << "SoftPlus(91) = " << collection.Activation().SoftPlus() << endl;

}
