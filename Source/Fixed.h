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

template<typename underlying_type, std::make_unsigned_t<underlying_type> precision>
class fixed
{
	static_assert(std::is_integral_v<underlying_type>, "fixed::underlying_type must be an integer type.");

public:
	using underlying_unsigned = std::make_unsigned_t<underlying_type>;

	constexpr static bool is_underlying_signed = std::is_signed_v<underlying_type>;

	constexpr static underlying_unsigned wholes_precision = (sizeof(underlying_type) * 8) - precision;
	constexpr static underlying_unsigned half_precision = precision / 2;
	constexpr static underlying_unsigned half_precision_mask = underlying_type(-1) << half_precision;
	constexpr static underlying_unsigned precision_remainder = precision % 2;
	constexpr static underlying_unsigned half_precision_plus_remainder = half_precision + precision_remainder;
	
	constexpr static underlying_unsigned whole_mask = wholes_precision ? underlying_unsigned(-1) << precision : underlying_unsigned(0);
	constexpr static underlying_unsigned decimals_mask = ~whole_mask;
	constexpr static underlying_unsigned sign_mask = 
		is_underlying_signed ? 
		underlying_type(1) << (sizeof(underlying_type) * 8 - 1) :
		underlying_type(0);

	constexpr static underlying_type unsign_mask = ~sign_mask;

	constexpr static underlying_type one = (underlying_unsigned)(decimals_mask) + (underlying_unsigned)1;
	constexpr static underlying_type half = one / 2;
	
	constexpr static underlying_type max_value = 
		is_underlying_signed ? 
		underlying_unsigned(-1) >> underlying_unsigned(1) :
		underlying_type(-1);

	constexpr static underlying_type max_int = max_value & underlying_type(whole_mask);

	constexpr static underlying_type min_value = ~(max_value);

	constexpr static underlying_type min_int = min_value & whole_mask;

	
public:
	constexpr fixed() noexcept = default;

	template<uint32 string_length>
	using string_literal = const char(&)[string_length];

	// Construction with string literal.
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

		if constexpr (is_underlying_signed)
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
		fixed sum;
		sum.underlying_value = underlying_unsigned(underlying_value) + underlying_unsigned(other.underlying_value);
		return sum;
	}

	constexpr fixed operator-(fixed other) const
	{
		fixed difference;
		difference.underlying_value = underlying_unsigned(underlying_value) - underlying_unsigned(other.underlying_value);
		return difference;
	}

	constexpr inline fixed operator*(fixed factor) const
	{
		if constexpr (is_underlying_signed)
		{
			const underlying_unsigned this_value = underlying_unsigned(
				(underlying_value < underlying_type(0)) ? 
				-underlying_value : 
				underlying_value
			);

			const underlying_unsigned factor_value =  underlying_unsigned(
				factor.underlying_value < underlying_type(0) ? 
				-factor.underlying_value : 
				factor.underlying_value
			); 

			const underlying_type sign = (factor.underlying_value ^ underlying_value) & sign_mask; 

			underlying_unsigned half_decimals;
			half_decimals = this_value * factor_value;
			half_decimals >>= precision;
			half_decimals &= ~half_precision_mask;

			underlying_unsigned factor_half_and_wholes;
			factor_half_and_wholes = (this_value >> half_precision) * (factor_value >> half_precision_plus_remainder);

			fixed product;
			product.underlying_value = underlying_type(factor_half_and_wholes | half_decimals);
			if (sign)
				product.underlying_value = -product.underlying_value;

			return product;
		}
		else
		{
			const underlying_unsigned& this_value = underlying_value; 
			const underlying_unsigned& factor_value = factor.underlying_value; 

			if constexpr (wholes_precision)
			{
				underlying_unsigned half_decimals;
				half_decimals = this_value * factor_value;
				half_decimals >>= precision;
				half_decimals &= ~half_precision_mask;

				underlying_unsigned factor_half_and_wholes;
				factor_half_and_wholes = (this_value >> half_precision) * (factor_value >> half_precision_plus_remainder);

				fixed product;
				product.underlying_value = underlying_type(factor_half_and_wholes | half_decimals);

				return product;
			}
			else
			{
				underlying_unsigned factor_half_and_wholes;
				factor_half_and_wholes = (this_value >> half_precision) * (factor_value >> half_precision_plus_remainder);

				fixed product;
				product.underlying_value = underlying_type(factor_half_and_wholes);

				return product;
			}

		}
	}

	constexpr fixed operator/(fixed other) const noexcept
	{
		// To be developed.
		fixed quoeficient;
		return quoeficient;
	}
	
private:
    template <typename intType, typename std::enable_if<std::is_integral<intType>::value>::type* = nullptr>
	static constexpr underlying_type init_underlyind_with_int(intType value)
	{
		constexpr bool is_input_signed = std::is_signed_v<intType>;

		if constexpr (is_underlying_signed && is_input_signed)
		{
			return value < intType(0) ? 
				-underlying_type(underlying_unsigned(-value) << precision) :
				underlying_type(underlying_unsigned(value) << precision);
		}
		else if constexpr (is_underlying_signed)
		{
			return underlying_type(underlying_unsigned(value) << precision);
		}
		else if constexpr (is_input_signed)
		{
			return value < intType(0) ? 
				underlying_type(underlying_unsigned(-value) << precision) :
				underlying_type(underlying_unsigned(value) << precision);
		}
		else
		{
			return underlying_type(value) << precision;
		}
	}

public:
    // Construction from a integral type
    template <typename intType, typename std::enable_if<std::is_integral<intType>::value>::type* = nullptr>
    constexpr fixed(intType value) noexcept
		: underlying_value(init_underlyind_with_int(value))
    {}

    // Construction from a floating-point type
    template <typename floatType, typename std::enable_if<std::is_floating_point<floatType>::value>::type* = nullptr>
    constexpr fixed(floatType value) noexcept
		: underlying_value()
    {}


    // Conversion to an integral type
    template <typename intType, typename std::enable_if<std::is_integral<intType>::value>::type* = nullptr>
    constexpr inline operator intType() const noexcept
    {
		if constexpr (wholes_precision == 0)
			return 0;
		
		if constexpr (is_underlying_signed)
		{
			if (underlying_value >= max_int)
			{
				return underlying_type(underlying_unsigned(max_int & unsign_mask) >> precision);
			}

			if (underlying_value & sign_mask)
			{
				return -underlying_type(underlying_unsigned(-underlying_value) >> precision);
			}
			return underlying_type(underlying_unsigned(underlying_value) >> precision);
		}
		else
		{
			return static_cast<intType>(
				underlying_value >= max_int ?
				max_int >> precision :
				(underlying_value + half) >> precision
			);
		}

    }

    // Conversion to a floating-point type
    template <typename floatType, typename std::enable_if<std::is_floating_point<floatType>::value>::type* = nullptr>
    constexpr inline operator floatType() const noexcept
    {
		constexpr floatType conversion_factor = floatType(0.0625 / double(underlying_unsigned(1) << (precision - underlying_unsigned(4))));
        return static_cast<floatType>(underlying_value) * conversion_factor;
    }

private:
	underlying_type underlying_value = {};
};
