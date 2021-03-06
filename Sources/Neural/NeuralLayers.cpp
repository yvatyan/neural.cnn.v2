#include "Headers/NeuralLayers.h"

using namespace neural;

#include <fstream>
#include <iostream>

std::ofstream fileOut("report.log");

ILayer::ILayer(const std::string& name, ILayer::Type layer)
	: layer_name(name)
	, layer_type(layer)
{
	deltas = NULL;
	output = NULL;
    function = new Function_struct;
    function->act = new Activation(Activation::Identity);
    function->comb = NULL;
}
ILayer::ILayer(const std::string& name, ILayer::Type layer, const Activation& func)
	: layer_name(name)
	, layer_type(layer)
{
    function = new Function_struct;
    function->act = new Activation(func); 
    function->comb = NULL;
	deltas = NULL;
	output = NULL;
}
ILayer::ILayer(const std::string& name, ILayer::Type layer, const Combination& func)
	: layer_name(name)
	, layer_type(layer)
{
    function = new Function_struct;
    function->comb = new Combination(func); 
    function->act = new Activation(Activation::Identity); 
	deltas = NULL;
	output = NULL;
}
ILayer::~ILayer() {
    delete output;
    delete deltas;
    delete function;
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
	deltas = new Buffer(z, y, x, 0.);
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
				//fileOut << input.ElementAt(i, j, h) << ' ';
			}
			//fileOut << std::endl;
		}
		//fileOut << "______________________\n";
	}
}
void Input::DeltaInput(const Buffer& input) {
	assert(input.Depth3D() == deltas->Depth3D());
	assert(input.Height3D() == deltas->Height3D());
	assert(input.Width3D() == deltas->Width3D());

	fileOut << std::endl << std::endl << "DELTAS\' Input\n";
	for(int i = 0; i < deltas->Depth3D(); ++i) {
		for(int j = 0; j < deltas->Height3D(); ++j) {
			for(int h = 0; h < deltas->Width3D(); ++h) {
				deltas->ElementTo(i, j, h, input.ElementAt(i, j, h));
				fileOut << deltas->ElementAt(i, j, h) << ' ';
			}
			fileOut << std::endl;
		}
		fileOut << "______________________\n";
	}
}
void Input::CalculateDeltas(ILayer* prev_layer) {
    
    assert(static_cast<Input*>(prev_layer)->deltas->Size() == deltas->Size());
    Input* deltas_vector = static_cast<Input*>(prev_layer);
    for(int i = 0; i < deltas->Size(); ++i) deltas_vector->deltas->ElementTo(i, deltas->ElementAt(i));
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
void Output::CalculateOutput(ILayer* prev_layer) {
    static int skip = 0;	
	assert(static_cast<Output*>(prev_layer)->output->Size() == weights->Height2D());
	
	fileOut << std::endl << std::endl << "Output : Output calculation\n";
    Output* input_vector = static_cast<Output*>(prev_layer);
    for(int h = 0; h < weights->Width2D(); ++h) {
        double value = 0;
    	for(int i = 0; i < input_vector->output->Size(); ++i) {
            value += input_vector->function->act->operator()(input_vector->output->ElementAt(i)) * weights->ElementAt(i, h);
	    //if(skip == 9) std::cout << value << " -- " << input_vector->function->act->operator()(input_vector->output->ElementAt(i)) * weights->ElementAt(i, h) << std::endl;
	    }
	   //if(skip == 9) system("pause");
	    output->ElementTo(h, value); 
		fileOut << h << ": " << value << "    Activated -> " << function->act->operator()(value) << std::endl;
    }
    skip++;
}
void Output::CalculateDeltas(ILayer* prev_layer) {
    
    loss = 0;
    double e_sum = 0; 
    for(int i = 0; i < output->Size(); ++i) {
        e_sum += std::pow(M_E, function->act->operator()(output->ElementAt(i)));
    }
    Activation softmax(Activation::SoftMax, e_sum);
    for(int i = 0; i < output->Size(); ++i) {
        loss += std::pow(training->ElementAt(i) - softmax(function->act->operator()(output->ElementAt(i))), 2);
    }
    loss *= 0.5;
	//fileOut << std::endl << std::endl << "full connected : Deltas calculation\n";
    Output* deltas_vector = static_cast<FullConnected*>(prev_layer);
    for(int h = 0; h < weights->Height2D(); ++h) {
        double value = 0;
        for(int i = 0; i < deltas->Size(); ++i) {
            value += deltas->ElementAt(i) * weights->ElementAt(h, i);
        }
        deltas_vector->deltas->ElementTo(h, value);
		//fileOut << h << ": " << value << std::endl;
    }
}
void FullConnected::DoCorrections(ILayer* prev_layer, double ffactor) {
    
    assert(static_cast<FullConnected*>(prev_layer)->output->Size() == weights->Height2D());
    //fileOut << std::endl << std::endl << "full connected : Correction calculation\n";
	fileOut << "WEIGHTS:" << std::endl;
    for(int i = 0; i < weights->Height2D(); ++i) {
        for(int j = 0; j < weights->Width2D(); ++j) {
            // w[i, j] += in[i] * ffactor * f'(o[j]) * d[j];
            double value = static_cast<FullConnected*>(prev_layer)->output->ElementAt(i) * ffactor * function->act->operator[](output->ElementAt(j)) * deltas->ElementAt(j);
            weights->ElementTo(i, j, weights->ElementAt(i, j) + value);
			fileOut << weights->ElementAt(i, j) << ' ';
			//fileOut << value << " = " << static_cast<FullConnected*>(prev_layer)->output->ElementAt(i) <<
				//" * (" << output->ElementAt(j) << " --> " << function->act->operator[](output->ElementAt(j)) << ") * " << deltas->ElementAt(j) << std::endl;
        }
		fileOut << std::endl;
    }
}
const Buffer& Output::DataOutput() const {
	return *output;
}
/*
double Convolution::getHeightZeroPadding(size_t index, size_t input_h) const {

	double zero_pad_h = (strides[index] * (output->Height3D() - 1) - input_h + layer_kernels[index]->Height3D()) / 2.;
fileOut << "Height: " << strides[index] << ' ' << output->Height3D() << ' ' << input_h << ' ' << layer_kernels[index]->Height3D() << std::endl;
	return zero_pad_h;
}
double Convolution::getWidthZeroPadding(size_t index, size_t input_w) const {

	double zero_pad_w = (strides[index] * (output->Width3D() - 1) - input_w + layer_kernels[index]->Width3D()) / 2.;
fileOut << "Width: " << strides[index] << ' ' << output->Width3D() << ' ' << input_w << ' ' << layer_kernels[index]->Width3D() << std::endl;
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
	deltas = new Buffer(kernels.size(), y, x, 0.);
}
Convolution::~Convolution() {
    for(int i = layer_kernels.size() - 1; i >= 0; --i) delete layer_kernels[i];
}
void Convolution::CalculateOutput(ILayer* prev_layer) {
	assert(canConvertHeight(static_cast<Convolution*>(prev_layer)->output->Height3D()));
	assert(canConvertWidth(static_cast<Convolution*>(prev_layer)->output->Width3D()));
	assert(layer_kernels[0]->Depth3D() == static_cast<Convolution*>(prev_layer)->output->Depth3D());

	for(int k = 0; k < layer_kernels.size(); ++k) { // kernels.size == output.depth
		int zph = (int)getHeightZeroPadding(k, static_cast<Convolution*>(prev_layer)->output->Height3D());
    	int zpw = (int)getWidthZeroPadding(k, static_cast<Convolution*>(prev_layer)->output->Width3D());
       fileOut << "zph & zpw: " << zph << ' ' << zpw << std::endl;
        for(int i = 0; i < output->Height3D(); ++i) {
			for(int j = 0 ; j < output->Width3D(); ++j) {
				double value = 0.;
				for(int depth = 0; depth < layer_kernels[k]->Depth3D(); ++depth) {
					for(int height = 0; height < layer_kernels[k]->Height3D(); ++height) {
						for(int width = 0; width < layer_kernels[k]->Width3D(); ++width) {

							int alpha = i * strides[k] + height - zph; 
							int beta = j * strides[k] + width - zpw;
                            fileOut << "i & j           : " << i << ' ' << j << std::endl;
                            fileOut << "|-height & width: " << height << ' ' << width << std::endl;
                            fileOut << "  |-alpha & beta: " << alpha << ' ' << beta << std::endl;
							     if(alpha < 0 || beta < 0) continue;
							else if(alpha >= static_cast<Convolution*>(prev_layer)->output->Height3D() || beta >= static_cast<Convolution*>(prev_layer)->output->Width3D()) continue;
                            
//                            fileOut << "Prev layer output: " << static_cast<Convolution*>(prev_layer)->output->ElementAt(depth, alpha, beta) << std::endl;
  //                          fileOut << "Kernel " << k << " element at " << depth << ' ' << height << ' ' << width << " is ";
                            //fileOut << layer_kernels[k]->ElementAt(depth, height, width) << std::endl;

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

                            int alpha = i * strides[k] + height - zph;
                            int beta = j * strides[k] + width - zpw;
                                 if(alpha < 0 || beta < 0) continue;
							else if(alpha >= static_cast<Convolution*>(prev_layer)->output->Height3D() || beta >= static_cast<Convolution*>(prev_layer)->output->Width3D()) continue;

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

                            int alpha = i * strides[k] + height - zph;
                            int beta = j * strides[k] + width - zpw;
                                 if(alpha < 0 || beta < 0) continue;
							else if(alpha >= static_cast<Convolution*>(prev_layer)->output->Height3D() || beta >= static_cast<Convolution*>(prev_layer)->output->Width3D()) continue;

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
*/
bool Pulling::canConvertHeight(size_t input_data_height) const {
    return input_data_height % kernel_height == 0; 
}
bool Pulling::canConvertWidth(size_t input_data_width) const {
    return input_data_width % kernel_width == 0;
}
Pulling::Pulling(const std::string& name, const Combination& func, size_t kernelHeight, size_t kernelWidth, size_t z, size_t y, size_t x)
    : ILayer(name, ILayer::Pulling, func) 
{
    kernel_height = kernelHeight;
    kernel_width = kernelWidth;

    output = new Buffer(z, y, x, 0.);
    deltas = new Buffer(z, y, x, 0.);
}
void Pulling::CalculateOutput(ILayer* prev_layer) {
	assert(canConvertHeight(static_cast<Pulling*>(prev_layer)->output->Height3D()));
	assert(canConvertWidth(static_cast<Pulling*>(prev_layer)->output->Width3D()));
	assert(output->Depth3D() == static_cast<Pulling*>(prev_layer)->output->Depth3D());


    for(int d = 0; d < output->Depth3D(); ++d) {
        for(int h = 0; h < output->Height3D(); ++h) {
            for(int w = 0; w < output->Width3D(); ++w) {
                for(int i = 0; i < kernel_height; ++i) {
                    for(int j = 0; j < kernel_width; ++j) {

                        int alpha = h * kernel_height + i; 
						int beta = w * kernel_width + j;
						assert(alpha >= 0 && beta >= 0);
						assert(alpha < static_cast<Pulling*>(prev_layer)->output->Height3D() && beta < static_cast<Pulling*>(prev_layer)->output->Width3D());

						function->comb->operator+( static_cast<Pulling*>(prev_layer)->output->ElementAt(d, alpha, beta) );
                    }
                }
                output->ElementTo(d, h, w, function->comb->operator()());
                function->comb->Clear();
            }
        }
    }
}
void Pulling::CalculateDeltas(ILayer* prev_layer) {
	assert(canConvertHeight(static_cast<Pulling*>(prev_layer)->deltas->Height3D()));
	assert(canConvertWidth(static_cast<Pulling*>(prev_layer)->deltas->Width3D()));
	assert(deltas->Depth3D() == static_cast<Pulling*>(prev_layer)->deltas->Depth3D());


    for(int d = 0; d < deltas->Depth3D(); ++d) {
        for(int h = 0; h < deltas->Height3D(); ++h) {
            for(int w = 0; w < deltas->Width3D(); ++w) {
                for(int i = 0; i < kernel_height; ++i) {
                    for(int j = 0; j < kernel_width; ++j) {

                        int alpha = h * kernel_height + i; 
						int beta = w * kernel_width + j;
						assert(alpha >= 0 && beta >= 0);
						assert(alpha < static_cast<Pulling*>(prev_layer)->deltas->Height3D() && beta < static_cast<Pulling*>(prev_layer)->deltas->Width3D());
                        
                        static_cast<Pulling*>(prev_layer)->deltas->ElementTo(alpha, beta, d, deltas->ElementAt(d, h, w));
                    }
                }
            }
        }
    }
}
const std::string Pulling::Properties() const {
    return "";
}

FullConnected::FullConnected(const std::string& name, const Activation& func, size_t output_, size_t input) 
    : ILayer(name, ILayer::FullConnected, func)  
{
   output = new Buffer(output_, 0.); 
   deltas = new Buffer(output_, 0.);
   weights = new Buffer(input, output_, 0.);
   for(int i = 0; i < weights->Height2D(); ++i) {
		for(int j = 0; j < weights->Width2D(); ++j) {
			double value = (rand()%1000) / 1000.;
			weights->ElementTo(i, j, value);
		}
   }
}
FullConnected::~FullConnected() {
    delete weights;
}
void FullConnected::CalculateOutput(ILayer* prev_layer) {
static int skip = 0;	
	assert(static_cast<FullConnected*>(prev_layer)->output->Size() == weights->Height2D());
	
	fileOut << std::endl << std::endl << "full connected : Output calculation\n";
    FullConnected* input_vector = static_cast<FullConnected*>(prev_layer);
    for(int h = 0; h < weights->Width2D(); ++h) {
        double value = 0;
    	for(int i = 0; i < input_vector->output->Size(); ++i) {
            value += input_vector->function->act->operator()(input_vector->output->ElementAt(i)) * weights->ElementAt(i, h);
	    //if(skip == 9) std::cout << value << " -- " << input_vector->function->act->operator()(input_vector->output->ElementAt(i)) * weights->ElementAt(i, h) << std::endl;
	    }
	   //if(skip == 9) system("pause");
	    output->ElementTo(h, value); 
		fileOut << h << ": " << value << "    Activated -> " << function->act->operator()(value) << std::endl;
    }
    skip++;
}
void FullConnected::CalculateDeltas(ILayer* prev_layer) {
    
    assert(static_cast<FullConnected*>(prev_layer)->deltas->Size() == weights->Height2D());
	
	//fileOut << std::endl << std::endl << "full connected : Deltas calculation\n";
    FullConnected* deltas_vector = static_cast<FullConnected*>(prev_layer);
    for(int h = 0; h < weights->Height2D(); ++h) {
        double value = 0;
        for(int i = 0; i < deltas->Size(); ++i) {
            value += deltas->ElementAt(i) * weights->ElementAt(h, i);
        }
        deltas_vector->deltas->ElementTo(h, value);
		//fileOut << h << ": " << value << std::endl;
    }
}
void FullConnected::DoCorrections(ILayer* prev_layer, double ffactor) {
    
    assert(static_cast<FullConnected*>(prev_layer)->output->Size() == weights->Height2D());
    //fileOut << std::endl << std::endl << "full connected : Correction calculation\n";
	fileOut << "WEIGHTS:" << std::endl;
    for(int i = 0; i < weights->Height2D(); ++i) {
        for(int j = 0; j < weights->Width2D(); ++j) {
            // w[i, j] += in[i] * ffactor * f'(o[j]) * d[j];
            double value = static_cast<FullConnected*>(prev_layer)->output->ElementAt(i) * ffactor * function->act->operator[](output->ElementAt(j)) * deltas->ElementAt(j);
            weights->ElementTo(i, j, weights->ElementAt(i, j) + value);
			fileOut << weights->ElementAt(i, j) << ' ';
			//fileOut << value << " = " << static_cast<FullConnected*>(prev_layer)->output->ElementAt(i) <<
				//" * (" << output->ElementAt(j) << " --> " << function->act->operator[](output->ElementAt(j)) << ") * " << deltas->ElementAt(j) << std::endl;
        }
		fileOut << std::endl;
    }
}
const std::string FullConnected::Properties() const {
    return "";
}

Simplifying::Simplifying(const std::string& name, const Combination& func, size_t kernelHeight, size_t kernelWidth, size_t z, size_t y, size_t x)
    : ILayer(name, ILayer::Simplifying, func)
{
    kernel_height = kernelHeight;
    kernel_width = kernelWidth;

    output = new Buffer(z, y, x, 0.);
    deltas = new Buffer(z, y, x, 0.);
}
void Simplifying::CalculateOutput(ILayer* prev_layer) {
    
    assert(static_cast<Simplifying*>(prev_layer)->output->Depth3D() == output->Depth3D());
    assert(static_cast<Simplifying*>(prev_layer)->output->Height3D() == output->Height3D());
    assert(static_cast<Simplifying*>(prev_layer)->output->Width3D() == output->Width3D());
    assert(output->Height3D() % kernel_height == 0);
    assert(output->Width3D() % kernel_width == 0);
    
    for(int d = 0; d < output->Depth3D(); ++d) {
        for(int h = 0; h < output->Height3D(); h += kernel_height) {
            for(int w = 0; w < output->Width3D(); w += kernel_width) {
                for(int i = h; i < h + kernel_height; ++i) {
                    for(int j = w; j < w + kernel_width; ++j) {
                        function->comb->operator+(static_cast<Simplifying*>(prev_layer)->output->ElementAt(d, i, j));
                    }
                }
                for(int i = h; i < h + kernel_height; ++i) {
                    for(int j = w; j < w + kernel_width; ++j) {
                        output->ElementTo(d, i, j, function->comb->operator()());
                    }
                }
                function->comb->Clear();
            }
        }
    }
}
void Simplifying::CalculateDeltas(ILayer* prev_layer) {

    assert(static_cast<Simplifying*>(prev_layer)->deltas->Depth3D() == deltas->Depth3D());
    assert(static_cast<Simplifying*>(prev_layer)->deltas->Height3D() == deltas->Height3D());
    assert(static_cast<Simplifying*>(prev_layer)->deltas->Width3D() == deltas->Width3D());
    assert(deltas->Height3D() % kernel_height == 0);
    assert(deltas->Width3D() % kernel_width == 0);

    for(int d = 0; d < deltas->Depth3D(); ++d) {
        for(int h = 0; h < deltas->Height3D(); ++h) {
            for(int w = 0; w < deltas->Width3D(); ++w) {
                static_cast<Simplifying*>(prev_layer)->deltas->ElementTo(d, h, w, deltas->ElementAt(d, h, w));
            }
        }
    }
}
const std::string Simplifying::Properties() const {
    return "";
}
