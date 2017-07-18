#include "Polygon.h"

using namespace SGE;

bool quit = false;

SPolygon red, green, blue, *polygon;
bool redgreen, redblue, greenblue;
SGE_Font text;


// SAT collision check
bool Collided(SPolygon &a, SPolygon &b)
{
	for (size_t i = 0; i < a.Size(); ++i)
	{
		float minA, maxA, minB, maxB;
		SVector2 p;
		i == a.Size() - 1 ?
			p = a.points[i] - a.points[0] :
			p = a.points[i] - a.points[i + 1];

		p.PerpendicularLH();
		a.Project(p, minA, maxA);
		b.Project(p, minB, maxB);

		if (maxA < minB || minA > maxB)
		{
			return false;
		}
	}

	for (size_t i = 0; i < b.Size(); ++i)
	{
		float minA, maxA, minB, maxB;
		SVector2 p;
		i == b.Size() - 1 ?
			p = b.points[i] - b.points[0] :
			p = b.points[i] - b.points[i + 1];

		p.PerpendicularLH();
		a.Project(p, minA, maxA);
		b.Project(p, minB, maxB);

		if (maxA < minB || minA > maxB)
		{
			return false;
		}
	}

	return true;
}

void SGE_Initialize()
{
	red.color = 0xFF0000;
	red.Add(SVector2(100.0f, 200.0f));
	red.Add(SVector2(200.0f, 230.0f));
	red.Add(SVector2(300.0f, 180.0f));

	green.color = 0x00FF00;
	green.Add(SVector2(100.0f, 100.0f));
	green.Add(SVector2(100.0f, 200.0f));
	green.Add(SVector2(150.0f, 250.0f));
	green.Add(SVector2(200.0f, 250.0f));
	green.Add(SVector2(250.0f, 200.0f));
	green.Add(SVector2(250.0f, 100.0f));
	green.Add(SVector2(200.0f, 50.0f));
	green.Add(SVector2(150.0f, 50.0f));

	blue.color = 0x0000FF;
	blue.Add(SVector2(500.0f, 200.0f));
	blue.Add(SVector2(200.0f, 200.0f));
	blue.Add(SVector2(100.0f, 300.0f));
	blue.Add(SVector2(300.0f, 400.0f));

	text.Load(28, true, false);
}

void SGE_Terminate()
{
	
}

bool SGE_Update(float deltaTime)
{
	// control
	if (!polygon || Input_IsKeyPressed(Keys::F1))
	{
		polygon = &red;
	}
	else if (Input_IsKeyPressed(Keys::F2))
	{
		polygon = &green;
	}
	else if (Input_IsKeyPressed(Keys::F3))
	{
		polygon = &blue;
	}

	if (Input_IsKeyDown(Keys::A))
	{
		for (auto &p : polygon->points)
		{
			p -= polygon->centroid;
			p.Rotate(-0.1f);
			p += polygon->centroid;
		}
	}
	else if (Input_IsKeyDown(Keys::D))
	{
		for (auto &p : polygon->points)
		{
			p -= polygon->centroid;
			p.Rotate(0.1f);
			p += polygon->centroid;
		}
	}

	if (Input_IsKeyDown(Keys::UP) && polygon->centroid.y > 0)
	{
		for (auto &p : polygon->points)
		{
			p.y -= 4;
		}
	}
	else if (Input_IsKeyDown(Keys::DOWN) && polygon->centroid.y < 768)
	{
		for (auto &p : polygon->points)
		{
			p.y += 4;
		}
	}
	if (Input_IsKeyDown(Keys::LEFT) && polygon->centroid.x > 0)
	{
		for (auto &p : polygon->points)
		{
			p.x -= 4;
		}
	}
	else if (Input_IsKeyDown(Keys::RIGHT) && polygon->centroid.x < 1024)
	{
		for (auto &p : polygon->points)
		{
			p.x += 4;
		}
	}

	red.Update();
	green.Update();
	blue.Update();

	// calculate
	redgreen = Collided(red, green);
	redblue = Collided(red, blue);
	greenblue = Collided(green, blue);

	if(Input_IsKeyPressed(Keys::ESCAPE))
	{
		quit = true;
	}
	return quit;
}

void SGE_Render()
{
	red.Render();
	green.Render();
	blue.Render();

	text.Print("Arrow keys: move", 700, 400);
	text.Print("A & D: rotate", 700, 450);
	text.Print("F1: select red", 700, 500);
	text.Print("F2: select green", 700, 550);
	text.Print("F3: select blue", 700, 600);

	text.Print("red & green", 700, 120);
	text.Print(redgreen ? "yes" : "no", 700, 150);
	
	text.Print("red & blue", 700, 180);
	text.Print(redblue ? "yes" : "no", 700, 210);
	
	text.Print("green & blue", 700, 240);
	text.Print(greenblue ? "yes" : "no", 700, 270);
}