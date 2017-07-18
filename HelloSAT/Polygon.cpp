#include "Polygon.h"

void SPolygon::Update()
{
	centroid.x = 0;
	centroid.y = 0;
	for (auto &p : points)
	{
		centroid += p;
	}

	centroid *= 1.0f / points.size();
}

void SPolygon::Render()
{
	if(points.size() < 2)
	{
		return;
	}

	for(size_t i = 0; i < points.size() - 1; ++i)
	{
		Graphics_DebugLine(points[i], points[i + 1], color);
	}
	Graphics_DebugLine(points[0], points[points.size() - 1], color);

	Graphics_DebugCircle(centroid, 1.0f, color);
}

// vec draw line with a vector
// min & max returns percentage of projection on that line
void SPolygon::Project(SVector2 &vec, float &min, float &max)
{
	if(points.size() < 2)
	{
		return;
	}
	
	min = 0;
	max = 0;

	for(auto p : points)
	{
		float d = Dot(vec, p);
		
		// initialize
		if(min == 0)
		{
			min = d;
		}
		// replace with lower value
		else if(min > d)
		{
			min = d;
		}
		
		// initialize
		if(max == 0)
		{
			max = d;
		}
		// replace with higher value
		else if(max < d)
		{
			max = d;
		}
		
	}
}