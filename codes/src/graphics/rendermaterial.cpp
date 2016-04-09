#include "graphics/rendermaterial.h"
#include "graphics/graphics.h"
#include "graphics/renderservice.h"
#include "core/config.h"
 #include "string"
#include "core/json_help.h"
#include "core/binary_reader_writer.h"
#include "resource/stream_res.h"
namespace tng
{
	bool Material::HasEmissive()const
	{
		return emissive_.link_.get()!=NULL;
	}
	bool Material::HasDiffuse()const
	{
		return diffuse_.link_.get() != NULL;
	}
	u32 MaterialCatchKey::GetKey()const
	{
		if (keyed_ != 0)
		{
			return keyed_;
		}
		if (mtl_name_.size()&&vertex_factory_!=-1&&pass_!=-1)
		{
			u32 crc = Crc32(0,(u8*)(&vertex_factory_),sizeof(u32));
			crc = Crc32(crc,(u8*)(&pass_),sizeof(u32));
			crc = Crc32(crc,(u8*)(mtl_name_.c_str()),mtl_name_.size());
			keyed_ = crc;
			return crc;
		}
		else
		{
			Log::GetLog()->Printf(Log::ERROR_CHN,"cant get material key before key param set (name:%s,vf:%d,pass:%d)"
				,mtl_name_.c_str(),vertex_factory_,pass_);
			return 0;
		}
		
	}
	MaterialMask Material::ParseMask(JsonObject& obj)
	{
		MaterialMask ret;

		if (obj.HasMember("red"))
		{
			GetValue(ret.red_mask_,obj,"red");
		}
		if (obj.HasMember("green"))
		{
			GetValue(ret.green_mask_,obj,"green");
		}
		if (obj.HasMember("blue"))
		{
			GetValue(ret.blue_mask_,obj,"blue");
		}
		if (obj.HasMember("alpha"))
		{
			GetValue(ret.alpha_mask_,obj,"alpha");
		}
		return ret;
	}
	void Material::ParseExp(MaterialInput& input,JsonObject& obj,const string& key)
	{
		if (obj.HasMember(key.c_str()))
		{
			JsonObject::MemberIterator first_mem = obj[key.c_str()].MemberBegin();
			if(first_mem!=obj.MemberEnd())
			{
				MaterialExpMap::iterator it = exp_map_.find(first_mem->name.GetString());
				if (it!=exp_map_.end())
				{
					MaterialExpression* exp =(*it).second;
					input.input_mask_ = ParseMask(first_mem->value);
					ParseExp(exp,first_mem->value);
					input.link_ = exp;
				}
				first_mem++;
			}
		}

	}

	void Material::ParseExp(MaterialExpression* inputexp,JsonObject& obj)
	{
		JsonObject::MemberIterator first_mem = obj.MemberBegin();
		s32 input_index = 0;
		while(first_mem!=obj.MemberEnd())
		{
			if (first_mem->value.IsObject())
			{
				MaterialExpMap::iterator it = exp_map_.find(first_mem->name.GetString());
				if (it!=exp_map_.end())
				{
					MaterialExpression* exp =(*it).second;
					MaterialInput* input = inputexp->GetInput(input_index);
					if (input)
					{
						input->input_mask_ = ParseMask(first_mem->value);
						input->link_= exp;
					}
					
					input_index++;
					ParseExp(exp,first_mem->value);
				}
			}
			
			first_mem++;
		}	
	}
	void Material::LoadAllEXP(JsonObject& obj)
	{
		JsonObject::MemberIterator first_mem = obj.MemberBegin();
		while(first_mem!=obj.MemberEnd())
		{
			JsonObject& expjson = first_mem->value;
			MaterialExpression* exp = Object::CreateObject(first_mem->name.GetString())->DynamicCast<MaterialExpression>();
			string name ;
			GetValue<string>(name,expjson,"name");
			Assert(exp);
			exp->Parse(expjson);
			Assert(exp_map_.find(name)==exp_map_.end());
			exp_map_[name] = exp;
			first_mem ++;
		}
		
	}


	void Material::Load(std::istream& stream)
	{
		JsonDocument json;
		string line;
		stream.seekg(0, std::ios::end);
		int length = (int)stream.tellg();
		stream.seekg(0, std::ios::beg);
		char* str = BufferAllocator<char>::allocate(length+1);
		BinaryReader br(stream);
		br.readRaw(str, length);
		str[length] = '\0';

		json.Parse<0>(str);
		if (json.HasParseError())
		{
			Log::GetLog()->Printf(Log::ERROR_CHN, "jsonerror:%s,code:%s,len %d", str, json.GetParseError(), length);
			return;
		}
		BufferAllocator<char>::deallocate(str, length + 1);
		GetValue(emmisive_only_,json,"emissive_only");
		GetValue(depth_write_,json,"depth_write");

		////load all expressions
		if (json.HasMember("expression"))
		{
			JsonObject& exp_json = json["expression"];
			LoadAllEXP(exp_json);
		}
		
		ParseExp(emissive_,json,"emissive");
		ParseExp(diffuse_,json,"diffuse");
		ParseExp(normal_,json,"normal");
		ParseExp(specular_color_,json,"specular_color");
		ParseExp(specular_power_,json,"specular_power");
		ParseExp(opacity_,json,"opacity");
		ParseExp(mask_,json,"mask");
		ParseExp(distortion_,json,"distortion");
		
	}
	void Material::Save(std::ostream& stream)
	{
		JsonObject json;
		json["emissive_only"].SetBool(emmisive_only_);
		json["depth_write"].SetBool(depth_write_);


		BinaryWriter br(stream);
		br.writeRaw(json.GetString());
	}

	
}