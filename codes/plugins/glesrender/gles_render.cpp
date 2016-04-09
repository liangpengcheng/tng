#include "glesrender/gles_render.h"
#include "glesrender/gles_shader.h"
#include "glesrender/gles_vertexbuffer.h"
#include "glesrender/gles_render_macros.h"
#include "glesrender/gles_indexbuffer.h"
#include "glesrender/gles_texture.h"
#include "core/log.h"
#include "resource/stream_res.h"
#include "core/stl_rebind.h"
namespace tng
{
	GLESRender::GLESRender(){
		for (s32 i = 0;i < sizeof(texcoord_active_state_);i ++)
		{
			texcoord_active_state_[i] = false;
		}
		  
	}

	GLESRender::~GLESRender(){
		OnDestroy();
	}
	

	

	void GLESRender::_loadExtentions()
	{
		// Set version string
		const GLubyte* pcRender = glGetString(GL_RENDERER);
		Log::GetLog()->Printf(Log::SYS_CHN,"GL Render:  %s\n", pcRender);

		const GLubyte* pcVer = glGetString(GL_VERSION);
		Log::GetLog()->Printf(Log::SYS_CHN,"GL Version:  %s\n", pcVer);

		// Get vendor
		const GLubyte* pcVendor = glGetString(GL_VENDOR);
		Log::GetLog()->Printf(Log::SYS_CHN,"Vendor:  %s\n", pcVendor);
		const GLubyte* glslver= glGetString(GL_SHADING_LANGUAGE_VERSION);
		Log::GetLog()->Printf(Log::SYS_CHN,"glsl version:  %s\n", glslver);

		stringstream ext;
		string       str;

		const GLubyte* pcExt = glGetString(GL_EXTENSIONS);

		ext << pcExt;

		while (ext >> str)
		{
			extensions_.insert( str.c_str() );
		}
	}

	bool GLESRender::_checkExtention(const string& ext)
	{
		return extensions_.find(ext)!=extensions_.end();
	}
	void GLESRender::SetColorMask(bool r,bool g, bool b , bool a)
	{
		glColorMask(r, g, b, a);
	}

	void GLESRender::_setZEnable(bool z)
	{
		GL_ASSERT(z?glEnable(GL_DEPTH_TEST):glDisable(GL_DEPTH_TEST));
	}
	void GLESRender::_setZWriteEnable(bool z)
	{
		GL_ASSERT(glDepthMask(z));
	}

	void GLESRender::Clear(const Color& color, f32 depth, u32 stencil)
	{
		glClearColor(color.r, color.g, color.b, color.a);
#ifndef TNG_OS_FAMILY_WINDOWS
		glClearDepthf(depth);
#else
		glClearDepth(depth);
#endif
		
		glClearStencil(stencil);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	}
	void GLESRender::ClearColor(const Color& color,bool ime)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		clearmask_ = clearmask_|GL_COLOR_BUFFER_BIT;
		if (ime)
		{
			glClear(clearmask_);
			clearmask_ =0;
		}
	}
#ifndef glDepthRange
#define glDepthRange glDepthRangef
#endif
	void GLESRender::SetDepthRange(f32 znear,f32 zfar)
	{
		GL_ASSERT(glDepthRangef(znear,zfar));
	}
	void GLESRender::SetViewport(const Rect& rect)
	{
		Assert(rect.right-rect.left>0&&
			rect.bottom-rect.top>0);
		GL_ASSERT(glViewport(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top));
	}
	void GLESRender::ClearDepth(f32 depth,bool ime)
	{
		GL_ASSERT(glClearDepthf(depth));
		clearmask_ = clearmask_|GL_DEPTH_BUFFER_BIT;
		if (ime)
		{
			glClear(clearmask_);
			clearmask_ =0;
		}
	}
	void GLESRender::ClearStencil(u32 stencil,bool ime)
	{
		glClearStencil(stencil);
		clearmask_ = clearmask_|GL_STENCIL_BUFFER_BIT;
		if (ime)
		{
			glClear(clearmask_);
			clearmask_ =0;
		}
	}
	

	Shader* GLESRender::_createShader(const string& vspro,const string& pspro)
	{
		Allocator<char> alloctor;
		GLuint vertexShader ;
		GLuint fragmentShader;
		
		GL_ASSERT(vertexShader = glCreateShader(GL_VERTEX_SHADER));

		string fvs = string(SHADER_VERSION)+DEAFULT_PRECISION+vspro;

		
		GLint len = (s32)fvs.length();
		const char* vscstr = fvs.c_str();
		GL_ASSERT( glShaderSource(vertexShader, 1, &vscstr, &len) );
		GL_ASSERT( glCompileShader(vertexShader) );
		GLint success;
		GL_ASSERT( glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success) );
		GLint length;
		if (success != GL_TRUE)
		{
			GL_ASSERT( glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length) );
			if (length == 0)
			{
				length = 4096;
			}
			if (length > 0)
			{
				char* infoLog = NULL;
				infoLog = alloctor.allocate(length);
				GL_ASSERT( glGetShaderInfoLog(vertexShader, length, NULL, infoLog) );
				infoLog[length-1] = '\0';
				Log::GetLog()->Printf(Log::ERROR_CHN, "vs error:%s", infoLog);
				Log::GetLog()->Printf(Log::ERROR_CHN, "vs:%s", fvs.c_str());
				alloctor.deallocate(infoLog,length);
			}

			return NULL;
		}


		GL_ASSERT(fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));

		string fps = string(SHADER_VERSION)+DEAFULT_PRECISION+pspro;

		len = (GLint)fps.length();
		const char* pscstr = fps.c_str();
		GL_ASSERT( glShaderSource(fragmentShader, 1, &pscstr, &len) );
		GL_ASSERT( glCompileShader(fragmentShader) );
		GL_ASSERT( glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success) );

		if (success != GL_TRUE)
		{
			GL_ASSERT( glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length) );
			if (length == 0)
			{
				length = 4096;
			}
			if (length > 0)
			{
				char* infoLog = NULL;
				infoLog = alloctor.allocate(length);
				//infoLog = new char[length];
				GL_ASSERT( glGetShaderInfoLog(fragmentShader, length, NULL, infoLog) );
				infoLog[length-1] = '\0';
				Log::GetLog()->Printf(Log::ERROR_CHN,"fs error:%s",infoLog);
				Log::GetLog()->Printf(Log::ERROR_CHN, "fs:%s", fps.c_str());
				alloctor.deallocate(infoLog, length);
				//delete []infoLog;
			}
			return NULL;
		}




		GLESEffect* effect = new GLESEffect;
		
		// Link program.
		GLuint program;

		GL_ASSERT( program = glCreateProgram() );
		GL_ASSERT( glAttachShader(program, vertexShader) );
		GL_ASSERT( glAttachShader(program, fragmentShader) );
		GL_ASSERT( glLinkProgram(program) );
		GL_ASSERT( glGetProgramiv(program, GL_LINK_STATUS, &success) );

		// Delete shaders after linking.
		GL_ASSERT( glDeleteShader(vertexShader) );
		GL_ASSERT( glDeleteShader(fragmentShader) );
		// Check link status.
		if (success != GL_TRUE)
		{
			char* infoLog = NULL;
			GL_ASSERT( glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length) );
			if (length == 0)
			{
				length = 4096;
			}
			if (length > 0)
			{
				infoLog = alloctor.allocate(length);
				//infoLog = new char[length];
				GL_ASSERT( glGetProgramInfoLog(program, length, NULL, infoLog) );
				infoLog[length-1] = '\0';
				Log::GetLog()->Printf(Log::ERROR_CHN, "shader link error:%s",infoLog);
				alloctor.deallocate(infoLog, length);
				//delete []infoLog;
			}
			// Clean up.
			GL_ASSERT( glDeleteProgram(program) );

			return NULL;
		}
		effect->program_ = program;
		
		// Query and store uniforms from the program.
		GLint activeUniforms;
		GL_ASSERT( glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &activeUniforms) );
		if (activeUniforms > 0)
		{
			GL_ASSERT( glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length) );
			if (length > 0)
			{
				GLchar* uniformName = alloctor.allocate(length + 1);
				//GLchar* uniformName = new GLchar[length + 1];
				GLint uniformSize;
				GLenum uniformType;
				GLint uniformLocation=0;
				unsigned int samplerIndex = 0;
				for (int i = 0; i < activeUniforms; ++i)
				{
					// Query uniform info.
					GL_ASSERT( glGetActiveUniform(program, i, length, NULL, &uniformSize, &uniformType, uniformName) );
					uniformName[length] = '\0';  // null terminate
					if (length > 3)
					{
						// If this is an array uniform, strip array indexers off it since GL does not
						// seem to be consistent across different drivers/implementations in how it returns
						// array uniforms. On some systems it will return "u_matrixArray", while on others
						// it will return "u_matrixArray[0]".
						char* c = strrchr(uniformName, '[');
						if (c)
						{
							*c = '\0';
						}
					}

					// Query the pre-assigned uniform location.
					GL_ASSERT( uniformLocation = glGetUniformLocation(program, uniformName) );

					Uniform uniform;
					uniform.program_ = 0;
					uniform.name_ = uniformName;
					uniform.location_ = uniformLocation;
					uniform.type_ = GetGLRT(uniformType);
					if (uniformType == GL_SAMPLER_2D)
					{
						uniform.index_ = samplerIndex;
						samplerIndex += uniformSize;
					}
					else
					{
						uniform.index_ = 0;
					}

					effect->uniforms_[uniformName] = uniform;
				}
				alloctor.deallocate(uniformName, length + 1);
				//delete []uniformName;
			}
		}

		GLint maxAttributeLen;
		GLint numAttribute;
		char *pAttributeName = NULL;

		GL_ASSERT(glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttribute));
		
		if (numAttribute)
		{
			GL_ASSERT(glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeLen));
			pAttributeName = alloctor.allocate(maxAttributeLen + 1);
			//pAttributeName = new char[maxAttributeLen+1];
			GLint size;
			GLenum type;
			for (s32 index = 0; index < numAttribute; ++index)
			{
				GL_ASSERT(glGetActiveAttrib(program, index, maxAttributeLen, NULL, &size, &type, pAttributeName));
				Attribute attri;
				GL_ASSERT(attri.location_ = glGetAttribLocation(program, pAttributeName));
				attri.type_=GetGLRT(type);
				attri.size_     = size;

				effect->attribute_[pAttributeName]= attri;
			}
			alloctor.deallocate(pAttributeName, maxAttributeLen + 1);
			//delete []pAttributeName;
		}

		

		return effect;
	}

	VertexBuffer* GLESRender::_createVertexBuffer(void* data,u32 count,u32 stridesize,bool dynamic)
	{
		GLuint vbo;
		GLESVertexBuffer* ret = new GLESVertexBuffer;
		ret->stridesize_ = stridesize;
		ret->count_ = count;
		GL_ASSERT( glGenBuffers(1, &vbo) );
		GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, vbo) );
		GL_ASSERT(glBufferData(GL_ARRAY_BUFFER, count*stridesize, data, dynamic ? GL_DYNAMIC_DRAW  : GL_STATIC_DRAW));
		GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, 0) );
		ret->dynamic_ =dynamic;
		ret->buffer_ = vbo;
		return ret;
	}
	IndexBuffer* GLESRender::_createIndexBuffer(void* data,u32 count,bool dynamic)
	{
		GLESIndexBuffer* pRet = new GLESIndexBuffer;
		pRet->count_ = count;
		GLuint ibo;
		GL_ASSERT( glGenBuffers(1, &ibo) );
		GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo) );
		u32 size = count>0xffff?count*sizeof(u32):count*sizeof(u16);
		GL_ASSERT(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, dynamic ? GL_DYNAMIC_DRAW  : GL_STATIC_DRAW ));
		GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
		pRet->indexbuffer_ = ibo;
		return pRet;
	}
	
// 	void GLESRender::_activeState(u32 state)
// 	{
// 		StateSet::iterator it = active_state_.find(state);
// 		if (it == active_state_.end())
// 		{
// 			GL_ASSERT(glEnableClientState(state));
// 			active_state_.insert(state);
// 		}
// 	}
// 	void GLESRender::_disableState(u32 state)
// 	{
// 		StateSet::iterator it = active_state_.find(state);
// 		if (it != active_state_.end())
// 		{
// 			GL_ASSERT(glDisableClientState(state));
// 			active_state_.erase(it);
// 		}
// 	}
// 	void GLESRender::_activeTextureCoord(u8 t)
// 	{
// 		if(!texcoord_active_state_[t])
// 		{
// 			texcoord_active_state_[t] =true;
// 			GL_ASSERT(glClientActiveTexture(GL_TEXTURE0 + t));
// 			GL_ASSERT(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
// 		}
// 	}
	void GLESRender::_setDepthFun(DEPTH_FUN dfun)
	{
		GLenum fun;
		switch (dfun)
		{
		case tng::Depth_Unknown:
		case tng::Depth_Never:
			fun = GL_NEVER;
			break;
		case tng::Depth_Less:
			fun = GL_LESS;
			break;
		case tng::Depth_Equal:
			fun = GL_EQUAL;
			break;
		case tng::Depth_LessEqual:
			fun = GL_LEQUAL;
			break;
		case tng::Depth_Greater:
			fun = GL_GREATER ;
			break;
		case tng::Depth_GreaterEqual:
			fun = GL_GEQUAL;
			break;
		case tng::Depth_NotEqual:
			fun = GL_NOTEQUAL;
			break;
		case tng::Depth_Always:
			fun = GL_ALWAYS;
			break;
		default:
			fun = GL_NEVER;
			break;
		}
		GL_ASSERT(glDepthFunc(fun));
	}
	void GLESRender::_setFillMode(FILL_MODE fm)
	{
		Assert(0);
		switch (fm)
		{
		case tng::R_F_UNKNOWN:
			break;
		case tng::R_F_SOLID:
			break;
		case tng::R_F_WIRE:
			break;
		case tng::R_F_SOLID_WIRE:
			break;
		default:
			break;
		}
	}
// 	void GLESRender::_disableTextureCoord(u8 t)
// 	{
// 		if(texcoord_active_state_[t])
// 		{
// 			texcoord_active_state_[t] =false;
// 			GL_ASSERT(glClientActiveTexture(GL_TEXTURE0 + t));
// 			GL_ASSERT(glDisableClientState(GL_TEXTURE_COORD_ARRAY));
// 		}
// 	}
	int sampler_min = -1;
	int sampler_mag = -1;
	int wrap_s = -1;
	int wrap_t = -1;
	void GLESRender::_bindTexture(RenderTexture* texture, s32 channel, s32 location, sampler_state& state)
	{
		GLESTexture* gltex = (GLESTexture*)texture;
		GL_ASSERT(glActiveTexture(GL_TEXTURE0+channel));
		GL_ASSERT(glBindTexture(GL_TEXTURE_2D,gltex->texture_));
		
		if (sampler_min != state.min_)
		{
			sampler_min = state.min_;
			if (state.min_ == TF_Point)
			{
				GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			}
			else if (state.min_ == TF_Linear)
			{
				GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			}
			else if (state.min_ == TF_PointMipPoint)
			{
				GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
			}
			else if (state.min_ == TF_LinerMipLiner)
			{
				GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
			}
			else if (state.min_ == TF_PointMipLiner)
			{
				GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR));
			}
			else if (state.min_ == TF_LinerMipPoint)
			{
				GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
			}
		}
		if (sampler_mag!=state.mag_)
		{
			sampler_mag = state.mag_;
			if (state.mag_ == TF_Point)
			{
				GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			}
			else if (state.mag_ == TF_Linear)
			{
				GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			}
			else if (state.min_ == TF_PointMipPoint)
			{
				GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST));
			}
			else if (state.min_ == TF_LinerMipLiner)
			{
				GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR));
			}
			else if (state.min_ == TF_PointMipLiner)
			{
				GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR));
			}
			else if (state.min_ == TF_LinerMipPoint)
			{
				GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST));
			}
		}
		if (wrap_s!=state.s_)
		{
			wrap_s = state.s_;
			if (state.s_ == WM_Clamp)
			{
				GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			}
			else if (state.s_== WM_REPEAT)
			{
				GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
			}
		}
		if (wrap_t != state.t_)
		{
			wrap_t = state.t_;
			if (state.t_ == WM_Clamp)
			{
				GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			}
			else if (state.t_ == WM_REPEAT)
			{
				GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
			}
		}
		GL_ASSERT(glUniform1i(location, channel));
	}
	GLuint current_program = -1;

	void GLESRender::_bindMesh(HardwareMesh* mesh,Shader* shader)
	{
		GLESEffect* effect = (GLESEffect*)(shader);
		if (current_program!=effect->program_)
		{
			current_program = effect->program_;
			GL_ASSERT( glUseProgram(effect->program_) );
		}
		
		
#ifdef USE_VAO
		if (glGenVertexArrays)
		{
			unordered_map<Shader*,VertexBufferPtr>::iterator itvao = mesh->vao_.find(shader);
			if (itvao==mesh->vao_.end()
				&&glGenVertexArrays)
			{
				GLESVAO* vao = new GLESVAO();
				mesh->vao_[shader]=vao;
				GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, 0));
				GL_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

				// Use hardware VAOs.
				GL_ASSERT(glGenVertexArrays(1, &vao->buffer_));

				if (vao->buffer_ == 0)
				{
					Log::GetLog()->Printf(Log::ERROR_CHN, "Failed to create VAO handle.");
				}

				// Bind the new VAO.
				GL_ASSERT(glBindVertexArray(vao->buffer_));


			}
			else 
			{
				GLESVAO* vao = (GLESVAO*)(itvao->second).get();
				GL_ASSERT(glBindVertexArray(vao->buffer_));
				if (mesh->indexbuffer_)
				{
					const GLESIndexBuffer* buffer = static_cast<const GLESIndexBuffer*>(mesh->indexbuffer_.get());
					GL_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->indexbuffer_));
				}
				return;
			}
		}
		
#endif // USE_VAO

		

		Assert(mesh->vertex_factory_->vertex_decl_.elem_.size()==mesh->vertexbuffer_.size());
		void* BufferData = 0;
		s32 current_attrib_acctived =0;
		for (s32 i = 0 ;i < mesh->vertex_factory_->vertex_decl_.elem_.size();i++)
		{
			GLESVertexBuffer* buffer= (GLESVertexBuffer*)mesh->vertexbuffer_[i].get();

			BufferData = BUFFER_OFFSET(mesh->vertex_factory_->vertex_decl_.elem_[i].offset_);
			u32 vsize = GetRenderTypeSize(mesh->vertex_factory_->vertex_decl_.elem_[i].format_);
			u32 count = GetRenderTypeCount(mesh->vertex_factory_->vertex_decl_.elem_[i].format_);
			if (mesh->vertex_factory_->vertex_decl_.elem_[i].start_)
			{
				BufferData =  static_cast<s8*>(BufferData) + mesh->vertex_factory_->vertex_decl_.elem_[i].start_ * vsize;
			}
			bool normalized = false;
			if (mesh->vertex_factory_->vertex_decl_.elem_[i].type_==VTYPE_NORMAL)
			{
				normalized = true;
			}
			const char* attname = GetVertexAttr(mesh->vertex_factory_->vertex_decl_.elem_[i].type_,mesh->vertex_factory_->vertex_decl_.elem_[i].index_);
			
			EffectAttribute::iterator it = shader->attribute_.find(attname);
			if (it!=shader->attribute_.end())
			{
				GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER,buffer->buffer_));

				current_attrib_acctived = it->second.location_;
				
				

				GL_ASSERT(glVertexAttribPointer(current_attrib_acctived, count,
					GetGLType(mesh->vertex_factory_->vertex_decl_.elem_[i].format_), 
					normalized, buffer->stridesize_, BufferData));


				GL_ASSERT(glEnableVertexAttribArray(current_attrib_acctived));

				
			}
		}
		if (mesh->indexbuffer_)
		{
			const GLESIndexBuffer* buffer = static_cast<const GLESIndexBuffer*>(mesh->indexbuffer_.get());
			GL_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->indexbuffer_));
		}

	}
	void GLESRender::_drawMesh(HardwareMesh* mesh)
	{
		GLint primType;

		switch (mesh->vertex_factory_->vertex_decl_.format_)
		{
		case MESH_LINE_LIST:
			primType = GL_LINES;
			break;
		case MESH_TRIANGLE_LIST:
			primType = GL_TRIANGLES;
			break;
		case MESH_LINE_STRIP:
			primType = GL_LINE_STRIP;
			break;
		case MESH_TRIANGLE_STRIP:
			primType = GL_TRIANGLE_STRIP;
			break;
		case MESH_TRIANGLE_FAN:
			primType = GL_TRIANGLE_FAN;
			break;
		}
		//active material here
		if (mesh->indexbuffer_)
		{
			GLenum indexType = mesh->indexbuffer_->count_ > 0xffff?GL_UNSIGNED_INT:GL_UNSIGNED_SHORT;
			GL_ASSERT(glDrawElements(primType,mesh->indexbuffer_->count_,indexType,NULL));
		}
		else
		{
			GL_ASSERT(glDrawArrays(primType,0,mesh->vertexbuffer_[0]->count_));
		}
	}

	void GLESRender::SetVertexShaderConstantVectorF(const int& reg, float* val, const int& vec4count)
	{
		GL_ASSERT(glUniform4fv(reg, vec4count, val));
	}

	void GLESRender::SetPixelShaderConstantVectorF(const int& reg, float* val, const int& vec4count)
	{
		GL_ASSERT(glUniform4fv(reg, vec4count, val));
	}

	void GLESRender::SetVertexShaderConstantFloat(const int& reg, float* val)
	{
		GL_ASSERT(glUniform1f(reg, *val));
	}

	void GLESRender::SetPixelShaderConstantFloat(const int& reg, float* val)
	{
		GL_ASSERT(glUniform1f(reg, *val));
	}
	void GLESRender::SetVertexShaderConstantMatrixF(const int& reg, float* val, const int& matrixCount)
	{
		GL_ASSERT(glUniformMatrix4fv(reg, matrixCount, GL_FALSE, val));
	}

	void GLESRender::SetPixelShaderConstantMatrixF(const int& reg, float* val, const int& matrixCount)
	{
		GL_ASSERT(glUniformMatrix4fv(reg, matrixCount, GL_FALSE, val));
	}

	TexPtr GLESRender::CreateTexture(u32 width,u32 height,PixelFormat::Code format)
	{
		GLESTexture* gltex = new GLESTexture;
		GL_ASSERT(glGenTextures( 1, &(gltex->texture_)));
		TexPtr ptr = gltex;
		return ptr;
	}
}
