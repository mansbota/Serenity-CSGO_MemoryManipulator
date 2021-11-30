#pragma once
#include <Windows.h>
#include "BasicException.h"

class WinException : public BasicException
{
	HRESULT hr;
public:
	WinException(const std::string& file, int line, HRESULT hr) noexcept;
	const char* what() const noexcept override;
	const char* getType() const noexcept override;
	static std::string translateError(HRESULT hr) noexcept;
};

#define WINLASTEXCEPT WinException(__FILE__, __LINE__, GetLastError())
#define WINEXCEPT(hr) WinException(__FILE__, __LINE__, hr)

