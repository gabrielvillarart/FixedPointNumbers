#include "Source/Fixed.h"
#include "Source/FixedTypes.h"

/*
 * Predefined types:
 *   fixed8, fixed16, fixed32, fixed64, ufixed8, ufixed16, ufixed32, ufixed64
 *   decimal8, decimal16, decimal32, decimal64, udecimal8, udecimal16, udecimal32, udecimal64
 *   precise_byte32, precise_byte64, uprecise_byte32, uprecise_byte64
*/

#define CONSTEXPR constexpr

int main() 
{
	using fixed_type = fixed32;

	CONSTEXPR auto max = fixed_type::max();
	CONSTEXPR int64 max_i = max;
	CONSTEXPR double max_d = max;

	CONSTEXPR auto min = fixed_type::min();
	int64 min_i = min; // Constexpr issue described below main function.
	CONSTEXPR double min_d = min;
	
	CONSTEXPR fixed_type a = "1";
	CONSTEXPR int64 a_i = a;
	CONSTEXPR double a_d = a;
	
	CONSTEXPR fixed_type b = "-0.51";
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

	// To be developed:
	//CONSTEXPR fixed_type quoeficient = a / b;
	//CONSTEXPR int64 quoeficient_i = quoeficient;
	//CONSTEXPR double quoeficient_d = quoeficient;
}

// Known issue: Minimum value of underline types int32 and int64 exceeds limit on int conversion.
