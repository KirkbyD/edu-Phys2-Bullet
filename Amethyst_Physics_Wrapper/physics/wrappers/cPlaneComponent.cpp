#include "cPlaneComponent.hpp"
#include <glm/vec3.hpp>

nPhysics::cPlaneComponent::cPlaneComponent(const sPlaneDef& def, const unsigned& id)
	: iPlaneComponent(id)
	, mPlane(new phys::cPlane(def.Normal, def.Constant)) {
	mRigidBody = new phys::cRigidBody(phys::sRigidBodyDef(), mPlane);	//Uses default rbDef as plane has no elements of it.
}

void nPhysics::cPlaneComponent::GetTransform(glm::mat4& transformOut) {
	mRigidBody->GetTransform(transformOut);
}

bool nPhysics::cPlaneComponent::GetBuildInfo(glm::vec3& normal, float& constant) {
	constant = mPlane->GetConstant();
	normal = mPlane->GetNormal();
	return true;
}

// implement later if planes need to move
bool nPhysics::cPlaneComponent::GetVelocity(glm::vec3& velocityOut) { return false; }
