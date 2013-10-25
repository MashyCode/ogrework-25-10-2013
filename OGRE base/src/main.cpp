

#include "Ogre.h"
#include <iostream>

#include "InputManager.h"  //the pre-built input handling class
#include <windows.h>

using namespace Ogre;

#ifdef _DEBUG
#pragma comment(lib, "OgreMain_d.lib")
#pragma comment(lib, "OIS_d.lib")
#else
#pragma comment(lib, "OgreMain.lib")
#pragma comment(lib, "OIS.lib")
#endif



INT WINAPI WinMain (
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{

	// tell Root not to load from any plugins or settings file
	Root *root = new Root("", "");

	// Load feature plugins. Scene managers will register 
	// themselves for all scene types they support
	#ifdef _DEBUG
		root->loadPlugin("Plugin_OctreeSceneManager_d.dll");
	#else
		root->loadPlugin("Plugin_OctreeSceneManager.dll");
	#endif

	// load rendersystem plugin(s). The order is important in that GL
	// should be available on on platforms, while D3D9 would be available 
	// only on Windows -- the try/catch will intercept the exception in this
	// case where D3D9 is not available and continue gracefully.
	try {
		#ifdef _DEBUG
			root->loadPlugin("RenderSystem_GL_d.dll");
			root->loadPlugin("RenderSystem_Direct3D9_d.dll");
		#else
			root->loadPlugin("RenderSystem_GL.dll");
			root->loadPlugin("RenderSystem_Direct3D9.dll");
		#endif
	}
	catch (...) {}

	try {
		// We'll simulate the selection of a rendersystem on an arbirtary basis; normally
		// you would have your own code to present the user with options and select the
		// rendersystem on that basis. Since a GUI is beyond the scope of this example, we'll
		// just assume the user selected OpenGL.



		//----------------------------------------//
		const Ogre::RenderSystemList& rList = root->getAvailableRenderers();

		Ogre::RenderSystemList::const_iterator it = rList.begin();
		RenderSystem *rSys = 0;

		//----------------------------------------//
		
		while (it != rList.end()) {
			
			rSys = *(it++);
			if (rSys->getName().find("OpenGL")) {
			
				root->setRenderSystem(rSys);
				break;
			}
		}
		//----------------------------------------//
		

		// check to see if a render system was selected; if we reached the end of the list
		// without selecting a render system then none was found.
		if (rSys == 0) {
			delete root;
			std::cerr << "No RenderSystem available, exiting..." << std::endl;
			return -1;
		}

		// We can initialize Root here if we want. "false" tells Root NOT to create
		// a render window for us
		root->initialise(false);

		// set up the render window with all default params
		RenderWindow *window = root->createRenderWindow(
			"Manual Ogre Window",	// window title
			800,					// window width, in pixels
			600,					// window height, in pixels
			false,					// fullscreen or not 
			0);						// use defaults for all other values


		ResourceGroupManager *rgm =ResourceGroupManager::getSingletonPtr();
		rgm->addResourceLocation("../../meshes","FileSystem");
		rgm->addResourceLocation("../../materials","FileSystem");
		rgm->addResourceLocation("../../materials/scripts","FileSystem");
		rgm->addResourceLocation("../../materials/textures","FileSystem");


		rgm->addResourceLocation("../meshes","FileSystem");

		//rgm->addResourceLocation("../meshes/penguin.zip", "Zip");
		rgm->addResourceLocation("penguin.zip", "Zip");

		rgm->addResourceLocation("../materials","FileSystem");
		rgm->addResourceLocation("../materials/scripts","FileSystem");
		rgm->addResourceLocation("../materials/textures","FileSystem");
		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		
		// setup the default base scene manager -- sufficient for our purposes
		SceneManager* sceneMgr = root->createSceneManager("OctreeSceneManager", "MainManager");
		
		//there are two kinds of shadows, texture shadows and stencil shadows
		//more about this later
		sceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
		sceneMgr->setShadowTextureSize(1024);
		
		sceneMgr->setSkyBox(true, "Examples/SceneSkyBox1",500);
		sceneMgr->setAmbientLight(ColourValue(1,1,1));

		
		//create directional light for terrain light map
		Ogre::Vector3 lightdir(0.55, -0.3, -0.75);
		lightdir.normalise();
 
		Ogre::Light* light = sceneMgr->createLight("testLight");
		light->setType(Ogre::Light::LT_DIRECTIONAL);
		light->setDirection(lightdir);
		light->setDiffuseColour(Ogre::ColourValue::White);
		light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
 
		sceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));


		//create 'player node' to position the player character
		Ogre::SceneNode * mPlayerNode = sceneMgr->getRootSceneNode()->createChildSceneNode("PlayerNode");
		Ogre::SceneNode * mPenguinNode = sceneMgr->getRootSceneNode()->createChildSceneNode("PenguinNode");
		Ogre::SceneNode * mBoxNode = sceneMgr->getRootSceneNode()->createChildSceneNode("BoxNode");


		//set initial position for player
		mPlayerNode->translate(0,0,0);
		mPenguinNode->translate(150,100,0);
		mBoxNode->translate(100,0,0);

		//rotate it about a vertical axis if desired		
		mPlayerNode->rotate(Vector3::UNIT_Y,Ogre::Degree(180),Ogre::Node::TS_LOCAL);
		mPenguinNode->rotate(Vector3::UNIT_Y, Ogre::Degree(0), Ogre::Node::TS_LOCAL);
		mBoxNode->rotate(Vector3::UNIT_Y, Ogre::Degree(0), Ogre::Node::TS_LOCAL);

		//load model
		Entity* ninjaMesh = sceneMgr->createEntity("ninja", "ninja.mesh");
		Entity* penguinMesh = sceneMgr->createEntity("penguin", "penguin.mesh");
		Entity* boxNode = sceneMgr->createEntity("box", "Box001.mesh");

		//attach model to mPlayerNode 
		mPlayerNode->attachObject(ninjaMesh);
		mPenguinNode->attachObject(penguinMesh);
		mBoxNode->attachObject(boxNode);

		//set player to cast shadows
		ninjaMesh->setCastShadows(true);



		// create a single camera, and a viewport that takes up the whole window (default behavior)
		Camera *camera = sceneMgr->createCamera("MainCam");
		//camera->setPolygonMode(Ogre::PolygonMode::PM_WIREFRAME);

		// second cam
		Camera *bCamera = sceneMgr->createCamera("BirdCam");

		Viewport *vp = window->addViewport(camera);
		vp->setDimensions(0.0f, 0.0f, 1.0f, 1.0f);
		camera->setAspectRatio((float)vp->getActualWidth() / (float) vp->getActualHeight());
		camera->setFarClipDistance(1500.0f);
		camera->setNearClipDistance(5.0f);
		
		// second node
		Viewport *bvp = window->addViewport(bCamera,1);
		bvp->setDimensions(0.2f, 0.2f, 0.2f, 0.2f);
		bCamera->setAspectRatio((float)bvp->getActualWidth() / (float) bvp->getActualHeight());
		bCamera->setFarClipDistance(1500.0f);
		bCamera->setNearClipDistance(5.0f);


		//create a node for the camera
		SceneNode* cameraNode = sceneMgr->getRootSceneNode()->createChildSceneNode("CameraNode");
		//attach the camera to it
		cameraNode->attachObject(camera);
		//move it into position
		cameraNode->translate(0,300,300);
		//rotate it downwards about a horizontal axis
		cameraNode->rotate(Ogre::Vector3::UNIT_X,Ogre::Degree(-30));

		// create second camera node
		SceneNode* bCameraNode = sceneMgr->getRootSceneNode()->createChildSceneNode("bCameraNode");
		bCameraNode->attachObject(bCamera);
		bCameraNode->translate(0, 300, 0);
		bCameraNode->rotate(Ogre::Vector3::UNIT_X,Ogre::Degree(-90));

		//create a plane to act as a backdrop
		//next week we will replace this with terrain
		Ogre::MovablePlane* pPlane;
		Ogre::Entity* pPlaneEnt;
		Ogre::SceneNode* pPlaneNode;
		pPlane = new Ogre::MovablePlane("Plane");
		pPlane->d = 0;
		pPlane->normal = Ogre::Vector3::UNIT_Y;
		Ogre::MeshManager::getSingleton().createPlane("PlaneMesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, *pPlane, 500, 500, 32,32, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);
		pPlaneEnt = sceneMgr->createEntity("PlaneEntity", "PlaneMesh");
		pPlaneEnt->setMaterialName("Ogre/Terrain");
		pPlaneEnt->setCastShadows(false);
		pPlaneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
		pPlaneNode->attachObject(pPlaneEnt);
		
		//setup input manager
		InputManager * mInputMgr = InputManager::getSingletonPtr();
		mInputMgr->initialise( window );


		// Run the manual render loop. 
		// quit out of the loop using the Q key
		bool bRenderLoop = true; 

		while (bRenderLoop && window->isActive()) {

			//refresh input system
			mInputMgr->capture();
			
			//render one frame
			bRenderLoop = root->renderOneFrame();

			//check if the user pressed the Q key, and quit if they did
			if(mInputMgr->getKeyboard()->isKeyDown(OIS::KC_Q))
				{             
				bRenderLoop=false;
				} 

			if(mInputMgr->getKeyboard()->isKeyDown(OIS::KC_RIGHT))
				{             
				mPlayerNode->rotate(Vector3::UNIT_Z,Ogre::Degree(0.2),Ogre::Node::TS_LOCAL);
				}
			if (mInputMgr->getKeyboard()->isKeyDown(OIS::KC_LEFT))
			{
				mPlayerNode->rotate(Vector3::UNIT_Z,Ogre::Degree(-0.2),Ogre::Node::TS_LOCAL);
			}

			if (mInputMgr->getKeyboard()->isKeyDown(OIS::KC_UP))
			{
				mPlayerNode->rotate(Vector3::UNIT_X,Ogre::Degree(-0.2),Ogre::Node::TS_LOCAL);
			}

			if (mInputMgr->getKeyboard()->isKeyDown(OIS::KC_DOWN))
			{
				mPlayerNode->rotate(Vector3::UNIT_X,Ogre::Degree(0.2),Ogre::Node::TS_LOCAL);
			}


		}
	}
	catch (Exception &e) {
		std::cerr << e.getFullDescription() << std::endl;
	}

	delete root;
	return 0;
}