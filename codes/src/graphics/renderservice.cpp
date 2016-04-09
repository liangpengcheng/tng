#include "graphics/renderservice.h"
#include "graphics/graphics.h"
#include "core/os.h"
namespace tng
{
	bool CreateMeshCommand::Do()
	{
		if (internal::GetRender())
		{
			//u32 id = internal::GetRenderService()->GenRenderID();

			//VertexBuffer* buffer = internal::GetRender()->_createVertexBuffer(data_,size_,dynamic_);

			//internal::GetRenderService()->_AddVertexBuffer(id_,buffer);
#ifndef NO_RENDER_THREAD
			if (mesh_->using_)
			{
				return false;
			}
			mesh_->using_ = true;
#endif
			HardwareMesh* mesh = new HardwareMesh;
			if (mesh_->vertex_)
			{
				VertexBuffer* buffer = internal::GetRender()->_createVertexBuffer(mesh_->vertex_, mesh_->vertex_number_, sizeof(float3), dynamic_);
				mesh->vertexbuffer_.push_back(buffer);
			}
			if (mesh_->normal_)
			{
				VertexBuffer* buffer = internal::GetRender()->_createVertexBuffer(mesh_->normal_, mesh_->vertex_number_, sizeof(float3), dynamic_);
				mesh->vertexbuffer_.push_back(buffer);
			}
			if (mesh_->uv_)
			{
				VertexBuffer* buffer = internal::GetRender()->_createVertexBuffer(mesh_->uv_, mesh_->vertex_number_, sizeof(float2), dynamic_);
				mesh->vertexbuffer_.push_back(buffer);
			}
			if (mesh_->binormal_)
			{
				VertexBuffer* buffer = internal::GetRender()->_createVertexBuffer(mesh_->binormal_, mesh_->vertex_number_, sizeof(float3), dynamic_);
				mesh->vertexbuffer_.push_back(buffer);
			}
			if (mesh_->tangent_)
			{
				VertexBuffer* buffer = internal::GetRender()->_createVertexBuffer(mesh_->tangent_, mesh_->vertex_number_, sizeof(float3), dynamic_);
				mesh->vertexbuffer_.push_back(buffer);
			}
			if (mesh_->u16Faces_)
			{
				mesh->indexbuffer_ = internal::GetRender()->_createIndexBuffer(mesh_->u16Faces_, mesh_->index_number_, dynamic_);
			}
			else if (mesh_->u32Faces_)
			{
				mesh->indexbuffer_ = internal::GetRender()->_createIndexBuffer(mesh_->u32Faces_, mesh_->index_number_, dynamic_);
			}
			mesh->vertex_factory_ = mesh_->vertex_factory_;
			internal::GetRenderService()->_AddMesh(id_, mesh);
#ifndef NO_RENDER_THREAD
			mesh_->using_ = false;
#endif
		}
		return true;
	}
	bool UpdateMeshCommand::Do(){
#ifndef NO_RENDER_THREAD
		if (mesh_->using_)
		{
			return false;
		}
		mesh_->using_ = true;
#endif
		if (internal::GetRender())
		{
			HardwareMesh* m = internal::GetRenderService()->GetMesh(id_);
			for (s32 i = 0; i < mesh_->vertex_factory_->vertex_decl_.elem_.size(); i++)
			{
				u32 vsize = GetRenderTypeSize(mesh_->vertex_factory_->vertex_decl_.elem_[i].format_);
				if (position_&&mesh_->vertex_factory_->vertex_decl_.elem_[i].type_ == VTYPE_VERTEX)
				{
					m->vertexbuffer_[i]->UpdateBuffer(start_*vsize, (end_ - start_)*vsize,(void**)(&mesh_->vertex_));
				}
				if (uv_&&mesh_->vertex_factory_->vertex_decl_.elem_[i].type_ == VTYPE_TEXCOORD){
					m->vertexbuffer_[i]->UpdateBuffer(start_*vsize, (end_ - start_)*vsize, (void**)(&mesh_->uv_));
				}
				if (normal_&&mesh_->vertex_factory_->vertex_decl_.elem_[i].type_ == VTYPE_NORMAL){
					m->vertexbuffer_[i]->UpdateBuffer(start_*vsize, (end_ - start_)*vsize, (void**)(&mesh_->normal_));
				}
				if (color_&&mesh_->vertex_factory_->vertex_decl_.elem_[i].type_ == VTYPE_COLOR){
					m->vertexbuffer_[i]->UpdateBuffer(start_*vsize, (end_ - start_)*vsize, (void**)(&mesh_->color_));
				}
				if (index_){
					if (mesh_->u16Faces_)
					{
						m->indexbuffer_->UpdateBuffer(0, mesh_->index_number_, (void**)(&mesh_->u16Faces_));
					}
					else if (mesh_->u32Faces_)
					{
						m->indexbuffer_->UpdateBuffer(0, mesh_->index_number_, (void**)(&mesh_->u32Faces_));
						
					}
					
				}
			}
		}
#ifndef NO_RENDER_THREAD
		mesh_->using_ = false;
#endif
		return true;
	}
	bool DeleteMeshCommand::Do()
	{
		internal::GetRenderService()->_DeleteMesh(id_);
		return true;
	}

	bool CreateShaderCommand::Do()
	{
		if (internal::GetRender())
		{
			Shader* shader = internal::GetRender()->_createShader(vs, ps);
			internal::GetRenderService()->_AddShader(id_, shader);
		}
		return true;
	}

	bool DeleteShaderCommand::Do(){
		internal::GetRenderService()->_DeleteShader(id_);
		return true;
	}

	bool CreateTextureCommand::Do()
	{
#ifndef NO_RENDER_THREAD
		if (texture_data_->using_)
		{
			return false;
		}
		texture_data_->using_ = true;
#endif


		TexPtr texture = internal::GetRender()->CreateTexture(texture_data_->head_.width_, texture_data_->head_.height_, PixelFormat::Code(texture_data_->head_.fmt_));
		texture->CreateTexture(texture_data_);
		internal::GetRenderService()->_AddTexture(id_, texture);
#ifndef NO_RENDER_THREAD
		texture_data_->using_ = false;
#endif
		return true;
	}


	RenderService::RenderService() :famer_counter_(0), render_(NULL)
	{

	}
	RenderService::~RenderService()
	{

	}
	AtomicCounter RenderService::init_render_;
	AtomicCounter RenderService::destroy_surface_;
	void RenderService::_update(f32 deltaTime)
	{
		if (Engine::GetInstance()->IsRunning())
		{
#ifndef NO_RENDER_THREAD
			if (destroy_surface_)
			{
				render_->OnDestroySurface();
				destroy_surface_ = 0;
			}
			if (!init_render_)
			{
				Log::GetLog()->Printf(Log::DBG_CHN, "create gles in re-thread");
				OS::Sleep(0);
				init_render_ = 1;
				render_->CreateRenderDevice(GFXService::GetInstance()->GetMainWindow());
				render_->InitializeDefaultState();
			}
			cmd_list_lock_.lock();
			list<Command*> usingcmd;
			for (list<Command*>::iterator it = commands_.begin();
				it != commands_.end();
				it++)
			{
				if ((*it)->Do())
					delete *it;
				else
					usingcmd.push_back(*it);
			}
			commands_.clear();
			commands_ = usingcmd;
			cmd_list_lock_.unlock();

			LockRenderService();
			render_cond_.wait(list_lock_);
#else
			for (list<Command*>::iterator it = commands_.begin();
				it != commands_.end();
				it++)
			{
				if ((*it)->Do())
					delete *it;

			}
			commands_.clear();
#endif
			if (famer_counter_ != GFXService::GetInstance()->GetSceneManger()->GetFrame())
			{
				famer_counter_ = GFXService::GetInstance()->GetSceneManger()->GetFrame();

				for (std::list<SceneRender*>::iterator it = render_list_.begin();
					it != render_list_.end();
					it++)
				{
					(*it)->Render();
				}
				render_list_.clear();
			}
#ifndef NO_RENDER_THREAD
			UnlockRenderService();
#endif
			//
		}
		// 		else
		// 		{
		// 			Log::GetLog()->Printf(Log::DBG_CHN,"skip render when paused");
		// 		}
	}

	void RenderService::LockRenderService(bool clearlist)
	{
#ifndef NO_RENDER_THREAD

		list_lock_.lock();
		if (clearlist)
		{
			render_list_.clear();
		}
#endif
	}
	void RenderService::UnlockRenderService()
	{
#ifndef NO_RENDER_THREAD
		list_lock_.unlock();
#endif
	}

	void RenderService::BeginRenderScene(Scene* scene)
	{
		scene->GetRender()->ClearAll();
		scene->SyncToRenderService();
		render_list_.push_back(scene->GetRender());
	}

	u32 RenderService::GenRenderID()
	{
#ifndef NO_RENDER_THREAD
		AutoLock<Mutex> lock(&id_list_lock_);
#endif
		if (freeid_list_.size())
		{
			std::list<u32>::iterator it = freeid_list_.begin();
			u32 ret = *it;
			freeid_list_.pop_front();
			return ret;
		}
		else
		{
			static u32 id = 1;
			Assert(id != 0);
			return id++;
		}
	}
	void RenderService::OnDestory()
	{
		mesh_.clear();
		textures_.clear();
		shader_.clear();
	}
	void RenderService::FreeRenderID(u32 id)
	{
#ifndef NO_RENDER_THREAD
		AutoLock<Mutex> lock(&id_list_lock_);
#endif
		freeid_list_.push_back(id);
	}

	void RenderService::_AddShader(u32 id, ShaderPtr shader)
	{
		shader_[id] = shader;
	}
	void RenderService::_DeleteShader(u32 id)
	{
		shader_.erase(id);
	}
	void RenderService::_AddTexture(u32 id, TexPtr tex)
	{
		textures_[id] = tex;
	}
	void RenderService::_DeleteTexture(u32 id)
	{
		textures_.erase(id);
	}


	void RenderService::_AddMesh(u32 id, HardwareMeshPtr mesh)
	{
		mesh_[id] = mesh;
	}
	void RenderService::_DeleteMesh(u32 id)
	{
		mesh_.erase(id);
	}

	u32 RenderService::CreateMesh(MMesh* mesh, bool dynamic)
	{
		u32 id = GenRenderID();
		CreateMeshCommand * cmd = new CreateMeshCommand;
		cmd->id_ = id;
		cmd->dynamic_ = dynamic;
		cmd->mesh_ = mesh;
		AddCmd(cmd);
		return id;
	}
	void RenderService::UpdateMesh(u32 id, int start, int end, MMesh* mesh, bool position /* =false */, bool uv/* = false */, bool normal /* = false */, bool index /* = false */, bool color/* =false */)
	{
		UpdateMeshCommand * cmd = new UpdateMeshCommand;
		cmd->id_ = id;
		cmd->start_ = start;
		cmd->end_ = end;
		cmd->mesh_ = mesh;
		cmd->position_ = position;
		cmd->uv_ = uv;
		cmd->normal_ = normal;
		cmd->index_ = index;
		cmd->color_ = color;
		AddCmd(cmd);
	}
	void RenderService::AddCmd(Command* cmd)
	{
#ifndef NO_RENDER_THREAD
		cmd_list_lock_.lock();
		commands_.push_back(cmd);
		cmd_list_lock_.unlock();
#else
		commands_.push_back(cmd);
#endif
	}
	void RenderService::DeleteMesh(u32 id)
	{
		FreeRenderID(id);
		DeleteMeshCommand* cmd = new DeleteMeshCommand;
		cmd->id_ = id;

		AddCmd(cmd);
	}

	u32 RenderService::CreateShader(const string& vs, const string& ps)
	{
		u32 id = GenRenderID();
		CreateShaderCommand* cmd = new CreateShaderCommand;
		cmd->id_ = id;
		cmd->vs = vs;
		cmd->ps = ps;
		AddCmd(cmd);
		return id;
	}
	void RenderService::DeleteTexture(u32 id)
	{

	}
	u32 RenderService::CreateTexture(TextureData* tex)
	{
		u32 id = GenRenderID();
		CreateTextureCommand *cmd = new CreateTextureCommand;
		cmd->id_ = id;
		cmd->texture_data_ = tex;

		AddCmd(cmd);
		return id;
	}

	void RenderService::DeleteShader(u32 id)
	{
		FreeRenderID(id);
		DeleteShaderCommand* cmd = new DeleteShaderCommand;
		cmd->id_ = id;

		AddCmd(cmd);
	}

	void Mesh::CreateRenderMesh(bool dynamic)
	{
		render_mesh_->meshid = internal::GetRenderService()->CreateMesh(memory_mesh_, false);
	}

	void Shader::SetVertexShaderConstantFloat(const int& reg, float* val)
	{
		internal::GetRender()->SetVertexShaderConstantFloat(reg, val);
	}
	void Shader::SetVertexShaderConstantVectorF(const int& reg, float* val, const int& vec4count)
	{
		internal::GetRender()->SetVertexShaderConstantVectorF(reg, val, vec4count);
	}

	void Shader::SetPixelShaderConstantFloat(const int& reg, float* val)
	{
		internal::GetRender()->SetPixelShaderConstantFloat(reg, val);
	}
	void Shader::SetPixelShaderConstantVectorF(const int& reg, float* val, const int& vec4count)
	{
		internal::GetRender()->SetPixelShaderConstantVectorF(reg, val, vec4count);
	}

	void Shader::SetVertexShaderConstantMatrixF(const int& reg, float* val, const int& matrixCount)
	{
		internal::GetRender()->SetVertexShaderConstantMatrixF(reg, val, matrixCount);
	}
	void Shader::SetPixelShaderConstantMatrixF(const int& reg, float* val, const int& matrixCount)
	{
		internal::GetRender()->SetPixelShaderConstantMatrixF(reg, val, matrixCount);
	}

	void DefaultModelVertexFactory::OnActiveGraphicThread(const float3x4& view_mat, const float4x4& proj_mat, const EffectUniforms& uniform, ShaderParam& param)
	{
		EffectUniforms::const_iterator it = uniform.find("mvp_matrix");
		MatrixParamMap::iterator itp = param.matrix_.find("world_matrix");
		if (it != uniform.end() &&
			itp != param.matrix_.end())
		{
			float4x4 mvp = proj_mat*view_mat*itp->second;
			const Uniform& uniform = it->second;
			internal::GetRender()->SetVertexShaderConstantMatrixF(uniform.location_, reinterpret_cast<float*>(&mvp), 1);
			//param.matrix_.erase(itp);
		}
	}
	HardwareMesh* RenderService::GetMesh(u32 id)
	{
		MeshMap::iterator it = mesh_.find(id);
		if (it != mesh_.end())
		{
			return it->second;
		}
		else
		{
			return NULL;
		}
	}
	Shader* RenderService::GetShader(u32 id)
	{
		ShaderMap::iterator it = shader_.find(id);
		if (it != shader_.end())
		{
			return it->second;
		}
		else
		{
			return NULL;
		}
	}
	RenderTexture* RenderService::GetTexture(u32 id)
	{
		TexMap::iterator it = textures_.find(id);
		if (it != textures_.end())
		{
			return it->second;
		}
		else
		{
			return NULL;
		}
	}

	void HardwareMesh::Bind(Shader* shader)
	{
		internal::GetRender()->Bind(this, shader);
	}
	void HardwareMesh::Draw()
	{
		internal::GetRender()->_drawMesh(this);
	}
	void RenderElem::render(RenderPass pass, const float3x4 view_mat, const float4x4& proj_mat)
	{
		HardwareMesh* mesh = internal::GetRenderService()->GetMesh(mesh_.meshid);
		std::unordered_map<RenderPass, MaterialRender>::iterator it = mtl_.find(pass);
		if (it != mtl_.end())
		{
			Shader* shader = internal::GetRenderService()->GetShader(it->second.effect_id_);
			if (mesh&&shader)
			{
				mesh->Bind(shader);
				mesh->vertex_factory_->OnActiveGraphicThread(view_mat, proj_mat, shader->uniforms_, mtl_param_);
				mtl_param_.Active(shader->uniforms_);
				mesh->Draw();
			}
		}


	}

	START_SUB_CLASS(RenderService, UpdateableService)
		END_CLASS
}