#include <iostream>
#include <iomanip>
#include <string>

#include "../Sources/Neural/Headers/NeuralLayers.h"
#include "../Sources/Neural/Headers/ImageGrid.h"

using namespace std;
using namespace my;
using namespace neural;

int main() {
    
    cout << fixed << setprecision(4);

    Activation logistic(Activation::SoftStep, 1./18000);
    Activation identity(Activation::Identity);

    ILayer* input  = new Input("in", 1, 28, 28);
    ILayer* inputD = new Input("inD", 1, 1, 10);
    ILayer* full1  = new FullConnected("full1", logistic, 500, 784);
    ILayer* full2  = new FullConnected("full2", identity, 10, 500);
    ILayer* output = new Output("out", 10);

    ImageBMPcore image;
    std::string imageNames[10];
    imageNames[0] = "0.bmp";
    imageNames[1] = "1.bmp";
    imageNames[2] = "2.bmp";
    imageNames[3] = "3.bmp";
    imageNames[4] = "4.bmp";
    imageNames[5] = "5.bmp";
    imageNames[6] = "6.bmp";
    imageNames[7] = "7.bmp";
    imageNames[8] = "8.bmp";
    imageNames[9] = "9.bmp";

    
    for(int i = 0; i < 10; ++i) {
        image.Open(imageNames[i].c_str());
        ImageGrid grid(image, 28, 28);
    
        for(int y = 0; y < grid.GridSizeY(); ++y) {
            for(int x = 0; x < grid.GridSizeX(); ++x) {

		// Phase 1: Forward iteration
                static_cast<Input*>(input)->DataInput(grid.YuvGreyBufferAt(y, x));
                full1->CalculateOutput(input);
                full2->CalculateOutput(full1);
                output->CalculateOutput(full2);

                Buffer ou(static_cast<Output*>(output)->DataOutput());
                cout << " ==== Object: " << i << " Image: " << y * grid.GridSizeX() + x + 1 << '/' << grid.GridSize() << endl;

                double e_sum = 0.;
                for(int s = 0; s < ou.Size(); ++s) e_sum += pow(M_E, ou.ElementAt(s));
                Activation softmax(Activation::SoftMax, e_sum);
		cout << "Before: ";
                for(int s = 0; s < ou.Size(); ++s) cout << softmax(ou.ElementAt(s)) << ' ';
                cout << endl;
		// Phase 1: Calculate difference
                Buffer deltas(1, 1, 10, 0.);
		cout << "Deltas : ";
                for(int s = 0; s < deltas.Size(); ++s) {

                    double value = 0.;
                    if(s == i) value = 1.;
                    deltas.ElementTo(s, std::pow((value - softmax(ou.ElementAt(s))), 2));
		    cout << deltas.ElementAt(s) << ' ';
                }
		cout << endl;
		// Phase 2: Backward iteration
                static_cast<Input*>(inputD)->DeltaInput(deltas);
                inputD->CalculateDeltas(full2);
                full2->CalculateDeltas(full1);
		// Phase 2: Forward iteration
                full1->DoCorrections(input, 0.8);
                full2->DoCorrections(full1, 0.8);
                output->CalculateOutput(full2);

                Buffer ou2(static_cast<Output*>(output)->DataOutput());
                e_sum = 0.;
                for(int s = 0; s < ou2.Size(); ++s) e_sum += pow(M_E, ou2.ElementAt(s));
                Activation softmax2(Activation::SoftMax, e_sum);
		cout << "After: ";
                for(int s = 0; s < ou2.Size(); ++s) cout << softmax2(ou2.ElementAt(s)) << ' ';
                cout << endl;

            }
        }
    }
        return 0;
}
