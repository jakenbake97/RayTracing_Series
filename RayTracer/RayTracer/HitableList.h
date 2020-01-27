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
	Hitable** list;
	int listSize;
};

inline bool HitableList::Hit(const Ray& r, const float tMin, const float tMax, HitRecord& rec) const {
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
#endif // !HITABLE_LIST_H
