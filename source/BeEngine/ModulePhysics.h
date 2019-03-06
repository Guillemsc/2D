#ifndef __MODULE_PHYSICS_H__
#define __MODULE_PHYSICS_H__

#include "Module.h"
#include "Box2D.h"
#include "GeometryMath.h"

#define PIXELS_PER_METER 25.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.04f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((float) PIXELS_PER_METER * m)
#define PIXELS_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

#define MIN_TRIANGLE_PIXELS_AREA 14.0f

class Event;
class PhysicsBody;
class PhysicsShape;
enum PhysicsShapeType;

class ModulePhysics : public Module, public b2ContactListener
{
public:
	ModulePhysics();
	~ModulePhysics();

	bool Awake();
	bool CleanUp();
	bool PreUpdate();
	void OnEvent(Event* ev);

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

	PhysicsBody* CreatePhysicsBody();
	void DestroyPhysicsBody(PhysicsBody* body);

	PhysicsShape* CreatePhysicsShape(PhysicsShapeType type);
	void DestroyPhysicsShape(PhysicsShape* shape);

	void SetWorldGravity(const float2& gravity);
	float2 GetWorldGravity() const;

	std::vector<std::vector<float2>> TriangulateShape(const std::vector<float2>& shape);
	bool GetTriangleIsTooSmall(const std::vector<float2>& triangle);

private:
	std::vector<PhysicsBody*> bodies;
	std::vector<PhysicsShape*> shapes;

private:
	b2World* b2world = nullptr;
};

#endif // !__MODULE_PHYSICS_H__