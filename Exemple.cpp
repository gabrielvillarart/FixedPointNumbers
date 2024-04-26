#include "Source/Fixed.h"
#include "Source/FixedTypes.h"

#define CONSTEXPR constexpr

int main() 
{
	using fixed_type = fixed16;

	constexpr auto one = fixed_type::half;
	//constexpr auto pointr = point / 10;

	constexpr auto max = fixed_type::max();
	constexpr int64 max_i = max;
	constexpr double max_d = max;

	constexpr auto min = fixed_type::min();
	constexpr int64 min_i = min;
	constexpr double min_d = min;
	
	CONSTEXPR fixed_type a = "6";
	CONSTEXPR int64 a_i = a;
	CONSTEXPR double a_d = a;
	
	CONSTEXPR fixed_type b = "0.25";
	CONSTEXPR int64 b_i = b;
	CONSTEXPR double b_d = b;
	
	CONSTEXPR fixed_type sum = a + b;
	CONSTEXPR int64 sum_i = sum;
	CONSTEXPR double sum_d = sum;
	
	CONSTEXPR fixed_type diff = a - b;
	CONSTEXPR int64 diff_i = diff;
	CONSTEXPR double diff_d = diff;
	
	CONSTEXPR fixed_type product = a * b;
	CONSTEXPR int64 product_i = product;
	CONSTEXPR double product_d = product;

	CONSTEXPR fixed_type quoeficient = a / b;
	CONSTEXPR int64 quoeficient_i = quoeficient;
	CONSTEXPR double quoeficient_d = quoeficient;
}

