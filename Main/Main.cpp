#include <memory>           
#include <windows.h>

#include <App/Application.h>

#if defined(DEBUG) || defined(_DEBUG)                                                                                                                                                            
#define _CRTDBG_MAP_ALLOC          
#include <cstdlib>             
#include <crtdbg.h>               
#endif    

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
	Application app(hInstance, nCmdShow);
	app.Run();
	return 0;
}