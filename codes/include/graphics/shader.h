#ifndef _TNG_SHADER__H_
#define _TNG_SHADER__H_
#include <unordered_map>
#include "core/refobject.h"
#include "MathGeoLib/Math/float4x4.h"
#include "MathGeoLib/Math/float4.h"
#include "graphics/graphics_macros.h"
#include "core/autoptr.h"
#include "graphics/rendertexture.h"

namespace tng
{
	
	struct Uniform
	{
		string		name_;
		s32				location_;
		RenderType		type_;
		s32				size_;
		s32				index_;///<used in opengl
		s32				program_;///<used in d3d 0 vs 1 ps
	};
	struct Attribute 
	{
		s32				location_;
		RenderType		type_;
		s32				size_;
	};

// #ifdef SPARSEHASH
// 	typedef google::dense_hash_map<string, u32 > TextureParamMap;
// 	typedef google::dense_hash_map<string, f32> ScalarParamMap;
// 	typedef google::dense_hash_map<string, float4  > VectorParamMap;
// 	typedef google::dense_hash_map<string, float4x4  > MatrixParamMap;
// 
// 	typedef google::dense_hash_map<string, Uniform > EffectUniforms;
// 	typedef google::dense_hash_map<string, Attribute > EffectAttribute;
// #else
	struct sampler
	{
		sampler_state	state_;
		u32				id_;
	};
	typedef unordered_map<string, sampler > TextureParamMap;
	typedef unordered_map<string, f32 > ScalarParamMap;
	typedef unordered_map<string, float4  > VectorParamMap;
	typedef unordered_map<string, float4x4  > MatrixParamMap;
	typedef unordered_map<string, Uniform > EffectUniforms;
	typedef unordered_map<string, Attribute > EffectAttribute;
	typedef unordered_map<string, TexturePtr > TextureObjParamMap;
//#endif

	///used in render thread
	class GFX_API Shader :public RefCountedObject
	{
	public:
		Shader(){

		}
		virtual ~Shader(){}
		void SetVertexShaderConstantVectorF(const int& reg, float* val, const int& vec4count);
		void SetPixelShaderConstantVectorF(const int& reg, float* val, const int& vec4count);
		void SetVertexShaderConstantFloat(const int& reg, float* val);
		void SetPixelShaderConstantFloat(const int& reg, float* val);
		void SetVertexShaderConstantMatrixF(const int& reg, float* val, const int& matrixCount);
		void SetPixelShaderConstantMatrixF(const int& reg, float* val, const int& matrixCount);
	
		EffectUniforms	uniforms_;
		EffectAttribute	attribute_;
	};
	typedef AutoPtr<Shader>	ShaderPtr;
	/// used in scene manger thread
	struct ShaderParam
	{
		ShaderParam()
		{

		}

		void Active(const EffectUniforms& u);

		void Merge(const ShaderParam& rh);

		TextureParamMap		texture_;
		ScalarParamMap		scalar_;
		VectorParamMap		vector_;
		MatrixParamMap		matrix_;
		TextureObjParamMap	texture_obj_;
	};


}

#endif