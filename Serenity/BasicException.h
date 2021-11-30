#pragma once
#include <string>
#include <stdexcept>

class BasicException : public std::exception
{
protected:
	std::string m_file;
	int m_line;
	mutable std::string m_whatBuffer;
	std::string getOrigin() const noexcept;
public:
	BasicException(const std::string& file, const int line) noexcept;
	virtual const char* getType() const noexcept;
};

