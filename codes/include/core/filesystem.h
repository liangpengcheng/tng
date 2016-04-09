#pragma once
#include "core/core_macros.h"
#include "core/types.h"
#include "core/path.h"
#include <string>
#ifdef TNG_ANDROID
#include <android/asset_manager.h>
#endif
namespace tng
{
	class CORE_API FileSystem
	{
	public:
		static size_t 	GetFileSize(const Path& filename);
		static bool 	IsFileExist(const Path& filename);
		static bool 	CreateDir(const Path& dir);
		///does not throw exception when "dirs" already exits
		static bool 	CreateDirs(const Path& dirs);
		static bool 	DeleteDir(const Path& dir);
		///windows system always return 'false'
		static bool		IsLinkDir(const Path& dir);
		static void		handleFileSystemError(const string& utf8str);
		enum FilterOption
		{
			FFile = BIT(0),
			FDir = BIT(1),
			FAll = FFile | FDir
		};
		///find all files or dirs under input param dir
		static bool		FindFiles(const Path& dir, vector<Path>& ret_paths, u32 filters = FileSystem::FFile | FileSystem::FDir, bool recursive = 0);
#ifdef TNG_ANDROID
		static bool		FindFilesInAPK(const Path& dir,vector<Path>& ret_paths);
		static AAssetManager* assetManager__;
#endif // TNG_ANDROID
	};
}