#include "glesrender/gles_render.h"
#include "flascc_libGL.cpp"
#include <AS3/AS3.h>
#include <Flash++.h>

// using namespace AS3::ui;
// flash::display::Stage3D s3d;
//flash::display::Stage stage;
//flash::display3D::Context3D ctx3d;
int __libgl_abc__ =0;
namespace tng
{
// 	static var context3DError(void *arg, var as3Args)
// 	{
// 	}
// 	static var initContext3D(void *arg, var as3Args)
// 	{
// 		printf("initContext3D\n");
// 
// 		// We have a context, we just need to configure the backbuffer to fill the
// 		// stage.
// 		ctx3d = s3d->context3D;
// 		ctx3d->enableErrorChecking = true;
// 		ctx3d->configureBackBuffer(stage->stageWidth, stage->stageHeight, 2,
// 			true, false);
// 		return internal::_undefined;
// 	}

	void GLESRender::CreateRenderDevice(TngWindow* mainwnd)
	{
		printf("CreateRenderDevice");

// 		stage = internal::get_Stage();
// 		stage->scaleMode = 0;
// 		stage->align = flash::display::StageAlign::TOP_LEFT;
// 		stage->frameRate = 60;
// 		s3d = var(var(stage->stage3Ds)[0]);
// 		s3d->addEventListener(flash::events::Event::CONTEXT3D_CREATE, Function::_new(initContext3D, NULL));
// 		s3d->addEventListener(flash::events::ErrorEvent::ERROR, Function::_new(context3DError, NULL));
// 		s3d->requestContext3D(flash::display3D::Context3DRenderMode::AUTO,
// 			flash::display3D::Context3DProfile::BASELINE_CONSTRAINED);

// 		inline_as3("import GLS3D.GLAPI;\n"\
// 			"var s3d:Stage3D = stage.stage3Ds[0];");
// 		inline_as3("import GLS3D.GLAPI;\n"\
// 			"s3d.requestContext3D(Context3DRenderMode.AUTO)");
// 		inline_as3("import GLS3D.GLAPI;\n"\
// 			"var ctx3d:Context3D = s3d.context3D");
// 		inline_as3("import GLS3D.GLAPI;\n"\
// 			" ctx3d.configureBackBuffer(%0, %1, 2, true /*enableDepthAndStencil*/ )": : "r"(renderconfig_.back_buffer_sizex_), "r"(renderconfig_.back_buffer_sizex_));
// 		inline_as3("import GLS3D.GLAPI;\n"\
// 			"GLAPI.init(ctx3d, null, stage);" );

	}
	void GLESRender::Swap(TngWindow*)
	{
		printf("swap\n");
		inline_as3("import GLS3D.GLAPI;\n"\
			"GLAPI.instance.context.present()");
	}
}