#include "../Sources/BMP/Headers/imageBMPcore.h"
#include "../Sources/Neural/Headers/ImageGrid.h"

#include <iostream>
using namespace neural;
using namespace my;
using namespace std;

int main() {

    ImageBMPcore mnist;
    mnist.Open("mnist.bmp");
    cout << "Opened" << endl;
    cout << "Enter stride by Oy, then by Ox: ";
    int sy, sx;
    cin >> sy >> sx;
    ImageGrid grid(mnist, sy, sx);
    cout << "\nGrid Created" << endl;
    mnist.Create("mnist_r.bmp", grid.Ruler()); 
    cout << "Image Created" << endl;

    return 0;
}
