#pragma once

// ゲーム上に存在するすべてのオブジェクトの基底となるクラス
class KdGameObject : public std::enable_shared_from_this<KdGameObject>
{
public:

	enum class [[maybe_unused]] Id : size_t
	{
		UnKnown        [[maybe_unused]] = Def::BitMaskPos0,

		Player         [[maybe_unused]] = Def::BitMaskPos1,
		Enemy          [[maybe_unused]] = Def::BitMaskPos2,
		OtherCreature  [[maybe_unused]] = Def::BitMaskPos3,

		Ground         [[maybe_unused]] = Def::BitMaskPos4,
		Obstacle       [[maybe_unused]] = Def::BitMaskPos5,
		BackGround     [[maybe_unused]] = Def::BitMaskPos6,
		OtherInanimate [[maybe_unused]] = Def::BitMaskPos7,

		Ui             [[maybe_unused]] = Def::BitMaskPos8,

		Camera         [[maybe_unused]] = Def::BitMaskPos9,
	};

	// どのような描画を行うのかを設定するTypeID：Bitフラグで複数指定可能
	enum
	{
		eDrawTypeLit           = Def::BitMaskPos1,
		eDrawTypeUnLit         = Def::BitMaskPos2,
		eDrawTypeBright        = Def::BitMaskPos3,
		eDrawTypeUI            = Def::BitMaskPos4,
		eDrawTypeDepthOfShadow = Def::BitMaskPos5,
	};

	KdGameObject() {}
	virtual ~KdGameObject() { Release(); }

	// 生成される全てに共通するパラメータに対する初期化のみ
	virtual void Init() {}

	virtual void PreUpdate() {}
	virtual void Update() {}
	virtual void PostUpdate() {}

	// それぞれの状況で描画する関数
	virtual void GenerateDepthMapFromLight() {}
	virtual void PreDraw() {}
	virtual void DrawLit() {}
	virtual void DrawUnLit() {}
	virtual void DrawBright() {}
	virtual void DrawSprite() {}

	virtual void DrawDebug();

	virtual void SetAsset(const std::string&) {}

	virtual void SetPos(const Math::Vector3& pos) { m_mWorld.Translation(pos); }
	virtual Math::Vector3 GetPos() const { return m_mWorld.Translation(); }

	// Entity Id Getter
	inline const auto GetEntityId() const noexcept { return m_entityId; }

	virtual void OnHit()                                    {}
	virtual void OnHit([[maybe_unused]] const int   damage) {}
	virtual void OnHit([[maybe_unused]] const float damage) {}

	// 拡大率を変更する関数
	void SetScale(float scalar);
	virtual void SetScale(const Math::Vector3& scale);
	virtual Math::Vector3 GetScale() const;

	const Math::Matrix& GetMatrix() const { return m_mWorld; }

	virtual bool IsExpired() const { return m_isExpired; }

	virtual bool IsVisible()	const { return false; }
	virtual bool IsRideable()	const { return false; }

	// 視錐台範囲内に入っているかどうか
	virtual bool CheckInScreen(const DirectX::BoundingFrustum&) const { return false; }

	// カメラからの距離を計算
	virtual void CalcDistSqrFromCamera(const Math::Vector3& camPos);

	float GetDistSqrFromCamera() const { return m_distSqrFromCamera; }

	UINT GetDrawType() const { return m_drawType; }

	bool Intersects(const KdCollider::SphereInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults);
	bool Intersects(const KdCollider::BoxInfo& targetBox, std::list<KdCollider::CollisionResult>* pResults);
	bool Intersects(const KdCollider::RayInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults);
protected:

	void Release() {}

	// 描画タイプ・何の描画を行うのかを決める / 最適な描画リスト作成用
	UINT m_drawType = 0;

	// カメラからの距離
	float m_distSqrFromCamera = 0;

	// 存在消滅フラグ
	bool m_isExpired = false;

	// 3D空間に存在する機能
	Math::Matrix	m_mWorld;

	// 当たり判定クラス
	std::unique_ptr<KdCollider> m_pCollider = nullptr;

	// デバッグ情報クラス
	std::unique_ptr<KdDebugWireFrame> m_pDebugWire = nullptr;

	// Entity Id Value (Initialization UnKnown)
	Id m_entityId{ Id::UnKnown };
};
