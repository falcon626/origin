#pragma once

namespace Container
{
	// Copy List To Vector
	template<typename T>
	std::vector<T> ListToVector(const std::list<T>& lst) {
		return std::vector<T>(lst.begin(), lst.end());
	}

	// Copy Vector To List
	template<typename T>
	std::list<T> VectorToList(const std::vector<T>& vec) {
		return std::list<T>(vec.begin(), vec.end());
	}

	// Inversion Vector
	template<typename T>
	void ReverseVector(std::vector<T>& vec) {
		std::reverse(vec.begin(), vec.end());
	}

	// Inversion List
	template<typename T>
	void ReverseList(std::list<T>& lst) {
		lst.reverse();
	}
}