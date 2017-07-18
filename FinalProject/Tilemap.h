#ifndef INCLUDED_TILEMAP_H
#define	INCLUDED_TILEMAP_H

#include <SGE.h>
#include <vector>
using namespace SGE;

class Tilemap
{
public:
	enum Layer { Visual, Collision, Design, MAX };

	Tilemap();
	~Tilemap();

	void Load(const char *layout, const char *texturePack);
	void Unload();
	void Update(const float deltaTime);
	void Render();

	void Save(const char *fileName);

	int GetTile(SVector2 pos) const;
	void SetTile(int x, int y, int value);

	int GetWidth() const { return mWidth * mTileSize; }
	int GetHeight() const { return mHeight * mTileSize; }
	int GetTileSize() const { return mTileSize; }
	void GetWaypoints(std::vector<SVector2> &ref) const { ref = mWaypoints; }
	int GetTile(int x, int y, Layer layer) const;

private:
	NONCOPYABLE(Tilemap);
	
	bool LoadLayout(const char *layout);
	bool LoadTexturePack(const char *texturePack);

	SGE_Sprite* mSprites;
	std::vector<SVector2> mWaypoints;
	int* mTiles[Layer::MAX];
	int mNumSprites;
	int mWidth;
	int mHeight;
	int mTileSize;
	int mMaxWaypoints;
};

#endif