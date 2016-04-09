#ifndef _tng_rendervertexbuffer__h_
#define _tng_rendervertexbuffer__h_
#include "renderbuffer.h"
#include "shader.h"
#include "MathGeoLib/Math/float3x4.h"
#include <vector>
namespace tng
{
	/**
		Vertex attribute enumeration.
	*/
	enum VertexType {
		VTYPE_UNKONWN	= -1, 
		VTYPE_GENERIC	= 0,
		VTYPE_VERTEX	= 1,  /**< POSITION */
		VTYPE_NORMAL	= 2,  /**< NORMAL */
		VTYPE_TEXCOORD	= 3,  /**< TEXCOORD<i>n</i> */
		VTYPE_COLOR		= 4,  /**< COLOR<i>n</i> */
		VTYPE_BLEND		= 5,  /**< BLEND */
		VTYPE_WEIGHT	= 6,  /**< WEIGHT */
		VTYPE_BINORMAL	= 7,
		VTYPE_TANGENT	= 8,
	};
	enum MeshElemType
	{
		MESH_LINE_LIST,
		MESH_LINE_STRIP,
		MESH_TRIANGLE_STRIP,
		MESH_TRIANGLE_LIST,
		MESH_TRIANGLE_FAN,
	};
	/**
		Stream vertex attribute.
	*/
	struct VertexElem{

		VertexElem():stream_(-1),type_(VTYPE_UNKONWN),format_(RT_UNSUPPORT),offset_(0),index_(0),start_(0){}
		s32 stream_;            /**< Stream mapping */
		VertexType type_;       /**< Specifies the vertex type_. */
		RenderType format_;		/**< Specifies the vertex format_ (float, bool, int, etc). */
		s32 offset_;            /**< Specifies the vertex format_ size (3 for <type_>3, for example, where type_ is float and size is 3, the attribute is a float3). */
		s32 index_;				/**index_*/
		s32 start_;
	};

	class GFX_API VertexDecl
	{
	public:
		VertexDecl():vertexformat_key_(0xffffffff){}
		void	PosNormalUv();
		void	PosUV();
		void	PosUVColor();
		std::vector<VertexElem>	elem_;
		MeshElemType			format_;
		u32						vertexformat_key_;
	};


	class GFX_API VertexBuffer :public RenderBuffer
	{
	public:
		VertexBuffer():dynamic_(false),stridesize_(0),count_(0){}
		virtual void			UpdateBuffer(u32 offset,u32 size,void** ppData) =0;
		u32						stridesize_;
		u32						count_;
		bool					dynamic_;
	};
	typedef AutoPtr<VertexBuffer> VertexBufferPtr;

	enum VertexFactoryType
	{
		VF_NULL,
		VF_DEFAULT_MODEL,
		VF_CANVAS,
	};
	class GFX_API VertexFactory:public RefCountedObject
	{
	public:
		virtual s32 GetVertexType()const =0;
		virtual AutoPtr<VertexFactory> Copy()const = 0;
		virtual void OnActive(class SceneNode* node,ShaderParam& param) = 0;
		virtual void OnActiveGraphicThread(const float3x4& view_mat,const float4x4& proj_mat,const EffectUniforms& uniform,ShaderParam& param) = 0;
		virtual const string& GetVertexShader()const = 0;
		VertexDecl			vertex_decl_;
		//string			vertex_shader_;
	};
	typedef AutoPtr<VertexFactory> VertexFactoryPtr;
	class GFX_API DefaultModelVertexFactory:public VertexFactory
	{
	public:
		DefaultModelVertexFactory();
		virtual s32 GetVertexType()const{return VF_DEFAULT_MODEL;}
		virtual void OnActive(SceneNode* node,ShaderParam& param);
		virtual VertexFactoryPtr Copy()const{
			DefaultModelVertexFactory* vf = new DefaultModelVertexFactory;
			vf->vertex_decl_ = vertex_decl_;

			return vf;
		}
		virtual const string& GetVertexShader()const;
		virtual void OnActiveGraphicThread(const float3x4& view_mat,const float4x4& proj_mat,const EffectUniforms& uniform,ShaderParam& param);
	};

	class GFX_API CanvasVertexFactory:public VertexFactory
	{
	public:
		virtual s32 GetVertexType()const{return VF_CANVAS;}
		CanvasVertexFactory();
		virtual void OnActive(SceneNode* node,ShaderParam& param){}
		virtual VertexFactoryPtr Copy()const{
			CanvasVertexFactory* vf = new CanvasVertexFactory;
			vf->vertex_decl_ = vertex_decl_;
			return vf;
		}
		virtual const string& GetVertexShader()const;
		virtual void OnActiveGraphicThread(const float3x4& view_mat,const float4x4& proj_mat,const EffectUniforms& uniform,ShaderParam& param){}
	};
}

#endif