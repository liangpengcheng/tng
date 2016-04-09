#include "graphics/scenenode.h"

namespace tng
{
	Transform::Transform() : position_(0.f), scale_(1.f), rotation_(Quat::identity),transform_(float3x4::identity)
	{

	}

	void Transform::update()
	{
		transform_ = float3x4::FromTRS(position_,rotation_,scale_);
	}
	void Transform::updateFromMat()
	{
		scale_ = transform_.GetScale();
		position_ = transform_.TranslatePart();
		rotation_ = transform_.RotatePart().ToQuat();
		rotation_.Normalize();
	}
	LinkedNode::LinkedNode() :dirty_transform_(true), father_(NULL), transform_code_(0), active_(true)
	{
	}

	float3x4 SceneNode::GetWorldTransform()const
	{
		float3x4 mat = GetLocalTransform().transform_;
		if (father_&&(transform_code_&EffectByFather))
		{
			float3x4 fmat = FatherSceneNode()->GetWorldTransform();
			mat = fmat*mat;
		}
		return mat;
	}

	RETURN_CODE LinkedNode::InsertChild(LinkedNode* pnode)
	{
		string node_name = pnode->GetName();
		Assert(node_name.length()>0);
		SceneObjectMap::iterator it_find = scene_objects_.find(node_name);
		if (it_find!=scene_objects_.end())
			if ((*it_find).second.get()==pnode)
				return RETURN_OPERATOR_DUPLICATE;
			else
				return RETURN_NAME_DUPLICATE;
		else
		{
			scene_objects_.insert(std::pair<string,AutoPtr<LinkedNode> >(node_name,NodePtr(pnode)));
			pnode->father_ = this;
			return RETURN_SUCCESS;
		}
	}
	NodePtr LinkedNode::RemoveChild(const string& node_name)
	{
		Assert(node_name.length()>0);
		SceneObjectMap::iterator it_find = scene_objects_.find(node_name);
		NodePtr ret;
		if (it_find==scene_objects_.end())
			return NodePtr(NULL);
		else
		{
			(*it_find).second->father_ = NULL;
			ret.assign((*it_find).second);
			scene_objects_.erase(node_name);
		}
		return ret;
	}

	LinkedNode*	LinkedNode::FindChild(const string& node_name)
	{
		Assert(node_name.length()>0);
		SceneObjectMap::iterator it_find = scene_objects_.find(node_name);
		if (it_find!=scene_objects_.end())
			return (*it_find).second.get();
		else
		{
			return NULL;
		}
	}

	NodePtr LinkedNode::FindChildPtr(const string& node_name)
	{
		return NodePtr(FindChild(node_name));
	}
	void LinkedNode::Detach()
	{
		if (father_)
		{
			father_->RemoveChild(name_);
		}
	}
	void LinkedNode::Attache(LinkedNode* father_node,u32 transformcode)
	{
		if (father_)
		{
			//ret == this 
			//use ret to keep this pointer not to be deleted
			NodePtr ret = father_->RemoveChild(name_);
			Assert(ret.get()==this);
			father_node->InsertChild(ret.get());
		}
		else
			father_node->InsertChild(this);

		transform_code_ = transformcode;
	}

	void LinkedNode::AttacheModel(const ModelInstance& model)
	{
		model_=model;
	}

	void LinkedNode::AttacheModel(Renderable* model)
	{
		model_.SetModel(model);
	}
	void LinkedNode::DetachModel()
	{
		model_.Clear();
	}

	void LinkedNode::Update(f32 delta_time)
	{
		if (active_)
		{
			UpdateActive(delta_time);
		}

		for (SceneObjectMap::iterator it = scene_objects_.begin();
			it!= scene_objects_.end();
			it++)
		{
			it->second->Update(delta_time);
		}
	}

	void LinkedNode::UpdateActive(f32 delta_time)
	{
		UpdateDirty();
		UpdateIfInCamera(NULL,delta_time);
	}

	void SceneNode::UpdateDirty()
	{
		if (dirty_transform_)
		{
			dirty_transform_ = false;
			transform_.update();
		}
	}
	START_SUB_ABSTRACT_CLASS(LinkedNode, Object)
		Property(LinkedNode,bool,"Active", GetActive, SetActive)
	END_CLASS
}