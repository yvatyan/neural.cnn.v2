#include "NeuralLayers.cpp"

using namespace std;

ILayer::ILayer(const std::string& name, ILayer::Type layer, FunctionCollection::Name function)
	: layerName(name)
	, f_collection()
	, functionName(function)
	, layerType(layer)
{
}
const std::string ILayer::LayerFunctionName() const {
	return FunctionName::FName(functionName);
}
const std::string ILayer::LayerTypeName() const {
	switch (layerType) {
		case	Input		:	return "Input";
		case	Convolution	:	return "Convolution";
		case	Pulling		:	return "Pulling";
		case	FullConnected	:	return "FullConnected";
		case	GreyScalling	:	return "GreyPulling";
		case	Simplifying	:	return "Simplifying";
		case	Output		:	return "Output";
	};
}
