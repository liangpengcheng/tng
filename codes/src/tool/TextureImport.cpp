#include "tool/TextureImport.h"
#include "tool/ModelImport.h"
#include "graphics/model.h"
#include "core/os.h"
#include "graphics/texture.h"
namespace tng
{
	bool ImportTexture(const string src,const string dest)
	{
		AutoPtr<FileTexture> texture = FileTexture::LoadResource(src, false);

		FileOutputStream saver(dest);
		texture->Save(saver);
		return true;
	}
}