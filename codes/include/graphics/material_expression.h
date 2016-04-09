#ifndef __TNG_MATERIAL_EXPRESSION_H__
#define __TNG_MATERIAL_EXPRESSION_H__
#include "string"
#include "core/math.h"
#include "core/object.h"
#include "core/autoptr.h"
#include "graphics_macros.h"
#include "graphics/shader.h"
#include "core/stl_rebind.h"
namespace tng
{
	enum ShaderParamType
	{
		SP_FLOAT,
		SP_VEC2,
		SP_VEC4,
		SP_MATRIX4x4,
		SP_SAMPLER2D,
	};
	GFX_FUNC_API void AddUnifrom(const string& name,
		ShaderParamType type,
		vector<string>& out_params);
	struct MaterialMask
	{
		MaterialMask():alpha_mask_(false)
			,red_mask_(false)
			,green_mask_(false)
			,blue_mask_(false){}

		void AllEnable()
		{
			alpha_mask_ =
				red_mask_ =
				green_mask_ =
				blue_mask_ = true;
		}

		///begin property
		bool alpha_mask_;
		bool red_mask_;
		bool green_mask_;
		bool blue_mask_;
		///end property
	};

	struct MaterialExpression:public Object
	{
		ENGINE_CLASS_DEF
		MaterialExpression():const_(true),input_number_(0){}
		s32					GetInputNumber(){return input_number_;}
		virtual string Complie(vector<string>& out_param, ShaderParam& out_default)const = 0;
		virtual void Parse(JsonObject& json);

		virtual struct MaterialInput* GetInput(s32 index){return NULL;}

		string		name_;
		MaterialMask output_mask_;
		s32		input_number_;
		bool	const_;
	};
	
	typedef AutoPtr<MaterialExpression> MaterialExpressionPtr;
	struct MaterialInput
	{
		MaterialInput()
		{

		}
		
		string Complie(vector<string>& out_param, ShaderParam& out_default)const;
		
		MaterialMask input_mask_;
		MaterialExpressionPtr link_;
	};

	struct ConstFloat4Expression:MaterialExpression
	{
		ENGINE_CLASS_DEF
		ConstFloat4Expression():default_(1,1,1,1){
			output_mask_.AllEnable();
		}
		virtual string Complie(vector<string>& out_param, ShaderParam& out_default)const;
		virtual void Parse(JsonObject& json);
		///begin property
		float4		default_;
	};

	struct ParamFloat4Expression:ConstFloat4Expression
	{
		ENGINE_CLASS_DEF
		ParamFloat4Expression(){
			const_ = false;
			output_mask_.AllEnable();
		}
		virtual string Complie(vector<string>& out_param, ShaderParam& out_default)const;
		virtual void Parse(JsonObject& json);
		///begin property
		string	float4_name_;
	};

	struct TextureSamplerExpression:MaterialExpression
	{
		ENGINE_CLASS_DEF
		TextureSamplerExpression(){
			const_ = false;
			output_mask_.AllEnable();
			default_asset_key_ = "engine:/Texture/default.tex";

		}
		virtual void Parse(JsonObject& json);
		virtual string Complie(vector<string>& out_param, ShaderParam& out_default)const;
		///begin property
		string sampler_name_;
		string default_asset_key_;
		mutable TexturePtr	default_texture_;

	};

	struct MulExpression :MaterialExpression
	{
		ENGINE_CLASS_DEF
		MulExpression(){}
		virtual void Parse(JsonObject& json);
		virtual string Complie(vector<string>& out_param, ShaderParam& out_default)const;
		virtual struct MaterialInput* GetInput(s32 index){
			if (index == 0)
			{
				return &lh_;
			}
			else
				return &rh_;
		}
		MaterialInput		lh_;
		MaterialInput		rh_;
	};


}
#endif