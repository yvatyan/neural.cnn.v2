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
				SoftMax = 0,
				SoftStep = 1,
				SoftPlus = 2,
				SoftSign = 3,
				BinaryStep = 4,
				TanH = 5,
				ArcTan = 6,
				Identity = 7,
				BentIdentity = 8,
				Gaussian = 9,

				Maximum = 128,
				Minimum = 129 ,
				Mean = 130,
				MaxQtyBoolean = 131,
				MinQtyBoolean = 132,
				GreyscaleHDTV = 133,
				GreyscaleYUV = 134,
				Sum = 135,
				BooleanSum = 136,
				Normalize = 137
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
			static const std::string& FunctionName(FunctionCollection::Name nameEnum) const;
			static const std::string& FunctionType(FunctionCollection::Name nameEnum) const;
	};
}

#include "../NeuralFunctions.cpp"

#endif
