#pragma once
#include <Ogre.h>
#include <OgreFrameListener.h>
#include <OISMouse.h>
#include <OISKeyboard.h>

class CTankListener :
	public Ogre::FrameListener,
	public OIS::MouseListener,
	public OIS::KeyListener
{
public:
	CTankListener(Ogre::SceneNode * pHullNode_, Ogre::SceneNode * pTurretNode_, Ogre::SceneNode* pGunNode_);
	~CTankListener(void);

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

	float speed;
	bool m_turretYaw;
	bool m_gunPitch;

private:
	Ogre::SceneNode * m_pHullNode;
	Ogre::SceneNode * m_pTurretNode;
	Ogre::SceneNode * m_pGunNode;


};
