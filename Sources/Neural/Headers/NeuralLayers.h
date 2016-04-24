#ifndef NEURALLAYERS_H_
#define NEURALLAYERS_H_

#include <string>
#include <vector>

#include "NeuralFunctions.h"
#include "NeuralData.h"

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
			virtual ~ILayer();
			
			virtual void CalculateOutput(ILayer* prevLayer) = 0;
			virtual void CalculateDeltas(ILayer* prev_layer) = 0;

			virtual void DoCorrections() { return; }
			virtual std::string Properties() const = 0;
			
			Function LayerFunction() const;
			Type LayerTyape() const;
			std::string Name() const;
	};
	class Convolution : public ILayer {
		private:
			std::vector<Buffer> layerKernels;
			int stride;
			int zeroPad;

			void autoZeroPadding();
		public:
			Convolution(const std::string& name, Function function, std::vector< std::pair<size_t, size_t> > kernels, Buffer input, int stride = 0, int zeroPadding = -1);
			Convolution(const std::string& name, Function function, std::vector< std::pair<size_t, size_t> > kernels,  size_t inputDepth, size_t inputHeight, size_t inputWidth, int stride = 0, int zeroPadding = -1);
			Convolution(const std::string& name, Function function, std::vector< std::pair<size_t, size_t> > kernels, const ILayer* prevLayer, int stride = 0, int zeroPadding = -1); 
	
			void CalculateOutput(ILayer* prev_layer);
			void CalculateDeltas(ILayer* prev_layer);
			void DoCorrections();
			
			std::string Properties() const;
	};
	class Pulling : public ILayer {
		private:
			size_t kernel_width;
			size_t kernel_height;
		public:
			Pulling(const std::string& name, Function function, size_t kernelHeight, size_t kernelWidth, Buffer input);
			Pulling(const std::string& name, Function function, size_t kernelHeight, size_t kernelWidth, size_t inputDepth, size_t inputHeight, size_t inputWidth);
			Pulling(const std::string& name, Function function, size_t kernelHeight, size_t kernelWidth, ILayer* prevLayer);
			
			void CalculateOutput(ILayer* prev_layer);
			void CalculateDeltas(ILayer* prev_layer);
			
			std::string Properties() const;
	};
	class FullConnected : public ILayer {
		public:
			FullConnected(const std::string& name, Function function, Buffer input);
			FullConnected(const std::string& name, Function function, size_t inputLength);
			FullConnected(const std::string& name, Function function, ILayer* prevLayer);
			
			void CalculateOutput(ILayer* prev_layer);
			void CalculateDeltas(ILayer* prev_layer);
			void DoCorrections();
			
			std::string Properties() const;
	};
	// For testing
	class GreyScalling : public ILayer {
		public:
			GreyScalling(const std::string& name, Function function, Buffer input);
			GreyScalling(const std::string& name, Function function, size_t inputDepth, size_t inputHeight, size_t inputWidth);
			GreyScalling(const std::string& name, Function function, ILayer* prevLayer);
			
			void CalculateOutput(ILayer* prev_layer);
			void CalculateDeltas(ILayer* prev_layer);
			
			std::string Properties() const;			
	};
	class Simplifying : public ILayer {
		private:
			size_t kernel_width;
			size_t kernel_height;
		public:
			Simplifying(const std::string& name, Function function, size_t kernelHeight, size_t kernelWidth, Buffer input);
			Simplifying(const std::string& name, Function function, size_t kernelHeight, size_t kernelWidth, size_t inputDepth, size_t inputHeight, size_t inputWidth);
			Simplifying(const std::string& name, Function function, size_t kernelHeight, size_t kernelWidth, ILayer* prevLayer);
			
			void CalculateOutput(ILayer* prev_layer);
			void CalculateDeltas(ILayer* prev_layer);
			
			std::string Properties() const;			
	};
}

#endif