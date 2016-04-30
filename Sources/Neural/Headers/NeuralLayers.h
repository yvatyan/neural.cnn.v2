#ifndef NEURALLAYERS_H_
#define NEURALLAYERS_H_

#include <string>
#include <vector>

#include "NeuralFunctions.h"
#include "NeuralData.h"

// TODO: move common part of "Properties" func to ILayer, add boost json support
// TODO: layers io is fixed size, so need "Scaling" layer to bring various input to that size

namespace neural {
	class ILayer {
		public:
			enum Type {
				Input,
				Convolution,
				Pulling,
				FullConnected,
				GreyScalling,
				Simplifying,
				Output
			};
		protected:
			Buffer* output;
			Buffer* deltas;
			std::string layer_name;
			FunctionCollection f_collection;
			FunctionCollection::Name function_name;
			ILayer::Type layer_type;
		public:
			ILayer(const std::string& name, ILayer::Type layer, FunctionCollection::Name function);
			virtual ~ILayer() {};
			
			virtual void CalculateOutput(ILayer* prevLayer) { return; }
			virtual void CalculateDeltas(ILayer* prev_layer) { return; }
			virtual void DoCorrections() { return; }

			virtual const std::string Properties() const = 0;
			
			const std::string LayerFunctionName() const;
			const std::string LayerTypeName() const;
			const std::string& LayerName() const;
	};
	class Input : public ILayer {
		public:
			Input(const std::string& name, size_t z, size_t y, size_t x);
			~Input() {};	

			const std::string Properties() const ;
			void DataInput(const Buffer& input);
	};
	class Output : public ILayer {
		public:
			Output(const std::string& name, size_t x);
			Output(const std::string& name, size_t pse_h, size_t x);
			Output(const std::string& name, size_t pse_h, size_t pse_w, size_t x);

			void CalculateOutput(ILayer* prev_layer);
			const std::string Properties() const ;
			const Buffer& DataOutput() const ;
	};
	class Convolution : public ILayer {
		private:
			std::vector<Buffer> layer_kernels;
			int stride;
			int zero_padding;

			void autoZeroPadding();
		public:
			Convolution(const std::string& name, FunctionCollection::Name function, std::vector< std::pair<size_t, size_t> > kernels, size_t z, size_t y, size_t x, int stride = 0, int zeroPadding = -1); 
	
			void CalculateOutput(ILayer* prev_layer);
			void CalculateDeltas(ILayer* prev_layer);
			void DoCorrections();
			
			const std::string Properties() const;
	};
	class Pulling : public ILayer {
		private:
			size_t kernel_width;
			size_t kernel_height;
		public:
			Pulling(const std::string& name, FunctionCollection::Name function, size_t kernelHeight, size_t kernelWidth, size_t z, size_t y, size_t x);
			
			void CalculateOutput(ILayer* prev_layer);
			void CalculateDeltas(ILayer* prev_layer);
			
			const std::string Properties() const;
	};
	class FullConnected : public ILayer {
		public:
			FullConnected(const std::string& name, FunctionCollection::Name function, size_t x);
			FullConnected(const std::string& name, FunctionCollection::Name function, size_t pse_h, size_t x);
			FullConnected(const std::string& name, FunctionCollection::Name function, size_t pse_h, size_t pse_w, size_t x);
			
			void CalculateOutput(ILayer* prev_layer);
			void CalculateDeltas(ILayer* prev_layer);
			void DoCorrections();
			
			const std::string Properties() const;
	};
	class GreyScalling : public ILayer {
		public:
			GreyScalling(const std::string& name, FunctionCollection::Name function, size_t y, size_t x);
			
			void CalculateOutput(ILayer* prev_layer);
			void CalculateDeltas(ILayer* prev_layer);
			
			const std::string Properties() const;			
	};
	class Simplifying : public ILayer {
		private:
			size_t kernel_width;
			size_t kernel_height;
		public:
			Simplifying(const std::string& name, FunctionCollection::Name function, size_t kernelHeight, size_t kernelWidth, size_t z, size_t y, size_t x);
			
			void CalculateOutput(ILayer* prev_layer);
			void CalculateDeltas(ILayer* prev_layer);
			
			const std::string Properties() const;			
	};
}

#include "../NeuralLayers.cpp"

#endif
