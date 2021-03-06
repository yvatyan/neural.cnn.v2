#ifndef NEURALLAYERS_H_
#define NEURALLAYERS_H_

#include <string>
#include <iostream>
#include <vector>
//#include <boost/tuple/tuple.hpp>

#include "NeuralFunctions.h"
#include "NeuralData.h"

// TODO: write "Properties" func with boost json support
// TODO: layers io is fixed size, so need "Scaling" layer to bring various input to that size

namespace neural {

//	class Convolution;
//    void ReadKernels(std::istream& fileIn, Convolution& l);

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
            struct Function_struct {
                Activation* act;
                Combination* comb;

                ~Function_struct() {
                    if(act != NULL) delete act;
                    if(comb != NULL) delete comb;
                }

            } *function;
			ILayer::Type layer_type;
		public:
			ILayer(const std::string& name, ILayer::Type layer);
			ILayer(const std::string& name, ILayer::Type layer, const Activation& func);
			ILayer(const std::string& name, ILayer::Type layer, const Combination& func);
			virtual ~ILayer();
			
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
			void DeltaInput(const Buffer& input);

			void CalculateDeltas(ILayer* prev_layer);
	};
	class Output : public ILayer {
        private:
            Buffer* training;
            double loss;
            Buffer* weights;
		public:
			Output(const std::string& name, size_t x);
			Output(const std::string& name, size_t pse_h, size_t x);
			Output(const std::string& name, size_t pse_h, size_t pse_w, size_t x);
            ~Output() {};

			void CalculateOutput(ILayer* prev_layer);
            void CalculateDeltas(ILayer* prev_layer);
            void DoCorrections(ILayer* prev_layer, double ffactor);

			const std::string Properties() const ;
			const Buffer& DataOutput() const ;
	};/*
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
	        ~Convolution();

			void CalculateOutput(ILayer* prev_layer);
			void CalculateDeltas(ILayer* prev_layer);
			void DoCorrections(ILayer* prev_layer, double ffactor);
			
			const std::string Properties() const;

            friend void ReadKernels(std::istream& fileIn, ILayer* layer);
	};*/
	class Pulling : public ILayer {
		private:
			size_t kernel_height;
			size_t kernel_width;
            bool canConvertHeight(size_t input_data_height) const ;
            bool canConvertWidth(size_t input_data_width) const ;
		public:
			Pulling(const std::string& name, const Combination& func, size_t kernelHeight, size_t kernelWidth, size_t z, size_t y, size_t x);
			~Pulling() {};

			void CalculateOutput(ILayer* prev_layer);
			void CalculateDeltas(ILayer* prev_layer);
			
			const std::string Properties() const;
	};
	class FullConnected : public ILayer {
		private:
            Buffer* weights;
		public:
			FullConnected(const std::string& name, const Activation& func, size_t input, size_t output);
			~FullConnected();

			void CalculateOutput(ILayer* prev_layer);
			void CalculateDeltas(ILayer* prev_layer);
			void DoCorrections(ILayer* prev_layer, double ffactor);
			
			const std::string Properties() const;
	};
	class Simplifying : public ILayer {
		private:
			size_t kernel_height;
			size_t kernel_width;
		public:
			Simplifying(const std::string& name, const Combination& func, size_t kernelHeight, size_t kernelWidth, size_t z, size_t y, size_t x);
			~Simplifying() {};

			void CalculateOutput(ILayer* prev_layer);
			void CalculateDeltas(ILayer* prev_layer);
			
			const std::string Properties() const;			
	};
/*

    void ReadKernels(std::istream& fileIn, ILayer* layer) {

        Convolution* l = static_cast<Convolution*>(layer);

        for(int i = 0; i < l->strides.size(); ++i) {
            fileIn >> l->strides[i];
        }
        for(int k = 0; k < l->layer_kernels.size(); ++k) {
            for(int d = 0; d < l->layer_kernels[k]->Depth3D(); ++d) {
                for(int h = 0; h < l->layer_kernels[k]->Height3D(); ++h) {
                    for(int w = 0; w < l->layer_kernels[k]->Width3D(); ++w) {
                        double value;
                        fileIn >> value;
                        l->layer_kernels[k]->ElementTo(d, h, w, value);
                    }
                }
            }
        }
    }*/
}
#include "../NeuralLayers.cpp"

#endif
