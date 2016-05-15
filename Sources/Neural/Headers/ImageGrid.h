#ifndef IMAGEGRID_H_
#define IMAGEGRID_H_

#include "../../BMP/Headers/imageBMPcore.h"
#include "NeuralData.h"
#include "MultiDdata.hpp"

class ImageGrid {
	private:
        matrix2d< my::Matrix<my::RGB> > grid;
        size_t stride_y;
	    size_t stride_x;

    public:
        ImageGrid(const my::ImageBMPcore& image, size_t stride_y, size_t stride_x);

        const my::Matrix<my::RGB>& ImageAt(size_t y, size_t x) const;

        Buffer ImageBufferAt(size_t y, size_t x) const;
        Buffer YuvGreyBufferAt(size_t y, size_t x) const;
        Buffer HdtvGreyBufferAt(size_t y, size_t x) const;

        my::Matrix<my::RGB> Ruler() const;
        
        size_t GridSizeX() const;
        size_t GridSizeY() const;
        size_t GridSize() const;
};

#include "../ImageGrid.cpp"    

#endif
