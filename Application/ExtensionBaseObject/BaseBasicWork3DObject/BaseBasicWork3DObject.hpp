#pragma once

class BaseBasicWork3DObject : public KdGameObject
{
public:
	BaseBasicWork3DObject () noexcept = default;
	~BaseBasicWork3DObject() noexcept override = default;

	// Not Virtual Function
	inline void GenerateDepthMapFromLight() noexcept override { KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModelWork, m_mWorld); }
	inline void DrawLit()                   noexcept override { KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModelWork, m_mWorld); }

	// Ius Vitae Necisque
	inline auto KillExistence() noexcept { m_isExpired = true; }
protected:

	// KdModelData
	inline auto SetModelData(const std::string_view& path) noexcept
	{
		if (path.empty()) return;
		m_spModelWork = std::make_shared<KdModelWork>(FlDataStorage::Instance().GetModelData(path.data()));
	}

	// Class Or Struct Value
	std::shared_ptr<KdModelWork> m_spModelWork;
};