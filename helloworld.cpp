/*Drop a rigid body onto an infinite plane.

The simulation disables gravity and applies an explicit force instead.

*/
#include <vector>

#include "PxPhysicsAPI.h"

using namespace physx;

// We need to hold on to these for the duration of the simulation.
PxDefaultAllocator gAllocator;
PxDefaultErrorCallback gErrorCallback;

PxScene *gScene = nullptr;
PxPhysics *gPhysics = nullptr;
PxFoundation *gFoundation = nullptr;
PxDefaultCpuDispatcher *gDispatcher = nullptr;

void setup() {
  // Initialise PhysX.
  gFoundation =
      PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
  gDispatcher = PxDefaultCpuDispatcherCreate(2);
  gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation,
                             PxTolerancesScale(), true);

  // Create a scene without gravity.
  PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
  sceneDesc.gravity = PxVec3(0);
  sceneDesc.cpuDispatcher = gDispatcher;
  sceneDesc.filterShader = PxDefaultSimulationFilterShader;
  gScene = gPhysics->createScene(sceneDesc);
}

void addRigidBodies() {
  // Create a default material.
  PxMaterial *gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

  // Add a static ground plane and assign it the default material.
  PxRigidStatic *groundPlane =
      PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
  gScene->addActor(*groundPlane);

  // Add a second body. The body comprises six collision spheres (left, right,
  // front, back, up down). Each sphere has unit diameter (ie radius 0.5).
  std::vector<PxVec3> offsets = {{1, 0, 0},  {-1, 0, 0}, {0, 1, 0},
                                 {0, -1, 0}, {0, 0, 1},  {0, 0, -1}};
  PxRigidDynamic *body = gPhysics->createRigidDynamic(PxTransform(0, 2, 0));
  for (PxVec3 ofs : offsets) {
    PxShape *sphere = gPhysics->createShape(PxSphereGeometry(0.5), *gMaterial);
    sphere->setLocalPose(PxTransform(ofs));
    body->attachShape(*sphere);
    sphere->release();
  }
  gScene->addActor(*body);
}

void step() {
  // Advance the simulation.
  gScene->simulate(1.0f / 60.0f);
  gScene->fetchResults(true);

  // Determine the number of actors in the scene.
  PxU32 nbActors = gScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
  printf("%d actors:\n", nbActors);

  // Query all actors.
  std::vector<PxRigidDynamic *> actors(nbActors);
  gScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC,
                    reinterpret_cast<PxActor **>(&actors[0]), nbActors);

  // Print the position of all actors and apply a downwards force.
  for (auto actor : actors) {
    PxTransform pose = actor->getGlobalPose();
    printf("  (%.3f, %.3f, %.3f)\n", pose.p[0], pose.p[1], pose.p[2]);

    // Apply a constant force in -y direction.
    actor->addForce(PxVec3(0, -10, 1));
  }
}

void shutdown() {
  PX_RELEASE(gScene);
  PX_RELEASE(gDispatcher);
  PX_RELEASE(gPhysics);
  PX_RELEASE(gFoundation);
}

int main(int argc, char **argv) {
  // Initialise PhysX and setup the scene.
  setup();
  addRigidBodies();

  // Run a few simulation steps.
  for (PxU32 i = 0; i < 3; i++) step();

  // Shutdown and exit.
  shutdown();

  printf("Simulation complete.\n");
  return 0;
}
