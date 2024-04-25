#pragma once

#include "Fixed.h"

// SIGNED FIXED

// range: -8 >= n < 8
// resolution: 2^-4 = 0.0625
using fixed8  = fixed<int8, 4>;         

// range: -128 >= n < 128
// resolution: 2^-8 = 0.00390625
using fixed16 = fixed<int16, 8>;        

// range: -32768 >= n < 32768
// resolution: 2^-16 = 0.0000152587890625
using fixed32 = fixed<int32, 16>;       

// range: -2147483648 >= n < 2147483648
// resolution: 2^-32 = 0.00000000023283064365386962890625
using fixed64 = fixed<int64, 32>;       

// UNSIGNED FIXED

// range: 0 >= n < 16 
// resolution: 2^-4 = 0.0625
using ufixed8  = fixed<uint8, 4>;

// range: 0 >= n < 256 
// resolution: 2^-8 = 0.00390625
using ufixed16 = fixed<uint16, 8>;
      
// range: 0 >= n < 65536 
// resolution: 2^-16 = 0.0000152587890625
using ufixed32 = fixed<uint32, 16>;
     
// range: 0 >= n < 4294967296 
// resolution: 2^-32 = 0.00000000023283064365386962890625
using ufixed64 = fixed<uint64, 32>;



// SIGNED PRECISE BYTE

// range: -128 >= n < 128
// precision: 2^-24 = 0.000000059604644775390625
using precise_byte32 = fixed<int32, 24>;

// range: -128 >= n < 128
// precision: 2^-56 = 0.00000000000000001387778780781445675529539585113525390625
using precise_byte64 = fixed<int64, 56>;

// UNSIGNED PRECISE BYTE

// range: 0 >= n < 256
// precision: 2^-24 = 0.000000059604644775390625
using uprecise_byte32 = fixed<uint32, 24>;

// range: 0 >= n < 256
// precision: 2^-56 = 0.00000000000000001387778780781445675529539585113525390625
using uprecise_byte64 = fixed<uint64, 56>;



// SIGNED DECIMAL

// range: -1 >= n < 1
// precision: 2^-7 = 0.0078125
using decimal8  = fixed<int8, 7>;	

// range: -1 >= n < 1
// precision: 2^-15 = 0.000030517578125
using decimal16 = fixed<int16, 15>;

// range: -1 >= n < 1
// precision: 2^-31 = 0.0000000004656612873077392578125
using decimal32 = fixed<int32, 31>;

// range: -1 >= n < 1
// precision: 2^-63 = 0.000000000000000000108420217248550443400745280086994171142578125
using decimal64 = fixed<int64, 63>;

// UNSIGNED DECIMAL

// range: 0 >= n < 1
// precision: 2^-8 = 0.00000000023283064365386962890625
using udecimal8  = fixed<uint8, 8>;

// range: 0 >= n < 1
// precision: 2^-16 = 0.00000000023283064365386962890625
using udecimal16 = fixed<uint16, 16>;

// range: 0 >= n < 1
// precision: 2^-32 = 0.00000000023283064365386962890625
using udecimal32 = fixed<uint32, 32>;

// range: 0 >= n < 1
// precision: 2^-64 = 0.0000000000000000000542101086242752217003726400434970855712890625
using udecimal64 = fixed<uint64, 64>;