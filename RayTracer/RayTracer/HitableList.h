#pragma once
#ifndef HITABLE_LIST_H
#define HITABLE_LIST_H

#include "Hitable.h"

//Stores any items that can be hit by a ray in the scene
class HitableList : public Hitable {
public:
	HitableList() = default;
	HitableList(Hitable** l, const int n) { list = l; listSize = n; }
	bool Hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override;
	bool BoundingBox(float t0, float t1, AABB& box) const;
	Hitable** list;
	int listSize;
};

 bool HitableList::Hit(const Ray& r, const float tMin, const float tMax, HitRecord& rec) const
{
	HitRecord tempRec;
	bool hitAnything = false;
	double closestSoFar = tMax;
	for (int i = 0; i < listSize; i++) {
		if (list[i]->Hit(r, tMin, closestSoFar, tempRec)) {
			hitAnything = true;
			closestSoFar = tempRec.t;
			rec = tempRec;
		}
	}
	return hitAnything;
}

bool HitableList::BoundingBox(float t0, float t1, AABB& box) const
 {
	 if(listSize < 1) return false;
 	AABB tempBox;
	bool fistTrue = list[0]->BoundingBox(t0, t1, tempBox);
	 if (!fistTrue)
		return false;
	 else
		 box = tempBox;
 	for (int i = 1; i < listSize; i++)
 	{
 		if(list[0]->BoundingBox(t0, t1, tempBox))
			box = SurroundingBox(box, tempBox);
		else
			return false;
 	}
 	return true;
 }
#endif // !HITABLE_LIST_H
