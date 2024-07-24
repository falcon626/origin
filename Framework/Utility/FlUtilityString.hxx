#pragma once

namespace Str // String Series
{
	[[nodiscard(L"Extension Not Used")]] static const auto FileExtensionSearcher(const std::string& filePath) noexcept
	{
		const auto dotPos{ filePath.rfind('.') };
		if (dotPos != std::string::npos) return filePath.substr(dotPos + Def::SizTOne);

		return std::string{ " " };
	}

	[[nodiscard(L"Result Not Used")]] static const auto ReplaceChar(const std::string& str, char oldChar, char newChar) noexcept
	{
		auto result{ str };

		for (char& c : result) 
			if (c == oldChar) c = newChar;

		return result;
	}

	[[nodiscard(L"Result Not Used")]] static const auto ReplaceString(const std::string& str, const std::string& oldStr, const std::string& newStr) 
	{
		auto result{ str };
		auto pos   { Def::SizTZero };

		while ((pos = result.find(oldStr, pos)) != std::string::npos) 
		{
			result.replace(pos, oldStr.length(), newStr);
			pos += newStr.length();
		}

		return result;
	}
}