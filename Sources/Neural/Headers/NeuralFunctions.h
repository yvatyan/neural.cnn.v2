#ifndef NEURALFUNCTIONS_H_
#define NEURALFUNCTIONS_H_

namespace neural {
	class Activation {
		public:
			enum Name {
				SoftMax = 1,
				SoftStep = 2,
				SoftPlus = 3,
				SoftSign = 4,
				BinaryStep = 5,
				TanH = 6,
				ArcTan = 7,
				Identity = 8,
				BentIdentity = 9,
				Gaussian = 10
			};
		private:
			double param1;
			Name selected_func;

			double fSoftMax(double value) const ;
			double fSoftStep(double value) const ;
			double fSoftPlus(double value) const ;
			double fSoftSign(double value) const ;
			double fBinaryStep(double value) const ;
			double fTanH(double value) const ;
			double fArcTan(double value) const ;
			double fIdentity(double value) const ;
			double fBentIdentity(double value) const ;
			double fGaussian(double value) const ;

			double dSoftMax(double value) const ;
			double dSoftStep(double value) const ;
			double dSoftPlus(double value) const ;
			double dSoftSign(double value) const ;
			double dBinaryStep(double value) const ;
			double dTanH(double value) const ;
			double dArcTan(double value) const ;
			double dIdentity(double value) const ;
			double dBentIdentity(double value) const ;
			double dGaussian(double value) const ;

			void init();
		public:
			Activation(Name function);
			Activation(Name function, double parameter);

			double operator()(double value) const;
			double operator[](double value) const;

	};
	class Combination {
		public:
			enum Name {
				Maximum = 1,
				Minimum = 2,
				Mean = 3,
				MaxQtyBoolean = 4,
				MinQtyBoolean = 5,
				Sum = 6,
				BooleanSum = 7,
				Normalize = 8
			};
		private:
			double accumulated_sum;
			double accumulated_qty;
			double element;
			Name selected_func;
			
			double fMaximum() const ;
			double fMinimum() const ;
			double fMean()const ;
			double fMaxQtyBoolean() const ;
			double fMinQtyBoolean() const ;
			double fSum() const ;
			double fBooleanSum() const ;
			double fNormalize(double value) const ;

			void init();
		public:
			Combination(Name function);
		
			void Clear();

			double operator()() const;
			double operator()(double value) const;
			void operator+(double value);
	};
}

#include "../NeuralFunctions.cpp"

#endif
