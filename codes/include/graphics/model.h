#ifndef _TNG_MODEL__H_
#define _TNG_MODEL__H_
#include "resource/resource.h"
#include "MathGeoLib/Math/MathFunc.h"
#include "mesh.h"
#include "rendermaterial.h"
#include <vector>

namespace tng
{
	class GFX_API Renderable:public Resource
	{
	public:
		virtual s32			GetMeshCount()=0;
		virtual MeshPtr		GetMesh(s32 i)=0;
	};
	class GFX_API Model:public Renderable
	{
	public:
		void				AddMesh(MeshPtr mesh);
		virtual s32			GetMeshCount(){return (s32)mesh_.size();}
		virtual MeshPtr		GetMesh(s32 i){Assert(mesh_.size()>i);return mesh_[i];}
		
		void				LoadConfig(JsonObject* cfg);
	
		void				Load(std::istream& stream) TNG_OVERRIDE;
		void				Save(std::ostream& stream) TNG_OVERRIDE;
	protected:
		vector<MeshPtr>		mesh_;
	};

	FORCEINLINE void Model::LoadConfig(JsonObject* cfg)
	{

	}

	class GFX_API FileModel :public Model
	{
	public:
		static AutoPtr<FileModel> LoadResource(const string& key, bool async = true);

	protected:

	};

	class GFX_API ModelInstance
	{
	public:
		void		SetModel(Renderable* model);
		s32			GetMeshCount(){return renderable_holder_?(s32)renderable_holder_->GetMeshCount():0;}
		MeshPtr		GetMesh(s32 i){return renderable_holder_?renderable_holder_->GetMesh(i):MeshPtr();}

		void		AddMaterial( MaterialPtr mtl);
		void		AddMaterial(const string& mtl_name);
		Material*	GetMaterial(s32 i){
			if (material_.size())
			{
				return material_[Min<s32>(i,(s32)material_.size()-1)].get();
			}
			return NULL;
		}
		s32					GetMaterialParamCount(){return (s32)material_param_.size();}
		ShaderParam&		GetMaterialParam(s32 i){
			Assert(material_param_.size()>0);
			i = Min<int>((int)material_param_.size()-1,i);
			return material_param_[i];
		}


		void		Clear();
		AutoPtr<Renderable>						renderable_holder_;
		vector<ShaderParam>						material_param_;
		vector<MaterialPtr>						material_;
	};
}
#endif