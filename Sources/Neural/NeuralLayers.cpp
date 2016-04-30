#include <sstream>

#include "Headers/NeuralLayers.h"

using namespace neural;

ILayer::ILayer(const std::string& name, ILayer::Type layer, FunctionCollection::Name function)
	: layer_name(name)
	, f_collection()
	, function_name(function)
	, layer_type(layer)
{
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

Input::Input(const std::string& name)
	: ILayer(name, ILayer::Input, FunctionCollection::None)
{
}
const std::string Input::Properties() const {
	std::stringstream props;

	props << "{\n";	
	props << '\t' << "\"layerName\": " << '\"' << LayerName() << '\"' << ",\n";
	props << '\t' << "\"layerType\": " << '\"' << LayerTypeName() << '\"' << ",\n";
	props << '\t' << "\"layerFunctions\": " << "{\n";
	props << "\t\t" << "\"activation\": " << "\"None\"" <<  ",\n";
	props << "\t\t" << "\"combination\": " << "\"None\"" <<  '\n';
	props << "\t}\n";
	props << "}\n";

	return props.str();
}
void Input::DataInput(const Buffer& input) {
}
