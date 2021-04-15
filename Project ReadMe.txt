INFO6022 - Physics 2
Project #3: Spheres but now its bullet!
(Decoupling and External Configuration for our existing code base!)

Submission by Dylan Kirkby, Brian Cowan, Ivan Parkhomenko

Created in Visual Studio 2019 Community Edition
Solution runs on Platforms and configurations.

Simply open in visual studio and build, it should all be configured with the correct dependencies.

Relevant Controls:
Keyboard:
	W		Forward Impulse
	S		Backward Impulse
	A		Leftward Impulse
	D		Rightwar Impulse
	Space		Change to next Ball (Current ball shown as yellow)
	' (Apostrophe)	Hotswap physics Library!
Mouse:
	Move		Rotate Camera Around Ball
	Wheel Up	Zoom Out (Camera starts at max distance)
	Wheel Down	Zoom In

	All impulses are relevant to the camera's Front/Right vector.
	The front vector is renormalized with a 0 Y value for this purpose in key commands.
	See cKeyBindings.cpp ln 247 - 299 for impulse controls.
	See cMouseBindings.cpp for the mouse controls.

External files can be found in:
	Libraries	$(SolutionDir)AmethystEngine\data\config\physics\PhysicsLibraries.json
	Entities	$(SolutionDir)AmethystEngine\data\config\complex\Entities.json
	
Libraries are gathered in cFileManager::LoadPhysicsLibraryStruct() utilizing a structure 
in the physics manager to have it store its critical information in a struct - sPhysicsLibs.

Entities are built in cFileManager::BuildEntities() with a series of structures in cFileManager
to capturte each bit of information: sEntitySerializer, sEntity, sPhysicsComponent, sGraphicsComponent.


cPhysicsManager is responsible for the creation of library interface components, being  passed to the 
loader to populate entity's (cComplexObject) physics components as well.
The libraries are loaded in as a .dll in preparations for hotswapping in future, 
and only have the wrapper's make factory function.

Camera in use is cCinemaCamera, which uses smooth(er)step for all movement.
Locked for vertical movement aside from zoom functionality.

Only RK4 has been added successfully to cIntegrator so far. We've opted to leave the other functions in in hopes of 
completing them in future.

The only liberty to know about with the spheres is that 'Fireball' is quite low mass for its size. 
Its quite easy to launch into the air with other spheres as a result.

Debug rendering not attempted via library as yet.


Changes:
The decoupled Branch has been merged back into our development branch, and a ton of comments removed,
so there are a ton of changes to the AmethystEngine project itself.

None should affect the physics though beyond the cPhysicsManager files which have been given a data struct for use in its 
new hotswap function, as well as the ability to assign hex value component IDs via component constructor calls.


In Physics Interfaces
iPhysicsComponent - added a componentID, parentID, and parent Pointer. These or to help facilitate our attempt
at an ECS system via ID's, while the pointer is a temp item to enable me to identify its parent right now for 
the physics hotswapping portion of the assignment.
The ID for the interface is passed into the factory constructors so the object is created with it, requiring an edit
to ever component construction method in the interface and wrapper.

iBall/iPlaneComponents - added a GetBuildInfo() method that retrieves the relevant shape and rigid body data for this specific 
component for use in hotswapping the physics library, and potentially for serialization. Corresponding changes in wrapper.

Amethyst Physics Library
I was looking into incorporating a maximum velocity variable for AI purposex on rigid bodys for an AI purpose. the relevant code 
that attempted to handle that in the Worlds update method is commented out.
The cRigidBody and sRigidBodyDef constructors have changed some as a result, but actual updates should be wholly unaffected.


All movement is currently impulse based so as not to run afoul of the diff requirements, but we would like to swap to force based.