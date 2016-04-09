#ifndef _RENDERMATERIAL_H_
#define _RENDERMATERIAL_H_
#include "graphics/shader.h"
#include "core/refobject.h"
#include "graphics/rendervertexbuffer.h"
#include "graphics/material_expression.h"
#include <unordered_map>
namespace tng
{
	
	/**
		used in render thread
	*/
	struct MaterialRender
	{
		u32						effect_id_;
	};
	
	/**
		used in logic(main) thread
	*/

	
	enum RenderPass
	{
		EARLY_Z_PASS,
		OPAQUE_DIFFUSE_PASS,
		OPAQUE_LIGHTING_PASS,
		OPAQUE_LIGHTMAP_PASS,
		EMISSIVE_PASS,
		TRANSPARENT_PASS,
	};
	class GFX_API Material:public RefCountedObject
	{
	public:
		typedef std::unordered_map<string,MaterialExpressionPtr>	MaterialExpMap;
		friend class MaterialCompiler;

		Material():layer_(OPAQUE_LAYER)
			,emmisive_only_(false)
			,depth_write_(true){}
		enum layer
		{
			BACK_GROUND_LAYER,
			TERRAIN_LAYER,
			OPAQUE_LAYER,
			TRANSPARENT_LAYER,
			FORE_GROUND_LAYER,
			UI_LAYER,
			AFTER_UI_LAYER,
		} layer_;

		void					ParseExp(MaterialInput& input,JsonObject& obj,const string& key);
		void					ParseExp(MaterialExpression* input,JsonObject& obj);
		MaterialMask			ParseMask(JsonObject& obj);
		void					Load(std::istream& stream);
		void					Save(std::ostream& stream);
		void					LoadAllEXP(JsonObject& obj);

		string				Name(){return name_;}
		void					SetName(const string& n){name_ = n;}

		bool					HasEmissive()const;
		bool					HasDiffuse()const;

		MaterialInput			diffuse_;
		MaterialInput			emissive_;
		MaterialInput			normal_;
		MaterialInput			specular_color_;
		MaterialInput			specular_power_;
		MaterialInput			opacity_;
		MaterialInput			mask_;
		MaterialInput			distortion_;

	public:
		MaterialExpMap			exp_map_;
		ShaderParam				default_param_;
		string					name_;
		bool					emmisive_only_;
		bool					depth_write_;
	};
	typedef AutoPtr<Material> MaterialPtr;
	struct MaterialCatchKey
	{
		MaterialCatchKey():vertex_factory_(-1),pass_(-1),keyed_(0)
		{

		}
		friend bool operator == (const MaterialCatchKey& lh,const MaterialCatchKey& rh)
		{
			return lh.vertex_factory_==rh.vertex_factory_
				&&lh.pass_ == rh.pass_
				&& lh.mtl_name_ == rh.mtl_name_;
		}
		u32 GetKey()const;
		s32 vertex_factory_;
		s32 pass_;
		string mtl_name_;
	protected:
		mutable u32 keyed_;
	};
}
namespace std{
	template<>
	struct hash<tng::MaterialCatchKey>
	{
		size_t operator()(const tng::MaterialCatchKey& _Keyval) const
		{	
			return _Keyval.GetKey();
		}
	};
}

namespace std{
	template<>
	struct hash<tng::RenderPass>
	{
		size_t operator()(const tng::RenderPass& _Keyval) const
		{	
			return (size_t)(_Keyval);
		}
	};
}
#endif