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

			double SoftMax(double value) const ;
			double SoftStep(double value) const ;
			double SoftPlus(double value) const ;
			double SoftSign(double value) const ;
			double BinaryStep(double value) const ;
			double TanH(double value) const ;
			double ArcTan(double value) const ;
			double Identity(double value) const ;
			double BentIdentity(double value) const ;
			double Gaussian(double value) const ;

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
			
			double Maximum() const ;
			double Minimum() const ;
			double Mean()const ;
			double MaxQtyBoolean() const ;
			double MinQtyBoolean() const ;
			double Sum() const ;
			double BooleanSum() const ;
			double Normalize(double value) const ;

			void init();
		public:
			Combination(Name function);
		
			void Clear();

			double operator()() const;
			double operator(double value) const;
			void operator+(double value);
	};
}

#endif
