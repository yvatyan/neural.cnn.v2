// #define NDEBUG
#include <assert.h>

#include "Headers\NeuralData.h"

using namespace neural;

double Buffer::get1dFrom2d(int x) const {
	assert(member2d);
	return member2d [ x / member2d->height() ]
			[ x % member2d->width() ];
}
double Buffer::get1dFrom3d(int x) const {
	assert(member3d);
	return member3d [ x / (member3d->width() * member3d->height()) ]
			[ (x / member3d->width()) % member3d->height() ]
			[ x % member3d->width() ];
}
double Buffer::get2dFrom1d(int y, int x) const {
	assert(member1d);
	assert(width_in_2d);
	return member1d [ y * width_in_2d + x ];
}
double Buffer::get2dFrom3d(int y, int x) const {
	assert(member3d);
	assert(width_in_2d);
	return member3d [ (y * width_in_2d + x) / (member3d->width() * member3d->height()) ] 
			[ ( (y * width_in_2d + x) / member3d->width()) % member3d->height() ]
			[ (y * width_in_2d + x) % member3d->width() ];
}
double Buffer::get3dFrom1d(int z, int y, int x) const {
	assert(member1d);
	assert(width_in_3d);
	assert(height_in_3d);
	return member1d [ z * width_in_3d * height_in_3d + y * width_in_3d + x ];
}
double Buffer::get3dFrom2d(int z, int y, int x) const {
	assert(member2d);
	assert(width_in_3d);
	assert(height_in_3d);
	return member2d [ (z * width_in_3d * height_in_3d + y * width_in_3d + x) / member2d->height() ]
			[ (z * width_in_3d * height_in_3d + y * width_in_3d + x) % member2d->width()]  ;
}
