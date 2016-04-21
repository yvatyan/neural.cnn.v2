#ifndef NEURALFUNCTIONS_H_
#define NEURALFUNCTIONS_H_

namespace neural {
	class Function {
		public:
			class Activation {
				public:
					static double softMax(double value, bool accumulate);

			};
			class Combination {
				public:
					static double Max(double value, bool accumulate);
					static double Min(double value, bool accumulate);
					static double MaxQty(double value, bool accumulate);
					static double MinQty(double value, bool accumulate);
			};
			
	};
	
}


#endif