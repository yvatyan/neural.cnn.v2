#include "Headers/ImageGrid.h"
#include <iostream>

using namespace my;

ImageGrid::ImageGrid(const ImageBMPcore& image, size_t stride_y, size_t stride_x) 
    : grid(image.ImHeight() / stride_y, image.ImWidth() / stride_x, Matrix<RGB>(stride_y, stride_x))
    , stride_y(stride_y)
    , stride_x(stride_x)
{ 

    image.Info();
    Matrix<RGB> image_matrix = image.ColorMatrix();
    size_t image_y = image_matrix.height();
    size_t image_x = image_matrix.width();

    for(int i = 0; i < image_y; ++i) {
        for(int j = 0; j < image_x; ++j) {/*
            std::cout << std::endl << i << " - " << j << ", " << i / stride_y << "   " << j / stride_x << "   " << i % stride_y << "   " << j % stride_x;
            std::cout << " Color: " << image_matrix[i][j];    
            std::cout << " Size: " << image.ImHeight() << "x" << image.ImWidth();
            grid[i / stride_y];
            std::cout << " Step 1";
            grid[i / stride_y][j / stride_x];
            std::cout << " Step 2";
            grid[i / stride_y][j / stride_x][i % stride_y];
            std::cout << " Step 3";
            std::cout << " " << grid[i / stride_y][j / stride_x].height();//[i % stride_y][j % stride_x];
            std::cout << " Step 4";*/
            if(i / stride_y >= grid.Height() || j / stride_x >= grid.Width()) continue;
            grid[i / stride_y][j / stride_x][i % stride_y][j % stride_x] = image_matrix[i][j];
//            std::cout << " Step 5";
        }
    }
}
const Matrix<RGB>& ImageGrid::ImageAt(size_t y, size_t x) const {
    
    assert(y < grid.Height());
    assert(x < grid.Width());

    return grid[y][x]; 
}
Buffer ImageGrid::ImageBufferAt(size_t y, size_t x) const {
    
    assert(y < grid.Height());
    assert(x < grid.Width());

    Buffer buf(3, grid[y][x].height(), grid[y][x].width(), 0.);
    for(int d = 0; d < buf.Depth3D(); ++d) {
        for(int h = 0; h < buf.Height3D(); ++h) {
            for(int w = 0; w < buf.Width3D(); ++w) {
                double color_component;
                switch(d) {
                    case    0   :   color_component = grid[y][x][h][w].R; break;
                    case    1   :   color_component = grid[y][x][h][w].G; break;
                    case    2   :   color_component = grid[y][x][h][w].B; break;
                };
                buf.ElementTo(d, h, w, color_component); 
            }
        }
    }
    return buf;
}
Buffer ImageGrid::YuvGreyBufferAt(size_t y, size_t x) const {
    
    assert(y < grid.Height());
    assert(x < grid.Width());

    Buffer buf(1, grid[y][x].height(), grid[y][x].width(), 0.);
    for(int h = 0; h < buf.Height3D(); ++h) {
        for(int w = 0; w < buf.Width3D(); ++w) {
            RGB color = grid[y][x][h][w];
            double color_component = 0.299*color.R + 0.587*color.G + 0.114*color.B;;
            buf.ElementTo(0, h, w, color_component); 
        }
    }
    return buf;
}
Buffer ImageGrid::HdtvGreyBufferAt(size_t y, size_t x) const {
    
    assert(y < grid.Height());
    assert(x < grid.Width());

    Buffer buf(1, grid[y][x].height(), grid[y][x].width(), 0.);
    for(int h = 0; h < buf.Height3D(); ++h) {
        for(int w = 0; w < buf.Width3D(); ++w) {
            RGB color = grid[y][x][h][w];
            double color_component = 0.2126*color.R + 0.7152*color.G + 0.0722*color.B;
            buf.ElementTo(0, h, w, color_component); 
        }
    }
    return buf;
}
Matrix<RGB> ImageGrid::Ruler() const {
    
    Matrix<RGB> res(grid.Height()*(stride_y + 1) - 1, grid.Width()*(stride_x + 1) - 1);
    
    for(int i = 0; i < grid.Height(); ++i) {
        for(int j = 0; j < grid.Width(); ++j) {
            for(int y = 0; y <= stride_y; ++y) {
                for(int x = 0; x <= stride_x; ++x) {

                    int alpha = i * (stride_y + 1) + y;
                    int beta = j * (stride_x + 1) + x;
                    if(x == stride_x || y == stride_y) {
                        bool line_to_x = (x == stride_x);
                        bool line_to_y = (y == stride_y);
                        if( alpha < res.height() && beta < res.width() ) {
                            RGB color = grid[i][j][y - line_to_y][x - line_to_x];
                            color.R = 255 - color.R;
                            color.G = 255 - color.G;
                            color.B = 255 - color.B;

                            res[alpha][beta] = color;
                        }
                    }
                    else {
                        res[alpha][beta] = grid[i][j][y][x];
                    }
                }
            }
        }
    }
    return res;
}
size_t ImageGrid::GridSizeY() const {
    return grid.Height();
}
size_t ImageGrid::GridSizeX() const {
    return grid.Width();
}
size_t ImageGrid::GridSize() const {
    return grid.Size();
}
