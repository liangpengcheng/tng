#ifndef __SCENENODE_2D_H__
#define __SCENENODE_2D_H__
#include "graphics/scenenode.h"
namespace tng
{
	/**
     * Defines the set of alignments for positioning controls and justifying text.
     */
    enum Alignment
    {
        // Specify horizontal alignment, use default vertical alignment (ALIGN_TOP).
        ALIGN_LEFT = 0x01,
        ALIGN_HCENTER = 0x02,
        ALIGN_RIGHT = 0x04,
    
        // Specify vertical alignment, use default horizontal alignment (ALIGN_LEFT).
        ALIGN_TOP = 0x10,
        ALIGN_VCENTER = 0x20,
        ALIGN_BOTTOM = 0x40,

        // Specify both vertical and horizontal alignment.
        ALIGN_TOP_LEFT = ALIGN_TOP | ALIGN_LEFT,
        ALIGN_VCENTER_LEFT = ALIGN_VCENTER | ALIGN_LEFT,
        ALIGN_BOTTOM_LEFT = ALIGN_BOTTOM | ALIGN_LEFT,
        ALIGN_TOP_HCENTER = ALIGN_TOP | ALIGN_HCENTER,
        ALIGN_VCENTER_HCENTER = ALIGN_VCENTER | ALIGN_HCENTER,
        ALIGN_BOTTOM_HCENTER = ALIGN_BOTTOM | ALIGN_HCENTER,
        ALIGN_TOP_RIGHT = ALIGN_TOP | ALIGN_RIGHT,
        ALIGN_VCENTER_RIGHT = ALIGN_VCENTER | ALIGN_RIGHT,
        ALIGN_BOTTOM_RIGHT = ALIGN_BOTTOM | ALIGN_RIGHT
    };

	class GFX_API SceneNode2d:public LinkedNode{
	public:
		ENGINE_CLASS_DEF
		typedef unordered_map<string,AutoPtr<SceneNode2d> > SceneObjectMap;
		SceneNode2d();

	protected:
		/**
			this function called only this node in camera
		*/
		virtual void					UpdateIfInCamera(class Camera* camera,f32 delta_time){}
		
		virtual void					UpdateDirty();
		float2							local_position_;
		Quat							rotation_;
		float2							scale_;
	};
}
#endif