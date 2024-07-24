
namespace Formula // Convenience Functions
{
	// Simple Rand Function
	[[nodiscard(L"Not Random Return Value")]] int Rand() noexcept
	{
		thread_local std::random_device rd;
		thread_local std::mt19937 mt(rd());

		std::uniform_int_distribution<int> dist(Def::IntZero, RAND_MAX);
		return dist(mt);
	}

	template<typename T> // Rand Function
	[[nodiscard(L"Not Random Return Value")]] decltype(auto) Rand
	(_In_ const T& min, _In_ const T& max) noexcept
	{
		thread_local std::random_device rd;
		thread_local std::mt19937 mt(rd());

		if constexpr (std::is_integral<T>::value)
		{
			std::uniform_int_distribution<T> dist(min, max);
			return dist(mt);
		}
		else if constexpr (std::is_floating_point<T>::value)
		{
			std::uniform_real_distribution<T> dist(min, max);
			return dist(mt);
		}
		else static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value, "Not A Valid Numeric Type");
	}

	template<typename _T> // Rand Function With Exclusion Function
	[[nodiscard(L"Not Random Return Value")]] decltype(auto) Rand
	(_In_ const _T& min, _In_ const _T& max, _In_ const std::initializer_list<_T>& exclusion) noexcept
	{
		thread_local std::random_device rd;
		thread_local std::mt19937 mt(rd());

		// Exclusion Check Function (Lambda)
		auto isExcluded = [&exclusion](const _T& value)
			{ return std::find(exclusion.begin(), exclusion.end(), value) != exclusion.end(); };

		if constexpr (std::is_integral<_T>::value)
		{
			std::uniform_int_distribution<_T> dist(min, max);
			_T value;
			do {
				value = dist(mt);
			} while (isExcluded(value));
			return value;
		}
		else if constexpr (std::is_floating_point<_T>::value)
		{
			std::uniform_real_distribution<_T> dist(min, max);
			_T value;
			do {
				value = dist(mt);
			} while (isExcluded(value));
			return value;
		}
		else static_assert(std::is_integral<_T>::value || std::is_floating_point<_T>::value, "Not A Valid Numeric Type");
	}

	// Item <Name, Percentage> Return Item Name
	const std::string Lottery(const std::unordered_map<std::string, double>& items) noexcept
	{
		if (items.empty()) return std::string{ " " };

		// Create CDF
		std::vector<std::pair<std::string, double>> cdf;
		auto cumulative{ Def::DoubleZero };
		for (const auto& item : items)
		{
			cumulative += item.second;
			cdf.emplace_back(item.first, cumulative);
		}

		auto randValue{ Rand(Def::DoubleZero,Def::DoubleOne) };

		// Select Item (Lambda)
		auto it = std::lower_bound(cdf.begin(), cdf.end(), randValue,
			[](const std::pair<std::string, double>& element, double value) 
			{ return element.second < value; });

		return it->first;
	}

	// Items Size == Percentages Size. Return ItemType
	template<typename ItemType, typename PercentageType>
	const auto Lottery(const std::vector<ItemType>& ids, const std::vector<PercentageType>& percentages) noexcept
	{
		if (ids.empty() || percentages.empty() || (ids.size() != percentages.size())) return static_cast<ItemType>(nullptr);
		std::unordered_map<ItemType, PercentageType> items;
		for (auto i{Def::SizTZero}; i < ids.size(); ++i)
			items.insert(ids[i], percentages[i]);

		// Create CDF
		std::vector<std::pair<std::string, PercentageType>> cdf;
		auto cumulative{ Def::DoubleZero };
		for (const auto& item : items)
		{
			cumulative += item.second;
			cdf.emplace_back(item.first, cumulative);
		}

		PercentageType sun{};
		for (const auto& percentage : percentages)
			sun += percentage;

		auto randValue{ Rand<PercentageType>(Def::DoubleZero, sun) };

		// Select Item (Lambda)
		auto it = std::lower_bound(cdf.begin(), cdf.end(), randValue,
			[](const std::pair<std::string, double>& element, double value)
			{ return element.second < value; });

		return it->first;
	}

	inline namespace Collider // So Deep Nest
	{
		using collider_type = uint32_t;

		typedef union _tagResult 
		{
			struct {
				bool isHit_{ false };
				Math::Vector3 hitPos_{ Def::Vec3 };
			}ray;

			struct {
				bool isHit_{ false };
				Math::Vector3 hitDir_{ Def::Vec3 };
			}sphere;
		}Result, &R_Result;

		// KdGameObject Only Ray Function
		template<class T = KdGameObject>
		static auto Ray(_Inout_ R_Result result,
			_In_ const std::list<std::shared_ptr<KdGameObject>>& objList,
			_In_ const Math::Vector3& rayDirection,              _In_ const collider_type& hitType,
			_In_ const Math::Vector3& startPos,                  _In_ const float& rayRange,
			_In_ const Math::Vector3& correctionPos = Def::Vec3, _In_ const float& enableStepHigh = Def::FloatZero,
			_In_ const bool& isSetMaxLength         = false,     _In_ const float& maxLength      = Def::FloatZero,
			_In_opt_ const T* const pThat = nullptr) noexcept
		{
			if(!!pThat) static_assert(std::is_base_of<KdGameObject, T>::value, "T Must Be Derived From KdGameObject");

			KdCollider::RayInfo rayInfo;
			rayInfo.m_pos    = startPos;
			rayInfo.m_dir    = rayDirection;
			rayInfo.m_pos   += correctionPos;
			rayInfo.m_pos.y += enableStepHigh;
			rayInfo.m_range  = rayRange + enableStepHigh + correctionPos.y;
			if (isSetMaxLength && maxLength < rayInfo.m_range) rayInfo.m_range = maxLength;
			rayInfo.m_type   = hitType;

			rayInfo.m_dir.Normalize();

			result.ray.isHit_  = false;
			result.ray.hitPos_ = Def::Vec3;

			std::list<KdCollider::CollisionResult> retRayList;
			for (decltype(auto) obj : objList)
			{
				if (obj.get() == pThat) continue;
				obj->Intersects(rayInfo, &retRayList);
			}

			auto maxOverLap{ Def::FloatZero };
			for (decltype(auto) ret : retRayList)
			{
				if (maxOverLap < ret.m_overlapDistance)
				{
					maxOverLap         = ret.m_overlapDistance;
					result.ray.isHit_  = true;
					result.ray.hitPos_ = ret.m_hitPos;
				}
			}
		}

		// KdGameObject Only Ray Debug Function
		template<class T = KdGameObject>
		static auto Ray(_In_opt_ const std::unique_ptr<KdDebugWireFrame>& upDebug,
			_Inout_ R_Result result,
			_In_ const std::list<std::shared_ptr<KdGameObject>>& objList,
			_In_ const Math::Vector3& rayDirection,              _In_ const collider_type& hitType,
			_In_ const Math::Vector3& startPos,                  _In_ const float& rayRange,
			_In_ const Math::Vector3& correctionPos = Def::Vec3, _In_ const float& enableStepHigh = Def::FloatZero,
			_In_ const bool& isSetMaxLength = false,             _In_ const float& maxLength = Def::FloatZero,
			_In_opt_ const T* const pThat = nullptr) noexcept
		{
			if (!!pThat) static_assert(std::is_base_of<KdGameObject, T>::value, "T Must Be Derived From KdGameObject");

			KdCollider::RayInfo rayInfo;
			rayInfo.m_pos = startPos;
			rayInfo.m_dir = rayDirection;
			rayInfo.m_pos += correctionPos;
			rayInfo.m_pos.y += enableStepHigh;
			rayInfo.m_range = rayRange + enableStepHigh + correctionPos.y;
			if (isSetMaxLength && maxLength < rayInfo.m_range) rayInfo.m_range = maxLength;
			rayInfo.m_type = hitType;

			rayInfo.m_dir.Normalize();

			result.ray.isHit_ = false;
			result.ray.hitPos_ = Def::Vec3;

			std::list<KdCollider::CollisionResult> retRayList;
			for (decltype(auto) obj : objList)
			{
				if (obj.get() == pThat) continue;
				obj->Intersects(rayInfo, &retRayList);
			}

			auto lineColor{ kGreenColor };

			auto maxOverLap{ Def::FloatZero };
			for (decltype(auto) ret : retRayList)
			{
				if (maxOverLap < ret.m_overlapDistance)
				{
					maxOverLap = ret.m_overlapDistance;
					result.ray.isHit_ = true;
					result.ray.hitPos_ = ret.m_hitPos;
					lineColor = kRedColor;
				}
			}

			if(!!upDebug) upDebug->AddDebugLine(rayInfo.m_pos, rayInfo.m_range, lineColor);
		}

		// Assignment RayInfo
		static auto Ray(_Inout_ KdCollider::RayInfo& rayInfoResult,
			_In_ const Math::Vector3& rayDirection,              _In_ const collider_type& hitType,
			_In_ const Math::Vector3& startPos,                  _In_ const float& rayRange,
			_In_ const Math::Vector3& correctionPos = Def::Vec3, _In_ const float& enableStepHigh = Def::FloatZero) noexcept
		{
			rayInfoResult.m_pos    = startPos;
			rayInfoResult.m_dir    = rayDirection;
			rayInfoResult.m_pos   += correctionPos;
			rayInfoResult.m_pos.y += enableStepHigh;
			rayInfoResult.m_range  = rayRange + enableStepHigh + correctionPos.y;
			rayInfoResult.m_type   = hitType;

			rayInfoResult.m_dir.Normalize();
		}

		// KdGameObject Only Sphere Function
		template<class _T = KdGameObject>
		static auto Sphere(_Inout_ R_Result result,
			_In_ const std::list<std::shared_ptr<KdGameObject>>& objList,
			_In_ const collider_type& hitType, _In_ const Math::Vector3& centerPos,
			_In_ const float& sphereRadius,    _In_ const Math::Vector3& correctionPos = Def::Vec3,
			_In_opt_ const _T* const pThat = nullptr) noexcept
		{
			if (!!pThat) static_assert(std::is_base_of<KdGameObject, _T>::value, "_T Must Be Derived From KdGameObject");

			KdCollider::SphereInfo sphereInfo;
			sphereInfo.m_sphere.Center = centerPos + correctionPos;
			sphereInfo.m_sphere.Radius = sphereRadius;
			sphereInfo.m_type          = hitType;

			result.sphere.isHit_  = false;
			result.sphere.hitDir_ = Def::Vec3;

			std::list<KdCollider::CollisionResult> retSphereList;
			for (decltype(auto) obj : objList)
			{
				if (obj.get() == pThat) continue;
				obj->Intersects(sphereInfo, &retSphereList);
			}

			auto maxOverLap{ Def::FloatZero };
			for (decltype(auto) ret : retSphereList)
			{
				if (maxOverLap < ret.m_overlapDistance)
				{
					maxOverLap            = ret.m_overlapDistance;
					result.sphere.isHit_  = true;
					result.sphere.hitDir_ = ret.m_hitDir;
				}
			}
		}

		// KdGameObject Only Sphere Debug Function
		template<class _T = KdGameObject>
		static auto Sphere(_In_opt_ const std::unique_ptr<KdDebugWireFrame>& upDebug,
			_Inout_ R_Result result,
			_In_ const std::list<std::shared_ptr<KdGameObject>>& objList,
			_In_ const collider_type& hitType, _In_ const Math::Vector3& centerPos,
			_In_ const float& sphereRadius,    _In_ const Math::Vector3& correctionPos = Def::Vec3,
			_In_opt_ const _T* const pThat = nullptr) noexcept
		{
			if (!!pThat) static_assert(std::is_base_of<KdGameObject, _T>::value, "_T Must Be Derived From KdGameObject");

			KdCollider::SphereInfo sphereInfo;
			sphereInfo.m_sphere.Center = centerPos + correctionPos;
			sphereInfo.m_sphere.Radius = sphereRadius;
			sphereInfo.m_type          = hitType;

			result.sphere.isHit_  = false;
			result.sphere.hitDir_ = Def::Vec3;

			std::list<KdCollider::CollisionResult> retSphereList;
			for (decltype(auto) obj : objList)
			{
				if (obj.get() == pThat) continue;
				obj->Intersects(sphereInfo, &retSphereList);
			}

			auto sphereColor{ kGreenColor };

			auto maxOverLap{ Def::FloatZero };
			for (decltype(auto) ret : retSphereList)
			{
				if (maxOverLap < ret.m_overlapDistance)
				{
					maxOverLap            = ret.m_overlapDistance;
					result.sphere.isHit_  = true;
					result.sphere.hitDir_ = ret.m_hitDir;
					sphereColor           = kRedColor;
				}
			}

			if (!!upDebug) upDebug->AddDebugSphere(sphereInfo.m_sphere.Center,sphereInfo.m_sphere.Radius,sphereColor);
		}

		// Assignment SphereInfo
		static auto Sphere(_Inout_ KdCollider::SphereInfo& sphereInfo,
			_In_ const collider_type& hitType, _In_ const Math::Vector3& centerPos,
			_In_ const float& sphereRadius,   _In_ const Math::Vector3& correctionPos) noexcept
		{
			sphereInfo.m_sphere.Center = centerPos + correctionPos;
			sphereInfo.m_sphere.Radius = sphereRadius;
			sphereInfo.m_type          = hitType;
		}
	} // Inline Name Space Collider
} // Name Space Formula