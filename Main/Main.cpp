#include <memory>           
#include <windows.h>

#include <App/Application.h>
#include <InstancingvsGeomShadervsVertexShader\Scene.h>
#include <Scene1\Scene.h>

#if defined(DEBUG) || defined(_DEBUG)                                                                                                                                                            
#define _CRTDBG_MAP_ALLOC          
#include <cstdlib>             
#include <crtdbg.h>               
#endif    

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
	Application app(hInstance, nCmdShow);
	app.Add(new EntitiesScene::Scene());
	app.Run();
	return 0;
}

