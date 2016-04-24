#ifndef NEURALFUNCTIONS_H_
#define NEURALFUNCTIONS_H_

#include <string>

namespace neural {
	class FunctionCollection {
		private:
			class Activation {
				private:
					double accumulated_sum;
				public:
					double SoftMax() const ;
					double SoftStep() const ;
					double SoftPlus() const ;
					double SoftSign() const ;
					double BinaryStep() const ;
					double TanH() const ;
					double ArcTan() const ;
					double Identity() const ;
					double BentIdentity() const ;
					double Gaussian() const ;

					void accumulate(double value);
			};
			class Combination {
				private:
					double accumulated_sum;
				public:
					double Maximum() const ;
					double Minimum() const ;
					double Mean()const ;
					double MaxBinaryQty() const ;
					double MinBinaryQty() const ;
					double Hdtv() const ;
					double Yuv() const ;
					double Sum() const ;

					void accumulate(double value);
			};

			Activation A;
			Combination C;
		public:	
			const Activation& Activation() const {
				return A;
			}
			const Combination& Combination() const {
				return C;
			}
			void Accumulate(double value) {
				A.accumulate(value);
				C.accumulate(value)
			}
	};
}

#include "../NeuralFunctions.cpp"

#endif
