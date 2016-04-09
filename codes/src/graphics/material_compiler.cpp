#include "graphics/material_compiler.h"
#include "resource/stream_res.h"
#include "graphics/graphics.h"
#include "graphics/renderservice.h"
namespace tng
{
	MaterialManger* MaterialManger::instance_ = NULL;
	MaterialManger::MaterialManger(){
		instance_ = this;
		MaterialExpression::InitStatic();
		ConstFloat4Expression::InitStatic();
		ParamFloat4Expression::InitStatic();
		TextureSamplerExpression::InitStatic();
		MulExpression::InitStatic();
	}
	MaterialManger::~MaterialManger(){
		instance_ = NULL;
	}
	void MaterialManger::Initialize()
	{
		std::istream* stram = ResourceManger::GetInstance()->LoadAssetStream("engine:/Material/template.txt");

		string line;
		while(std::getline(*stram,line))
		{
			mtl_template_ = mtl_template_+line;
		}
		stram = ResourceManger::GetInstance()->LoadAssetStream("engine:/Material/predefine.txt");

		while(std::getline(*stram,line))
		{
			predefine_ = predefine_+line;
		}

		LoadPass(EMISSIVE_PASS,"engine:/Material/emissive.txt");	
		LoadPass(EARLY_Z_PASS,"engine:/Material/early_z.txt");	

	}
	void MaterialManger::LoadPass(RenderPass i , const string& pass_path)
	{
		std::istream* stram = ResourceManger::GetInstance()->LoadAssetStream(pass_path);

		string pass_ps;
		string line;
		while(std::getline(*stram,line))
		{
			pass_ps = pass_ps+line;
		}
		pass_program_map_[i] = pass_ps;
	}
	MaterialRender MaterialManger::GetShader(Material* mtl,VertexFactory* factory,RenderPass pass)
	{
		Assert(mtl&&factory);
		MaterialCatchKey key;
		key.mtl_name_ = mtl->Name();
		key.pass_ = pass;
		key.vertex_factory_ = factory->GetVertexType();

		ShaderCacheMap::iterator it = catche_map_.find(key);
		if (it!=catche_map_.end())
		{
			return (*it).second;
		}
		else
		{
			MaterialRender ret ;


			string vs = predefine_ + factory->GetVertexShader();

			vector<string> outparams;
			string diffuse_fun = mtl->diffuse_.Complie(outparams, mtl->default_param_);
			string emissive_fun = mtl->emissive_.Complie(outparams, mtl->default_param_);
			string normal_fun = mtl->normal_.Complie(outparams, mtl->default_param_);
			string specular_color = mtl->normal_.Complie(outparams, mtl->default_param_);
			string specular_power = mtl->specular_power_.Complie(outparams, mtl->default_param_);
			string opacity = mtl->opacity_.Complie(outparams, mtl->default_param_);
			string mask = mtl->mask_.Complie(outparams, mtl->default_param_);
			string distortion = mtl->distortion_.Complie(outparams, mtl->default_param_);
			string params;
			for (s32 i = 0 ;i < outparams.size() ;i ++)
			{
				params.append(outparams[i]);
				params.append(";\n");
			}
			string ps = format(mtl_template_)
				%diffuse_fun.c_str()
				%emissive_fun.c_str()
				%normal_fun.c_str()
				%specular_color.c_str()
				%specular_power.c_str()
				%opacity.c_str()
				%mask.c_str()
				%distortion.c_str();
			ps = predefine_+params+ps+pass_program_map_[pass];

			ret.effect_id_ = internal::GetRenderService()->CreateShader(vs,ps);

			catche_map_[key] = ret;
			return ret;
		}
	}

	MaterialPtr MaterialManger::FindMaterial(const string& key)
	{
		MaterialMap::iterator it = mtl_map_.find(key);
		if (it!=mtl_map_.end())
		{
			return it->second;
		}
		else
		{
			Log::GetLog()->Printf(Log::WARNING_CHN,"can't find material : %s",key.c_str());
			return MaterialPtr(NULL);
		}
	}

	void MaterialManger::LoadPath(const string& path)
	{
		Log::GetLog()->Printf(Log::DBG_CHN, "loading material path : %s", path.c_str());
		//FileSystem::FindFiles()
		vector<Path> paths;
		ResourceManger::FindFilesInAssetPath(path,paths);

		for (s32 i = 0 ;i < paths.size();i++)
		{
			
			string dir,name,ext;
			paths[i].Split(dir,name,ext);
			ToLower(ext);
			if (ext == "mat")
			{
				Log::GetLog()->Printf(Log::DBG_CHN, "material path : %s", string(paths[i]).c_str());
				Assert(mtl_map_.find(name)==mtl_map_.end());
				std::istream* stream = ResourceManger::GetInstance()->LoadAssetStream(paths[i]);
				AutoPtr<Material> mtl = new Material();
				mtl->Load(*stream);
				mtl->SetName(name);
				mtl_map_[name]=mtl;
			}
			
		}
	}
}