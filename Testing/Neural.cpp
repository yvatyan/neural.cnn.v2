#include <iostream>
#include <string>

#include "../Sources/Neural/Headers/NeuralLayers.h"
#include "../Sources/Neural/Headers/ImageGrid.h"

using namespace std;
using namespace my;
using namespace neural;

int main() {
    
    Activation logistic(Activation::SoftStep);
    Activation identity(Activation::Identity);

    ILayer* input  = new Input("in", 1, 28, 28);
    ILayer* full1  = new FullConnected("full1", logistic, 8000, 784);
    ILayer* full2  = new FullConnected("full2", identity, 10, 8000);
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

    
    bool skip = true;
    for(int i = 0; i < 10; ++i) {
        image.Open(imageNames[i].c_str());
        ImageGrid grid(image, 28, 28);
    
        for(int y = 0; y < grid.GridSizeY(); ++y) {
            for(int x = 0; x < grid.GridSizeX(); ++x) {

                static_cast<Input*>(input)->DataInput(grid.YuvGreyBufferAt(y, x));
                full1->CalculateOutput(input);
                full2->CalculateOutput(full1);
                output->CalculateOutput(full2);

                Buffer ou(static_cast<Output*>(output)->DataOutput());
                cout << " ==== Object: " << i << " Image: " << y * grid.GridSizeX() + x + 1 << '/' << grid.GridSize() << endl;

                double e_sum = 0.;
                for(int s = 0; s < ou.Size(); ++s) e_sum += pow(M_E, ou.ElementAt(s));
                Activation softmax(Activation::SoftMax, e_sum);
                for(int s = 0; s < ou.Size(); ++s) cout << softmax(ou.ElementAt(s)) << ' ';
                cout << endl;

                //if(skip) cin >> skip;


            }
        }
    }
        return 0;
}
