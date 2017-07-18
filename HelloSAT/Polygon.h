#ifndef	INCLUDED_SHAPE_H
#define INCLUDED_SHAPE_H

#include <SGE.h>
#include <vector>
using namespace SGE;

struct SPolygon
{
public:
	void Add(const SVector2 &p) { points.push_back(p); }
	void Clear() { points.clear(); }
	size_t Size() const { return points.size(); }

	void Update();
	void Render();
	void Project(SVector2&, float&, float&);

	SVector2 centroid;

	std::vector<SVector2> points;
	unsigned int color = 0xFFFFFF;
private:
};

#endif