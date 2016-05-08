#include "Headers/NeuralLayers.h"

using namespace neural;

ILayer::ILayer(const std::string& name, ILayer::Type layer)
	: layer_name(name)
	, layer_type(layer)
{
	deltas = NULL;
	output = NULL;
    function = NULL;
}
ILayer::ILayer(const std::string& name, ILayer::Type layer, const Activation& func)
	: layer_name(name)
	, layer_type(layer)
{
    function = new Function_union;
    function->act = new Activation(func); 
	deltas = NULL;
	output = NULL;
}
ILayer::ILayer(const std::string& name, ILayer::Type layer, const Combination& func)
	: layer_name(name)
	, layer_type(layer)
{
    function = new Function_union;
    function->comb = new Combination(func); 
	deltas = NULL;
	output = NULL;
}
const std::string ILayer::LayerTypeName() const {
	switch (layer_type) {
		case	Input		:	return "Input";
		case	Convolution	:	return "Convolution";
		case	Pulling		:	return "Pulling";
		case	FullConnected	:	return "FullConnected";
		case	GreyScalling	:	return "GreyPulling";
		case	Simplifying	:	return "Simplifying";
		case	Output		:	return "Output";
	};
}
const std::string& ILayer::LayerName() const {
	return layer_name;
}

Input::Input(const std::string& name, size_t z, size_t y, size_t x)
	: ILayer(name, ILayer::Input)
{
	output = new Buffer(z, y, x, 0.);
}
const std::string Input::Properties() const {
    return "";
}
void Input::DataInput(const Buffer& input) {
	assert(input.Depth3D() == output->Depth3D());
	assert(input.Height3D() == output->Height3D());
	assert(input.Width3D() == output->Width3D());

	for(int i = 0; i < output->Depth3D(); ++i) {
		for(int j = 0; j < output->Height3D(); ++j) {
			for(int h = 0; h < output->Width3D(); ++h) {
				output->ElementTo(i, j, h, input.ElementAt(i, j, h));
			}
		}
	}
}

Output::Output(const std::string& name, size_t x) 
	: ILayer(name, ILayer::Output)
{
	output = new Buffer(x, 0.);
}
Output::Output(const std::string& name, size_t pse_w, size_t x)
	: ILayer(name, ILayer::Output)
{
	assert(x % pse_w == 0);
	output = new Buffer(x, 0.);
	output->SetPseudo2Dwidth(pse_w);
}
Output::Output(const std::string& name, size_t pse_h, size_t pse_w, size_t x) 
	: ILayer(name, ILayer::Output)
{
	assert(x % (pse_h * pse_w) == 0);
	output = new Buffer(x, 0.);
	output->SetPseudo3Dheight(pse_h);
	output->SetPseudo3Dwidth(pse_w);
}
const std::string Output::Properties() const {
	return "";
}
void Output::CalculateOutput(ILayer* prevLayer) {
	assert(static_cast<Output*>(prevLayer)->output->Size() == output->Size());
	for(int i = 0; i < output->Size(); ++i) {
		output->ElementTo(i, static_cast<Output*>(prevLayer)->output->ElementAt(i));
	}
}
const Buffer& Output::DataOutput() const {
	return *output;
}

double Convolution::getHeightZeroPadding(size_t index, size_t input_h) const {

	double zero_pad_h = (strides[index] * (output->Height3D() - 1) - input_h - layer_kernels[index]->Height3D()) / 2.;

	return zero_pad_h;
}
double Convolution::getWidthZeroPadding(size_t index, size_t input_w) const {

	double zero_pad_w = (strides[index] * (output->Width3D() - 1) - input_w - layer_kernels[index]->Width3D()) / 2.;

	return zero_pad_w;
}
bool Convolution::canConvertHeight(size_t input_data_height) const {

	for( int i = 0; i < layer_kernels.size(); ++i) {
		double zph = getHeightZeroPadding(i, input_data_height);
		if( zph != (int) zph) return false;
	}
	return true;
}
bool Convolution::canConvertWidth(size_t input_data_width) const {

	for( int i = 0; i < layer_kernels.size(); ++i) {
		double zpw = getWidthZeroPadding(i, input_data_width);
		if( zpw != (int) zpw) return false;
	}
	return true;
}
void Convolution::generateKernel(size_t index, size_t z, size_t y, size_t x) {
	layer_kernels[index] = new Buffer(z, y, x, 0.);
	double initial;
	for(int i = 0; i < z; ++i) {
		for(int j = 0; j < y; ++j) {
			for(int h = 0; h < x; ++h) {
				initial = 0.; // TODO: initialize with random values
				layer_kernels[index]->ElementTo(i, j, h, initial);
			}
		}
	}
}
Convolution::Convolution(const std::string& name, const Activation& func, std::vector< boost::tuple< size_t, size_t, int > > kernels, size_t input_depth, size_t y, size_t x) 
	: ILayer(name, ILayer::Convolution, func)
	, layer_kernels(kernels.size())
	, strides(kernels.size())
{
	for(int i = 0; i < kernels.size(); ++i) {
		generateKernel(i, input_depth, kernels[i].get<0>(), kernels[i].get<1>());
		strides[i] = kernels[i].get<2>();
	}
	output = new Buffer(kernels.size(), y, x, 0.);
}
void Convolution::CalculateOutput(ILayer* prev_layer) {
	assert(canConvertHeight(static_cast<Convolution*>(prev_layer)->output->Height3D()));
	assert(canConvertWidth(static_cast<Convolution*>(prev_layer)->output->Width3D()));
	assert(layer_kernels[0]->Depth3D() == static_cast<Convolution*>(prev_layer)->output->Depth3D());

	for(int k = 0; k < layer_kernels.size(); ++k) { // kernels.size == output.depth
		int zph = (int)getHeightZeroPadding(k, static_cast<Convolution*>(prev_layer)->output->Height3D());
    	int zpw = (int)getWidthZeroPadding(k, static_cast<Convolution*>(prev_layer)->output->Width3D());
        for(int i = 0; i < output->Height3D(); ++i) {
			for(int j = 0 ; j < output->Width3D(); ++j) {
				double value = 0.;
				for(int depth = 0; depth < layer_kernels[k]->Depth3D(); ++depth) {
					for(int height = 0; height < layer_kernels[k]->Height3D(); ++height) {
						for(int width = 0; width < layer_kernels[k]->Width3D(); ++width) {

							int alpha = j * strides[k] + width - zpw; 
							int beta = i * strides[k] + height - zph;
							     if(alpha < 0 || beta < 0) continue;
							else if(alpha > static_cast<Convolution*>(prev_layer)->output->Height3D() || beta > static_cast<Convolution*>(prev_layer)->output->Width3D()) continue;

							value += static_cast<Convolution*>(prev_layer)->function->act->operator()(static_cast<Convolution*>(prev_layer)->output->ElementAt(depth, alpha, beta)) * layer_kernels[k]->ElementAt(depth, height, width);
						}
					}
				}
				output->ElementTo(k, i, j, value);
			}
		}
	}

}
void Convolution::CalculateDeltas(ILayer* prev_layer) {
	assert(canConvertHeight(static_cast<Convolution*>(prev_layer)->deltas->Height3D()));
	assert(canConvertWidth(static_cast<Convolution*>(prev_layer)->deltas->Width3D()));
	assert(layer_kernels[0]->Depth3D() == static_cast<Convolution*>(prev_layer)->deltas->Depth3D());

	for(int i = 0; i < deltas->Height3D(); ++i) {
       	for(int j = 0; j < deltas->Width3D(); ++j) {
			for(int k = 0; layer_kernels.size(); ++k) {
		        int zph = (int)getHeightZeroPadding(k, static_cast<Convolution*>(prev_layer)->output->Height3D());
    	        int zpw = (int)getWidthZeroPadding(k, static_cast<Convolution*>(prev_layer)->output->Width3D());
		        for(int depth = 0; depth < layer_kernels[k]->Depth3D(); ++depth) {
					for(int height = 0; height < layer_kernels[k]->Height3D(); ++height) {
						for(int width = 0; width < layer_kernels[k]->Width3D(); ++width) {

                            int alpha = j * strides[k] + width - zpw;
                            int beta = i * strides[k] + height - zph;
                                 if(alpha < 0 || beta < 0) continue;
							else if(alpha > static_cast<Convolution*>(prev_layer)->output->Height3D() || beta > static_cast<Convolution*>(prev_layer)->output->Width3D()) continue;

							static_cast<Convolution*>(prev_layer)->deltas->ElementTo(depth, alpha, beta, static_cast<Convolution*>(prev_layer)->deltas->ElementAt(depth, alpha, beta) + layer_kernels[k]->ElementAt(depth, height,width) * deltas->ElementAt(k, i, j));
						}
					}
				}
			}
		}
	}
}
void Convolution::DoCorrections(ILayer* prev_layer, double ffactor) {

	assert(canConvertHeight(static_cast<Convolution*>(prev_layer)->output->Height3D()));
	assert(canConvertWidth(static_cast<Convolution*>(prev_layer)->output->Width3D()));
	assert(layer_kernels[0]->Depth3D() == static_cast<Convolution*>(prev_layer)->output->Depth3D());

    for(int k = 0; k < layer_kernels.size(); ++k) {
        int zph = (int)getHeightZeroPadding(k, static_cast<Convolution*>(prev_layer)->output->Height3D());
    	int zpw = (int)getWidthZeroPadding(k, static_cast<Convolution*>(prev_layer)->output->Width3D());
        for(int i = 0; i < output->Height3D(); ++i) {
            for(int j = 0; j < output->Width3D(); ++j) {
                for(int depth = 0; depth < layer_kernels[k]->Depth3D(); ++depth) {
                    for(int height = 0; height < layer_kernels[k]->Height3D(); ++height) {
                        for(int width = 0; width < layer_kernels[k]->Width3D(); ++width) {

                            int alpha = j * strides[k] + width - zpw;
                            int beta = i * strides[k] + height - zph;
                                 if(alpha < 0 || beta < 0) continue;
							else if(alpha > static_cast<Convolution*>(prev_layer)->output->Height3D() || beta > static_cast<Convolution*>(prev_layer)->output->Width3D()) continue;

                            layer_kernels[k]->ElementTo(depth, height, width, layer_kernels[k]->ElementAt(depth, height, width) + ffactor * static_cast<Convolution*>(prev_layer)->output->ElementAt(depth, alpha, beta) * deltas->ElementAt(k, i, j) * function->act->operator[](output->ElementAt(k, i, j)));
                        }
                    }
                }
            }
        }
    }
}
const std::string Convolution::Properties() const {
    return "";
}
