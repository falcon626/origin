#pragma once

class BaseBasic2DObject : public KdGameObject
{
public:
	BaseBasic2DObject () noexcept = default;
	~BaseBasic2DObject() noexcept override = default;

	// Not Virtual Function
	inline void DrawSprite() noexcept override{ KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<const int>(m_pos.x), static_cast<const int>(m_pos.y)); }

	// Ius Vitae Necisque
	inline auto KillExistence() noexcept { m_isExpired = true; }
protected:

	// KdTexture
	inline auto SetTexturelData(const std::string_view& path) noexcept
	{
		if (path.empty()) return;
		m_spTex = FlDataStorage::Instance().GetTexture(path.data());

		*m_spTexInfo = m_spTex->GetInfo();
	}

	// Class Or Struct Value
	std::shared_ptr<D3D11_TEXTURE2D_DESC> m_spTexInfo;
	Math::Vector2 m_pos;
private:

	// Class Or Struct Value
	std::shared_ptr<KdTexture> m_spTex;
};