#include "Headers\NeuralFunctions.h"
#include <cmath>
#include <float.h>

#include <iostream>

using namespace neural;

double Activation::fSoftMax(double value) const {
	return std::pow(M_E, value) / param1;
}
double Activation::fSoftStep(double value) const {
	return 1. / ( 1. + std::pow( M_E, -1.*param1*value ) );
}
double Activation::fSoftPlus(double value) const {
	return std::log( std::pow(M_E, value) + 1. );
}
double Activation::fSoftSign(double value) const {
	return value / ( std::abs( value ) + 1. );
}
double Activation::fBinaryStep(double value) const {
	return ( value < 0. ? 0. : 1. ); 
}
double Activation::fTanH(double value) const {
	return ( 2. / (1. + std::pow(M_E, -2.*value)) ) - 1.; 
}
double Activation::fIdentity(double value) const {
	return value;
}
double Activation::fArcTan(double value) const {
	return atan(value);
}
double Activation::fBentIdentity(double value) const {
	return ( std::sqrt( value*value + 1) - 1 ) / 2 + value;
}
double Activation::fGaussian(double value) const {
	return std::pow(M_E, -1.*value*value);
}
double Activation::dSoftMax(double value) const {
	return fSoftMax(value);
}
double Activation::dSoftStep(double value) const {
	return param1 * (fSoftStep(value)) * (1. - fSoftStep(value));
}
double Activation::dSoftPlus(double value) const {
	return 1. / ( 1 + std::pow(M_E, -1.*value));
}
double Activation::dSoftSign(double value) const {
	return 1. / ( ( std::abs( value ) + 1. ) * ( std::abs( value ) + 1. ) );
}
double Activation::dBinaryStep(double value) const {
	return 0.; 
}
double Activation::dTanH(double value) const {
	return 1 - fTanH(value) * fTanH(value); 
}
double Activation::dIdentity(double value) const {
	return 1.;
}
double Activation::dArcTan(double value) const {
	return 1 / ( value * value + 1);
}
double Activation::dBentIdentity(double value) const {
	return value / (2 * std::sqrt( value * value + 1 )) + 1;
}
double Activation::dGaussian(double value) const {
	return -2. * value * std::pow(M_E, -1.*value*value);
}
void Activation::init() {
	param1 = 0;	
}
Activation::Activation(Name function) {
	selected_func = function;
	init();
}
Activation::Activation(Name function, double parameter) {
	selected_func = function;
	param1 = parameter;
	init();
}
double Activation::operator()(double value) const {
	switch(selected_func) {
		case	SoftMax		:	return fSoftMax(value);
		case	SoftStep	:	return fSoftStep(value);
		case	SoftPlus	:	return fSoftPlus(value);
		case	SoftSign	:	return fSoftSign(value);
		case	BinaryStep	:	return fBinaryStep(value);
		case	TanH		:	return fTanH(value);
		case	ArcTan		:	return fArcTan(value);
		case	Identity	:	return fIdentity(value);
		case	BentIdentity	:	return fBentIdentity(value);
		case	Gaussian	:	return fGaussian(value);
	};
}
double Activation::operator[](double value) const {
	switch(selected_func) {
		case	SoftMax		:	return dSoftMax(value);
		case	SoftStep	:	return dSoftStep(value);
		case	SoftPlus	:	return dSoftPlus(value);
		case	SoftSign	:	return dSoftSign(value);
		case	BinaryStep	:	return dBinaryStep(value);
		case	TanH		:	return dTanH(value);
		case	ArcTan		:	return dArcTan(value);
		case	Identity	:	return dIdentity(value);
		case	BentIdentity	:	return dBentIdentity(value);
		case	Gaussian	:	return dGaussian(value);
	};
}


double Combination::fMaximum() const {
	return element;
}
double Combination::fMinimum() const {
	return element;
}
double Combination::fMean() const {
	return accumulated_sum / accumulated_qty;
}
double Combination::fMaxQtyBoolean() const {
	return (accumulated_sum > accumulated_qty / 2. ? 1. : 0.);
}
double Combination::fMinQtyBoolean() const {
	return (accumulated_sum < accumulated_qty / 2. ? 1. : 0.);
}
double Combination::fSum() const {
	return accumulated_sum;
}
double Combination::fBooleanSum() const {
	return accumulated_sum;
}
double Combination::fNormalize(double value) const {
	return value / accumulated_sum;
}
void Combination::init() {
	if(Maximum == selected_func) {
		element = DBL_MIN;
	}
	else {
		element = DBL_MAX;
	}
	accumulated_sum = 0;
	accumulated_qty = 0;
}
Combination::Combination(Combination::Name function) {
	selected_func = function;
	init();
}
void Combination::Clear() {
	init();
}
double Combination::operator()() const {
	assert(Normalize != selected_func);
	switch(selected_func) {
		case	Maximum		:	return fMaximum();
		case	Minimum		:	return fMinimum();
		case	Mean		:	return fMean();
		case	MaxQtyBoolean	:	return fMaxQtyBoolean();
		case	MinQtyBoolean	:	return fMinQtyBoolean();
		case	Sum		:	return fSum();
		case	BooleanSum	:	return fBooleanSum();
	};
}
double Combination::operator()(double value) const {
	assert(Normalize == selected_func);
	return fNormalize(value);
}
void Combination::operator+(double value) {

	switch(selected_func) {
		case	Maximum		:
						if(element < value) element = value;
						break;
		case	Minimum		:
						if(element > value) element = value;
						break;
		case	Mean		:
						accumulated_sum += value;
						accumulated_qty ++;
						break;
		case	MaxQtyBoolean	:
						accumulated_sum += (bool) value;
						accumulated_qty ++;
						break;
		case	MinQtyBoolean	:	accumulated_sum += (bool) value;
						accumulated_qty ++;
						break;
		case	Sum		:	
						accumulated_sum += value;
						break;
		case	BooleanSum	:
						accumulated_sum += (bool) value;
						break;
		case	Normalize	:
						accumulated_sum += value;
						break;
	};
}
