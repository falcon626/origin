#pragma once

class BaseScene
{
public :

	BaseScene() { Init(); }
	virtual ~BaseScene() {}

	void PreUpdate();
	void Update(const float deltaTime);
	void PostUpdate();

	void PreDraw();
	void Draw();
	void DrawSprite();
	void DrawDebug();

	// オブジェクトリストを取得
	const std::list<std::shared_ptr<KdGameObject>>& GetObjList()
	{
		return m_objList;
	}
	
	// オブジェクトリストに追加
	void AddObject(const std::shared_ptr<KdGameObject>& obj)
	{
		m_objList.push_back(obj);
	}

	// Add ObjctList Function (Emplace_Back)
	template <class T, typename... Args>
	inline auto AddObjList(Args&&... args) noexcept
	{
		static_assert(std::is_base_of<KdGameObject, T>::value, "T Must Be Derived From KdGameObject");
		m_objList.emplace_back(std::make_shared<T>(std::forward<Args>(args)...));
	}

	// Add ObjctList And Create WeakPtr Function (Push_Back)
	template <class _T, typename... _Args>
	inline auto AddObjListAndWeak(std::weak_ptr<_T>& wpObj, _Args&&... args) noexcept
	{
		static_assert(std::is_base_of<KdGameObject, _T>::value, "_T Must Be Derived From KdGameObject");
		auto obj = std::make_shared<_T>(std::forward<_Args>(args)...);
		wpObj = obj;
		m_objList.push_back(obj);
	}

	// Add ObjctList And Init Function (Push_Back)
	template <class __T, typename... __Args>
	inline auto AddObjListAndInit(__Args&&... args) noexcept
	{
		static_assert(std::is_base_of<KdGameObject, __T>::value, "__T Must Be Derived From KdGameObject");
		auto obj = std::make_shared<__T>(std::forward<__Args>(args)...);
		obj->Init();
		m_objList.push_back(obj);
	}

	// Add ObjctList, Init And Create WeakPtr Function (Push_Back)
	template <class __T_, typename... __Args_>
	inline auto AddObjListInitAndWeak(std::weak_ptr<__T_>& wpObj, __Args_&&... args) noexcept
	{
		static_assert(std::is_base_of<KdGameObject, __T_>::value, "__T_ Must Be Derived From KdGameObject");
		auto obj = std::make_shared<__T_>(std::forward<__Args_>(args)...);
		obj->Init();
		wpObj = obj;
		m_objList.push_back(obj);
	}
protected :

	// 継承先シーンで必要ならオーバーライドする
	virtual void Event();
	virtual void Init();

	// Srot Priority
	inline static const auto CompareById(const std::shared_ptr<KdGameObject>& a, const std::shared_ptr<KdGameObject>& b) noexcept
	{ return static_cast<size_t>(a->GetEntityId()) < static_cast<size_t>(b->GetEntityId()); }

	virtual void PreLoad() noexcept {}

	// Create WeakPtr
	template <class Ty>
	inline const std::shared_ptr<Ty> WeakPtrIsExpired(const std::weak_ptr<Ty>& wpObj) noexcept
	{
		if (wpObj.expired()) return nullptr;
		else return wpObj.lock();
	}

	// 全オブジェクトのアドレスをリストで管理
	std::list<std::shared_ptr<KdGameObject>> m_objList;
};