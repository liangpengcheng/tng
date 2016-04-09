#ifndef _TNG_RENDERSERVICE_H_
#define _TNG_RENDERSERVICE_H_
#include "graphics/graphics_macros.h"
#include "engine/engine.h"
#include "graphics/render.h"
#include "graphics/scene.h"
#include "graphics/scenerender.h"
#include <list>
#include <unordered_map>

namespace tng
{
#ifndef NO_RENDER_THREAD
#define RSLOOP "RenderServiceLoop"
#else
	#define RSLOOP "DefaultLoop"
#endif
	class CreateMeshCommand:public Command
	{
	public:
		virtual bool Do();
		MMesh*				mesh_;
		u32					id_;
		bool				dynamic_;
	};
	class UpdateMeshCommand:public CreateMeshCommand
	{
	public :
		virtual bool Do();
		int start_,end_;
		bool position_,uv_,normal_,index_,color_;
		u32					id_;
	};

	class DeleteMeshCommand:public Command
	{
	public:
		virtual bool Do();
		u32 id_;
	};

	class CreateShaderCommand:public Command
	{
	public:
		virtual bool Do();
		u32 id_;
		string vs;
		string ps;
	};
	class DeleteShaderCommand:public Command
	{
	public :
		virtual bool Do();
		u32 id_;
	};
	class CreateTextureCommand:public Command
	{
	public:
		virtual bool Do();
		TextureData*			texture_data_;
		u32						id_;
	};
	
	typedef AutoPtr<HardwareMesh>		HardwareMeshPtr;

	class GFX_API RenderService:public UpdateableService
	{
	public:
		typedef std::unordered_map<u32, HardwareMeshPtr > MeshMap;
		typedef std::unordered_map<u32, ShaderPtr > ShaderMap;
		typedef std::unordered_map<u32, TexPtr > TexMap;


		ENGINE_CLASS_DEF
		RenderService();
		~RenderService();

		static AtomicCounter					init_render_;
		static AtomicCounter					destroy_surface_;
		void									LockRenderService(bool clearlist = false);
		void									BeginRenderScene(Scene* scene);
		void									UnlockRenderService();
		void									SetRender(Render* r){render_ =r;}

		///called in main thread
		u32										GenRenderID();
		///called in main thread
		void									FreeRenderID(u32 id);
		///called in main thread
		///@return mesh id
		u32										CreateMesh(MMesh* mesh,bool dynamic);
		void									DeleteMesh(u32 id);
		///update mesh data
		void									UpdateMesh(u32 id,
			int start,
			int end,
			MMesh* mesh,
			bool position =false,
			bool uv= false,
			bool normal = false,
			bool index = false,
			bool color= false);
		///called in main thread
		///@return shader id
		u32										CreateShader(const string& vs,const string& ps);
		void									DeleteShader(u32 id);

		//called in main thread
		//@return texture id
		u32										CreateTexture(TextureData* tex);
		void									DeleteTexture(u32 id);
		///should only be called in render thread
		void									_AddMesh(u32 id, HardwareMeshPtr mesh);
		void									_DeleteMesh(u32 id);
		HardwareMesh*							GetMesh(u32);
		Shader*									GetShader(u32);
		RenderTexture*							GetTexture(u32);
		void									_AddShader(u32 id, ShaderPtr shader);
		void									_DeleteShader(u32 id);

		void									_AddTexture(u32 id,TexPtr tex);
		void									_DeleteTexture(u32 id);
		
		virtual							void	OnDestory();
	protected:
		std::list<u32>							freeid_list_;
		MeshMap									mesh_;
		ShaderMap								shader_;
		TexMap									textures_;
		// render here;
		virtual void							_update(f32 deltaTime);
		std::list<SceneRender*>					render_list_;

		void									AddCmd(Command* cmd);
#ifndef NO_RENDER_THREAD
	public:
		void									NotifyMe(){
			render_cond_.notify_all();
		}
	protected:
		
		Mutex									list_lock_;
		Mutex									cmd_list_lock_;
		Mutex									id_list_lock_;
		Condition								render_cond_;

#endif // !NO_RENDER_THREAD
		list<Command*>							commands_;
		Render*									render_;
		u32										famer_counter_;
		
	};
}

#endif