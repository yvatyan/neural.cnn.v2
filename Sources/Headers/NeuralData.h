#ifndef NEURALDATA_H_
#define NEURALDATA_H_

#include "MultiDdata.h"

namespace neural {
	class Buffer {
		private:
			matrix1d<double>* member1d;
			matrix2d<double>* member2d;
			matrix3d<double>* member3d;
			
			double get1dFrom2d(int x) const ;
			double get1dFrom3d(int x)const ;
			double get2dFrom1d(int y, int x) const ;
			double get2dFrom3d(int y, int x)const ;
			double get3dFrom1d(int z, int y, int x)const ;
			double get3dFrom2d(int z, int y, int x)const ;
		public:
			// wrapper constructors
			Buffer(matrix1d<double>* data);
			Buffer(matrix2d<double>* data);
			Buffer(matrix3d<double>* data);
			// data container constructors
			Buffer(int x, double def = 0);
			Buffer(int y, int x, double def = 0);
			Buffer(int z, int y, int x, double def = 0);
	
			double ElementAt(int x) const ;
			double ElementAt(int y, int x) const ;
			double ElementAt(int z, int y, int x) const ;
			
			void ElementTo(int x, double elem);
			void ElementTo(int y, int x, double elem);
			void ElementTo(int z, int y, int x, double elem);
	};
}

#endif
