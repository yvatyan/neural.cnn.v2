// #define NDEBUG
#include <assert.h>

#include "Headers\NeuralData.h"

using namespace neural;

void Buffer::checkRange(int x) const {
	assert(x < Width1D());
	return;
}
void Buffer::checkRange(int y, int x) const {
	assert(width_in_2d != 0);
	if(member1d) {
		assert(member1d->Size() % width_in_2d == 0);
	}
	if(member3d) {
		assert(member3d->Size() % width_in_2d == 0);
	}
	assert(y < Height2D());
	assert(x < Width2D());
	return;
}
void Buffer::checkRange(int z, int y, int x) const {
	assert(height_in_3d != 0);
	assert(width_in_3d != 0);
	if(member1d) {
		assert(member1d->Size() % (width_in_3d * height_in_3d) == 0);
	}
	if(member2d) {
		assert(member2d->Size() % (width_in_3d * height_in_3d) == 0);
	}
	assert(z < Depth3D());
	assert(y < Height3D());
	assert(x < Width3D());
	return;
}

double& Buffer::get1dFrom2d(int x) const {
	assert(member2d);
	return (*member2d) [ x / member2d->Width() ]
			[ x % member2d->Width() ];
}
double& Buffer::get1dFrom3d(int x) const {
	assert(member3d);
	return (*member3d) [ x / (member3d->Width() * member3d->Height()) ]
			[ (x / member3d->Width()) % member3d->Height() ]
			[ x % member3d->Width() ];
}
double& Buffer::get2dFrom1d(int y, int x) const {
	assert(member1d);
	return (*member1d) [ y * width_in_2d + x ];
}
double& Buffer::get2dFrom3d(int y, int x) const {
	assert(member3d);
	return (*member3d) [ (y * width_in_2d + x) / (member3d->Width() * member3d->Height()) ] 
			[ ( (y * width_in_2d + x) / member3d->Width()) % member3d->Height() ]
			[ (y * width_in_2d + x) % member3d->Width() ];
}
double& Buffer::get3dFrom1d(int z, int y, int x) const {
	assert(member1d);
	return (*member1d) [ z * width_in_3d * height_in_3d + y * width_in_3d + x ];
}
double& Buffer::get3dFrom2d(int z, int y, int x) const {
	assert(member2d);
	return (*member2d) [ (z * width_in_3d * height_in_3d + y * width_in_3d + x) / member2d->Width() ]
			[ (z * width_in_3d * height_in_3d + y * width_in_3d + x) % member2d->Width()]  ;
}
Buffer::Buffer(matrix1d<double>* data) {
	container_mode = false;
	member1d = data;
	member2d = NULL; member3d = NULL;
	width_in_2d = width_in_3d = height_in_3d = 0;
}
Buffer::Buffer(matrix2d<double>* data) {
	container_mode = false;
	member2d = data;
	member1d = NULL; member3d = NULL;
	width_in_2d = width_in_3d = height_in_3d = 0;
}
Buffer::Buffer(matrix3d<double>* data) {
	container_mode = false;
	member3d = data;
	member1d = NULL; member2d = NULL;
	width_in_2d = width_in_3d = height_in_3d = 0;
}
Buffer::Buffer(int x, double def) {
	container_mode = true;
	member1d = new matrix1d<double>(x, def);
	member2d = NULL; member3d = NULL;
	width_in_2d = width_in_3d = height_in_3d = 0;
}
Buffer::Buffer(int y, int x, double def) {
	container_mode = true;
	member2d = new matrix2d<double>(y, x, def);
	member1d = NULL; member3d = NULL;
	width_in_2d = width_in_3d = height_in_3d = 0;
}
Buffer::Buffer(int z, int y, int x, double def) {
	container_mode = true;
	member3d = new matrix3d<double>(z, y, x, def);
	member2d = NULL; member2d = NULL;
	width_in_2d = width_in_3d = height_in_3d = 0;
}
Buffer::~Buffer() {
	if(container_mode && member1d) delete member1d;
	if(container_mode && member2d) delete member2d;
	if(container_mode && member3d) delete member3d;
}
void Buffer::SetPseudo3Dheight(size_t heightIn3D) {
	height_in_3d = heightIn3D;
}
void Buffer::SetPseudo3Dwidth(size_t widthIn3D) {
	width_in_3d = widthIn3D;
}
void Buffer::SetPseudo2Dwidth(size_t widthIn2D) {
	width_in_2d = widthIn2D;
}
double Buffer::ElementAt(int x) const {
	assert(member1d || member2d || member3d);
	if(member1d) {
		checkRange(x);
		return (*member1d)[x];
	}
	if(member2d) {
		checkRange(x);
		return get1dFrom2d(x);
	}
	if(member3d) {
		checkRange(x);
		return get1dFrom3d(x);
	}
}
double Buffer::ElementAt(int y, int x) const {
	assert(member1d || member2d || member3d);
	if(member1d) {
		checkRange(y, x);
		return get2dFrom1d(y, x);
	}
	if(member2d) {
		checkRange(y, x);
		return (*member2d)[y][x];
	}
	if(member3d) {
		checkRange(y, x);
		return get2dFrom3d(y, x);
	}
}
double Buffer::ElementAt(int z, int y, int x) const {
	assert(member1d || member2d || member3d);
	if(member1d) {
		checkRange(z, y, x);
		return get3dFrom1d(z, y, x);
	}
	if(member2d) {
		checkRange(z, y, x);
		return get3dFrom2d(z, y, x);
	}
	if(member3d) {
		checkRange(z, y, x);
		return (*member3d)[z][y][x];
	}
}
void Buffer::ElementTo(int x, double elem) {
	assert(member1d || member2d || member3d);
	if(member1d) (*member1d)[x] = elem;
	if(member2d) get1dFrom2d(x) = elem;
	if(member3d) get1dFrom3d(x) = elem;
}
void Buffer::ElementTo(int y, int x, double elem) {
	assert(member1d || member2d || member3d);
	if(member1d) get2dFrom1d(y, x) = elem;
	if(member2d) (*member2d)[y][x] = elem;
	if(member3d) get2dFrom3d(y, x) = elem;
}
void Buffer::ElementTo(int z, int y, int x, double elem) {
	assert(member1d || member2d || member3d);
	if(member1d) get3dFrom1d(z, y, x) = elem;
	if(member2d) get3dFrom2d(z, y, x) = elem;
	if(member3d) (*member3d)[z][y][x] = elem;
}
size_t Buffer::Depth3D() const {
	assert(member1d || member2d || member3d);
	if(member1d) return member1d->Size() / (height_in_3d * width_in_3d);
	if(member2d) return member2d->Size() / (height_in_3d * width_in_3d);
	if(member3d) return member3d->Depth();
}
size_t Buffer::Height2D() const {
	assert(member1d || member2d || member3d);
	if(member1d) return member1d->Size() / width_in_2d;
	if(member2d) return member2d->Height();
	if(member3d) return member3d->Size() / width_in_2d;
}
size_t Buffer::Height3D() const {
	assert(member1d || member2d || member3d);
	if(member1d) return height_in_3d;
	if(member2d) return height_in_3d;
	if(member3d) return member3d->Height();
}
size_t Buffer::Width1D() const {
	assert(member1d || member2d || member3d);
	if(member1d) return member1d->Width();
	if(member2d) return member2d->Size();
	if(member3d) return member3d->Size();
}
size_t Buffer::Width2D() const {
	assert(member1d || member2d || member3d);
	if(member1d) return width_in_2d;
	if(member2d) return member2d->Width();
	if(member3d) return width_in_2d;
}
size_t Buffer::Width3D() const {
	assert(member1d || member2d || member3d);
	if(member1d) return width_in_3d;
	if(member2d) return width_in_3d;
	if(member3d) return member3d->Width();
}
