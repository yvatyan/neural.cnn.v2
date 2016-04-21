#ifndef NEURALFUNCTIONS_H_
#define NEURALFUNCTIONS_H_

namespace neural {
	class Function {
		private:
			double(*function) (double, bool);
		public:
			class Activation {
				public:
					static double softMax(double value, bool accumulate);
					static double softStep(double value, bool accumulate);
					static double softPlus(double value, bool accumulate);
					static double softSign(double value, bool accumulate);
					static double binaryStep(double value, bool accumulate);
					static double tanH(double value, bool accumulate);
					static double arcTan(double value, bool accumulate);
					static double indentity(double value, bool accumulate);
					static double bentIdentity(double value, bool accumulate);
					static double gaussian(double value, bool accumulate);
			};
			class Combination {
				public:
					static double max(double value, bool accumulate);
					static double min(double value, bool accumulate);
					static double mean(double value, bool accumulate);
					static double maxQty(double value, bool accumulate);
					static double minQty(double value, bool accumulate);
					static double hdtv(double value, bool accumulate);
					static double yuv(double value, bool accumulate);
					static double sum(double value, bool accumulate);
			};
	};
}

#endif