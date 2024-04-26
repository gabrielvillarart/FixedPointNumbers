#pragma once

#include "Core.h"

struct fixed_utils
{
	constexpr static uint64 cached_decimals[] = 
	{
		0,
		1844674407370955162,
		3689348814741910323,
		5534023222112865485,
		7378697629483820646,
		9223372036854775808,
		11068046444225730970,
		12912720851596686131,
		14757395258967641293,
		16602069666338596454,
	};
};

template<typename underlying_type, underlying_type precision>
class fixed
{
	static_assert(std::is_integral_v<underlying_type>, "fixed::underlying_type must be an integer type.");

public:
	using underlying_type_unsigned = std::make_unsigned_t<underlying_type>;

	constexpr static underlying_type max_value = 
		std::is_signed_v<underlying_type> ? 
		underlying_type_unsigned(-1) >> underlying_type_unsigned(1) :
		underlying_type(-1);

	constexpr static underlying_type min_value = ~(max_value);

	constexpr static underlying_type whole_mask = underlying_type(-1) << precision;
	constexpr static underlying_type decimals_mask = ~whole_mask;
	constexpr static uint64 one = (uint64)(decimals_mask) + (uint64)1;
	constexpr static uint64 half = one / 2;
	
	constexpr static uint64 wholes_precision = (sizeof(underlying_type) * 8) - precision;
	constexpr static underlying_type half_precision = precision / 2;
	constexpr static underlying_type half_precision_mask = underlying_type(-1) << half_precision;
	constexpr static underlying_type precision_remainder = precision % 2;
	constexpr static underlying_type half_precision_plus_remainder = half_precision + precision_remainder;

	
public:
	constexpr fixed() noexcept = default;

	template<uint32 string_length>
	using string_literal = const char(&)[string_length];

	template<uint32 string_length>
	constexpr fixed(string_literal<string_length> string)
	{
		uint64 wholes = 0;
		uint64 decimals = 0;

		size_t string_end = string_length - 1;
		size_t point_index = 0;

		bool is_negative = false;
		
		// Whole numbers
		for (size_t i = 0; i < string_end; i++)
		{
			const char& character = string[i];

			if (character == '.')
			{
				point_index = i + 1;
				break;
			}
			else if (character >= '0' && character <= '9')
			{
				uint64 character_value = (uint64)(character - '0');
				wholes = wholes * 10 + character_value;
			}
			else if (i == 0 && character == '-')
			{
				is_negative = true;
			}
			else // Invalid Input.
			{
				underlying_value = 0;
				return;
			}
		}

		if constexpr (wholes_precision > 0)
		{
			wholes = wholes << precision;
		}

		// Decimals
		uint64 divisor = 1;
		if (point_index)
		{
			for (size_t i = point_index; i < string_end; i++)
			{
				const char& character = string[i];

				if (character >= '0' && character <= '9')
				{
					char character_value = (char)(character - '0');
					decimals += fixed_utils::cached_decimals[character_value] / divisor;
					divisor *= (uint64)10;
				}
				else // Invalid Input.
				{
					underlying_value = 0;
					return;
				}
			}
		}
		
		if (precision < 64)
		{
			uint64 shift = 64-precision;
			uint64 half = uint64(1) << (shift - 1);
			decimals = (decimals + half) >> shift;
		}

		if constexpr (std::is_signed_v<underlying_type>)
		{

			if (is_negative) // Negative
			{
				uint64 result = ~(wholes + decimals) + 1;
				underlying_value = underlying_type(result);
				return;
			}
		}
		underlying_value = underlying_type(wholes + decimals);
	}

public:
	static constexpr fixed max()
	{
		fixed new_fixed;
		new_fixed.underlying_value = max_value;
		return new_fixed;
	}

	static constexpr fixed min()
	{
		fixed new_fixed;
		new_fixed.underlying_value = min_value;
		return new_fixed;
	}
	
	static constexpr fixed create_directly(underlying_type value)
	{
		fixed ret;
		ret.underlying_value = value;
		return ret;
	}

public:
	constexpr fixed operator+(fixed other) const
	{
		using uT = underlying_type_unsigned;

		fixed sum;
		sum.underlying_value = std::bit_cast<uT>(underlying_value) + std::bit_cast<uT>(other.underlying_value);
		return sum;
	}

	constexpr fixed operator-(fixed other) const
	{
		using uT = underlying_type_unsigned;

		fixed difference;
		difference.underlying_value = std::bit_cast<uT>(underlying_value) - std::bit_cast<uT>(other.underlying_value);
		return difference;
	}

	constexpr fixed operator*(fixed other) const
	{
		//using uT = underlying_type_unsigned;
		using uT = underlying_type;

		const uT this_value = std::bit_cast<uT>(underlying_value); 
		const uT other_value = std::bit_cast<uT>(other.underlying_value); 

		uT decimals = this_value * other_value;
		decimals >>= uT(precision);
		decimals &= ~half_precision_mask;

		uT overflow = (this_value >> uT(half_precision)) * (other_value >> uT(half_precision_plus_remainder));

		uT ret = decimals | overflow;

		fixed product;
		product.underlying_value = std::bit_cast<underlying_type>(ret);
		return product;
	}

	constexpr fixed operator/(fixed other) const
	{
		fixed product;
		product.underlying_value = (underlying_value << half_precision) / (other.underlying_value << half_precision) << precision;
		product.underlying_value &= half_precision_mask;
		return product;
	}
	
public:
	constexpr operator int32() const
	{
		if constexpr (wholes_precision == 0)
			return 0;

		underlying_type ret = underlying_value + half;
		ret = ret >> precision;
		return int32(ret);
	}

	constexpr operator int64() const
	{
		if constexpr (wholes_precision == 0)
			return 0;

		underlying_type ret = underlying_value + half;
		return int64(ret);
	}

	constexpr fixed(double value)
	{
		constexpr double cached_conversion_from_double = (double)((uint64)1 << (uint64)precision);
		underlying_value = (value * cached_conversion_from_double);
	}

	constexpr operator double() const
	{
		constexpr double cached_conversion_to_double = 0.0625 / (double)((uint64)1 << ((uint64)precision - (uint64)4));

		return (double)underlying_value * cached_conversion_to_double;

		//if constexpr (wholes_precision > 0)
		//{
		//	underlying_type integer = underlying_value >> precision;
		//	underlying_type decimals = underlying_value & decimals_mask;

		//	return (double)(integer) + ((double)decimals * cached_conversion_to_double);
		//}
		//else
		//{
		//}
	}

private:
	underlying_type underlying_value = {};
};
