#include <sstream>

#include "Headers/NeuralLayers.h"

using namespace neural;

ILayer::ILayer(const std::string& name, ILayer::Type layer, Function* func)
	: layer_name(name)
	, function(*func)
	, layer_type(layer)
{
	deltas = NULL;
	output = NULL;
}
const std::string ILayer::LayerFunctionName() const {
	return FunctionCollection::FunctionName(function_name);
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
	: ILayer(name, ILayer::Input, FunctionCollection::None)
{
	output = new Buffer(z, y, x, 0.);
}
const std::string Input::Properties() const {

	std::stringstream props;
	props << "{\n";	
	props << '\t' << "\"layerName\": " << '\"' << LayerName() << '\"' << ",\n";
	props << '\t' << "\"layerType\": " << '\"' << LayerTypeName() << '\"' << ",\n";
	props << '\t' << "\"layerFunctions\": " << "{\n";
	props << "\t\t" << "\"activation\": " << "\"None\"" << ",\n";
	props << "\t\t" << "\"combination\": " << "\"None\"" << '\n';
	props << "\t},\n";
	props << "\t\"extended\": [\n";
	props << "\t]\n";
	props << "}\n";

	return props.str();
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
	: ILayer(name, ILayer::Output, FunctionCollection::None)
{
	output = new Buffer(x, 0.);
}
Output::Output(const std::string& name, size_t pse_w, size_t x)
	: ILayer(name, ILayer::Output, FunctionCollection::None)
{
	assert(x % pse_w == 0);
	output = new Buffer(x, 0.);
	output->SetPseudo2Dwidth(pse_w);
}
Output::Output(const std::string& name, size_t pse_h, size_t pse_w, size_t x) 
	: ILayer(name, ILayer::Output, FunctionCollection::None)
{
	assert(x % (pse_h * pse_w) == 0);
	output = new Buffer(x, 0.);
	output->SetPseudo3Dheight(pse_h);
	output->SetPseudo3Dwidth(pse_w);
}
const std::string Output::Properties() const {

	std::stringstream props;
	props << "{\n";	
	props << '\t' << "\"layerName\": " << '\"' << LayerName() << '\"' << ",\n";
	props << '\t' << "\"layerType\": " << '\"' << LayerTypeName() << '\"' << ",\n";
	props << '\t' << "\"layerFunctions\": " << "{\n";
	props << "\t\t" << "\"activation\": " << "\"None\"" << ",\n";
	props << "\t\t" << "\"combination\": " << "\"None\"" << '\n';
	props << "\t},\n";
	props << "\t\"extended\": [\n";
	props << "\t]\n";
	props << "}\n";

	return props.str();
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

double Convolution::getHeightZeroPadding(size_t index, size_t input_d, size_t input_h, size_t output_h) {

	assert(input_d == kernels[index].Depth3D());
	double zero_pad_h = (strides[index] * (output->Height3D() - 1) - input_h - kernels[index].Height3D()) / 2.;

	return zero_pad_h;
}
double Convolution::getWidthZeroPadding(size_t index, size_t input_d, size_t input_w, size_t output_w) {

	assert(input_d == kernels[index].Depth3D());
	double zero_pad_w = (strides[index] * (output->Width3D() - 1) - input_w - kernels[index].Width3D()) / 2.;

	return zero_pad_w;
}
bool canConvertToOutputHeight(size_t input_data_height) const {

	for( int i = 0; i < layer_kernels.size(); ++i) {
		double zph = getHeightZeroPadding(i, input_data_height);
		if( zph != (int) zph) return false;
	}
	return true;
}
bool canConvertToOutputWidth(size_t input_data_width) const {

	for( int i = 0; i < layer_kernels.size(); ++i) {
		double zpw = getWidthZeroPadding(i, input_data_width);
		if( zpw != (int) zpw) return false;
	}
	return true;
}
void Convolution::generateKernel(size_t index, size_t z, size_t y, size_t x) {
	layer_kernels[i] = new Buffer(z, y, x, 0.);
	double initial;
	for(int i = 0; i < z; ++i) {
		for(int j = 0; j < y; ++j) {
			for(int h = 0; h < x; ++h) {
				initial = 0.;
				layer_kernels[i] = initial;
			}
		}
	}
}
Convolution::Convolution(const std::string& name, const Activation func, size_t input_depth, size_t y, size_t x, std::vector< boost::tuple< size_t, size_t, int > kernels) 
	: ILayer(name, ILayer::Convolution, &function)
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
	assert(canConvertOutputHeight(static_cast<Convolution*>(prevLayer)->output->Height3D()));
	assert(canConvertOutputWidth(static_cast<Convolution*>(prevLayer)->output->Width3D()));
	assert(layer_kernels[0]->Depth3D() == static_cast<Convolution*>(prevLayer)->output->Depth3D());

	int zph = getHeightZeroPadding();
	int zpw = getWidthZeroPadding();
	for(int k = 0; k < layer_kernels->size(); ++k) { // kernels.size == output.depth
		for(int i = 0; i < output->Height3D(); ++i) {
			for(int j = 0 ; j < output->Width3D(); ++j) {
				double value = 0.;
				for(int depth = 0; depth < layer_kernels[k]->Depth3D(), ++depth) {
					for(int height = 0; height < layer_kernels[k]->Height3D(); ++height) {
						for(int width = 0; width < layer_kernels[k]->Width3D(); ++width) {

							int alpha = j * strides[k] + width - zpw; 
							int beta = i * strides[k] + height - zph;
							     if(alpha < 0 || beta < 0) continue;
							else if(alpha > static_cast<Convolution*>(prevLayer)->output->Height3D() || beta > static_cast<Convolution*>(prevLayer)->output->Width3D()) continue;
							value += static_cast<Convolution*>(prevLayer)->output->ElementAt(depth, alpha, beta) * layer_kernels->ElementAt(depth, height, width);
						}
					}
				}
				output.ElementTo(k, i, j, value);
			}
		}
	}

}
void Convolution::CalculateDeltas(ILayer* prev_layer) {
	assert(canConvertDeltasHeight(static_cast<Convolution*>(prevLayer)->deltas->Height3D()));
	assert(canConvertDeltasWidth(static_cast<Convolution*>(prevLayer)->deltas->Width3D()));
	assert(layer_kernels[0]->Depth3D() == static_cast<Convolution*>(prevLayer)->deltas->Depth3D());

	for(int i = 0; i < static_cast<Convolution*>(prev_layer)->Height3D(); ++i) {
		for(int j = 0; j < static_cast<Convolution*>(prev_layer)->Width3D(); ++j) {
			for(int k = 0; layer_kernels->size(); ++k) {
				for(int depth = 0; depth < layer_kernels[k]->Depth3D(); ++depth) {
					for(int height = 0; height < layer_kernels[k]->Height3D(); ++height) {
						for(int width = 0; width < layers_kernels[k]->Widtrh3D(); ++width) {
							deltas.ElementTo(depth, alpha, beta, deltas.ElementAt(depth, alpha, beta) + layers_kernels[k].ElementAt(depth, height,width) * static_cast<Convolution*>(prev_layer)->deltas.ElementAt(k, i, j));
						}
					}
				}
			}
		}
	}

}
void Convilution::DoCorrection() {
	
}
