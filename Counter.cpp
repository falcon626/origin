#include "Counter.h"
#include "../../Data/ResourceManager.h"
#include "../../Data/BinaryAccessor.hpp"
#include "../../Utility/UtilityDefault.hxx"

Counter::Counter() noexcept
	: m_oneSize    (Def::IntNull)
	, m_computed   (Def::IntNull)
	, m_countNumber(Def::IntNull)
{ Init(); }

void Counter::Init()
{
	std::vector<int>  parameter;
	auto counter(Def::SizTNull);

	{
#if _DEBUG
		const auto IsAssert = DATA.Load("Asset/Data/CounterParameter/Initial_Int.dat", parameter, counter);
		_ASSERT_EXPR(IsAssert, L"BinaryData Not Found");
#else
		DATA.Load("Asset/Data/CounterParameter/Initial_Int.dat", parameter, counter);
#endif // _DEBUG
	}

	m_color    = Def::Color;
	m_computed = parameter[--counter];
}

void Counter::DrawSpriteCounter() noexcept
{
	auto x(Def::FloatNull);
	for (decltype(auto) digitRect : m_rect)
	{
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x - (x * digitRect.height)), static_cast<int>(m_pos.y), m_oneSize, m_oneSize, &digitRect, &m_color);
		++x;
	}
}

void Counter::Update() noexcept
{
	SetCounterRect();
}

void Counter::SetCounterParameter(const int& countNumber, const Math::Vector2& pos) noexcept
{
	m_countNumber = countNumber;
	m_pos = pos;
}

void Counter::SetCounterParameter(const int& countNumber, const Math::Vector2& pos, const Math::Color& color) noexcept
{
	m_countNumber = countNumber;
	m_pos = pos;
	m_color = color;
}

void Counter::SetCounterParameter(const int& countNumber, const Math::Vector2& pos, const int& rectSize) noexcept
{
	m_countNumber = countNumber;
	m_pos = pos;
	m_oneSize = rectSize;
}

void Counter::SetCounterParameter(const int& countNumber, const Math::Vector2& pos, const int& rectSize, const Math::Color& color) noexcept
{
	m_countNumber = countNumber;
	m_pos = pos;
	m_oneSize = rectSize;
	m_color = color;
}

void Counter::SetCounterRect() noexcept
{
	auto hundred = m_countNumber / (m_computed * m_computed);
	auto teen = (m_countNumber / m_computed) % m_computed;
	auto one = m_countNumber % m_computed;
	m_rect[static_cast<size_t>(Digit::Hundreds)] = { m_oneSize * hundred, Def::IntNull, m_oneSize, m_oneSize };
	m_rect[static_cast<size_t>(Digit::Tens)]     = { m_oneSize * teen, Def::IntNull, m_oneSize, m_oneSize };
	m_rect[static_cast<size_t>(Digit::Ones)]     = { m_oneSize * one, Def::IntNull, m_oneSize, m_oneSize };
}