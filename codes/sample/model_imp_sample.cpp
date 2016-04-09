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
MAIN
{
#if defined(TNG_ANDROID)
	app_dummy();
	OS::state__ = state;
	FileSystem::assetManager__ = state->activity->assetManager;
#endif
	tng::Log* log = new tng::ConsoleLog;
	tng::Log::SetLog(log);
	graphics::InitGraphics();
	

	string exename,exepath,exeext;
	Path::GetExePath().Split(exepath,exename,exeext);
	Path::SetWorkPath(exepath);
	GFXService* gfx = GFXService::CreateGraphics();
	StreamLoader* loader = ResourceManger::GetInstance()->AddLoader(LocalFilesLoader);
	ResourceManger::GetInstance()->SetAssetPath("../SampleAsset/");
	InitModelImpPlugin();

	gfx->CreateMainRenderWindow("gfx window",200,200,NORMAL_STYLE);
	GLESRender* r = new GLESRender;

	const char* jss= "{\"bufferx\": 512,"
		"\"buffery\": 512,"
		"\"color\": 32,"
		"\"depth\": 24,"
		"\"stencil\": 8,"
		"\"vsync\": false,"
		"\"earlyz\": true,"
		"\"multisampling\": 0}";
	tng::JsonDocument jd;
	jd.Parse<0>(jss);

	r->Initialize(jd);
	gfx->SetRender(r);

	AutoPtr<FileTexture> texture = FileTexture::LoadResource("asset:/test.tga");

	AutoPtr<FileModel> model = FileModel::LoadResource("asset:/vv.FBX");

	ModelInstance mi;


	

	mi.SetModel(model);
	mi.AddMaterial("emissive_texture");
	mi.GetMaterialParam(0).vector_["ColorParam"] = float4(1, 1, 1, 1);

	mi.GetMaterialParam(0).texture_obj_["TextureParam"] = texture;
	ScenePtr scene =gfx->GetSceneManger()->CreateEmptyScene("empty_scene");
	SceneNode* node = new SceneNode();
	node->SetName("box");
	node->Attache(scene);
	node->AttacheModel(mi);

	Camera* camera = new Camera;
	camera->SetRender(0,true);
	camera->SetFrustum(0.1f,10000.0f,phi/2.0f,1.0f);
	camera->SetLocalTranslation(float3(0,-500,0));
	camera->SetLookAt(float3(0,0,0),float3(0,0,1));
	camera->SetName("camera");
	camera->Attache(scene);

	scene->SetMainDisplay();
	Log::GetLog()->Printf(Log::SYS_CHN,"go....");
	Engine::GetInstance()->MainLoop();
	Engine::Destory();
	delete log;
	ReleaseModelImpPlugin();
#ifdef TNG_ANDROID
	ANativeActivity_finish(state->activity);
#else
	return 1;
#endif // TNG_ANDROID
}