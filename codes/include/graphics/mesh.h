#ifndef _TNG_MESH__H_
#define _TNG_MESH__H_
#include "core/types.h"
#include "core/refobject.h"
#include "core/autoptr.h"
#include "core/threadfun.h"
#include "graphics/color.h"
#include "graphics/renderindexbuffer.h"
#include "graphics/rendervertexbuffer.h"
#include "graphics/rendermaterial.h"
#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/Math/float2.h"
#include "MathGeoLib/Math/float4.h"
#include "core/stl_rebind.h"
#include "MathGeoLib/Geometry/AABB.h"
#include <list>
namespace tng
{
	/// mesh in memory
	struct GFX_API MMesh
	{
		MMesh& operator = (const MMesh& rh);
		MMesh();
		~MMesh();

		float3*			vertex_;
		float2*			uv_;
		float3*			normal_;
		float3*			binormal_;
		float3*			tangent_;
		u32*			color_;
		/// if vertex number >65536 u16Faces_ will be null,else u32Faces_ will be null
		u32				vertex_number_;
		union{
			u16*			u16Faces_;
			u32*			u32Faces_;
		};
		u32					index_number_;
		VertexFactoryPtr	vertex_factory_;
		AtomicCounter		using_;

	};

	///mesh in video memory
	struct GFX_API RMesh
	{
		RMesh();
		~RMesh();

		u32 meshid;
	};

	class GFX_API Mesh :public RefCountedObject
	{
	public:
		Mesh();
		~Mesh();

		void			CreateBox(const AABB& box);

		void			CreateSphere(f32 radiu, u32 segments = 20, u32 rings = 80);

		void			CreateRenderMesh(bool dynamic);

		//-1~1
		void			CreateCanvas(const float2& start, const float2& end);
		void			CreateCanvas(const float2& start, const float2& end, const Color& color);


		void				Load(std::istream& stream);
		void				Save(std::ostream& stream);

		MMesh*			memory_mesh_;
		RMesh*			render_mesh_;
	};
	typedef AutoPtr<Mesh>		MeshPtr;

	struct RenderElem
	{
		RenderElem() :has_emissive_(false), has_diffuse_(false){}
		RMesh											mesh_;
		unordered_map<RenderPass, MaterialRender>	mtl_;
		ShaderParam										mtl_param_;
		bool											has_emissive_;
		bool											has_diffuse_;
		void			render(RenderPass pass, const float3x4 view_mat, const float4x4& proj_mat);
	};

	class RenderLayer
	{
	public:
		list<RenderElem>		render_list_;
	};


}

#endif