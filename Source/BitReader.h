#pragma once

#include "Core.h"

template<typename T>
class bit_reader
{
	struct buffer_type
	{
		constexpr buffer_type() = default;
		constexpr uint8 operator[](uint8 i) const { return data[i]; }
		uint8 data[sizeof(T)];
	};

public:
	constexpr bit_reader(const T& object)
	{
		const buffer_type obj_bytes = std::bit_cast<const buffer_type>(object);

		uint8 end = sizeof(T) - 1;
		for (size_t i = 0; i < sizeof(T); i++)
		{
			const uint8& obj_byte = obj_bytes[end - i];
			bytes[i] = obj_byte;
			uint8 index_shift = i * 8;

			for (size_t j = 0; j < 8; j++)
			{
				uint8 bit_mask = uint8(0b10000000) >> j;
				uint8 index = index_shift + j;

				if (obj_byte & bit_mask)
				{
					bits[index] = 1;
				}
				else
				{
					bits[index] = 0;
				}
			}
		}
	}

private:
	uint8 bytes[sizeof(T)] = {};
	uint8 bits[sizeof(T) * 8] = {};
};