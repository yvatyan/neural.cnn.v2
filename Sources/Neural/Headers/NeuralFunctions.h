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
					Activation();

					double SoftMax(double param) const ;
					double SoftStep(double param) const ;
					double SoftPlus() const ;
					double SoftSign() const ;
					double BinaryStep() const ;
					double TanH() const ;
					double ArcTan() const ;
					double Identity() const ;
					double BentIdentity() const ;
					double Gaussian() const ;

					void accumulate(double value);
					void erase();
			};
			class Combination {
				private:
					double accumulated_sum;
					double accumulated_bool_sum;
					double element_qty;
					double max_element;
					double min_element;
					
					void init();
				public:
					Combination();

					double Maximum() const ;
					double Minimum() const ;
					double Mean()const ;
					double MaxBooleanQty() const ;
					double MinBooleanQty() const ;
					double GreyscaleHDTV(double red, double green, double blue) const ;
					double GreyscaleYUV(double red, double green, double blue) const ;
					double Sum() const ;
					double BooleanSum() const ;

					void accumulate(double value);
					void erase();
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
				C.accumulate(value);
			}
			void ClearStorage() {
				A.erase();
				C.erase();
			}
	};
}

#include "../NeuralFunctions.cpp"

#endif
