#pragma once
#include "stl_rebind.h"
#include "core/core_exports.h"
#include "core/memobj.h"
#include "core/variant.h"
namespace tng
{


	class PropertySet
	{
	private:

		///property map
		unordered_map<string, AccesserPtr>	map_property_;
	public:
		typedef unordered_map<string, AccesserPtr> PropertyMap;
		///return false means already has property named @name
		__inline bool				AddProperty(const string& name, AccesserPtr pty){
			if (map_property_.find(name) == map_property_.end())
			{
				map_property_.insert(std::pair<string, AccesserPtr>(name, pty));
				return true;
			}
			else
				return false;
		}
		///
		__inline AccesserPtr			GetProperty(const string& name){

			unordered_map<string, AccesserPtr>::iterator it = map_property_.find(name);
			if (it != map_property_.end())
			{
				return it->second;
			}
			else{
				return AccesserPtr();
			}
		}
		__inline size_t				Size(){ return map_property_.size(); }

		__inline PropertyMap::iterator Begin(){ return map_property_.begin(); }
		__inline PropertyMap::iterator End(){ return map_property_.end(); }
	};
}