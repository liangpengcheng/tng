#include "graphics/material_expression.h"
#include "graphics/texture.h"
#include "core/json_help.h"
namespace tng
{
	void AddUnifrom(const string& name,
		ShaderParamType type,
		vector<string>& out_params)
	{
		string typestr ;
		switch (type)
		{
		case tng::SP_FLOAT:
			typestr = typestr + "uniform "+LOWP+" float";
			break;
		case tng::SP_VEC2:
			typestr = typestr + "uniform "+LOWP+" vec2";
			break;
		case tng::SP_VEC4:
			typestr = typestr + "uniform "+LOWP+" vec4";
			break;
		case tng::SP_MATRIX4x4:
			typestr = typestr + "uniform "+LOWP+" mat4";
			break;
		case tng::SP_SAMPLER2D:
			typestr = typestr + "uniform "+LOWP+" sampler2D";
			
			break;
		default:
			break;
		}
		typestr = typestr + " "+ name;
		for (s32 i =0;i < out_params.size() ;i ++)
		{
			if (out_params[i]==typestr)
			{
				return;
			}
		}
		out_params.push_back(typestr);
	}
	string MaterialInput::Complie(vector<string>& out_param, ShaderParam& out_default)const
	{
		if (link_)
		{
			string mask;
			if (input_mask_.red_mask_)
			{
				mask = mask+"x";
			}
			if (input_mask_.green_mask_)
			{
				mask = mask+"y";
			}
			if (input_mask_.blue_mask_)
			{
				mask = mask+"z";
			}
			if (input_mask_.alpha_mask_)
			{
				mask = mask+"w";
			}
			if (mask.length()>0)
			{
				mask = string(".")+mask;
			}
			return link_->Complie(out_param, out_default) + mask;
		}
		else
		{
			return "1.0";
		}
	}
	void MaterialExpression::Parse(JsonObject& json)
	{

	}

	string ConstFloat4Expression::Complie(vector<string>& out_param, ShaderParam& out_default)const
	{
		return format("V4Type(%f,%f,%f,%f)")
			%default_.x
			%default_.y
			%default_.z
			%default_.w;
	}
	void ConstFloat4Expression::Parse(JsonObject& json)
	{
		if(json.HasMember("default"))
		{
			GetValue<float4>(default_,json,"default");
		}
	}
	string ParamFloat4Expression::Complie(vector<string>& out_param, ShaderParam& out_default)const
	{
		AddUnifrom(float4_name_,SP_VEC4,out_param);
		out_default.vector_[float4_name_] = default_;
		return format("%s")%float4_name_.c_str();
	}

	void ParamFloat4Expression::Parse(JsonObject& json)
	{
		ConstFloat4Expression::Parse(json);
		if(json.HasMember("float4_name"))
		{
			GetValue<string>(float4_name_,json,"float4_name");
		}
	}
	void TextureSamplerExpression::Parse(JsonObject& json)
	{
		if (json.HasMember("sampler_name"))
		{
			GetValue<string>(sampler_name_, json, "sampler_name");
		}
		if (json.HasMember("default"))
		{
			GetValue<string>(default_asset_key_, json, "default");
		}
		TexturePtr ft =  FileTexture::LoadResource(default_asset_key_);
		default_texture_ = ft;
	}
	string TextureSamplerExpression::Complie(vector<string>& out_param, ShaderParam& out_default)const
	{
		AddUnifrom(sampler_name_,SP_SAMPLER2D,out_param);
		out_default.texture_obj_[sampler_name_] = default_texture_;
		return format("texture2D(%s,texturecoord)")%sampler_name_.c_str();
	}

	void MulExpression::Parse(JsonObject& json)
	{

	}

	string MulExpression::Complie(vector<string>& out_param, ShaderParam& out_default)const
	{

		return format("(%s)*(%s)")
			% lh_.Complie(out_param, out_default).c_str()
			% rh_.Complie(out_param, out_default).c_str();

		
	}

	START_SUB_ABSTRACT_CLASS(MaterialExpression,Object)
	END_CLASS

	START_SUB_CLASS(ConstFloat4Expression,MaterialExpression)
	END_CLASS

	START_SUB_CLASS(ParamFloat4Expression,ConstFloat4Expression)
	END_CLASS

	START_SUB_CLASS(TextureSamplerExpression,MaterialExpression)
	END_CLASS
	START_SUB_CLASS(MulExpression, MaterialExpression)
	END_CLASS
}