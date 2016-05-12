#include <iostream>
#include <string>

#include "Sources/Neural/Headers/ImageGrid.h"
#include "Sources/BMP/Headers/imageBMPcore.h"

using namespace neural;
using namespace my;
using namespace std;

int main() {
    
    ImageBMPcore image;
    std::string test = "test.bmp";
    std::string testRuler = "ruler.bmp";
    image.Open(test.c_str());
    
    ImageGrid grid(image, 3, 3);

    cout << "Grid Size: " << grid.GridSizeY() << 'x' << grid.GridSizeX() << endl;

    for(int i = 0; i < grid.GridSizeY(); ++i) {
        for(int j = 0; j < grid.GridSizeX(); ++j) {
            
            Matrix<RGB> local_im = grid.ImageAt(i, j);
            cout << "This is image at " << i << ' ' << j << endl;
            for(int h = 0; h < local_im.Height(); ++h) {
                for(int w = 0; w < local_im.Width(); ++w) {
                    cout << local_im[h][w] << ' ';
                }
                cout << endl;
            }

            Buffer local_buf = grid.ImageBufferAt(i, j);
            cout << "\n\nThis is buffer at " << i << ' ' << j << endl;
            for(int d = 0; d < local_buf.Depth3D(); ++d) {
                for(int h = 0; h < local_buf.Height3D(); ++h) {
                    for(int w = 0; w < local_buf.Width3D(); ++i) {
                       cout << local_buf.ElementAt(d, h, w) << ' '; 
                    }
                    cout << endl;
                }
                cout << "=======================" << endl;
            }
            system("pause");
            system("cls");
        }
    }

    image.Create(testRuler.c_str(), grid.Ruler());
    return 0;
}
