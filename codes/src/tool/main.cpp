#include "tool/ModelImport.h"
#include "tool/TextureImport.h"
#include "graphics/graphics.h"
#include "graphics/window.h"
#include "engine/engine.h"
#include "glesrender/gles_render.h"
#include "core/log.h"
#include "graphics/camera.h"
#include "core/path.h"
#include "modelimport/port.h"
#include "graphics/texture.h"
#include "fstream"


using namespace tng;
int main(int argc, const char **argv)
{
	//-t default_tex.tga default_tex.tex
	tng::Log* log = new tng::ConsoleLog;
	tng::Log::SetLog(log);
	graphics::InitGraphics();
	string exename,exepath,exeext;
	Path::GetExePath().Split(exepath,exename,exeext);
	Path::SetWorkPath(exepath);
	GFXService* gfx = GFXService::CreateGraphics();
	StreamLoader* loader = ResourceManger::GetInstance()->AddLoader(LocalFilesLoader);

	InitModelImpPlugin();
	if (argc==4)
	{
		string src = argv[2];
		string dest = argv[3];
		//导入贴图
		if (strcmp(argv[1],"-t")==0)
		{
			ImportTexture(src,dest);
		}
		else if (strcmp(argv[1],"-m")==0)//导入模型
		{
			ImportModel(src,dest);
		}
	}
	else
	{
		Log::GetLog()->Printf(Log::ERROR_CHN,"argc is not 3");
	}
	return 0;
}
	
