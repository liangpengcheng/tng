#include "graphics/mesh.h"
#include "MathGeoLib/Math/MathConstants.h"
#include "MathGeoLib/Math/MathFunc.h"
#include "core/binary_reader_writer.h"
namespace tng
{
	void VertexDecl::PosNormalUv()
	{
		elem_.clear();
		format_ = MESH_TRIANGLE_LIST;
		VertexElem ve;
		ve.stream_ = 0;
		ve.index_ = 0;
		ve.offset_ = 0;
		ve.start_ = 0;
		ve.type_ = VTYPE_VERTEX;
		ve.format_ = RT_FLOAT3;
		elem_.push_back(ve);

		ve.stream_ = 1;
		ve.index_ = 0;
		ve.offset_ = 0;
		ve.start_ = 0;
		ve.type_ = VTYPE_TEXCOORD;
		ve.format_ = RT_FLOAT2;
		elem_.push_back(ve);

		ve.stream_ = 2;
		ve.index_ = 0;
		ve.offset_ = 0;
		ve.start_ = 0;
		ve.type_ = VTYPE_NORMAL;
		ve.format_ = RT_FLOAT3;
		elem_.push_back(ve);
	}
	void VertexDecl::PosUV()
	{
		elem_.clear();
		format_ = MESH_TRIANGLE_LIST;
		VertexElem ve;
		ve.stream_ = 0;
		ve.index_ = 0;
		ve.offset_ = 0;
		ve.start_ = 0;
		ve.type_ = VTYPE_VERTEX;
		ve.format_ = RT_FLOAT3;
		elem_.push_back(ve);

		ve.stream_ = 1;
		ve.index_ = 0;
		ve.offset_ = 0;
		ve.start_ = 0;
		ve.type_ = VTYPE_TEXCOORD;
		ve.format_ = RT_FLOAT2;
		elem_.push_back(ve);
	}
	void VertexDecl::PosUVColor()
	{
		PosUV();

		VertexElem ve;
		ve.stream_ = 2;
		ve.index_ = 0;
		ve.offset_ = 0;
		ve.start_ = 0;
		ve.type_ = VTYPE_COLOR;
		ve.format_ = RT_BYTE4;
		elem_.push_back(ve);
	}
	MMesh::MMesh() :
		vertex_(NULL)
		, normal_(NULL)
		, uv_(NULL)
		, binormal_(NULL)
		, tangent_(NULL)
		, color_(NULL)
		, vertex_number_(0)
		, u16Faces_(NULL)
		, index_number_(0)
		, vertex_factory_(NULL)
#ifndef NO_RENDER_THREAD
		, using_(false)
#endif
	{

	}
	MMesh::~MMesh()
	{
#ifndef NO_RENDER_THREAD
		Assert(!using_);
#endif
		SAFE_DELETE_ARRAY(vertex_);
		SAFE_DELETE_ARRAY(normal_);
		SAFE_DELETE_ARRAY(uv_);
		SAFE_DELETE_ARRAY(binormal_);
		SAFE_DELETE_ARRAY(color_);
		SAFE_DELETE_ARRAY(tangent_);
		if (vertex_number_ > 0xffff)
		{
			SAFE_DELETE_ARRAY(u32Faces_);
		}
		else
		{
			SAFE_DELETE_ARRAY(u16Faces_);
		}

	}
	MMesh& MMesh::operator =(const MMesh& rh)
	{
#ifndef NO_RENDER_THREAD
		using_ = true;
#endif
		vertex_number_ = rh.vertex_number_;
		index_number_ = rh.index_number_;
		vertex_factory_ = rh.vertex_factory_->Copy();
		if (vertex_number_)
		{
			if (rh.vertex_)
			{
				vertex_ = new float3[vertex_number_];
				memcpy(vertex_, rh.vertex_, sizeof(float3)*vertex_number_);
			}
			if (rh.normal_)
			{
				normal_ = new float3[vertex_number_];
				memcpy(normal_, rh.normal_, sizeof(float3)*vertex_number_);
			}
			if (rh.uv_)
			{
				uv_ = new float2[vertex_number_];
				memcpy(uv_, rh.uv_, sizeof(float2)*vertex_number_);
			}
			if (rh.binormal_)
			{
				binormal_ = new float3[vertex_number_];
				memcpy(binormal_, rh.binormal_, sizeof(float3)*vertex_number_);
			}
			if (rh.tangent_)
			{
				tangent_ = new float3[vertex_number_];
				memcpy(tangent_, rh.tangent_, sizeof(float3)*vertex_number_);
			}
			if (rh.color_)
			{
				color_ = new u32[vertex_number_];
				memcpy(color_, rh.color_, sizeof(u32)*vertex_number_);
			}
		}
		if (index_number_)
		{
			if (index_number_ > 0xffff)
			{
				u32Faces_ = new u32[index_number_];
				memcpy(u32Faces_, rh.u32Faces_, sizeof(u32)*index_number_);
			}
			else
			{
				u16Faces_ = new u16[index_number_];
				memcpy(u16Faces_, rh.u16Faces_, sizeof(u16)*index_number_);
			}
		}
#ifndef NO_RENDER_THREAD
		using_ = false;
#endif
		return *this;
	}


	void Mesh::CreateCanvas(const float2& start, const float2& end)
	{

		memory_mesh_ = new MMesh;
#ifndef NO_RENDER_THREAD
		memory_mesh_->using_ = true;
#endif
		memory_mesh_->vertex_factory_ = new CanvasVertexFactory;
		memory_mesh_->vertex_factory_->vertex_decl_.PosUV();
		memory_mesh_->vertex_number_ = 4;
		memory_mesh_->vertex_ = new float3[memory_mesh_->vertex_number_];
		memory_mesh_->uv_ = new float2[memory_mesh_->vertex_number_];
		render_mesh_ = new RMesh;
		memory_mesh_->vertex_[0] = float3(start.x, start.y, 0);
		memory_mesh_->uv_[0] = float2(0, 0);
		memory_mesh_->vertex_[1] = float3(end.x, start.y, 0);
		memory_mesh_->uv_[1] = float2(1, 0);
		memory_mesh_->vertex_[2] = float3(start.x, end.y, 0);
		memory_mesh_->uv_[2] = float2(0, 1);
		memory_mesh_->vertex_[3] = float3(end.x, end.y, 0);
		memory_mesh_->uv_[3] = float2(1, 1);



		memory_mesh_->index_number_ = 6;
		memory_mesh_->u16Faces_ = new u16[memory_mesh_->index_number_];

		memory_mesh_->u16Faces_[0] = 0;
		memory_mesh_->u16Faces_[1] = 1;
		memory_mesh_->u16Faces_[2] = 2;
		memory_mesh_->u16Faces_[3] = 1;
		memory_mesh_->u16Faces_[4] = 3;
		memory_mesh_->u16Faces_[5] = 2;
#ifndef NO_RENDER_THREAD
		memory_mesh_->using_ = false;
#endif
		CreateRenderMesh(false);
	}
	void Mesh::CreateCanvas(const float2& start, const float2& end, const Color& color)
	{

		memory_mesh_ = new MMesh;
#ifndef NO_RENDER_THREAD
		memory_mesh_->using_ = true;
#endif
		memory_mesh_->vertex_factory_ = new CanvasVertexFactory;
		memory_mesh_->vertex_factory_->vertex_decl_.PosUV();
		memory_mesh_->vertex_number_ = 4;
		memory_mesh_->vertex_ = new float3[memory_mesh_->vertex_number_];
		memory_mesh_->uv_ = new float2[memory_mesh_->vertex_number_];
		memory_mesh_->color_ = new u32[memory_mesh_->vertex_number_];
		render_mesh_ = new RMesh;
		memory_mesh_->vertex_[0] = float3(start.x, start.y, 0);
		memory_mesh_->uv_[0] = float2(0, 0);
		memory_mesh_->color_[0] = color.ToU32();
		memory_mesh_->vertex_[1] = float3(end.x, start.y, 0);
		memory_mesh_->uv_[1] = float2(1, 0);
		memory_mesh_->color_[1] = color.ToU32();
		memory_mesh_->vertex_[2] = float3(start.x, end.y, 0);
		memory_mesh_->uv_[2] = float2(0, 1);
		memory_mesh_->color_[2] = color.ToU32();
		memory_mesh_->vertex_[3] = float3(end.x, end.y, 0);
		memory_mesh_->uv_[3] = float2(1, 1);
		memory_mesh_->color_[3] = color.ToU32();



		memory_mesh_->index_number_ = 6;
		memory_mesh_->u16Faces_ = new u16[memory_mesh_->index_number_];

		memory_mesh_->u16Faces_[0] = 0;
		memory_mesh_->u16Faces_[1] = 1;
		memory_mesh_->u16Faces_[2] = 2;
		memory_mesh_->u16Faces_[3] = 1;
		memory_mesh_->u16Faces_[4] = 3;
		memory_mesh_->u16Faces_[5] = 2;
#ifndef NO_RENDER_THREAD
		memory_mesh_->using_ = false;
#endif
		CreateRenderMesh(false);
	}

	void Mesh::CreateBox(const AABB& box)
	{

		memory_mesh_ = new MMesh;
#ifndef NO_RENDER_THREAD
		memory_mesh_->using_ = true;
#endif
		memory_mesh_->vertex_factory_ = new DefaultModelVertexFactory;
		memory_mesh_->vertex_factory_->vertex_decl_.PosNormalUv();
		memory_mesh_->vertex_number_ = 8;
		memory_mesh_->vertex_ = new float3[memory_mesh_->vertex_number_];
		memory_mesh_->normal_ = new float3[memory_mesh_->vertex_number_];
		memory_mesh_->uv_ = new float2[memory_mesh_->vertex_number_];

		render_mesh_ = new RMesh;

		float w2 = 0.5f * box.Size().x;
		float h2 = 0.5f * box.Size().y;
		float d2 = 0.5f * box.Size().z;

		// Fill in vertex data
		memory_mesh_->vertex_[0] = float3(+w2, +h2, -d2);
		memory_mesh_->normal_[0] = (float3(+w2, +h2, -d2)).Normalized();
		memory_mesh_->uv_[1] = float2(0, 0);

		memory_mesh_->vertex_[1] = (float3(-w2, +h2, -d2));
		memory_mesh_->normal_[1] = (float3(-w2, +h2, -d2)).Normalized();
		memory_mesh_->uv_[1] = float2(0, 1);

		memory_mesh_->vertex_[2] = (float3(-w2, +h2, +d2));
		memory_mesh_->normal_[2] = (float3(-w2, +h2, +d2)).Normalized();
		memory_mesh_->uv_[2] = float2(1, 1);

		memory_mesh_->vertex_[3] = (float3(+w2, +h2, +d2));
		memory_mesh_->normal_[3] = (float3(+w2, +h2, +d2)).Normalized();
		memory_mesh_->uv_[3] = float2(1, 0);

		memory_mesh_->vertex_[4] = (float3(+w2, -h2, -d2));
		memory_mesh_->normal_[4] = (float3(+w2, -h2, -d2)).Normalized();
		memory_mesh_->uv_[4] = float2(0, 0);

		memory_mesh_->vertex_[5] = (float3(-w2, -h2, -d2));
		memory_mesh_->normal_[5] = (float3(-w2, -h2, -d2)).Normalized();
		memory_mesh_->uv_[5] = float2(0, 1);

		memory_mesh_->vertex_[6] = (float3(-w2, -h2, +d2));
		memory_mesh_->normal_[6] = (float3(-w2, -h2, +d2)).Normalized();
		memory_mesh_->uv_[6] = float2(1, 1);

		memory_mesh_->vertex_[7] = (float3(+w2, -h2, +d2));
		memory_mesh_->normal_[7] = (float3(+w2, -h2, +d2)).Normalized();
		memory_mesh_->uv_[7] = float2(1, 0);

		memory_mesh_->index_number_ = 36;
		memory_mesh_->u16Faces_ = new u16[memory_mesh_->index_number_];


		memory_mesh_->u16Faces_[0] = 0;
		memory_mesh_->u16Faces_[1] = 1;
		memory_mesh_->u16Faces_[2] = 2;

		memory_mesh_->u16Faces_[3] = 0;
		memory_mesh_->u16Faces_[4] = 2;
		memory_mesh_->u16Faces_[5] = 3;

		memory_mesh_->u16Faces_[6] = 0;
		memory_mesh_->u16Faces_[7] = 4;
		memory_mesh_->u16Faces_[8] = 5;

		memory_mesh_->u16Faces_[9] = 0;
		memory_mesh_->u16Faces_[10] = 5;
		memory_mesh_->u16Faces_[11] = 1;

		memory_mesh_->u16Faces_[12] = 1;
		memory_mesh_->u16Faces_[13] = 5;
		memory_mesh_->u16Faces_[14] = 6;

		memory_mesh_->u16Faces_[15] = 1;
		memory_mesh_->u16Faces_[16] = 6;
		memory_mesh_->u16Faces_[17] = 2;

		memory_mesh_->u16Faces_[18] = 2;
		memory_mesh_->u16Faces_[19] = 6;
		memory_mesh_->u16Faces_[20] = 7;

		memory_mesh_->u16Faces_[21] = 2;
		memory_mesh_->u16Faces_[22] = 7;
		memory_mesh_->u16Faces_[23] = 3;

		memory_mesh_->u16Faces_[24] = 3;
		memory_mesh_->u16Faces_[25] = 7;
		memory_mesh_->u16Faces_[26] = 4;

		memory_mesh_->u16Faces_[27] = 3;
		memory_mesh_->u16Faces_[28] = 4;
		memory_mesh_->u16Faces_[29] = 0;

		memory_mesh_->u16Faces_[30] = 4;
		memory_mesh_->u16Faces_[31] = 7;
		memory_mesh_->u16Faces_[32] = 6;

		memory_mesh_->u16Faces_[33] = 4;
		memory_mesh_->u16Faces_[34] = 6;
		memory_mesh_->u16Faces_[35] = 5;
#ifndef NO_RENDER_THREAD
		memory_mesh_->using_ = false;
#endif
		CreateRenderMesh(false);
	}

	void Mesh::CreateSphere(f32 radiu, u32 segments /* =20  */, u32 rings /* = 80 */)
	{

		u32 vertices = (rings + 1) * (segments + 1);
		u32 indices = 6 * rings * (segments + 1);
		Assert(indices < 0xffff);
		memory_mesh_ = new MMesh;
#ifndef NO_RENDER_THREAD
		memory_mesh_->using_ = true;
#endif
		memory_mesh_->vertex_factory_ = new DefaultModelVertexFactory;
		memory_mesh_->vertex_factory_->vertex_decl_.PosNormalUv();

		render_mesh_ = new RMesh;

		memory_mesh_->vertex_ = new float3[vertices];
		memory_mesh_->normal_ = new float3[vertices];
		memory_mesh_->uv_ = new float2[vertices];
		memory_mesh_->u16Faces_ = new u16[indices];

		float deltaRingAngle = (pi / rings);
		float deltaSegAngle = (2.0f * pi / segments);

		float3* vertex = memory_mesh_->vertex_;
		float3* normal = memory_mesh_->normal_;
		float2* uv = memory_mesh_->uv_;
		u16* index = memory_mesh_->u16Faces_;

		memory_mesh_->index_number_ = indices;
		memory_mesh_->vertex_number_ = vertices;

		u16 verticeIndex = 0;
		for (u32 ring = 0; ring < rings + 1; ring++)
		{
			float2 r0z0;
			SinCos(ring * deltaRingAngle, r0z0.x, r0z0.y);

			// Generate the group of segments for the current ring		
			for (u32 seg = 0; seg < segments + 1; seg++)
			{
				float2 x0y0;
				SinCos(seg * deltaSegAngle, x0y0.x, x0y0.y);
				x0y0.x *= r0z0.x;
				x0y0.y *= r0z0.x;

				normal->x = x0y0.x;
				normal->y = x0y0.y;
				normal->z = r0z0.y;
				vertex->x = x0y0.x * radiu;
				vertex->y = x0y0.y * radiu;
				vertex->z = r0z0.y * radiu;

				uv->x = f32(seg) / f32(segments);
				uv->y = f32(ring) / f32(rings);

				vertex++;
				normal++;
				uv++;


				// add two indices except for last ring 			
				if (ring != rings)
				{
					*index = verticeIndex + (u16)(segments + 1);
					index++;
					*index = verticeIndex;
					index++;
					*index = verticeIndex + (u16)(segments);
					index++;
					*index = verticeIndex + (u16)(segments + 1);
					index++;
					*index = verticeIndex + 1;
					index++;
					*index = verticeIndex;
					index++;
					verticeIndex++;
				};
			}

		}
#ifndef NO_RENDER_THREAD
		memory_mesh_->using_ = false;
#endif
		CreateRenderMesh(false);
	}
	void Mesh::Load(std::istream& stream)
	{

		if (!memory_mesh_)
			memory_mesh_ = new MMesh;
#ifndef NO_RENDER_THREAD
		memory_mesh_->using_ = true;
#endif
		BinaryReader br(stream);
		br >> memory_mesh_->vertex_number_;
		bool point, uv, normal, binormal,color;
		br >> point;// has point
		br >> uv;// has uv
		br >> normal;// has normal
		br >> binormal;// has binormal
		br >> color;
		br >> memory_mesh_->index_number_;// index num

		memory_mesh_->vertex_factory_ = new DefaultModelVertexFactory;
		memory_mesh_->vertex_factory_->vertex_decl_.format_ = MESH_TRIANGLE_LIST;

		u8 streamidx = 0;
		if (point)
		{
			VertexElem ve;
			ve.stream_ = streamidx++;
			ve.index_ = 0;
			ve.offset_ = 0;
			ve.start_ = 0;
			ve.type_ = VTYPE_VERTEX;
			ve.format_ = RT_FLOAT3;
			memory_mesh_->vertex_factory_->vertex_decl_.elem_.push_back(ve);

			SAFE_DELETE_ARRAY(memory_mesh_->vertex_);
			memory_mesh_->vertex_ = new float3[memory_mesh_->vertex_number_];
		}
		if (uv)
		{
			VertexElem ve;
			ve.stream_ = streamidx++;
			ve.index_ = 0;
			ve.offset_ = 0;
			ve.start_ = 0;
			ve.type_ = VTYPE_TEXCOORD;
			ve.format_ = RT_FLOAT2;
			memory_mesh_->vertex_factory_->vertex_decl_.elem_.push_back(ve);

			SAFE_DELETE_ARRAY(memory_mesh_->uv_);
			memory_mesh_->uv_ = new float2[memory_mesh_->vertex_number_];
		}
		if (normal)
		{
			VertexElem ve;
			ve.stream_ = streamidx++;
			ve.index_ = 0;
			ve.offset_ = 0;
			ve.start_ = 0;
			ve.type_ = VTYPE_NORMAL;
			ve.format_ = RT_FLOAT3;
			memory_mesh_->vertex_factory_->vertex_decl_.elem_.push_back(ve);

			SAFE_DELETE_ARRAY(memory_mesh_->normal_);
			memory_mesh_->normal_ = new float3[memory_mesh_->vertex_number_];
		}
		if (binormal)
		{
			VertexElem ve;
			ve.stream_ = streamidx++;
			ve.index_ = 0;
			ve.offset_ = 0;
			ve.start_ = 0;
			ve.type_ = VTYPE_BINORMAL;
			ve.format_ = RT_FLOAT3;
			memory_mesh_->vertex_factory_->vertex_decl_.elem_.push_back(ve);

			SAFE_DELETE_ARRAY(memory_mesh_->binormal_);
			memory_mesh_->binormal_ = new float3[memory_mesh_->vertex_number_];
		}
		if (binormal)
		{
			VertexElem ve;
			ve.stream_ = streamidx++;
			ve.index_ = 0;
			ve.offset_ = 0;
			ve.start_ = 0;
			ve.type_ = VTYPE_TANGENT;
			ve.format_ = RT_FLOAT3;
			memory_mesh_->vertex_factory_->vertex_decl_.elem_.push_back(ve);

			SAFE_DELETE_ARRAY(memory_mesh_->tangent_);
			memory_mesh_->tangent_ = new float3[memory_mesh_->vertex_number_];
		}
		if (color)
		{
			VertexElem ve;
			ve.stream_ = streamidx++;
			ve.index_ = 0;
			ve.offset_ = 0;
			ve.start_ = 0;
			ve.type_ = VTYPE_COLOR;
			ve.format_ = RT_BYTE4;
			memory_mesh_->vertex_factory_->vertex_decl_.elem_.push_back(ve);

			SAFE_DELETE_ARRAY(memory_mesh_->color_);
			memory_mesh_->color_= new u32[memory_mesh_->vertex_number_];
		}
		if (memory_mesh_->index_number_ > 0xffff)
		{
			SAFE_DELETE_ARRAY(memory_mesh_->u32Faces_);
			memory_mesh_->u32Faces_ = new u32[memory_mesh_->index_number_];
		}
		else
		{
			SAFE_DELETE_ARRAY(memory_mesh_->u32Faces_);
			memory_mesh_->u16Faces_ = new u16[memory_mesh_->index_number_];
		}
		for (u32 i = 0; i < memory_mesh_->vertex_number_; i++)
		{
			if (memory_mesh_->vertex_)
				br >> memory_mesh_->vertex_[i];
			if (memory_mesh_->uv_)
				br >> memory_mesh_->uv_[i];
			if (memory_mesh_->normal_)
				br >> memory_mesh_->normal_[i];
			if (memory_mesh_->binormal_)
				br >> memory_mesh_->binormal_[i];
			if (memory_mesh_->tangent_)
				br >> memory_mesh_->tangent_[i];
			if (memory_mesh_->color_)
				br >> memory_mesh_->color_[i];
		}
		for (u32 i = 0; i<memory_mesh_->index_number_; i++)
		{
			if (memory_mesh_->index_number_>0xffff)
				br >> memory_mesh_->u32Faces_[i];
			else
				br >> memory_mesh_->u16Faces_[i];
		}
		if (render_mesh_)
			SAFE_DELETE(render_mesh_);
		render_mesh_ = new RMesh;
#ifndef NO_RENDER_THREAD
		memory_mesh_->using_ = false;
#endif
		CreateRenderMesh(false);
	}
	void Mesh::Save(std::ostream& stream)
	{

		BinaryWriter bw(stream);
		bw << memory_mesh_->vertex_number_;
		bw << (memory_mesh_->vertex_ ? true : false);// has point
		bw << (memory_mesh_->uv_ ? true : false);// has uv
		bw << (memory_mesh_->normal_ ? true : false);// has normal
		bw << (memory_mesh_->binormal_ ? true : false);// has binormal
		bw << (memory_mesh_->color_ ? true : false);// has color
		bw << memory_mesh_->index_number_;// index num
		for (u32 i = 0; i < memory_mesh_->vertex_number_; i++)
		{
			if (memory_mesh_->vertex_)
				bw << memory_mesh_->vertex_[i];
			if (memory_mesh_->uv_)
				bw << memory_mesh_->uv_[i];
			if (memory_mesh_->normal_)
				bw << memory_mesh_->normal_[i];
			if (memory_mesh_->binormal_)
				bw << memory_mesh_->binormal_[i];
			if (memory_mesh_->tangent_)
				bw << memory_mesh_->tangent_[i];
			if (memory_mesh_->color_)
				bw << memory_mesh_->color_[i];
		}
		for (u32 i = 0; i<memory_mesh_->index_number_; i++)
		{
			if (memory_mesh_->index_number_>0xffff)
				bw << memory_mesh_->u32Faces_[i];
			else
				bw << memory_mesh_->u16Faces_[i];
		}
	}
	RMesh::RMesh() :meshid(0)
	{

	}
	RMesh::~RMesh()
	{

	}

	Mesh::Mesh() :memory_mesh_(NULL), render_mesh_(NULL)
	{

	}
	Mesh::~Mesh()
	{
		SAFE_DELETE(memory_mesh_);
		SAFE_DELETE(render_mesh_);
	}


}
