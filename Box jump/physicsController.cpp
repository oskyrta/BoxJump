//////////////////////////////////////////////////
// Include 
#include "physicsController.h"
#include "gameObject.h"
#include "game.h"
#include "collision.h"

#include <iostream>

//////////////////////////////////////////////////
// Const 
const Vec2 m_zeroVec = Vec2();

//////////////////////////////////////////////////
// Declaration
float Vec2Dot(Vec2 a, Vec2 axis);

//////////////////////////////////////////////////
// Class PhysicsController
PhysicsController::PhysicsController()
{
	m_game = 0;

	m_ups = 200;
	m_deltatime = 1.0f / m_ups;
	m_accumulator = 0;
}

void PhysicsController::initialize(Game* game)
{
	m_game = game;

	m_objects = m_game->getObjects();

	for (int i = 0; i < kMaxObjectsCount; i++)
	{
		m_collisionList[i] = 0;
		m_lastUpdateCollisionList[i] = 0;
	}
}

void PhysicsController::update(float dt, float& alpha)
{
	m_accumulator += dt;

	if (m_accumulator > 0.2f)
		m_accumulator = 0.2f;

	while (m_accumulator > m_deltatime)
	{
		m_accumulator -= m_deltatime;

		updatePhysics();
	}

	alpha = m_accumulator / m_deltatime;
}

void PhysicsController::updatePhysics()
{
	checkCollisions();
	useGravity();
	sendCollisionsToObjects();
	resolveCollisions();

	for (int i = 0; i < kMaxObjectsCount; i++)
	{
		if (m_objects[i] != 0)
			m_objects[i]->physicsUpdate(m_deltatime);
	}
}

void PhysicsController::resolveCollisions()
{
	for (int i = 0; i < kMaxObjectsCount; i++)
	{
		if (m_collisionList[i] != 0 && m_collisionList[i]->m_needToResolve)
		{
			resolveCollision(m_collisionList[i]);
		}
	}
}

void PositionalCorrection(Collision* coll)
{
	float persent = 0.2f;
	float slop = 0.01f;
	Vec2 correction;

	float a_inv_mass = coll->getObjects().object1->getMass().inv_mass;
	float b_inv_mass = coll->getObjects().object2->getMass().inv_mass;

	correction.x = fmax(coll->getDepth() - slop, 0.0f) / (a_inv_mass + b_inv_mass) * coll->getNormal().x * persent;
	correction.y = fmax(coll->getDepth() - slop, 0.0f) / (a_inv_mass + b_inv_mass) * coll->getNormal().y * persent;

	coll->getObjects().object1->addToPosition(correction * a_inv_mass);
	coll->getObjects().object2->addToPosition(correction * -b_inv_mass);
}

float PythagoreanSolve(float a, float b)
{
	return sqrt(a*a + b*b);
}

float getAverage(float a, float b)
{
	return (a*b) / 2;
}

void PhysicsController::resolveCollision(Collision* coll)
{
	GameObject* A = coll->getObjects().object1;
	GameObject* B = coll->getObjects().object2;

	// Get relative velocity
	Vec2 rv = B->getVelocity() - A->getVelocity();

	// Find velocity along collision normal
	float velAlongNormal = Vec2Dot(rv, coll->getNormal());


	// Don't resolve collision if object move in opposite directions
	if (velAlongNormal <= 0)
		return;

	// Find minimal restitution
	float e = fmin(A->getMaterial().restitution, B->getMaterial().restitution);

	// Find impulse
	float j = -(1 + e) * velAlongNormal;
	j /= A->getMass().inv_mass + B->getMass().inv_mass;

	// Find impulse along collision normal
	Vec2 impulse = coll->getNormal() * j;

	// Apply impulse
	A->addVelocity(impulse* -1);
	B->addVelocity(impulse);

	// Correction floating point error
	PositionalCorrection(coll);

	// Get relative velocity after applyng impulse
	rv = B->getVelocity() - A->getVelocity();

	// Find tangent vector
	Vec2 tangent = Vec2(coll->getNormal().y, -coll->getNormal().x) ;

	// Velosity along tangent vector
	float vat = Vec2Dot(rv, tangent);
	if (vat == 0) return;

	tangent = tangent * vat;
	tangent = tangent.normalize();

	// Find impuse
	float jt = -Vec2Dot(rv, tangent);
	jt /= A->getMass().inv_mass + B->getMass().inv_mass;

	// Find static friction 
	float mu = PythagoreanSolve(A->getMaterial().staticFriction, B->getMaterial().staticFriction);

	Vec2 frictionImpuse;

	if (abs(jt) < abs(j * mu))
	{
		// Impulse less than minimal impulse to start move
		frictionImpuse = tangent * jt;
	}
	else
	{
		// Impulse greater than minimal impulse
		// then we need to use dynamic friction
		float dynamicFriction = PythagoreanSolve(A->getMaterial().dynamicFriction, B->getMaterial().dynamicFriction);
		frictionImpuse = tangent * j * dynamicFriction;
	}

	// Apply friction impulse
	A->addVelocity(frictionImpuse * -1);
	B->addVelocity(frictionImpuse);
}

void PhysicsController::useGravity()
{
	for (int i = 0; i < kMaxObjectsCount; i++)
	{
		if (m_objects[i] != 0 && m_objects[i]->objectUseGravity())
		{
			m_objects[i]->addForse(Vec2(0, 120000.0));
		}
	}
}

float Vec2Dot(Vec2 a, Vec2 axis)
{
	// Calculate dot product
	float aLength = a.length();
	float axisLength = axis.length();

	// Test on division by zero
	if (aLength == 0 || axisLength == 0) return 0;

	float cosBetweenVectors = (a.x * axis.x + a.y * axis.y) / (aLength * axisLength);
	return cosBetweenVectors * aLength;
}

void GetInterval(ObjectPolygon* a, Vec2 axis, float& min0, float& max0)
{
	// Set first min and max
	min0 = max0 = Vec2Dot(a->vertex[0] + *a->position, axis);

	for (int i = 1; i < a->vertexCount; i++)
	{
		float dot = Vec2Dot(a->vertex[i] + *a->position, axis);
		// Found min and max points
		if (dot < min0) min0 = dot;
		if (dot > max0) max0 = dot;
	}
}

void PhysicsController::checkCollisions()
{
	float minDepth = 10000;
	Vec2 axis = Vec2();
	Vec2 exitDirection = Vec2();

	bool existCollision = false;
	bool intersected = false;
	int collisionsCount = 0;

	for (int i = 0; i < kMaxObjectsCount; i++)
	{
		m_collisionList[i] = 0;
	}

	for (int obj1 = 0; obj1 < kMaxObjectsCount; obj1++)
	{
		// Go to next object if this object not exist or not active 
		if (m_objects[obj1] == 0 || !m_objects[obj1]->getActive())
			continue;


		for (int obj2 = obj1 + 1; obj2 < kMaxObjectsCount; obj2++)
		{
			// Go to next object if this object not exist or not active 
			if (m_objects[obj2] == 0 || !m_objects[obj2]->getActive())
				continue;

			
			// Go to next object if object is far
			if (abs(m_objects[obj1]->getPosition().x - m_objects[obj2]->getPosition().x) > kCellSize
				||
				abs(m_objects[obj1]->getPosition().y - m_objects[obj2]->getPosition().y) > kCellSize)
				continue;
			

			Vec2 exitVector;
			float depth;
			intersected = true;

			exitDirection = (m_objects[obj1]->getPosition() - m_objects[obj2]->getPosition());
			minDepth = 10000;

			// Check first object normal
			for (int i = 0; i < m_objects[obj1]->getPolygon()->vertexCount; i++)
			{
				if (ObjectsIntersectedOnNormal(m_objects[obj1]->getPolygon(), m_objects[obj2]->getPolygon(), m_objects[obj1]->getNormal(i), depth))
				{
					// Claculate shortest vector
					if (depth <= minDepth)
					{
						minDepth = depth;
						axis = m_objects[obj1]->getNormal(i);

						if (Vec2Dot(exitDirection, axis) < 0 )
							axis = axis * -1;
					}
				}
				else
				{
					intersected = false;
					break;
				}
			}

			// If separating axies not found, check second object normals
			if (intersected)
			{
				for (int i = 0; i < m_objects[obj2]->getPolygon()->vertexCount; i++)
				{
					if (ObjectsIntersectedOnNormal(m_objects[obj1]->getPolygon(), m_objects[obj2]->getPolygon(), m_objects[obj2]->getNormal(i), depth))
					{
						// Claculate shortest vector
						if (depth <= minDepth)
						{
							minDepth = depth;
							axis = m_objects[obj2]->getNormal(i);

							if (Vec2Dot(exitDirection, axis) < 0)
								axis = axis * -1;
						}
					}
					else
					{
						intersected = false;
						break;
					}
				}


				if (intersected)
				{
					existCollision = true;

					if (m_collisionList[collisionsCount] == 0)
						m_collisionList[collisionsCount] = new Collision();

					m_collisionList[collisionsCount]->setObjects(m_objects[obj1], m_objects[obj2]);
					m_collisionList[collisionsCount]->setNormal(axis);
					m_collisionList[collisionsCount]->setDepth(minDepth);
					m_collisionList[collisionsCount]->setEnterDiretction(axis);
					collisionsCount++;
				}
			}
			
			
		}
	}
}

bool PhysicsController::ObjectsIntersectedOnNormal(ObjectPolygon* a, ObjectPolygon* b, Vec2 axis, float& depth)
{
	float min0;
	float max0;
	float min1;
	float max1;

	depth = 0;

	// Found first interval
	GetInterval(a, axis, min0, max0);

	// Found second interval
	GetInterval(b, axis, min1, max1);

	float d0 = min0 - max1;
	float d1 = min1 - max0;

	if (d0 <= 0 && d1 <= 0)
	{
		// Calculate minimal distance to exit
		if (abs(d0) < abs(d1))
		{
			depth = -d0;
		}
		else
		{
			depth = -d1;
		}

		return true;
	}
	else
	{
		return false;
	}
}


void PhysicsController::sendCollisionsToObjects()
{
	for(int i = 0; i < kMaxObjectsCount; i++)
	{
		if (m_collisionList[i] == 0)
			continue;

		bool isCollisionStay = false;
		for (int j = 0; j < kMaxObjectsCount; j++)
		{
			if (m_lastUpdateCollisionList[j] == 0)
				continue;


			if (m_collisionList[i]->getObjects() == m_lastUpdateCollisionList[j]->getObjects())
			{
				m_collisionList[i]->setEnterDiretction(m_lastUpdateCollisionList[j]->getEnterDirection());
				m_collisionList[i]->setState(CollisionState_Stay);
				m_lastUpdateCollisionList[j]->setState(CollisionState_Stay);

				m_collisionList[i]->getObjects().object1->collisionStay(m_collisionList[i]);
				m_collisionList[i]->getObjects().object2->collisionStay(m_collisionList[i]);

				isCollisionStay = true;
				break;
			}
		}
		if (!isCollisionStay) 
		{
			m_collisionList[i]->setState(CollisionState_Start);
			m_collisionList[i]->getObjects().object1->collisionEnter(m_collisionList[i]);
			m_collisionList[i]->getObjects().object2->collisionEnter(m_collisionList[i]);
		}
	};

	for (int i = 0; i < kMaxObjectsCount; i++)
	{
		if (m_lastUpdateCollisionList[i] == 0)
			continue;

		if (m_lastUpdateCollisionList[i]->getState() == CollisionState_Exit)
		{
			m_lastUpdateCollisionList[i]->getObjects().object1->collisionExit(m_lastUpdateCollisionList[i]);
			m_lastUpdateCollisionList[i]->getObjects().object2->collisionExit(m_lastUpdateCollisionList[i]);
		}

		delete m_lastUpdateCollisionList[i];
		m_lastUpdateCollisionList[i] = 0;
	};

	for (int i = 0; i < kMaxObjectsCount; i++)
	{
		if (m_collisionList[i] == 0)
			continue;

		m_lastUpdateCollisionList[i] = m_collisionList[i];
		m_lastUpdateCollisionList[i]->setState(CollisionState_Exit);
	};
}