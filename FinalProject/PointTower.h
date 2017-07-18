#ifndef INCLUDED_POINTTOWER_H
#define	INCLUDED_POINTTOWER_H

#include "Tower.h"
using namespace SGE;
using namespace std;

class PointTower : public Tower
{
public:
	PointTower();
	~PointTower();

	void Render();
private:

};
#endif