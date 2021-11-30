#include "App.h"    
#include "WinException.h"

int WINAPI WinMain(
	_In_ HINSTANCE,          //hIsntance
	_In_opt_ HINSTANCE,      //hLastInstance
	_In_ LPSTR,              //lpCmdLine
	_In_ int)                //nCmdShow
{
	try
	{
		return App().run();
	}
	catch (BasicException & ex)
	{
		MessageBoxA(nullptr, ex.what(), ex.getType(), MB_ICONEXCLAMATION | MB_OK);
	}
	catch (std::exception & ex)
	{
		MessageBoxA(nullptr, ex.what(), "Standard Exception", MB_ICONEXCLAMATION | MB_OK);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "Unidentified Exception", "Unknown Exception", MB_ICONEXCLAMATION | MB_OK);
	}

	return -1;
}