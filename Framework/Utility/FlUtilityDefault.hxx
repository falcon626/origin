#pragma once

namespace Def // Only Constexpr Value
{
	constexpr Math::Color   Color { 1.F,1.F,1.F,1.F };

	constexpr Math::Vector2 Vec2  { 0.F,0.F };
	constexpr Math::Vector3 Vec3  { 0.F,0.F,0.F };
	constexpr Math::Vector4 Vec4  { 0.F,0.F,0.F,0.F };

	constexpr Math::Matrix  Mat   { 1.F, 0.F, 0.F, 0.F,
									0.F, 1.F, 0.F, 0.F,
									0.F, 0.F, 1.F, 0.F,
									0.F, 0.F, 0.F, 1.F };

	constexpr auto IntZero{ 0 };
	constexpr auto IntOne { 1 };

	constexpr auto LongZero{ 0L };
	constexpr auto LongOne { 1L };

	constexpr auto SizTZero{ 0U };
	constexpr auto SizTOne { 1U };

	constexpr auto FloatZero{ 0.F };
	constexpr auto FloatOne { 1.F };

	constexpr auto DoubleZero{ 0. };
	constexpr auto DoubleOne { 1. };

	constexpr auto LongLongZero{ 0LL };
	constexpr auto LongLongOne { 1LL };

	constexpr auto SizeTLongZero{ 0UL };
	constexpr auto SizeTLongOne { 1UL };

	constexpr auto LongDoubleZero{ 0.L };
	constexpr auto LongDoubleOne { 1.L };

	constexpr auto SizeTLongLongZero{ 0ULL };
	constexpr auto SizeTLongLongOne { 1ULL };

	constexpr auto BitMaskPos0 { 0B00000000U };
	constexpr auto BitMaskPos1 { 0B00000001U };
	constexpr auto BitMaskPos2 { 0B00000010U };
	constexpr auto BitMaskPos3 { 0B00000100U };
	constexpr auto BitMaskPos4 { 0B00001000U };
	constexpr auto BitMaskPos5 { 0B00010000U };
	constexpr auto BitMaskPos6 { 0B00100000U };
	constexpr auto BitMaskPos7 { 0B01000000U };
	constexpr auto BitMaskPos8 { 0B10000000U };
	constexpr auto BitMaskPos9 { 0B0000000100000000U };
	constexpr auto BitMaskPos10{ 0B0000001000000000U };
	constexpr auto BitMaskPos11{ 0B0000010000000000U };
	constexpr auto BitMaskPos12{ 0B0000100000000000U };
	constexpr auto BitMaskPos13{ 0B0001000000000000U };
	constexpr auto BitMaskPos14{ 0B0010000000000000U };
	constexpr auto BitMaskPos15{ 0B0100000000000000U };
	constexpr auto BitMaskPos16{ 0B1000000000000000U };

#if   _WIN64
	constexpr auto HalfBit{ 32 };
#elif _WIN32
	constexpr auto HalfBit{ 16 };
#endif // _WIN64	// _WIN32
}

/*
local        localValue

argument   = local

macro        MACRO_NAME

bool         isName

const        ConstValue

constexpr  = const

class      = const
	member   m_memberValue

namespace  = const
	menmber  menber_value

enum       = const

enum class = const

struct       tagStruct
	member   memberValue_

union        _tagUnion

matrix       mMat

pointer
	raw      p
	share    sp
	unique   up
	work     wp

function   = const
	lambda = local

priority
 matrix = bool = local = member < constexpr = const < argument < pointer

.h   This Extension Has    Cpp And Classes
.hpp This Extension Has No Cpp
.hxx This Extension Has No Cpp And Classes
*/