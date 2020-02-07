#pragma once
#ifndef BVHNODE_H
#define BVHNODE_H
#include "Hitable.h"
#include "Random.h"

class BVHNode : public Hitable
{
public:
	BVHNode() {}
	BVHNode(Hitable** l, int n, float time0, float time1);
	virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;
	virtual bool BoundingBox(float t0, float t1, AABB& b) const;
	Hitable* left;
	Hitable* right;
	AABB box;
};

bool BVHNode::BoundingBox(float t0, float t1, AABB& b) const
{
	b = box;
	return true;
}

bool BVHNode::Hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const
{
	if(box.Hit(r, tMin, tMax))
	{
		HitRecord leftRec, rightRec;
		bool hitLeft = left->Hit(r, tMin,tMax, leftRec);
		bool hitRight = right->Hit(r, tMin, tMax, rightRec);
		if(hitLeft && hitRight)
		{
			if (leftRec.t < rightRec.t)
				rec = leftRec;
			else
				rec = rightRec;
			return true;
		}
		else if(hitLeft)
		{
			rec = leftRec;
			return true;
		}
		else if(hitRight)
		{
			rec = rightRec;
			return true;
		}
		else
			return false;
	}
	return false;
}

int boxXCompare (const void* a, const void* b)
{
	AABB boxLeft, boxRight;
	Hitable* ah = *(Hitable**)a;
	Hitable* bh = *(Hitable**)b;

	if(!ah->BoundingBox(0,0, boxLeft) || !bh->BoundingBox(0,0, boxRight))
		std::cerr << "no bounding box in BVHNode constructor\n";
	if(boxLeft.Min().X() - boxRight.Min().X() < 0.0)
		return -1;
	else
		return  1;
}

int boxYCompare (const void* a, const void* b)
{
	AABB boxLeft, boxRight;
	Hitable* ah = *(Hitable**)a;
	Hitable* bh = *(Hitable**)b;

	if(!ah->BoundingBox(0,0, boxLeft) || !bh->BoundingBox(0,0, boxRight))
		std::cerr << "no bounding box in BVHNode constructor\n";
	if(boxLeft.Min().Y() - boxRight.Min().Y() < 0.0)
		return -1;
	else
		return  1;
}

int boxZCompare (const void* a, const void* b)
{
	AABB boxLeft, boxRight;
	Hitable* ah = *(Hitable**)a;
	Hitable* bh = *(Hitable**)b;

	if(!ah->BoundingBox(0,0, boxLeft) || !bh->BoundingBox(0,0, boxRight))
		std::cerr << "no bounding box in BVHNode constructor\n";
	if(boxLeft.Min().Z() - boxRight.Min().Z() < 0.0)
		return -1;
	else
		return  1;
}

BVHNode::BVHNode(Hitable** l, int n, float time0, float time1)
{
	int axis = int(3 * DRand());
	if(axis == 0)
		qsort(l, n, sizeof(Hitable*), boxXCompare);
	else if(axis == 1)
		qsort(l, n, sizeof(Hitable*), boxYCompare);
	else
		qsort(l, n, sizeof(Hitable*), boxZCompare);
	if(n == 1)
	{
		left = right = l[0];
	}
	else if(n == 2)
	{
		left = l[0];
		right = l[1];
	}
	else
	{
		left = new BVHNode(l, n/2, time0, time1);
		right = new BVHNode(l + n/2, n - n/2, time0, time1);
	}
	AABB boxLeft, boxRight;
	if(!left->BoundingBox(time0, time1, boxLeft) || !right->BoundingBox(time0, time1, boxRight))
		std::cerr << "no bounding box in BVHNode constructor\n";
	box = SurroundingBox(boxLeft, boxRight);
}

#endif
