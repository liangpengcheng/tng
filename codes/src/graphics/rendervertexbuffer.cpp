#include "graphics/rendervertexbuffer.h"
#include "resource/stream_res.h"
namespace tng
{
	DefaultModelVertexFactory::DefaultModelVertexFactory(){
		//vertex_shader_ = "engine:/VertexFactory/local_world_3d.vf";
	}

	CanvasVertexFactory::CanvasVertexFactory(){
		//vertex_shader_="engine:/VertexFactory/screen_space_2d.vf";
	}
	const string& DefaultModelVertexFactory::GetVertexShader()const{
		static string vs;
		if (vs.length() == 0)
		{
			AutoPtr<AsyncStream> ptr = ResourceManger::GetInstance()->Create<AsyncStream>("engine:/VertexFactory/local_world_3d.vf",false);
			std::istream& stram = (ptr).get()->Get().stream();
			stram.seekg(0, stram.beg);
			string line;
			while(std::getline(stram,line))
			{
				vs = vs+line;
			}

		}
		return vs;
	}
	const string& CanvasVertexFactory::GetVertexShader()const{
		static string vs;
		if (vs.length() == 0)
		{
			AutoPtr<AsyncStream> ptr = ResourceManger::GetInstance()->Create<AsyncStream>("engine:/VertexFactory/screen_space_2d.vf",false);
			std::istream& stram = (ptr).get()->Get().stream();
			stram.seekg(0, stram.beg);
			string line;
			while(std::getline(stram,line))
			{
				vs = vs+line;
			}

		}
		return vs;
	}
}