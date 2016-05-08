#ifndef NEURALLAYERS_H_
#define NEURALLAYERS_H_

#include <string>

#include <vector>
#include <boost/tuple/tuple.hpp>

#include "NeuralFunctions.h"
#include "NeuralData.h"

// TODO: write "Properties" func with boost json support
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
            union Function_union {
                Activation* act;
                Combination* comb;
            } *function;
			ILayer::Type layer_type;
		public:
			ILayer(const std::string& name, ILayer::Type layer);
			ILayer(const std::string& name, ILayer::Type layer, const Activation& func);
			ILayer(const std::string& name, ILayer::Type layer, const Combination& func);
			virtual ~ILayer() {};
			
			virtual void CalculateOutput(ILayer* prevLayer) { return; }
			virtual void CalculateDeltas(ILayer* prev_layer) { return; }
			virtual void DoCorrections(ILayer* prev_layer, double ffactor) { return; }

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
			Output(const std::string& name, size_t pse_w, size_t x);
			Output(const std::string& name, size_t pse_h, size_t pse_w, size_t x);

			void CalculateOutput(ILayer* prev_layer);
			const std::string Properties() const ;
			const Buffer& DataOutput() const ;
	};
	class Convolution : public ILayer {
		private:
			std::vector<Buffer*> layer_kernels;
			std::vector<int> strides;

			void generateKernel(size_t index, size_t z, size_t y, size_t x); 
			double getHeightZeroPadding(size_t kernel_index, size_t input_h) const;
			double getWidthZeroPadding(size_t kernel_index, size_t input_w) const;
			bool canConvertHeight(size_t input_data_height) const;
			bool canConvertWidth(size_t input_data_width) const;
		public:
			Convolution(const std::string& name, const Activation& func, std::vector< boost::tuple< size_t, size_t, int > > kernels, size_t input_depth, size_t y, size_t x); // kernel = { kernel_h, kernel_w, stride } 
	
			void CalculateOutput(ILayer* prev_layer);
			void CalculateDeltas(ILayer* prev_layer);
			void DoCorrections(ILayer* prev_layer, double ffactor);
			
			const std::string Properties() const;
	};
	class Pulling : public ILayer {
		private:
			size_t kernel_width;
			size_t kernel_height;
		public:
			Pulling(const std::string& name, const Combination& func, size_t kernelHeight, size_t kernelWidth, size_t z, size_t y, size_t x);
			
			void CalculateOutput(ILayer* prev_layer);
			void CalculateDeltas(ILayer* prev_layer);
			
			const std::string Properties() const;
	};
	class FullConnected : public ILayer {
		public:
			FullConnected(const std::string& name, const Activation& func, size_t x);
			FullConnected(const std::string& name, const Activation& func, size_t pse_w, size_t x);
			FullConnected(const std::string& name, const Activation& func, size_t pse_h, size_t pse_w, size_t x);
			
			void CalculateOutput(ILayer* prev_layer);
			void CalculateDeltas(ILayer* prev_layer);
			void DoCorrections(ILayer* prev_layer, double ffactor);
			
			const std::string Properties() const;
	};
	class Simplifying : public ILayer {
		private:
			size_t kernel_width;
			size_t kernel_height;
		public:
			Simplifying(const std::string& name, const Combination& func, size_t kernelHeight, size_t kernelWidth, size_t z, size_t y, size_t x);
			
			void CalculateOutput(ILayer* prev_layer);
			void CalculateDeltas(ILayer* prev_layer);
			
			const std::string Properties() const;			
	};
}

#include "../NeuralLayers.cpp"

#endif
