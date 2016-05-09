#include "../Sources/Neural/Headers/NeuralLayers.h"
#include <iostream>
#include <fstream>
using namespace std;
using namespace neural;

int main() {
	
    // CONVOLUTION LAYER TESTING

	ifstream fileIn("input_convolution.txt");


    Activation func(Activation::Identity);

    std::vector< boost::tuple< size_t, size_t, int > > kernels;
    kernels.push_back( boost::tuple< size_t, size_t, int> (3, 3, 2) );
    kernels.push_back( boost::tuple< size_t, size_t, int> (3, 3, 2) );

	ILayer* neuralIn = new Input("input1", 3, 5, 5);
    ILayer* neuralConv = new Convolution("conv1", func, kernels, 3, 3, 3);
	ILayer* neuralOut = new Output("output1", 3, 3, 18);

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
    ReadKernels(fileIn, neuralConv); 

	static_cast<Input*>(neuralIn)->DataInput(in1);
    neuralConv->CalculateOutput(neuralIn);
	neuralOut->CalculateOutput(neuralConv);	
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

    // PULLING LAYER TESTING
    system("pause");
    system("cls");
    cout << "==== PULLING LAYER TEST ==== " << std::endl;

    fileIn.close();
    fileIn.open("input_pulling.txt");
    
    Combination func2(Combination::Maximum);

    ILayer* neuralIn2 = new Input("input2", 2, 10, 10);
    ILayer* neuralPull = new Pulling("pull2", func2, 2, 2, 2, 5, 5);
    ILayer* neuralOut2 = new Output("output2", 5, 5, 50);

    Buffer in2(2, 10, 10, 0);
	system("pause");
	for(int i = 0; i < in2.Depth3D(); ++i) {
		for(int j = 0; j < in2.Height3D(); ++j) {
			for(int h = 0; h < in2.Width3D(); ++h) {
				double temp;
				fileIn >> temp;
				in2.ElementTo(i, j, h, temp);
			}
		}
	}

	static_cast<Input*>(neuralIn2)->DataInput(in2);
    neuralPull->CalculateOutput(neuralIn2);
	neuralOut2->CalculateOutput(neuralPull);	
	Buffer ou2(static_cast<Output*>(neuralOut2)->DataOutput());

	for(int i = 0; i < ou2.Depth3D(); ++i) {
		for(int j = 0; j < ou2.Height3D(); ++j) {
			for(int h = 0; h < ou2.Width3D(); ++h) {
				cout << ou2.ElementAt(i, j, h) << ' ';
			}
			cout << endl;
		}
		cout << "============\n";
	}

	return 0;
}
