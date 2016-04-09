#include "graphics/scenerender.h"
#include "graphics/graphics.h"
#include "graphics/camera.h"
#include "engine/engine.h"
namespace tng
{
	SceneRender::SceneRender() :rendercontex_(NULL)
		, viewMatrix_(float3x4::identity)
		, projectionMatrix_(float4x4::identity){}
	void SceneRender::ClearAll()
	{
		back_ground_.render_list_.clear();
		terrain_.render_list_.clear();
		opaque_.render_list_.clear();
		transparent_.render_list_.clear();
		fore_ground_.render_list_.clear();
		ui_.render_list_.clear();
		after_ui_.render_list_.clear();
	}
	void SceneRender::Render()
	{
		GFXService::GetInstance()->GetRender()->Clear(Color(1.0f, 0, 0, 1), 1, 0);

		GFXService::GetInstance()->GetRender()->SetZEnable(true);
		GFXService::GetInstance()->GetRender()->SetZWriteEnable(true);
		GFXService::GetInstance()->GetRender()->SetDepthFun(Depth_LessEqual);

		if (RenderConfig::GetInstance().earlyZ_)
		{
			RenderEarlyZ();
		}

		RenderOpaque();
		RenderTransparent();
		RenderUI();
		RenderAfterUI();

		rendercontex_->OnFinish(GFXService::GetInstance()->GetRender());

		rendercontex_->SetViewPort(GFXService::GetInstance()->GetRender());
	}
	void SceneRender::RenderUI()
	{
		for (list<RenderElem>::iterator ren = ui_.render_list_.begin(); ren != ui_.render_list_.end(); ren++)
		{

			ren->render(TRANSPARENT_PASS, viewMatrix_, projectionMatrix_);
		}
	}
	void SceneRender::RenderAfterUI()
	{
		for (list<RenderElem>::iterator ren = ui_.render_list_.begin(); ren != ui_.render_list_.end(); ren++)
		{
			if (ren->has_diffuse_)
				ren->render(OPAQUE_DIFFUSE_PASS, viewMatrix_, projectionMatrix_);
		}
		for (list<RenderElem>::iterator ren = ui_.render_list_.begin(); ren != ui_.render_list_.end(); ren++)
		{
			if (ren->has_emissive_)
				ren->render(EMISSIVE_PASS, viewMatrix_, projectionMatrix_);
		}
		for (list<RenderElem>::iterator ren = ui_.render_list_.begin(); ren != ui_.render_list_.end(); ren++)
		{
			ren->render(TRANSPARENT_PASS, viewMatrix_, projectionMatrix_);
		}
	}
	void SceneRender::RenderOpaque()
	{
		for (list<RenderElem>::iterator it = opaque_.render_list_.begin()
			; it != opaque_.render_list_.end();
			it++)
		{
			if (it->has_diffuse_)
				it->render(OPAQUE_DIFFUSE_PASS, viewMatrix_, projectionMatrix_);
		}

		for (list<RenderElem>::iterator it = opaque_.render_list_.begin()
			; it != opaque_.render_list_.end();
			it++)
		{
			if (it->has_emissive_)
				it->render(EMISSIVE_PASS, viewMatrix_, projectionMatrix_);
		}
	}
	void SceneRender::RenderEarlyZ()
	{
		GFXService::GetInstance()->GetRender()->SetColorMask(false, false, false, false);
		for (list<RenderElem>::iterator it = opaque_.render_list_.begin()
			; it != opaque_.render_list_.end();
			it++)
		{
			it->render(EARLY_Z_PASS, viewMatrix_, projectionMatrix_);
		}

		GFXService::GetInstance()->GetRender()->SetZWriteEnable(false);
		GFXService::GetInstance()->GetRender()->SetDepthFun(Depth_Equal);
		GFXService::GetInstance()->GetRender()->SetColorMask(true, true, true, true);
	}

	void SceneRender::AddMesh(Material::layer layer, const RenderElem& re)
	{
		switch (layer)
		{
		case tng::Material::BACK_GROUND_LAYER:
			back_ground_.render_list_.push_back(re);
			break;
		case tng::Material::TERRAIN_LAYER:
			terrain_.render_list_.push_back(re);
			break;
		case tng::Material::OPAQUE_LAYER:
			opaque_.render_list_.push_back(re);
			break;
		case tng::Material::TRANSPARENT_LAYER:
			transparent_.render_list_.push_back(re);
			break;
		case tng::Material::FORE_GROUND_LAYER:
			fore_ground_.render_list_.push_back(re);
			break;
		case tng::Material::UI_LAYER:
			ui_.render_list_.push_back(re);
			break;
		case tng::Material::AFTER_UI_LAYER:
			after_ui_.render_list_.push_back(re);
			break;
		default:
			Assert(0);
			break;
		}
	}

	void LinkedNode::_collectDataToRenderService(SceneRender* sr)
	{
		if (model_.GetMeshCount())
		{
			for (s32 i = 0; i < model_.GetMeshCount(); i++)
			{
				Material* mtl = model_.GetMaterial(i);
				if (mtl&&model_.GetMesh(i)->render_mesh_)
				{
					RenderElem re;
					re.mesh_ = *(model_.GetMesh(i)->render_mesh_);
					//re.mtl_ = mtl->material_;
					re.mtl_param_ = mtl->default_param_;
					if (model_.GetMaterialParamCount())
						re.mtl_param_.Merge(model_.GetMaterialParam(i));
					if (mtl->HasDiffuse())
					{
						re.mtl_[OPAQUE_DIFFUSE_PASS] = GFXService::GetInstance()->GetMaterialManger()->GetShader(mtl, model_.GetMesh(i)->memory_mesh_->vertex_factory_, OPAQUE_DIFFUSE_PASS);
						re.mtl_[OPAQUE_LIGHTING_PASS] = GFXService::GetInstance()->GetMaterialManger()->GetShader(mtl, model_.GetMesh(i)->memory_mesh_->vertex_factory_, OPAQUE_LIGHTING_PASS);
						re.has_diffuse_ = true;
					}
					if (mtl->HasEmissive())
					{
						re.mtl_[EMISSIVE_PASS] = GFXService::GetInstance()->GetMaterialManger()->GetShader(mtl, model_.GetMesh(i)->memory_mesh_->vertex_factory_, EMISSIVE_PASS);
						re.has_emissive_ = true;
					}
					re.mtl_[EARLY_Z_PASS] = GFXService::GetInstance()->GetMaterialManger()->GetShader(mtl, model_.GetMesh(i)->memory_mesh_->vertex_factory_, EARLY_Z_PASS);
					sr->AddMesh(mtl->layer_, re);
				}
			}

		}
		for (SceneObjectMap::iterator it = scene_objects_.begin();
			it != scene_objects_.end();
			it++)
		{
			(*it).second->_collectDataToRenderService(sr);
		}
	}

	void Camera::_collectDataToRenderService(SceneRender* sr)
	{
		OnAspect(float(sr->rendercontex_->GetWidth()) / float(sr->rendercontex_->GetHeight()));
		sr->viewMatrix_ = frustum_.ViewMatrix();
		sr->projectionMatrix_ = frustum_.ProjectionMatrix();
		SceneNode::_collectDataToRenderService(sr);
	}
}