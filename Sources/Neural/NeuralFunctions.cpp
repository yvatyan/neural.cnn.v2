#include "Headers\NeuralFunctions.h"
#include <cmath>
#include <float.h>

#include <iostream>

using namespace neural;

double Activation::SoftMax(double value) const {
	return std::pow(M_E, value) / param1;
}
double Activation::SoftStep(double value) const {
	return 1. / ( 1. + std::pow( M_E, -1.*param1*value ) );
}
double Activation::SoftPlus(double value) const {
	return std::log( std::pow(M_E, value) + 1. );
}
double Activation::SoftSign(double value) const {
	return value / ( std::abs( value ) + 1. );
}
double Activation::BinaryStep(double value) const {
	return ( value < 0. ? 0. : 1. ); 
}
double Activation::TanH(double value) const {
	return ( 2. / (1. + std::pow(M_E, -2.*value)) ) - 1.; 
}
double Activation::Identity(double value) const {
	return value;
}
double Activation::ArcTan(double value) const {
	return atan(value);
}
double Activation::BentIdentity(double value) const {
	return ( std::sqrt( value*value + 1) - 1 ) / 2 + value;
}
double Activation::Gaussian(double value) const {
	return std::pow(M_E, -1.*value*value);
}
double Activation::dSoftMax(double value) const {
	return SoftMax(value);
}
double Activation::dSoftStep(double value) const {
	return param1 * (SoftStep(value)) * (1. - SoftStep(value));
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
	return 1 - TanH(value) * TanH(value); 
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
	selected_func = fuction;
	param1 = parameter;
	init();
}
double operator()(double value) const {
	switch(selected_func) {
		case	SoftMax		:	return SoftMax(value);
		case	SoftStep	:	return SoftStep(value);
		case	SoftPlus	:	return SoftPlus(value);
		case	SoftSign	:	return SoftSign(value);
		case	BinaryStep	:	return BinaryStep(value);
		case	TanH		:	return TanH(value);
		case	ArcTan		:	return ArcTan(value);
		case	Identity	:	return Identity(value);
		case	BentIdentity	:	return BentIdentity(value);
		case	Gaussian	:	return Gaussian(value);
	};
}
double operator[](double value) const {
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


double Combination::Maximum() const {
	return element;
}
double Combination::Minimum() const {
	return element;
}
double Combination::Mean() const {
	return accumulated_sum / accumulated_qty;
}
double Combination::MaxQtyBoolean() const {
	return (accumulated_sum > accumulated_qty / 2. ? 1. : 0.);
}
double Combination::MinQtyBoolean() const {
	return (accumulated_sum < accumulated_qty / 2. ? 1. : 0.);
}
double Combination::Sum() const {
	return accumulated_sum;
}
double Combination::BooleanSum() const {
	return accumulated_sum;
}
double Combination::Normalize(double value) const {
	return value / accumulated_sum;
}
void Combination::init() {
	if(Maximum == selected_function) {
		element = DBL_MIN;
	}
	else {
		element = DBL_MAX;
	}
	accumulated_sum = 0;
	accumulated_qty = 0;
}
double Combination::Combination(Combination::Name function) {
	selected_func = function;
	init();
}
void Clear() {
	init();
}
double operator()() const {
	assert(Normalize != selected_func);
	switch(selected_func) {
		case	Maximum		:	return Maximum();
		case	Minimum		:	return Minimum();
		case	Mean		:	return Mean();
		case	MaxQtyBoolean	:	return MaxQtyBoolean();
		case	MinQtyBoolean	:	return MinQtyBoolean();
		case	Sum		:	return Sum();
		case	BooleanSun	:	return BooleanSum();
	};
}
double operator(double value) const {
	assert(Normalize == selected_func);
	return Normalize(value);
}
void operator+(double value) {

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
						accumulayed_qty ++;
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
