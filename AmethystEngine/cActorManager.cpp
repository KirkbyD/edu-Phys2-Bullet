#include "cActorManager.hpp"
#include <iostream>
#include "cModelObject.hpp"
#include "cGameObjectFactory.hpp"

// REMOVE LATER QUICK AND DIRTY
#include "cCinemaCamera.hpp"
extern cCinemaCamera mainCamera;
extern std::vector<cModelObject*> g_vec_pGameObjects;
cGameObjectFactory* ActorMngrFactory;

#pragma region SINGLETON
cActorManager cActorManager::stonActMngr;
cActorManager* cActorManager::GetActorManager() { return &(cActorManager::stonActMngr); }
cActorManager::cActorManager()
	: pMediator(nullptr)
{
	std::cout << "Actor Manager Created" << std::endl;
}
#pragma endregion



void cActorManager::update(sData& data) {
	// check the actors decision trees...
	// stop being stupid and add the actors to the damned complex objects...
	float dot = 0.0f;
	float dist = 0.0f;
	bool isWander = true;
	glm::vec3 targetOrientation;
	for (nDecisionTree::cDecision* dt : data.getSourceGameObj()->GetActor()->GetDecisionTree()) {
		// do the checks for the appropriate decision triggers...
		
		switch (data.getSourceGameObj()->GetActor()->getEnemyType()) {
		case nDecisionTree::eEnemyType::TYPE_A:
			// Step 1: Check the type of trigger...
			// WAY THIS SHOULD BE DONE ONCE OBJECT FACING IS NO LONGER BOUND TO CAMERA
			//targetOrientation = data.getTargetGameObj()->getEulerAngle();
			//targetOrientation.x = cos(targetOrientation.x) * cos(targetOrientation.y);
			//targetOrientation.y = sin(targetOrientation.y);
			//targetOrientation.z = sin(targetOrientation.x) * cos(targetOrientation.y);
			//targetOrientation = glm::normalize(targetOrientation) * -1.0f;
			targetOrientation = mainCamera.GetFront();

			dot = glm::dot(targetOrientation, glm::normalize(data.getSourceGameObj()->getPosition() - data.getTargetGameObj()->getPosition()));

			if (dot > 0.95f) {
				for (nDecisionTree::cDecision* aib : data.getSourceGameObj()->GetActor()->GetDecisionTree()) {
					if (aib->behaviour == nAI::eAIBehviours::FLEE)
						data.getSourceGameObj()->GetActor()->setActiveBehaviour(aib->behaviour);
				}
			}
			else {
				for (nDecisionTree::cDecision* aib : data.getSourceGameObj()->GetActor()->GetDecisionTree()) {
					if (aib->behaviour == nAI::eAIBehviours::SEEK)
						data.getSourceGameObj()->GetActor()->setActiveBehaviour(aib->behaviour);
				}
			}

			/*
				for line of sight use this type of idea (from unity)
				float dot = Vector3.Dot(transform.forward, (other.position - transform.position).normalized);
				if(dot > 0.7f) { Debug.Log("Quite facing");}


				this->front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
				this->front.y = sin(glm::radians(this->pitch));
				this->front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
				this->front = glm::normalize(this->front);

			*/
			break;

		case nDecisionTree::eEnemyType::TYPE_B:
			// pursues the player and evades closest bullet fired by the player...
			for (cModelObject* pco : g_vec_pGameObjects) {
				dist = 0.0f;
				if (pco->actorOwned == "theFighter")
					dist = glm::distance(pco->positionXYZ, data.getSourceGameObj()->getPosition());

				if (dist < data.GetCircleRadius()) {
					// is the bullet close enough
					data.setTargetPos(pco->positionXYZ);
					data.setTargetVel(pco->velocity);
					for (nDecisionTree::cDecision* aib : data.getSourceGameObj()->GetActor()->GetDecisionTree()) {
						if (aib->behaviour == nAI::eAIBehviours::EVADE)
							data.getSourceGameObj()->GetActor()->setActiveBehaviour(aib->behaviour);
					}
				}
			}

			// otherwise pursure
			for (nDecisionTree::cDecision* aib : data.getSourceGameObj()->GetActor()->GetDecisionTree()) {
				if (aib->behaviour == nAI::eAIBehviours::PURSURE)
					data.getSourceGameObj()->GetActor()->setActiveBehaviour(aib->behaviour);
			}

			break;

		case nDecisionTree::eEnemyType::TYPE_C:
			// AI TYPE C NYI
			// approaches the player and maintains a radius, if in that radius faces the player and fires at them...
			// check for firing the bullet if inside the radius...
			dist = glm::distance(data.getTargetGameObj()->getPosition(), data.getSourceGameObj()->getPosition());
			if (dist < data.GetCircleRadius()) {
				// fire bullet its inside the radius...
				iGameObject* created = ActorMngrFactory->CreateMediatedGameObject("model", "Sphere", "enemyBullet");
				cModelObject* theBullet = ((cModelObject*)created);
				theBullet->positionXYZ = data.getSourceGameObj()->getPosition();
				glm::vec3 vel;

				theBullet->velocity = data.getSourceGameObj()->getVelocity() + (100.0f * (glm::normalize(data.getSourceGameObj()->getPosition() - data.getTargetGameObj()->getPosition())));
				theBullet->scale = 0.5f;
				theBullet->v_textureNames = { "Fire" };
				theBullet->v_texureBlendRatio = { 1.0f };
				theBullet->inverseMass = 1.0f;
				theBullet->actorOwned = "enemy";
				theBullet->physicsShapeType = eShapeTypes::SPHERE;
			}
			
			for (nDecisionTree::cDecision* aib : data.getSourceGameObj()->GetActor()->GetDecisionTree()) {
				if (aib->behaviour == nAI::eAIBehviours::APPROACH)
					data.getSourceGameObj()->GetActor()->setActiveBehaviour(aib->behaviour);
			}

			break;

		case nDecisionTree::eEnemyType::TYPE_D:
			// AI TYPE D NYI
			// wanders around for 6 seconds then switches to idle for 3 seconds before resuming wandering, action repeats at 6 and 3 second intervals...
			if (data.GetElapsedTime() > 6.0f && isWander) {
				data.IncrementElapsedTime();
				for (nDecisionTree::cDecision* aib : data.getSourceGameObj()->GetActor()->GetDecisionTree()) {
					if (aib->behaviour == nAI::eAIBehviours::WANDER)
						data.getSourceGameObj()->GetActor()->setActiveBehaviour(aib->behaviour);
				}
			}
			else {
				data.ResetElaspedTime();
				isWander = false;
			}
			if (data.GetElapsedTime() > 3.0f && !isWander) {
				data.IncrementElapsedTime();
				for (nDecisionTree::cDecision* aib : data.getSourceGameObj()->GetActor()->GetDecisionTree()) {
					if (aib->behaviour == nAI::eAIBehviours::IDLE)
						data.getSourceGameObj()->GetActor()->setActiveBehaviour(aib->behaviour);
				}
			}
			else {
				data.ResetElaspedTime();
				isWander = true;
			}

			break;

		default:
			// HAS NO AI ASSIGNED TO IT
			break;
		}
	}
}



#pragma region MEDIATOR SETUP
void cActorManager::setMediatorPointer(iMediatorInterface* pMediator) { this->pMediator = pMediator; }
#pragma endregion



#pragma region MEDIATOR COMMUNICATIONS
sData cActorManager::RecieveMessage(sData& data) {
	data.setResult(OK);
	return data;
}
#pragma endregion
