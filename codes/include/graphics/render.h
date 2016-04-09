#ifndef _TNG_RENDER_H__
#define _TNG_RENDER_H__
#include "core/types.h"
#include "window.h"
#include "color.h"
#include "graphics_macros.h"
#include "graphics/shader.h"
#include "graphics/rendervertexbuffer.h"
#include "graphics/renderindexbuffer.h"
#include "graphics/renderstate.h"
#include "graphics/rendertexture.h"
#include "graphics/texture.h"
#define MAX_TEXTURE 16
namespace tng
{
	class GFX_API RenderConfig
	{
	public:
		__inline static RenderConfig& GetInstance()
		{
			static RenderConfig cfg;
			return cfg;
		}
		RenderConfig() :back_buffer_sizex_(0),
			back_buffer_sizey_(0),
			depthsize_(24),
			stencilsize_(8),
			colorsize_(32),
			multisampling_(4), vsync_(false){}
		u32			back_buffer_sizex_;
		u32			back_buffer_sizey_;
		u32			depthsize_;
		u32			stencilsize_;
		u32			colorsize_;
		u32			multisampling_;
		bool		vsync_;
		bool		earlyZ_;
		void		Load(JsonObject& doc);
	};

	struct GFX_API HardwareMesh :public RefCountedObject
	{
		vector<VertexBufferPtr>			vertexbuffer_;
		IndexBufferPtr					indexbuffer_;
		VertexFactoryPtr				vertex_factory_;
		void							Bind(class Shader*);
		void							Draw();
#ifdef USE_VAO
		//used in opengl
		unordered_map<Shader*, VertexBufferPtr>					vao_;
#endif
	};

	class GFX_API Render
	{
	public:
		friend class RenderService;
		Render();
		virtual ~Render(){  }
		void Initialize(JsonObject& doc);

		/// don't call this function
		void InitializeDefaultState();

		///set z test enable or not
		__inline void SetZEnable(bool enable){
			if (enable != current_render_state_.depth_enable_)
			{
				current_render_state_.depth_enable_ = enable;
				_setZEnable(enable);
			}
		}
		/// set z write enable or not
		__inline void SetZWriteEnable(bool enable)
		{
			if (enable != current_render_state_.depth_write_enable_)
			{
				current_render_state_.depth_enable_ = enable;
				_setZEnable(enable);
			}
		}
		__inline void SetFillMode(FILL_MODE fm)
		{
			if (fm != current_render_state_.fill_)
			{
				current_render_state_.fill_ = fm;
				_setFillMode(fm);
			}
		}
		__inline void SetDepthFun(DEPTH_FUN depth)
		{
			if (depth != current_render_state_.depthfun_)
			{
				current_render_state_.depthfun_ = depth;
				_setDepthFun(depth);
			}
		}

		__inline void Bind(HardwareMesh* mesh, Shader* shader)
		{
			if (current_mesh_ != mesh || current_shader_ != shader)
			{
				_bindMesh(mesh, shader);
				current_mesh_ = mesh;
				current_shader_ = shader;
			}
		}
		__inline void Bind(RenderTexture* texture, s32 channel, s32 location, sampler_state& state)
		{
			Assert(channel < MAX_TEXTURE);
			if (current_texture_[channel] != texture)
			{
				_bindTexture(texture, channel, location, state);
				current_texture_[channel] = texture;
			}
		}
		virtual void SetColorMask(bool r, bool g, bool b, bool a) = 0;
		virtual void OnDestroySurface() = 0;
		virtual void OnDestroy() = 0;
		virtual void CreateRenderDevice(TngWindow*) = 0;
		virtual void Clear(const Color& color, f32 depth, u32 stencil) = 0;
		virtual void ClearColor(const Color& color, bool ime) = 0;
		virtual void ClearDepth(f32 depth, bool ime) = 0;
		virtual void SetDepthRange(f32 znear, f32 zfar) = 0;
		virtual void ClearStencil(u32 stencil, bool ime) = 0;
		virtual void Swap(TngWindow*) = 0;
		virtual void SetVertexShaderConstantVectorF(const int& reg, float* val, const int& vec4count) = 0;
		virtual void SetPixelShaderConstantVectorF(const int& reg, float* val, const int& vec4count) = 0;
		virtual void SetVertexShaderConstantFloat(const int& reg, float* val) = 0;
		virtual void SetPixelShaderConstantFloat(const int& reg, float* val) = 0;
		virtual void SetVertexShaderConstantMatrixF(const int& reg, float* val, const int& matrixCount) = 0;
		virtual void SetPixelShaderConstantMatrixF(const int& reg, float* val, const int& matrixCount) = 0;
		virtual TexPtr CreateTexture(u32 width, u32 height, PixelFormat::Code format) = 0;

		virtual class SceneRender* CreateSceneRender();
		virtual void SetViewport(const Rect& rect) = 0;

		/// create a shader from string
		/// @param vspro the vertex shader program string
		/// @param pspro the pixel shader program string
		virtual Shader*			_createShader(const string& vspro, const string& pspro) = 0;
		virtual VertexBuffer*	_createVertexBuffer(void* data, u32 count, u32 stridesize, bool dynamic) = 0;
		virtual IndexBuffer*	_createIndexBuffer(void* data, u32 count, bool dynamic) = 0;
		virtual void			_bindMesh(HardwareMesh* mesh, Shader* shader) = 0;
		virtual void			_bindTexture(RenderTexture* texture, s32 channel, s32 location, sampler_state& state) = 0;
		virtual void			_drawMesh(HardwareMesh* mesh) = 0;
		virtual void			_setZEnable(bool z) = 0;
		virtual void			_setZWriteEnable(bool z) = 0;

		virtual void			_setFillMode(FILL_MODE fm) = 0;
		virtual void			_setDepthFun(DEPTH_FUN depth) = 0;

	protected:
		u32						clearmask_;
		RenderState				current_render_state_;
		Shader*					current_shader_;
		HardwareMesh*			current_mesh_;
		RenderTexture*			current_texture_[MAX_TEXTURE];
	};

}

#endif