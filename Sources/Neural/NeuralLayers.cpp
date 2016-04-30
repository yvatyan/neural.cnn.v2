#include <sstream>

#include "Headers/NeuralLayers.h"

using namespace neural;

ILayer::ILayer(const std::string& name, ILayer::Type layer, FunctionCollection::Name function)
	: layer_name(name)
	, f_collection()
	, function_name(function)
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
