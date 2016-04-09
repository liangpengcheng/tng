#ifndef __TNG_CAMERA_H__
#define __TNG_CAMERA_H__
#include "scenenode.h"
#include "MathGeoLib/Geometry/Frustum.h"
namespace tng
{
	class GFX_API Camera:public SceneNode
	{
	public:
		Camera();
		~Camera();
		void							SetLookAt(const float3& pos,const float3& up);
		/**
			@param dir the front direction of camera ,should be normalized
			@param up the up direction of camera,should be normalized
		*/
		void							SetLookDir(const float3& dir,const float3& up);

		/**
			set frustum detail this function will set frustum as a PerspectiveFrustum
			@param near near plane
			@param far far plane
			@param horizontalFov x-fov
			@param aspect w/h
		*/
		void							SetFrustum(f32 nearplane,f32 farplane,f32 horizontalFov,f32 aspect);

		/**
			set frustum detail this function will set frustum as a OrthographicFrustum
			@param near near plane
			@param far far plane
			@param horizontalFov x-fov
			@param aspect w/h
		*/
		void							SetFrustumOrth(f32 nearplane,f32 farplane,f32 orthographicWidth,f32 aspect);

		void							OnAspect(f32 aspect);
		
		float3x4						GetViewMatrix()const{return frustum_.ViewMatrix();}
		float4x4						GetProjectMatrix()const{return frustum_.ProjectionMatrix();}
		float4x4						GetViewProjectMatrix()const{return frustum_.ViewProjMatrix();}

		/**
			set render param
			@param render_texture set render target texture id if not render to texture or render to main window,set to 0
			@param main_camera set as main render camera,if this param set to true ,set render_texture to 0
		*/
		void							SetRender(u32 render_texture,bool main_camera){
			render_texture_ = main_camera?0:render_texture;
			main_camera_ = main_camera;
		}

		u32								GetRenderTarget(){return  render_texture_;}
		bool							IsMainCamera(){return main_camera_;}

	protected:
		virtual void					_collectDataToRenderService(class SceneRender* sr);
		virtual void					UpdateDirty();
		virtual void					UpdateActive(f32 delta_time);
		/// frustum_ hold world-transform
		Frustum							frustum_;
		/// set render target texture id if not render to texture or render to main window,set to 0
		u32								render_texture_;
		/// set as main render camera
		bool							main_camera_;

	};
}
#endif