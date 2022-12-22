#pragma once
#include <PxScene.h>
#include <PxPhysics.h>
#include <vector>
#include <string>
#include <iostream>

#include "RenderUtils.hpp"

using namespace physx;
using namespace std;

class PlayerPlatformCollision : public PxSimulationEventCallback
{
public:
    void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override
    {
        std::cout << "Collision detected between actors " << pairHeader.actors[0]->getName() << " and " << pairHeader.actors[1]->getName() << std::endl;

        // Iterate through the pairs of colliding objects
        for (PxU32 i = 0; i < nbPairs; i++)
        {
            // Access the colliding objects
            PxActor* actor1 = pairHeader.actors[0];
            PxActor* actor2 = pairHeader.actors[1];

            // Do something with the colliding objects
            if (actor1->getConcreteType() == PxConcreteType::eRIGID_DYNAMIC) { // the moving object (player) is actor1
                // stop the player's movement
                PxRigidDynamic* player = (PxRigidDynamic*)actor1;
                player->setLinearVelocity({ 0.0,0.0,0.0 });
                player->setAngularVelocity({0.0,0.0,0.0});

            }
            else { // it must be actor2
                PxRigidDynamic* player = (PxRigidDynamic*)actor2;
                player->setLinearVelocity({ 0.0,0.0,0.0 });
                player->setAngularVelocity({ 0.0,0.0,0.0 });
            }
        }
    }

    void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override {};
    void onWake(PxActor** actors, PxU32 count) override {};
    void onSleep(PxActor** actors, PxU32 count) override {};
    void onTrigger(PxTriggerPair* pairs, PxU32 count) override {
    
        std::cout << "Trigger event detected between actors " << pairs[0].triggerActor->getName() << " and " << pairs[0].otherActor->getName() << std::endl;

    };
    void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override {};
};
