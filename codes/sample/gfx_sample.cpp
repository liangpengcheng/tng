
#include "graphics/graphics.h"
 #include "graphics/window.h"
 #include "engine/engine.h"
#include "glesrender/gles_render.h"
 #include "core/core_macros.h"
#include "core/log.h"
 #include "graphics/camera.h"
 #include "core/path.h"
 #include "fstream"
#include "core/stl_rebind.h"
#ifdef TNG_ANDROID
#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>
#include <android/log.h>
#endif
using namespace tng;


class App :public tng::UpdateableService
{
public:
	ENGINE_CLASS_DEF
		ScenePtr scene;
	GLESRender* r;
	bool					Initialize(Loop* loop)TNG_OVERRIDE{
		
		GFXService* gfx = GFXService::GetInstance();
		gfx->CreateMainRenderWindow("gfx window", 200, 200, NORMAL_STYLE);
		r = new GLESRender;

		const char* jss = "{\"bufferx\": 200,"
			"\"buffery\": 200,"
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

		Mesh* mesh = new Mesh;
		//mesh->CreateBox(AABB(vec(-100,-100,-100,1.0f),vec(100,100,100,1.0f)));
		//mesh->CreateSphere(50);
		mesh->CreateCanvas(float2(-1, -1), float2(1, 1));
		Model* model = new Model;
		model->AddMesh(mesh);

		ModelInstance mi;
		mi.SetModel(model);
		mi.AddMaterial("emissive_texture");
		mi.GetMaterialParam(0).vector_["ColorParam"] = float4(1, 1, 1, 1);
		//mi.AddMaterial(material_);
		scene = gfx->GetSceneManger()->CreateEmptyScene("empty_scene");
		SceneNodePtr node = SceneNodePtr(new SceneNode());
		node->SetName("box");
		node->Attache(scene);
		node->AttacheModel(mi);

		AutoPtr<Camera> camera = AutoPtr<Camera>(new Camera);
		camera->SetRender(0, true);
		camera->SetFrustum(0.1f, 10000.0f, phi / 2.0f, 1.0f);
		camera->SetLocalTranslation(float3(0, -500, 0));
		camera->SetLookAt(float3(0, 0, 0), float3(0, 0, 1));
		camera->SetName("camera");
		camera->Attache(scene);

		scene->SetMainDisplay();
		Log::GetLog()->Printf(Log::SYS_CHN, "go....");

		return UpdateableService::Initialize(loop);
	}
	virtual void			OnDestory()TNG_OVERRIDE{
		scene = NULL;
		//delete r;
	}
};

START_SUB_CLASS(App, UpdateableService)
END_CLASS

MAIN
{
	
	tng::Log* log;
#if defined(TNG_ANDROID)
	app_dummy();
	OS::state__ = state;
	FileSystem::assetManager__ = state->activity->assetManager;
#endif
	log = new tng::ConsoleLog;
	tng::Log::SetLog(log);

	graphics::InitGraphics();

	string exename, exepath, exeext;
	Path::GetExePath().Split(exepath, exename, exeext);
	Path::SetWorkPath(exepath);

	GFXService* gfx = GFXService::CreateGraphics();
	App::InitStatic();
	App* service = (App*)Engine::GetInstance()->CreateService("App", "app", "DefaultLoop");

 	Engine::GetInstance()->MainLoop();
	
 	Engine::Destory();
	
 	
#ifdef TNG_ANDROID
	ANativeActivity_finish(state->activity);
#else
	delete log;
	return 1;
#endif // TNG_ANDROID
	
}