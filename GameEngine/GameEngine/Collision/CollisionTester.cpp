#include "CollisionTester.h"

<<<<<<< HEAD
bool CollisionTester::Test(Collider* colliderA, Collider* colliderB, Simplex& simplex)
=======
bool CollisionTester::Test(const Collider* colliderA, const Collider* colliderB, Simplex& simplex)
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
{
	glm::vec3 direction = glm::vec3(1, 0, 0);
	glm::vec3 support = Support(colliderA, colliderB, direction);
	simplex.PushFront(support);

	direction = -support;

<<<<<<< HEAD
	int counter = 0;
=======
	//int count = 0;
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	while (true)
	{
		support = Support(colliderA, colliderB, direction);

		if (glm::dot(support, direction) <= 0)
			return false;

		simplex.PushFront(support);

		if (NextSimplex(simplex, direction))
			return true;

<<<<<<< HEAD
		counter++;
		if (counter > 50)
			return false;
=======
		/*
		count++;
		if (count > 15)
			return false;
		*/
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	}
}

bool CollisionTester::NextSimplex(Simplex& simplex, glm::vec3& direction)
{
	switch (simplex.GetSize())
	{
	case 2: return LineCase(simplex, direction);
	case 3: return TriangleCase(simplex, direction);
	case 4: return TetrahedronCase(simplex, direction);
	}

	return false;
}

bool CollisionTester::LineCase(Simplex& simplex, glm::vec3& direction)
{
	glm::vec3 a = simplex[0];
	glm::vec3 b = simplex[1];
	glm::vec3 o = glm::vec3(0);

	glm::vec3 ab = b - a;
	glm::vec3 ao = o - a;

	if (SameDirection(ab, ao))
	{
		direction = glm::cross(glm::cross(ab, ao), ab);
	}
	else
	{
		simplex = { a };
		direction = ao;
	}

	return false;
}

bool CollisionTester::TriangleCase(Simplex& simplex, glm::vec3& direction)
{
	glm::vec3 a = simplex[0];
	glm::vec3 b = simplex[1];
	glm::vec3 c = simplex[2];
	glm::vec3 o = glm::vec3(0);

	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 ao = o - a;

	glm::vec3 abc = glm::cross(ab, ac);

	if (SameDirection(glm::cross(abc, ac), ao))
	{
		if (SameDirection(ac, ao))
		{
			simplex = { a, c };
			direction = glm::cross(glm::cross(ac, ao), ac);
		}
		else
		{
			return LineCase(simplex = { a, b }, direction);
		}
	}
	else
	{
		if (SameDirection(glm::cross(ab, abc), ao))
		{
			return LineCase(simplex = { a, b }, direction);
		}
		else
		{
			if (SameDirection(abc, ao))
			{
				direction = abc;
			}
			else
			{
				simplex = { a, c, b };
				direction = -abc;
			}
		}
	}

	return false;
}

bool CollisionTester::TetrahedronCase(Simplex& simplex, glm::vec3& direction)
{
	glm::vec3 a = simplex[0];
	glm::vec3 b = simplex[1];
	glm::vec3 c = simplex[2];
	glm::vec3 d = simplex[3];
	glm::vec3 o = glm::vec3(0);

	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 ad = d - a;
	glm::vec3 ao = o - a;

	glm::vec3 abc = glm::cross(ab, ac);
	glm::vec3 acd = glm::cross(ac, ad);
	glm::vec3 adb = glm::cross(ad, ab);

	if (SameDirection(abc, ao))
	{
		return TriangleCase(simplex = { a, b, c }, direction);
	}

	if (SameDirection(acd, ao))
	{
		return TriangleCase(simplex = { a, c, d }, direction);
	}

	if (SameDirection(adb, ao))
	{
		return TriangleCase(simplex = { a, d, b }, direction);
	}

	return true;
}

<<<<<<< HEAD
glm::vec3 CollisionTester::Support(Collider* colliderA, Collider* colliderB, const glm::vec3& direction)
=======
glm::vec3 CollisionTester::Support(const Collider* colliderA, const Collider* colliderB, const glm::vec3& direction)
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
{
	return colliderA->FindFurthestPoint(direction) - colliderB->FindFurthestPoint(-direction);
}

bool CollisionTester::SameDirection(const glm::vec3& direction, const glm::vec3& ao)
{
	return glm::dot(direction, ao) > 0;
}