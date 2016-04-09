#ifndef _TNG_SCENERENDER__H_
#define _TNG_SCENERENDER__H_
#include "rendercontex.h"
#include "rendermaterial.h"
#include "mesh.h"
#include "MathGeoLib/Math/float3x4.h"
namespace tng
{
	/// simple scene render
	/// do not share any data with scene
	class GFX_API SceneRender
	{
	public:
		SceneRender();

		//will be called in RenderService loop
		virtual void		Render();
		void				RenderEarlyZ();
		void				RenderTransparent(){}
		void				RenderUI();
		void				RenderAfterUI();
		void				RenderOpaque();
		virtual void		AddMesh(Material::layer layer, const RenderElem& re);

		void				ClearAll();

		///main render contex
		RenderContexPtr		rendercontex_;

		float3x4			viewMatrix_;
		float4x4			projectionMatrix_;
	protected:

		RenderLayer			back_ground_;
		RenderLayer			terrain_;
		RenderLayer			opaque_;
		RenderLayer			transparent_;
		RenderLayer			fore_ground_;
		RenderLayer			ui_;
		RenderLayer			after_ui_;
	};
}

#endif