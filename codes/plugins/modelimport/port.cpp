#include "assimp/include/assimp/cimport.h"
#include "assimp/include/assimp/postprocess.h"
#include "assimp/include/assimp/scene.h"
#include "resource/resource_manger.h"
#include "modelimport/port.h"
#include "modelimport/texture_imp.h"
namespace tng
{
	struct aiLogStream stream;
	void ReleaseModelImpPlugin()
	{
		aiDetachAllLogStreams();
		TextureDecoder::Exit();
	}
	void InitModelImpPlugin()
	{
		stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"assimp_log.txt");
		aiAttachLogStream(&stream);

		ResourceManger::GetInstance()->AddDecoder("fbx",new ModelDecoder("fbx"));
		ResourceManger::GetInstance()->AddDecoder("md2",new ModelDecoder("md2"));
		ResourceManger::GetInstance()->AddDecoder("md3",new ModelDecoder("md3"));
		ResourceManger::GetInstance()->AddDecoder("md4",new ModelDecoder("md4"));
		ResourceManger::GetInstance()->AddDecoder("md5",new ModelDecoder("md5"));
		ResourceManger::GetInstance()->AddDecoder("ms3d",new ModelDecoder("ms3d"));
		ResourceManger::GetInstance()->AddDecoder("3ds",new ModelDecoder("3ds"));
		ResourceManger::GetInstance()->AddDecoder("blend",new ModelDecoder("blend"));
		ResourceManger::GetInstance()->AddDecoder("bsp",new ModelDecoder("bsp"));
		ResourceManger::GetInstance()->AddDecoder("dae",new ModelDecoder("dae"));
		ResourceManger::GetInstance()->AddDecoder("x",new ModelDecoder("x"));
		ResourceManger::GetInstance()->AddDecoder("obj",new ModelDecoder("obj"));
		TextureDecoder::Init();
	}
}