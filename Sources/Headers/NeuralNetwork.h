#ifndef NEURALNETWORK_H_
#define NEURALNETWORK_H_

#include <list>
#include <string>

#include "NeuralData.h"
#include "NeuralLayers.h"

namespace neural {
	class NeuralNetwork {
		private:
			std::list<Layer*> networkLayers;
			
			bool checkLayerConfiguration(const std::string& layerConf) const;
		public:
			NeuralNetwork(const std::string& confFilePath);
			NeuralNetwork(int input_x, int input_y);
			NeuralNetwork(const NeuralNetwork& copy);
			~NeuralNetwork();
			
			void Study(const Buffer& data, const Buffer& teacher);
			Buffer Recognize(const Buffer& data) const;
			
			void InsertLayer(const std::string& layerConfiguration);
			void RemoveLayer(const std::string& layerName);
			void ReplaceLayer(const std::string& layerConfiguration);
			void LoadNetwork(const std::string& path);
			void LoadNetworkConfiguration(const std::string& path);
			void SaveNetwork(const std::string& path) const;
			void SaveNetworkConfiguration(const std::string& path) const;
	};
}

#endif