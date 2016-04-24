#include "Headers\NeuralFunctions.h"
#include <cmath>

using namespace neural;

double Function::Activation::softMax(double value, Function::Control todo) {
	static double sum = 0;
	
	switch(todo) {
		case	Accumulate :
					sum += std::pow(M_E, value);
					return 0;
		case	Erase :
					sum = 0;
					return 0;
		case	Answer :
					return std::pow(M_E, value) / sum;
	};
	return -1;
}
double Function::Activation::softStep(double value, Function::Control todo) {
	static double sum = 0;
	static double param = 1;

	switch(todo) {
		case	Accumulate :
					sum += value;
					return 0;
		case	Erase :
					sum = 0;
					return 0;
		case	Answer :
					return 1 / (1 + std::pow(M_E, -1*param*sum);
		case	Param1 :
					param = value;
					return 0;
	};
	return -1;
}
double Function::Activation::softPlus(double value, Function::Control todo) {
	static double sum = 0;
	
	switch(todo) {
		case	Accumulate :
					sum += value;
					return 0;
		case	Erase :
					sum = 0;
					return 0;
		case	Answer :
					return std::log( std::pow(M_E, sum) + 1 );
	};
	return -1;
}
double Function::Activation::softSign(double value, Function::Control todo) {
	static double sum = 0;

	switch(todo) {
		case	Accumulate :
					sum += value;
					return 0;
		case	Erase :
					sum = 0;
					return 0;
		case	Answer :
					return sum / ( std::abs(sum) + 1 );
	};
	return -1;
}
double Function::Activation::binaryStep(double value, Function::Control todo) {
	static double sum = 0;

	switch(todo) {
		case	Accumulate :
					sum += value;
					return 0;
		case	Erase :
					sum = 0;
					return 0;
		case	Answer :
					return ( sum < 0 ? 0 : 1 ); 
	};
	return -1;
}
double Function::Activation::tanH(double value, Function::Control todo) {
	static double sum = 0;

	switch(todo) {
		case	Accumulate :
					sum += value;
					return 0;
		case	Erase :
					sum = 0;
					return 0;
		case	Answer :
					return ( 2 / (1 + std::pow(M_E, -2*sum)) ) - 1; 
	};
	return -1;
}
double Function::Activation::identity(double value, Function::Control todo) {
	return Function::Combination::sum(value, todo);
}
double Function::Activation::bentIdentity(double value, Function::Control todo) {
	static double sum = 0;

	switch(todo) {
		case	Accumulate :
					sum += value;
					return 0;
		case	Erase :
					sum = 0;
					return 0;
		case	Answer :
					return ( 2 / (1 + std::pow(M_E, -2*sum)) ) - 1; 
	};
	return -1;
}

