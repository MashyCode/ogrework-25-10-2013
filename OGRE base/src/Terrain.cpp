#include "Terrain.h"

CTerrain::CTerrain(Ogre::SceneManager * pSceneManager_, Ogre::Light * pLight_)
{
    m_bTerrainsImported=false;

	m_pTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();

	m_pTerrainGroup = OGRE_NEW Ogre::TerrainGroup(pSceneManager_, Ogre::Terrain::ALIGN_X_Z, 513, 1000.0f); //size of terrain in world units
    m_pTerrainGroup->setFilenameConvention(Ogre::String("Terrain"), Ogre::String("dat"));
    m_pTerrainGroup->setOrigin(Ogre::Vector3::ZERO);


	configureTerrainDefaults(pSceneManager_,pLight_);

	for (long x = 0; x <= 0; ++x)
        for (long y = 0; y <= 0; ++y)
            defineTerrain(x, y);
 
    // sync load since we want everything in place when we start
    m_pTerrainGroup->loadAllTerrains(true);


	

	if (m_bTerrainsImported) //we had to build the terrain rather than loading from disk
    {
        Ogre::TerrainGroup::TerrainIterator ti = m_pTerrainGroup->getTerrainIterator();
        while(ti.hasMoreElements())
        {
            Ogre::Terrain* t = ti.getNext()->instance;
            initBlendMaps(t);
        }
    }

	m_pTerrainGroup->freeTemporaryResources();



}

CTerrain::~CTerrain(void)
{
	 if (m_bTerrainsImported)
        {
			Ogre::LogManager::getSingleton().logMessage( "++saving terrain dat file++ \n");
            m_pTerrainGroup->saveAllTerrains(true);
            m_bTerrainsImported = false;
        }

	OGRE_DELETE m_pTerrainGroup;
    OGRE_DELETE m_pTerrainGlobals;
}

void 
CTerrain::getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
    img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    if (flipX)
        img.flipAroundY();
    if (flipY)
        img.flipAroundX();
 
}


void 
CTerrain::defineTerrain(long x, long y)
{
	Ogre::String filename = m_pTerrainGroup->generateFilename(x, y);
    if (Ogre::ResourceGroupManager::getSingleton().resourceExists(m_pTerrainGroup->getResourceGroup(), filename))
    {
		//terrain mesh already exists
		Ogre::LogManager::getSingleton().logMessage( "**terrain mesh found**\n");
        m_pTerrainGroup->defineTerrain(x, y);
    }
    else
    {
		//need to create it - first time only
		Ogre::LogManager::getSingleton().logMessage( "**terrain not found, generating from image file**\n");
        Ogre::Image img;
        getTerrainImage(x % 2 != 0, y % 2 != 0, img);
        m_pTerrainGroup->defineTerrain(x, y, &img);
        m_bTerrainsImported = true;
    }
}
    
void 
CTerrain::initBlendMaps(Ogre::Terrain* terrain)
{

	Ogre::Terrain::ImportData& defaultimp = m_pTerrainGroup->getDefaultImportSettings();

    float worldHeight =defaultimp.inputScale;

    Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
    Ogre::Real minHeight0 = 0.50f*worldHeight;
    Ogre::Real fadeDist0 = 0.50f*worldHeight;
    Ogre::Real minHeight1 = 0.70f*worldHeight;
    Ogre::Real fadeDist1 = 0.15f*worldHeight;
    float* pBlend0 = blendMap0->getBlendPointer();
    float* pBlend1 = blendMap1->getBlendPointer();
    for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
    {
        for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
        {
            Ogre::Real tx, ty;
 
            blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
            Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
            Ogre::Real val = (height - minHeight0) / fadeDist0;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend0++ = val;
 
            val = (height - minHeight1) / fadeDist1;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend1++ = val;
        }
    }
    blendMap0->dirty();
    blendMap1->dirty();
    blendMap0->update();
    blendMap1->update();
}
    
void 
CTerrain::configureTerrainDefaults(Ogre::SceneManager * pSceneManager_,Ogre::Light* light)
{
	// Configure global
    m_pTerrainGlobals->setMaxPixelError(8); //A lower number will mean a more accurate terrain, at the cost of performance (because of more vertices). 
    // testing composite map
    m_pTerrainGlobals->setCompositeMapDistance(3000); //how far the Ogre terrain will render the lightmapped terrain.

	// Important to set these so that the terrain knows what to use for derived (non-realtime) data
    m_pTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
    m_pTerrainGlobals->setCompositeMapAmbient(pSceneManager_->getAmbientLight());
    m_pTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());

	// Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& defaultimp = m_pTerrainGroup->getDefaultImportSettings();
    defaultimp.terrainSize = 513; //should match values given to TerrainGroup
    defaultimp.worldSize = 100.0f;  //should match values given to TerrainGroup
    defaultimp.inputScale = 100; // due terrain.png is 8 bpp
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;

	// textures
    defaultimp.layerList.resize(3);
    defaultimp.layerList[0].worldSize = 100;
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    defaultimp.layerList[1].worldSize = 30;
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
    defaultimp.layerList[2].worldSize = 200;
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
	//each layer uses two textures -diffuse_specular - diffuse texture with a specular map in the alpha channel
	//normal_height - normal map with a height map in the alpha channel
}
