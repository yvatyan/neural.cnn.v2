#include "Headers\NeuralFunctions.h"
#include <cmath>
#include <float.h>

using namespace neural;

FunctionCollection::Activation::Activation() {
	accumulated_sum = 0.;
}
double FunctionCollection::Activation::SoftMax(double value) const {
	return std::pow(M_E, value) / accumulated_sum;
}
double FunctionCollection::Activation::SoftStep(double value) const {
	return 1. / ( 1. + std::pow( M_E, -1.*value*accumulated_sum ) );
}
double FunctionCollection::Activation::SoftPlus() const {
	return std::log( std::pow(M_E, accumulated_sum) + 1. );
}
double FunctionCollection::Activation::SoftSign() const {
	return accumulated_sum / ( std::abs(accumulated_sum ) + 1. );
}
double FunctionCollection::Activation::BinaryStep() const {
	return ( accumulated_sum < 0. ? 0. : 1. ); 
}
double FunctionCollection::Activation::TanH() const {
	return ( 2. / (1. + std::pow(M_E, -2.*accumulated_sum)) ) - 1.; 
}
double FunctionCollection::Activation::Identity() const {
	return accumulated_sum;
}
double FunctionCollection::Activation::BentIdentity() const {
	return ( 2. / (1. + std::pow(M_E, -2.*accumulated_sum)) ) - 1.; 
}
double FunctionCollection::Activation::Gaussian() const {
	return std::pow(M_E, -1.*accumulated_sum*accumulated_sum);
}
void FunctionCollection::Activation::accumulate(double value) {
	accumulated_sum += value;
}
void FunctionCollection::Activation::erase() {
	accumulated_sum = 0.;
}

void FunctionCollection::Combination::init() {
	accumulated_sum = 0.;
	accumulated_bool_sum = 0.;
	element_qty = 0.;
	max_element = DBL_MIN;
	min_element = DBL_MAX;
}
FunctionCollection::Combination::Combination() {
	init();
}
double FunctionCollection::Combination::Maximum() const {
	return max_element;
}
double FunctionCollection::Combination::Minimum() const {
	return min_element;
}
double FunctionCollection::Combination::Mean() const {
	return accumulated_sum / element_qty;
}
double FunctionCollection::Combination::MaxBooleanQty() const {
	return (accumulated_bool_sum > element_qty / 2. ? 1. : 0.);
}
double FunctionCollection::Combination::MinBooleanQty() const {
	return (accumulated_bool_sum < element_qty / 2. ? 1. : 0.);
}
double FunctionCollection::Combination::GreyscaleHDTV(double red, double green, double blue) const {
	return 0.2126*red + 0.7152*green + 0.0722*blue;
}
double FunctionCollection::Combination::GreyscaleYUV(double red, double green, double blue) const {
	return 0.299*red + 0.587*green + 0.114*blue;
}
double FunctionCollection::Combination::Sum() const {
	return accumulated_sum;
}
double FunctionCollection::Combination::BooleanSum() const {
	return accumulated_bool_sum;
}
void FunctionCollection::Combination::accumulate(double value) {
	accumulated_sum += value;
	accumulated_bool_sum += (bool) value;
	element_qty ++;
	max_element = (value > max_element ? value : max_element);
	min_element = (value < min_element ? value : min_element);
}
void FunctionCollection::Combination::erase() {
	init();
}
