#pragma once
#ifndef HITABLELISTH
#define HITABLELISTH

#include "Hitable.h"

class HitableList : public Hitable {
public:
	HitableList() {}
	HitableList(Hitable **l, int n) { list = l; listSize = n; }
	virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const;
	Hitable **list;
	int listSize;
};

bool HitableList::hit(const ray& r, float tMin, float tMax, hitRecord& rec) const {
	hitRecord tempRec;
	bool hitAnything = false;
	double closestSoFar = tMax;
	for (int i = 0; i < listSize; i++) {
		if (list[i]->hit(r, tMin, closestSoFar, tempRec)) {
			hitAnything = true;
			closestSoFar = tempRec.t;
			rec = tempRec;
		}
	}
	return hitAnything;
}
#endif // !HITABLELISTH
