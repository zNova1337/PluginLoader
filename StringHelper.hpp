#pragma once

#undef max
//credits to stackoverflow for this resizable buffer str formatter template
namespace details
{
	template <typename... Args>
	std::unique_ptr<char[]> formatImplS(
		size_t bufSizeGuess,
		char const* formatCStr,
		Args&&... args)
	{
		std::unique_ptr<char[]> buf(new char[bufSizeGuess]);

		size_t expandedStrLen = std::snprintf(buf.get(), bufSizeGuess, formatCStr, args...);

		if (expandedStrLen >= 0 && expandedStrLen < bufSizeGuess)
		{
			return buf;
		}
		else if (expandedStrLen >= 0
			&& expandedStrLen < std::numeric_limits<size_t>::max())
		{
			// buffer was too small, redo with the correct size
			return formatImplS(expandedStrLen + 1, formatCStr, std::forward<Args>(args)...);
		}
		else {
			throw std::runtime_error("snprintf failed with return value: " + std::to_string(expandedStrLen));
		}
	}

	char const* ifStringThenConvertToCharBuf(std::string const& cpp)
	{
		return cpp.c_str();
	}

	template <typename T>
	T ifStringThenConvertToCharBuf(T const& t)
	{
		return t;
	}
}

template <typename... Args>
std::string formatS(std::string const& formatString, Args&&... args)
{
	// unique_ptr<char[]> calls delete[] on destruction
	std::unique_ptr<char[]> chars = details::formatImplS(4096, formatString.c_str(),
		details::ifStringThenConvertToCharBuf(args)...);

	// string constructor copies the data
	return std::string(chars.get());
}