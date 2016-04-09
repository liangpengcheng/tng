#ifndef __RETURN_CODE_H__
#define __RETURN_CODE_H__
namespace tng
{
	enum RETURN_CODE
	{
		RETURN_SUCCESS =0,
		RETURN_FAILED,
		RETURN_NAME_DUPLICATE,
		RETURN_OPERATOR_DUPLICATE,
		RETURN_NOT_EXIST,
		RETURN_TIMEOUT,
	};
	__inline bool CheckSuccessed(RETURN_CODE r){return !r;}
}
#endif
