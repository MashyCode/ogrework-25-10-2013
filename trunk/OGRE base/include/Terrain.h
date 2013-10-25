#pragma once

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

class CTerrain
{
public:
	CTerrain(Ogre::SceneManager * pSceneManager_, Ogre::Light * pLight_);
	~CTerrain(void);
	Ogre::TerrainGroup* m_pTerrainGroup;

private:
    Ogre::TerrainGlobalOptions* m_pTerrainGlobals;
    
    bool m_bTerrainsImported;
	
 
    void defineTerrain(long x, long y);
    void initBlendMaps(Ogre::Terrain* terrain);
    void configureTerrainDefaults(Ogre::SceneManager * pSceneManager_,Ogre::Light* light);
	void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img);
	
};
