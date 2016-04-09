#include "graphics/shader.h"
#include "graphics/renderservice.h"
#include "graphics/graphics.h"
#include "core/os.h"
namespace tng
{
	void ShaderParam::Active(const EffectUniforms& u)
	{
		for (TextureParamMap::iterator it = texture_.begin();
			it!=texture_.end();
			it++)
		{
			EffectUniforms::const_iterator itu = u.find(it->first);
			if (itu!=u.end())
			{
				const Uniform& uniform= itu->second;
				sampler* s = &(it->second);
				RenderTexture* tex = internal::GetRenderService()->GetTexture(s->id_);
				if (tex)
				{
					internal::GetRender()->Bind(tex, uniform.index_,uniform.location_,s->state_);
				}
				else
					Log::GetLog()->Printf(Log::ERROR_CHN,"can't find texture %d",it->second);
			}
		}
		for (ScalarParamMap::iterator it = scalar_.begin();
			it!=scalar_.end();
			it++)
		{
			EffectUniforms::const_iterator itu = u.find(it->first);
			if (itu!=u.end())
			{
				const Uniform& uniform= itu->second;
				uniform.program_==0?internal::GetRender()->SetVertexShaderConstantFloat(uniform.location_,&it->second):
					internal::GetRender()->SetPixelShaderConstantFloat(uniform.location_,&it->second);
			}
		}
		for (VectorParamMap::iterator it = vector_.begin();
			it!=vector_.end();
			it++)
		{
			EffectUniforms::const_iterator itu = u.find(it->first);
			if (itu!=u.end())
			{
				const Uniform& uniform= itu->second;
				uniform.program_==0?internal::GetRender()->SetVertexShaderConstantVectorF(uniform.location_,reinterpret_cast<float*>(&it->second),1):
					internal::GetRender()->SetPixelShaderConstantVectorF(uniform.location_,reinterpret_cast<float*>(&it->second),1);
			}
		}
		for (MatrixParamMap::iterator it = matrix_.begin();
			it!=matrix_.end();
			it++)
		{
			EffectUniforms::const_iterator itu = u.find(it->first);
			if (itu!=u.end())
			{
				const Uniform& uniform= itu->second;
				uniform.program_==0?internal::GetRender()->SetVertexShaderConstantMatrixF(uniform.location_,reinterpret_cast<float*>(&it->second),1):
					internal::GetRender()->SetPixelShaderConstantMatrixF(uniform.location_,reinterpret_cast<float*>(&it->second),1);
			}
		}
	}
	void ShaderParam::Merge(const ShaderParam& rh)
	{
		
		for (TextureObjParamMap::const_iterator it = texture_obj_.begin();
			it != texture_obj_.end();
			it++)
		{
			if (it->second->GetTexture())
			{
				sampler s;
				s.state_ = it->second->head_.state_;
				s.id_ = it->second->GetTexture();
				texture_[it->first] = s;
			}
		}
		for (TextureObjParamMap::const_iterator it = rh.texture_obj_.begin();
			it != rh.texture_obj_.end();
			it++)
		{
			if (it->second->GetTexture())
			{
				sampler s;
				s.state_ = it->second->head_.state_;
				s.id_ = it->second->GetTexture();
				texture_[it->first] = s;
			}
		}

		for (TextureParamMap::const_iterator it = rh.texture_.begin();
			it != rh.texture_.end();
			it++)
		{
			//if (texture_.find(it->first)==texture_.end())
			{
				texture_[it->first] = it->second;
			}
		}
		
		for (ScalarParamMap::const_iterator it = rh.scalar_.begin();
			it != rh.scalar_.end();
			it++)
		{
			//if (scalar_.find(it->first) == scalar_.end())
			{
				scalar_[it->first] = it->second;
			}
		}
		for (VectorParamMap::const_iterator it = rh.vector_.begin();
			it != rh.vector_.end();
			it++)
		{
			//if (vector_.find(it->first) == vector_.end())
			{
				vector_[it->first] = it->second;
			}
		}
		for (MatrixParamMap::const_iterator it = rh.matrix_.begin();
			it != rh.matrix_.end();
			it++)
		{
			//if (matrix_.find(it->first) == matrix_.end())
			{
				matrix_[it->first] = it->second;
			}
		}
	}

}