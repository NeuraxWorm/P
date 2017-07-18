#include "Tilemap.h"

Tilemap::Tilemap()
	: mSprites(nullptr)
	, mNumSprites(0)
	, mWidth(0)
	, mHeight(0)
{
	for(int i = Visual; i < Layer::MAX; ++i)
	{
		mTiles[i] = nullptr;
	}
}

Tilemap::~Tilemap()
{
	ASSERT(mSprites == nullptr, "mSprites not deleted.");
	for(int i = Visual; i < Layer::MAX; ++i)
	{
		ASSERT(mTiles[i] == nullptr, "mTiles not deleted.");
	}
}

void Tilemap::Load(const char *layout, const char *texturePack)
{
	ASSERT(mSprites == nullptr, "mSprites already loaded.");
	ASSERT(mTiles[Visual] == nullptr, "mTiles already loaded.");
	
	bool success = LoadLayout(layout);
	ASSERT(success, "Failed to load layout.");
	
	success = LoadTexturePack(texturePack);
	ASSERT(success, "Failed to load texture pack.");
}

bool Tilemap::LoadLayout(const char *layout)
{
	FILE *file = nullptr;
	errno_t error = fopen_s(&file, layout, "r");
	if(error != 0)
	{
		return false;
	}

	fscanf_s(file, "%*s %d\n", &mWidth);
	ASSERT(mWidth > 0, "Invalid width value");
	fscanf_s(file, "%*s %d\n", &mHeight);
	ASSERT(mHeight > 0, "Invalid height value");
	fscanf_s(file, "%*s %d\n", &mTileSize);
	ASSERT(mTileSize > 0, "Invalid size value");
	fscanf_s(file, "%*s %d\n", &mMaxWaypoints);
	ASSERT(mMaxWaypoints > 0, "Invalid max waypoints");

	mTiles[Visual] = new int[mWidth * mHeight];
	mTiles[Collision] = new int[mWidth * mHeight];
	mWaypoints.resize(mMaxWaypoints);

	for(int y = 0; y < mHeight; ++y)
	{
		for(int x = 0; x < mWidth; ++x)
		{
			const int index = y * mWidth + x;
			char c = fgetc(file);
			mTiles[Visual][index] = c - '0';
		}
		fgetc(file);
	}

	int counter = 0;
	for(int y = 0; y < mHeight; ++y)
	{
		for(int x = 0; x < mWidth; ++x)
		{
			int buf = fgetc(file);
			mTiles[Collision][y * mWidth + x] = buf;
			buf -= '0';
			if(buf < mMaxWaypoints && buf >= 0)
			{
				ASSERT(counter < mMaxWaypoints, "Invalid waypoint numbers");
				mWaypoints[buf].x = (float)(x + 0.5f) * (float)mTileSize;
				mWaypoints[buf].y = (float)(y + 0.5f) * (float)mTileSize;
				counter++;
			}
		}
		fgetc(file);
	}
	fclose(file);
	return true;
}

bool Tilemap::LoadTexturePack(const char *texturePack)
{
	FILE *file = nullptr;
	errno_t error = fopen_s(&file, texturePack, "r");
	if(error != 0)
	{
		return false;
	}

	fscanf_s(file, "%*s %d\n", &mNumSprites);
	ASSERT(mWidth > 0, "Invalid number of textures");

	mSprites = new SGE_Sprite[mNumSprites];

	char filename[256];
	for(int i = 0; i < mNumSprites; ++i)
	{
		fscanf_s(file, "%s", filename, 256);
		mSprites[i].Load(filename);
	}
	fclose(file);
	return true;
}


void Tilemap::Unload()
{
	if(mSprites != nullptr)
	{
		for(int i = 0; i < mNumSprites; ++i)
		{
			mSprites[i].Unload();
		}
		delete[] mSprites;
		mSprites = nullptr;
	}

	for(int i = Visual; i < Layer::MAX; ++i)
	{
		if(mTiles[i] != nullptr)
		{
			delete[] mTiles[i];
			mTiles[i] = nullptr;
		}
	}
}


void Tilemap::Update(float deltaTime)
{
	
}

void Tilemap::Render(/*SVector2 &renPos*/)
{
	for(int y = 0; y < mHeight; ++y)
	{
		for(int x = 0; x < mWidth; ++x)
		{
			const int index = y * mWidth + x;
			const int tile = mTiles[Visual][index];
			SVector2 renderPos((float)(x * mTileSize), (float)(y * mTileSize));
			//renderPos += renPos;
			mSprites[tile].SetPosition(renderPos);
			mSprites[tile].Render();
		}
	}
}

int Tilemap::GetTile(int x, int y, Layer layer) const
{
	if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
	{
		return -1;
	}

	return mTiles[layer][y * mWidth + x];
}