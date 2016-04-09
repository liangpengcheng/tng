#include "tool/ModelImport.h"
#include "graphics/model.h"
#include "core/os.h"
namespace tng
{
	bool ImportModel(const string src,const string dest)
	{
		AutoPtr<FileModel> model = FileModel::LoadResource(src, false);
	
		FileOutputStream saver(dest);
		model->Save(saver);
		return true;
	}
}