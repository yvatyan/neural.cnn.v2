#ifndef IMAGEGRID_H_
#define IMAGEGRID_H_

#include "../../BMP/Headers/imageBMPcore.h"
#include "NeuralData.h"
#include "MultiData.h"

class ImageGrid {
	private:
        matrix2d< Matrix<RGB> > grid;
        size_t stride_y;
	    size_t stride_x;

    public:
        ImageGrid(const ImageBMPcore& image, size_t stride_y, size_t stride_x);

        const Matrix<RGB>& ImageAt(size_t y, size_t x) const;
        Buffer ImageBufferAt(size_t y, size_t x) const;

        ImageBMPcore Ruler() const;
        
        size_t ImageSizeX() const;
        size_t ImageSizeY() const;
};

#endif
