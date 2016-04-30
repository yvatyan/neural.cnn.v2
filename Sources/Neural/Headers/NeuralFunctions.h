#ifndef NEURALFUNCTIONS_H_
#define NEURALFUNCTIONS_H_

#include <string>

namespace neural {
	class FunctionCollection {
		private:
			class Activation {
				private:
					double accumulated_sum;
					double accumulated_e_sum;

					void init();
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
					double MaxQtyBoolean() const ;
					double MinQtyBoolean() const ;
					double GreyscaleHDTV(double red, double green, double blue) const ;
					double GreyscaleYUV(double red, double green, double blue) const ;
					double Sum() const ;
					double BooleanSum() const ;
					double Normalize(double value) const ;

					void accumulate(double value);
					void erase();
			};

			Activation A;
			Combination C;
		public:	
			enum Name {
				None = 0,
				SoftMax = 1,
				SoftStep = 2,
				SoftPlus = 3,
				SoftSign = 4,
				BinaryStep = 5,
				TanH = 6,
				ArcTan = 7,
				Identity = 8,
				BentIdentity = 9,
				Gaussian = 10,

				Maximum = 129,
				Minimum = 130 ,
				Mean = 131,
				MaxQtyBoolean = 132,
				MinQtyBoolean = 133,
				GreyscaleHDTV = 134,
				GreyscaleYUV = 135,
				Sum = 136,
				BooleanSum = 137,
				Normalize = 138
			};
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
			static const std::string FunctionName(FunctionCollection::Name nameEnum);
			static const std::string FunctionType(FunctionCollection::Name nameEnum);
	};
}

#include "../NeuralFunctions.cpp"

#endif
