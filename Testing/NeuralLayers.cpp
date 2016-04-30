#include "../Sources/Neural/Headers/NeuralLayers.h"
#include <iostream>
#include <fstream>
using namespace std;
using namespace neural;

int main() {
	
	ifstream fileIn("input.txt");

	ILayer* neuralIn = new Input("input1", 3, 5, 5);
	cout <<	neuralIn->Properties();
	ILayer* neuralOut = new Output("output1", 5, 5, 75);

	Buffer in1(3, 5, 5, 0);
	system("pause");
	for(int i = 0; i < in1.Depth3D(); ++i) {
		for(int j = 0; j < in1.Height3D(); ++j) {
			for(int h = 0; h < in1.Width3D(); ++h) {
				double temp;
				fileIn >> temp;
				in1.ElementTo(i, j, h, temp);
			}
		}
	}
	
	
	static_cast<Input*>(neuralIn)->DataInput(in1);
	neuralOut->CalculateOutput(neuralIn);	
	Buffer ou1(static_cast<Output*>(neuralOut)->DataOutput());


	for(int i = 0; i < ou1.Depth3D(); ++i) {
		for(int j = 0; j < ou1.Height3D(); ++j) {
			for(int h = 0; h < ou1.Width3D(); ++h) {
				cout << ou1.ElementAt(i, j, h) << ' ';
			}
			cout << endl;
		}
		cout << "============\n";
	}

	return 0;
}

