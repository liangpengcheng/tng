#ifndef __TNG_MTL_COMPILER_H__
#define __TNG_MTL_COMPILER_H__

#include "graphics/material_expression.h"
#include "graphics/rendermaterial.h"
#include "core/dynamic_factory.h"
namespace tng
{

	class GFX_API MaterialManger
	{
	public:
		MaterialManger();
		~MaterialManger();
		void Initialize();
		static MaterialManger* instance_ ;
		typedef unordered_map<MaterialCatchKey,MaterialRender> ShaderCacheMap;
		typedef unordered_map<string,AutoPtr<Material> >	MaterialMap;
	
		MaterialPtr		FindMaterial(const string& key);
		MaterialRender	GetShader(Material* mtl,VertexFactory* factory,RenderPass pass);
		/// load all material in path directory(not include sub-directory)
		void			LoadPath(const string& path);
	protected:
		void LoadPass(RenderPass i , const string& pass_path);
		ShaderCacheMap catche_map_;
		MaterialMap		mtl_map_;
		string	mtl_template_;
		string predefine_;
		unordered_map<RenderPass,string>		pass_program_map_;
	};
}

#endif