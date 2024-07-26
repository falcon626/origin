#pragma once

class Counter
{
public:
	Counter ()noexcept;
	~Counter()noexcept = default;

	void DrawSpriteCounter() noexcept;
	void Update() noexcept;
	
	void SetTex(const std::shared_ptr<KdTexture>& spTex) noexcept { m_spTex = spTex; m_oneSize = m_spTex->GetHeight(); }

	void SetCounterParameter(const int& countNumber, const Math::Vector2& pos) noexcept;
	void SetCounterParameter(const int& countNumber, const Math::Vector2& pos, const Math::Color& color) noexcept;
	void SetCounterParameter(const int& countNumber, const Math::Vector2& pos, const int& rectSize) noexcept;
	void SetCounterParameter(const int& countNumber, const Math::Vector2& pos, const int& rectSize, const Math::Color& color) noexcept;

private:

	enum class Digit : size_t
	{
		Ones,
		Tens,
		Hundreds,
		Max
	};

	void Init();
	void SetCounterRect() noexcept;

	std::array<Math::Rectangle, static_cast<size_t>(Digit::Max)> m_rect;
	std::shared_ptr<KdTexture> m_spTex;
	Math::Vector2 m_pos;
	Math::Color m_color;

	int m_oneSize;
	int m_computed;
	int m_countNumber;
};