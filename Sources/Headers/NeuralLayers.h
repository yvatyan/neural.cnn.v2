#ifndef NEURALLAYERS_H_
#define NEURALLAYERS_H_

#include <string>
#include <vector>

#include "NeuralFunctions.h"

namespace neural {
	class ILayer {
		protected:
			std::string layerName;
			Buffer output;
			Buffer deltas;
			Function layerFunction;
			Type layerType;
		public:
			ILayer(const std::string& name, Type type, Function function);
			
			LayerDeltas(const ILayer* prevLayer) = 0;
			LayerOutput(const ILayer* prevLayer) = 0;
			LayerCorrection() = 0;
			
			Function LayerFunction() const;
			Type LayerTyape() const;
	};
	class Convolution : public ILayer {
		private:
			std::vector<Buffer> kernels;
			int stride;
			int zeroPad;

			void autoZeroPadding();
		public:
      Convolution(const std::string& name, Function function, std::vector< std::pair<size_t, size_t> > kernels, Buffer data, int stride = 0, int zeroPadding = -1);
      Convolution(const std::string& name, Function function, std::vector< std::pair<size_t, size_t> > kernels, size_t width, size_t height, size_t depth, int stride = 0, int zeroPadding = -1);

      static Layer* CreateNextLayer(const Layer* layer); 
	};
}

#endif