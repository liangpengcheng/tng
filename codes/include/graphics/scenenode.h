#ifndef __tng_scenenode_h__
#define __tng_scenenode_h__

#include "graphics_macros.h"
#include <string>
#include "core/object.h"
#include "core/math.h"
#include "core/autoptr.h"
#include "core/stl_rebind.h"


#include "graphics/model.h"

namespace tng
{
	class GFX_API Transform
	{
	public:
		Transform();
		void					update();
		void					updateFromMat();
		float3					position_;
		float3					scale_;
		Quat					rotation_;
		float3x4				transform_;
	};
	enum SubNodeTransform
	{
		EffectByFather			=BIT(0),
		NoBoneRotation			=BIT(1),
	};
	class GFX_API LinkedNode:public Object
	{
	public:
		ENGINE_CLASS_DEF
		LinkedNode();
		typedef unordered_map<string,AutoPtr<LinkedNode> > SceneObjectMap;
		__inline const string&		GetName()const{return name_;}

		__inline void					SetName(const string& name){name_=name;}
		void							AttacheModel(Renderable* model);
		void							DetachModel();
		void							AttacheModel(const ModelInstance& model);
		ModelInstance&					GetModel(){return model_;}
		/**
			Always update when father node update
		*/
		virtual	void					Update(f32 delta_time);

		
		/**
			Attach to father node
		*/
		void							Attache(LinkedNode* father_node,u32 transformcode =EffectByFather);
		/**
			Detach from father
			warning : this operation will reduce reference count ,if the ref count equal 0 ,it will be deleted
		*/
		void							Detach();

		/**
			Insert a scene node to scene.
			this function will increase pnode reference count
		*/
		RETURN_CODE						InsertChild(LinkedNode* pnode);
		/**
			Remove a scene node from scene
			this function will reduce the node's reference count
			warning : this operation will reduce reference count ,if the ref count equal 0 ,it will be deleted,if you don't want it be deleted,hold the return value
		*/
		AutoPtr<LinkedNode>				RemoveChild(const string& node_name);

		AutoPtr<LinkedNode>				FindChildPtr(const string& node_name);

		LinkedNode*						FindChild(const string& node_name);
		__inline		bool			GetActive()const{ return active_; }
		void							SetActive(bool a){ active_ = a; }
	protected:
		/**
			this function will be called when this node marked as an active object
		*/
		virtual void					UpdateActive(f32 delta_time);
		virtual void					UpdateDirty() = 0;
		/**
			this function called only this node in camera
		*/
		virtual void					UpdateIfInCamera(class Camera* camera,f32 delta_time)=0;
		/**
			copy data to render thread from main thread
			this method called in main thread
		*/
		virtual void					_collectDataToRenderService(class SceneRender* sr);
		LinkedNode*						father_;
		ModelInstance					model_;
		string							name_;
		SceneObjectMap					scene_objects_;

		u32								transform_code_;
		bool							dirty_transform_;
		bool							active_;

	};
	class GFX_API SceneNode:public LinkedNode
	{
	public:
		/**
			Set position in local space
		*/
		__inline void					SetLocalTranslation(const float3& ltrans);
		/**
			Set rotation in local space
		*/
		__inline void					SetLocalRotation(const Quat& lrot);
		/**
			Set scale in local space
		*/
		__inline void					SetLocalScale(const float3& scale);
		/**
			Dirty transform data
			When transform changed this function will be called
			call this function the transform will be recalculate in next update
		*/
		__inline void					DirtyTransform(){dirty_transform_ = true;}

		__inline bool					IsTransformDirty()const{return dirty_transform_;}

		/**
			Get transform in local space
		*/
		__inline const Transform&		GetLocalTransform()const{return transform_;}

		float3x4						GetWorldTransform()const;
		class Scene*					FindScene();
		SceneNode*						FatherSceneNode()
		{
			return father_->DynamicCast<SceneNode>();
		}
		const SceneNode*						FatherSceneNode()const
		{
			return father_->DynamicCast<SceneNode>();
		}
	protected:
		
		/**
			this function called only this node in camera
		*/
		virtual void					UpdateIfInCamera(class Camera* camera,f32 delta_time);
		

		virtual void					UpdateDirty();

		void							OnClick(){}
		void							OnDoubleClick(){}

		Transform						transform_;
		
	};
	typedef AutoPtr<LinkedNode>		NodePtr;
	typedef AutoPtr<SceneNode>		SceneNodePtr;
	#include "scenenode.inl"
}

#endif