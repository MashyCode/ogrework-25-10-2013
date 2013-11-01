#include "TankListener.h"

CTankListener::CTankListener(Ogre::SceneNode * pHullNode_, Ogre::SceneNode * pTurretNode_, Ogre::SceneNode* pGunNode_)
{

	m_pHullNode = pHullNode_;
	m_pTurretNode = pTurretNode_;
	m_pGunNode = pGunNode_;

	bool m_turretYaw = false;
	bool m_gunPitch = false;

}

CTankListener::~CTankListener(void)
{
}

bool 
CTankListener::frameStarted(const Ogre::FrameEvent& evt)
{
	return true;
}

bool 
CTankListener::mouseMoved( const OIS::MouseEvent &arg )
{
	float mRotate = 0.5;

	if (m_turretYaw)
	{
		m_pTurretNode->yaw(Ogre::Degree(-mRotate * arg.state.X.rel), Ogre::Node::TS_LOCAL);
	}

	if (m_gunPitch)
	{
		m_pGunNode->pitch(Ogre::Degree(mRotate * arg.state.Y.rel), Ogre::Node::TS_LOCAL);
	}
	return true;
}


bool 
CTankListener::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return true;
}


bool 
CTankListener::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return true;
}


bool 
CTankListener::keyPressed(const OIS::KeyEvent &arg)
{
	if (arg.key==OIS::KC_Z)
	{
		m_turretYaw = true;
	}

	if (arg.key==OIS::KC_X)
	{
		m_gunPitch = true;
	}

	return true;
}



bool 
CTankListener::keyReleased(const OIS::KeyEvent &arg)
{
	if (arg.key == OIS::KC_Z)
	{
		m_turretYaw = false;
	}

	if (arg.key == OIS::KC_X)
	{
		m_gunPitch = false;
	}

	return true;
}