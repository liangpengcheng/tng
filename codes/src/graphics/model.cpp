#include "core/encode.h"
#include "graphics/model.h"
#include "core/binary_reader_writer.h"
#include "resource/resource_manger.h"
#include "graphics/material_compiler.h"
namespace tng
{
	void Model::AddMesh(MeshPtr mesh)
	{
		mesh_.push_back(mesh);
	}
	void Model::Load(std::istream& stream)
	{
		std::iostream* cstream = new stringstream(std::ios_base::binary | std::ios_base::in | std::ios_base::out);
		Coder &coder = *Coder::GetDefaultCoder();
		coder.Decode(stream, *cstream);
		cstream->seekp(0);
		BinaryReader br(*cstream);
		u32 version;
		br>>version;
		u32 sz;
		br>>sz;
		for (u32 i=0;i< sz;i++)
		{
			Mesh* mesh = new Mesh;
			mesh->Load(*cstream);
			mesh_.push_back(mesh);
		}
		delete cstream;
	}
	void Model::Save(std::ostream& stream)
	{
		std::iostream* cstream = new stringstream(std::ios_base::binary | std::ios_base::in | std::ios_base::out);
		BinaryWriter bw(*cstream);
		bw<<(u32)MODEL_FORMAT_VERSION;
		bw<<(u32)mesh_.size();
		for (u32 i =0;i<mesh_.size();i++)
		{
			mesh_[i]->Save(*cstream);
		}
		cstream->seekp(0);
		Coder &coder = *Coder::GetDefaultCoder();
		coder.Encode(*cstream, stream);
		delete cstream;
	}
	AutoPtr<FileModel> FileModel::LoadResource(const string& key, bool async)
	{
		return ResourceManger::GetInstance()->Create<FileModel>(key, async);
	}
	void ModelInstance::SetModel(Renderable* model)
	{
		Clear();
		renderable_holder_= model;
		/// alloc at least one param
		//material_param_.push_back(ShaderParam());
	}

	void ModelInstance::AddMaterial(const string& mtl_name)
	{
		AddMaterial(MaterialManger::instance_->FindMaterial(mtl_name));
	}

	void ModelInstance::AddMaterial(MaterialPtr mtl)
	{
		material_.push_back(mtl);
		material_param_.push_back(mtl->default_param_);
	}

	void ModelInstance::Clear()
	{
		renderable_holder_ = NULL;
		material_param_.clear();
		material_.clear();
	}


}