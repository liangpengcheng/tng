#ifndef _TNG_RENDERSTATE_H__
#define _TNG_RENDERSTATE_H__

namespace tng
{
	enum CullMode
	{
		C_M_UNKNOWN = 0,
		C_M_NONE,
		C_M_CW,
		C_M_CCW,
	};

	enum FILL_MODE
	{
		R_F_UNKNOWN = 0,
		R_F_SOLID,
		R_F_WIRE,
		R_F_SOLID_WIRE,
	};

	enum DEPTH_FUN
	{
		Depth_Unknown = 0,
		Depth_Never,
		Depth_Less,
		Depth_Equal,
		Depth_LessEqual,
		Depth_Greater,
		Depth_GreaterEqual,
		Depth_NotEqual,
		Depth_Always,
	};

	enum BlendMode
	{
		B_Unknown =0,
		B_Opaque  ,	///close all
		B_Masked,		///enable alpha test
		B_Translucent,	///enable alpha blend
		B_Additive,		///enable alpha blend
		B_Subtract,
		B_RevSubtract,
		B_Min,
		B_Max,
	};

	enum StencilOp
	{
		Stencil_Unknown,
		Stencil_KEEP,
		Stencil_ZERO,
		Stencil_REPLACE,
		Stencil_INCRSAT,
		Stencil_DECRSAT,
		Stencil_INVERT,
		Stencil_INCR,
		Stencil_DECR,
		Stencil_MAX,
	};
	enum TextureFilter
	{
		TF_Point,
		TF_Linear,
		TF_PointMipPoint,
		TF_PointMipLiner,
		TF_LinerMipLiner,
		TF_LinerMipPoint,
	};
	enum WrapMode
	{
		WM_Clamp,
		WM_REPEAT,
	};
	struct RenderState 
	{
		RenderState():stencilop_(Stencil_Unknown)
		,depthfun_(Depth_Unknown)
		,cull_(C_M_UNKNOWN)
		,blend_(B_Unknown)
		,depth_enable_(true)
		,depth_write_enable_(true){}
		StencilOp	stencilop_;
		DEPTH_FUN	depthfun_;
		CullMode	cull_;
		FILL_MODE	fill_;
		BlendMode	blend_;
		bool		depth_enable_;
		bool		depth_write_enable_;
	};
}
#endif