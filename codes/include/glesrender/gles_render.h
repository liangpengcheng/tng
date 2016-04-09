#ifndef _TNG_GLES_H__
#define _TNG_GLES_H__
#include "graphics/render.h"
#include "gles_render_macros.h"
#include "core/arch_detect.h"
#include "core/stl_rebind.h"

namespace tng
{
	class GLES_API GLESRender:public Render
	{
	public:
		GLESRender();
		~GLESRender();
		virtual void OnDestroy();
		virtual void CreateRenderDevice(TngWindow*);
		void Clear(const Color& color, f32 depth, u32 stencil);
		void ClearColor(const Color& color,bool ime)	;
		void ClearDepth(f32 depth,bool ime)				;
		virtual void SetDepthRange(f32 znear,f32 zfar)	;
		void ClearStencil(u32 stencil,bool ime)			;
		void Swap(TngWindow*);
		virtual void OnDestroySurface();
		virtual void SetVertexShaderConstantVectorF(const int& reg, float* val, const int& vec4count);
		virtual void SetPixelShaderConstantVectorF(const int& reg, float* val, const int& vec4count);

		virtual void SetVertexShaderConstantFloat(const int& reg, float* val);
		virtual void SetPixelShaderConstantFloat(const int& reg, float* val);

		virtual void SetVertexShaderConstantMatrixF(const int& reg, float* val, const int& matrixCount);
		virtual void SetPixelShaderConstantMatrixF(const int& reg, float* val, const int& matrixCount);
		
		virtual TexPtr CreateTexture(u32 width,u32 height,PixelFormat::Code format);
		virtual void SetViewport(const Rect& rect);
		Shader*					_createShader(const string& vspro,const string& pspro);
		VertexBuffer*			_createVertexBuffer(void* data,u32 count,u32 stridesize,bool dynamic);
		IndexBuffer*			_createIndexBuffer(void* data,u32 count,bool dynamic);

		virtual void			SetColorMask(bool r,bool g, bool b , bool a);
		virtual void			_bindMesh(HardwareMesh* mesh,Shader* shader);
		virtual void			_bindTexture(RenderTexture* texture, s32 channel, s32 location, sampler_state& state);
		virtual void			_drawMesh(HardwareMesh* mesh);
		virtual void			_setZEnable(bool z);
		virtual void			_setZWriteEnable(bool z);
		virtual void			_setFillMode(FILL_MODE fm);
		virtual void			_setDepthFun(DEPTH_FUN depth);

		bool					_checkExtention(const string& ext);
	protected:
		void					_loadExtentions();
// 		void					_activeState(u32 state);
// 		void					_disableState(u32 state);
// 		void					_activeTextureCoord(u8 t);
// 		void					_disableTextureCoord(u8 t);

		typedef unordered_set<u32>			StateSet;
		typedef unordered_set<string>		ExtentionSet;

		StateSet							active_state_;
		ExtentionSet						extensions_;
		bool								texcoord_active_state_[16];


	};


}

#endif