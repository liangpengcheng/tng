#include "core/encode.h"
#include "modelimport/model_imp.h"
#include "assimp/include/assimp/cimport.h"
#include "assimp/include/assimp/postprocess.h"
#include "resource/resource_manger.h"
#include "core/binary_reader_writer.h"
#include "assimp/include/assimp/scene.h"
#include "graphics/graphics_macros.h"
namespace tng
{
	
	bool ModelDecoder::Decode(std::istream&stream_in,JsonObject* config,std::ostream&stream_out)
	{
		stream_in.seekg (0, stream_in.end);
		int length = (int)stream_in.tellg();
		stream_in.seekg (0, stream_in.beg);

		if (length>=0)
		{
			BinaryReader reader(stream_in);
			char* buf = new char[length];
			reader.readRaw(buf,length);
			const aiScene* scene = aiImportFileFromMemory(buf,length,aiProcessPreset_TargetRealtime_MaxQuality,GetExt());
			delete []buf;
			if(scene)
			{
				if (scene->HasMeshes())
				{
					std::iostream* cstream = new stringstream(std::ios_base::binary | std::ios_base::in | std::ios_base::out);

					BinaryWriter bw(*cstream);
					bw<<(u32)MODEL_FORMAT_VERSION;
					bw<<(u32)(scene->mNumMeshes);
					for (u32 m=0;m<scene->mNumMeshes;m++)
					{
						aiMesh *mesh = scene->mMeshes[m];
						Assert(mesh->mPrimitiveTypes==aiPrimitiveType_TRIANGLE);
						//mesh header
						bw<<mesh->mNumVertices;
						bw<<(mesh->HasPositions()?true:false);// has point
						bw<<(mesh->HasTextureCoords(0)?true:false);// has uv
						bw<<(mesh->HasNormals()?true:false);// has normal
						bw<<(mesh->HasTangentsAndBitangents()?true:false);//
						bw << false;//color is not set
						bw<<(mesh->mNumFaces*3);// index num

						
						for (u32 i =0 ;i < mesh->mNumVertices;i++ )
						{
							if (mesh->HasPositions())
							{
								bw<<mesh->mVertices[i].x;
								bw<<mesh->mVertices[i].y;
								bw<<mesh->mVertices[i].z;
							}
							if (mesh->HasTextureCoords(0))
							{
								bw<<mesh->mTextureCoords[0][i].x;
								bw<<mesh->mTextureCoords[0][i].y;
							}
							if (mesh->HasNormals())
							{
								bw<<mesh->mNormals[i].x;
								bw<<mesh->mNormals[i].y;
								bw<<mesh->mNormals[i].z;
							}
							if (mesh->HasTangentsAndBitangents())
							{
								bw<<mesh->mBitangents[i].x;
								bw<<mesh->mBitangents[i].y;
								bw<<mesh->mBitangents[i].z;
								bw<<mesh->mTangents[i].x;
								bw<<mesh->mTangents[i].y;
								bw<<mesh->mTangents[i].z;
							}
						}
						for (u32 i =0;i<mesh->mNumFaces;i++)
						{
							if(mesh->mNumFaces*3>0xffff)
							{
								bw<<mesh->mFaces[i].mIndices[0];
								bw<<mesh->mFaces[i].mIndices[1];
								bw<<mesh->mFaces[i].mIndices[2];
							}
							else
							{
								bw<<(u16)mesh->mFaces[i].mIndices[0];
								bw<<(u16)mesh->mFaces[i].mIndices[1];
								bw<<(u16)mesh->mFaces[i].mIndices[2];
							}
							
						}
						
					}
					Coder &coder = *Coder::GetDefaultCoder();
					coder.Encode(*cstream, stream_out);
					delete cstream;
				}
				
				aiReleaseImport(scene);
				return true;
			}else
				return false;
		}else
			return false;


	}
	
}