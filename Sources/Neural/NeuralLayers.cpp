#include <sstream>

#include "NeuralLayers.cpp"

using namespace std;

ILayer::ILayer(const std::string& name, ILayer::Type layer, FunctionCollection::Name function)
	: layer_name(name)
	, f_collection()
	, function_name(function)
	, layer_type(layer)
{
}
const std::string ILayer::LayerFunctionName() const {
	return FunctionName::FName(function_name);
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
const std::string& LayerName() const {
	return layer_name;
}

Input::Input(const std::string& name)
	: ILayer(name, ILayer::Input, FunctionCollection::None)
{
}
const std::string Properties() const {
	std::stringstream props;

	
}
