#include "BasicException.h"
#include <sstream>

BasicException::BasicException(const std::string& file, const int line) noexcept :
	m_file{ file }, m_line{ line } {}

const char* BasicException::getType() const noexcept
{
	return "Basic Exception";
}

std::string BasicException::getOrigin() const noexcept
{
	std::ostringstream str;
	str << "[Line] " << m_line << '\n' << "[File] " << m_file;
	m_whatBuffer = str.str();
	return m_whatBuffer;
}
