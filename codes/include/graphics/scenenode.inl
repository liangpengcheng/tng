__inline void SceneNode::SetLocalTranslation(const float3& ltrans)
{
	transform_.position_ = ltrans;
	DirtyTransform();
}

__inline void SceneNode::SetLocalRotation(const Quat& lrot)
{
	transform_.rotation_ = lrot;
	DirtyTransform();
}
__inline void SceneNode::SetLocalScale(const float3& scale)
{
	transform_.scale_ = scale;
	DirtyTransform();
}