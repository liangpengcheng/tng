#ifndef __SCENE_2D_H__
#define __SCENE_2D_H__
#include "graphics/scenenode2d.h"
namespace tng
{
	class GFX_API Scene2D:public SceneNode2d
	{
	public:
		Scene2D();

		__inline const float2&	GetMinSize()const{return min_size_;}
		__inline const float2&	GetMaxSize()const{return max_size_;}
		__inline const float2&	GetCurrentSize()const{return current_contex_size_;}

		__inline void			SetMinSize(const float2& m){min_size_ = m;}
		__inline void			SetMaxSize(const float2& m){max_size_ = m;}
		__inline void			SetCurrentize(const float2& m){current_contex_size_ = m;}

		/// get view-space(-1,1) size
		float2					GetFixedSize(const float2& insize)const;
	protected:
		
		/***
			if screen size between min_size_ and max_size_ or min_size_ max_size_ is zero the rev2d object in this scene will maintain the original size,
			if not the size will scale according min_size_ or max_size_.
		*/
		float2				min_size_;
		float2				max_size_;

		float2				current_contex_size_;
	};
	typedef AutoPtr<Scene2D>		Scene2DPtr;
}

#endif