#pragma once

/*#pragma comment (lib, "..\\external\\glfw-3.2.1\\lib-vc2015\\glfw3.lib")
#pragma comment (lib, "..\\external\\glew-2.1.0\\lib\\Release\\Win32\\glew32.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "..\\external\\Assimp\\lib\\Win32\\assimp-vc140-mt.lib")*/


#ifdef _WIN64
#pragma comment(lib,"..\\external\\glfw-3.3\\libs\\x64\\glfw3.lib")
#else
#pragma comment(lib,"..\\external\\glfw-3.3\\libs\\x86\\glfw3.lib")
#endif // WIN64

#ifdef _WIN64
#pragma comment(lib,"..\\external\\glew-2.1.0\\lib\\Release\\x64\\glew32.lib")
#else
#pragma comment(lib,"..\\external\\glew-2.1.0\\lib\\Release\\Win32\\glew32.lib")
#endif // WIN64

#ifdef _WIN64
#pragma comment(lib,"..\\external\\Assimp\\libs\\x64\\assimp-vc140-mt.lib")
#else
#pragma comment(lib,"..\\external\\Assimp\\libs\\x86\\assimp-vc140-mt.lib")
#endif // WIN64

#pragma comment(lib,"opengl32.lib")

#pragma comment(lib,"glu32.lib")

#pragma comment(lib,"assimp-vc140-mt.lib")