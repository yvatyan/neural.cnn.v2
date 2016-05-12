#include "Headers/ImageGrid.h"

ImageGrid(const ImageBMPcore& image, size_t stride_y, size_t stride_x) 
    : grid(image.ImHeight() / stride_y, image.ImWidth() / stride_x)
    , stride_y(stride_y)
    , stride_x(stride_x)
{
    
    Matrix<RGB> image_matrix = image.ColorMatrix();
    size_t image_y = image_matrix.Heigth();
    size_t image_x = image_matrix.Width();

    for(int i = 0; i < image_y; ++i) {
        for(int j = 0; j < image_x; ++j) {
            grid[i / stride_y][j / stride_x][i % stride_y][j % stride_x] = image_matrix[i][j];
        }
    }
}
const Matrix<RGB>& ImageAt(size_t y, size_t x) const {
    
    assert(y < grid.Height());
    assert(x < grid.Width());

    return grid[y][x]; 
}
Buffer ImageBufferAt(size_t y, size_t x) const {
    
    assert(y < grid.Height());
    assert(x < grid.Width());

    Buffer buf(3, grid[y][x].Height(), grid[y][x].Width(), 0.);
    for(int d = 0; d < buf.Depth3D(); ++d) {
        for(int h = 0; h < buf.Height3D(); ++h) {
            for(int w = 0; w < buf.Width3D(); ++w) {
                double color_component;
                switch(d) {
                    case    0   :   color_component = grid[y][x][h][w].R;
                    case    1   :   color_component = grid[y][x][h][w].G;
                    case    2   :   color_component = grid[y][x][h][w].B;
                };
                buf.ElementTo(d, h, w, color_component); 
            }
        }
    }
    return buf;
}
