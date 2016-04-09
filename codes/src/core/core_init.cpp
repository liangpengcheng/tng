#include "core/os.h"
#include "core/object.h"
namespace tng
{
	namespace core
	{
		void InitCore()
		{
			static bool inited = 0;
			if (!inited)
			{
				inited = 1;
				OS::InitPlatform();

				Object::InitStatic();
			}

		}
	}
}