#include "graphics/camera.h"

namespace tng
{
	Camera::Camera():render_texture_(0),main_camera_(false){
		UpdateDirty();
	}
	Camera::~Camera(){}

	void Camera::UpdateDirty()
	{
		if (dirty_transform_)
		{
			SceneNode::UpdateDirty();
			frustum_.SetWorldMatrix(GetWorldTransform());
		}
	}

	void Camera::SetLookAt(const float3& pos,const float3& up)
	{
		UpdateDirty();
#ifdef MATH_VEC_IS_FLOAT4
		frustum_.front.Float3Part() = pos - frustum_.pos.Float3Part();
		frustum_.front.Normalize();
		SetLookDir(frustum_.front.Float3Part(),up);
#else
		frustum_.front = pos - frustum_.pos;
		frustum_.front.Normalize();
		SetLookDir(frustum_.front,up);
#endif // MATH_VEC_IS_FLOAT4

		
	}
	void Camera::SetLookDir(const float3& dir,const float3& up)
	{
		UpdateDirty();
#ifdef MATH_VEC_IS_FLOAT4
		frustum_.front.Float3Part() = dir;
		frustum_.up.Float3Part() = up;
#else
		frustum_.front = dir;
		frustum_.up = up;
#endif // MATH_VEC_IS_FLOAT4

		
		transform_.transform_ = frustum_.WorldMatrix();
		if (father_&&(transform_code_&EffectByFather))
		{
			float3x4 fmat = FatherSceneNode()->GetWorldTransform();
			transform_.transform_ = fmat.Inverted()*transform_.transform_;
		}
		transform_.updateFromMat();
	}
	void Camera::SetFrustum(f32 nearplane,f32 farplane,f32 horizontalFov,f32 aspect)
	{
		frustum_.type = PerspectiveFrustum;
		frustum_.nearPlaneDistance = nearplane;
		frustum_.farPlaneDistance = farplane;
		frustum_.SetHorizontalFovAndAspectRatio(horizontalFov,aspect);
	}
	void Camera::OnAspect(f32 aspect)
	{
		frustum_.SetHorizontalFovAndAspectRatio(frustum_.horizontalFov, aspect);
	}
	void Camera::SetFrustumOrth(f32 nearplane,f32 farplane,f32 orthographicWidth,f32 aspect)
	{
		frustum_.type = OrthographicFrustum;
		frustum_.nearPlaneDistance = nearplane;
		frustum_.farPlaneDistance = farplane;
		frustum_.orthographicWidth = orthographicWidth;
		frustum_.orthographicHeight = orthographicWidth*aspect;
	}

	void Camera::UpdateActive(f32 deltaTime)
	{
		SceneNode::UpdateActive(deltaTime);
	}

	void SceneNode::UpdateIfInCamera(class Camera* camera,f32 delta_time)
	{
		for (s32 m =0;m<model_.GetMeshCount();m++)
		{
			Mesh* mesh = model_.GetMesh(m).get();
			if(mesh->memory_mesh_&&mesh->memory_mesh_->vertex_factory_)
			{
				mesh->memory_mesh_->vertex_factory_->OnActive(this,model_.GetMaterialParam(m));
			}
		}
	}

	void DefaultModelVertexFactory::OnActive(SceneNode* node,ShaderParam& param)
	{
		param.matrix_["world_matrix"] = node->GetWorldTransform();
	}
}