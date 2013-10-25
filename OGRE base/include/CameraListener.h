#pragma once
#include <Ogre.h>
#include <OgreFrameListener.h>
#include <OISMouse.h>
#include <OISKeyboard.h>

class CCameraListener :
	public Ogre::FrameListener,
	public OIS::MouseListener,
	public OIS::KeyListener
{
public:
	CCameraListener(Ogre::SceneNode * pCameraNode_);
	~CCameraListener(void);

	//inherited from FrameListener
	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameRenderingQueued(const Ogre::FrameEvent& evt){return true;}
	bool frameEnded(const Ogre::FrameEvent& evt){return true;}

	//inherited from MouseListener
	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	//inheristed from KeyListener
	bool keyPressed(const OIS::KeyEvent &arg);
	bool keyReleased(const OIS::KeyEvent &arg);	

	float m_fowardSpeed;
	
private:
	Ogre::SceneNode * m_pCameraNode;


};
