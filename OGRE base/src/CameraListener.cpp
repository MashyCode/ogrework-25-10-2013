#include "CameraListener.h"

CCameraListener::CCameraListener(Ogre::SceneNode * pCameraNode_)
{
	m_pCameraNode=pCameraNode_;
	m_fowardSpeed = 0.0f;
}

CCameraListener::~CCameraListener(void)
{
}

bool 
CCameraListener::frameStarted(const Ogre::FrameEvent& evt)
{
	// evt should be considered the dt
	m_pCameraNode-> translate(0,0, m_fowardSpeed*evt.timeSinceLastFrame, Ogre::SceneNode::TS_LOCAL);
	return true;
}

bool 
CCameraListener::mouseMoved( const OIS::MouseEvent &arg )
{
	float mRotate = 0.95f;

	m_pCameraNode ->yaw(Ogre::Degree(-mRotate * arg.state.X.rel), Ogre::Node::TS_WORLD);
	m_pCameraNode->pitch(Ogre::Degree(-mRotate*arg.state.Y.rel), Ogre::Node::TS_LOCAL);
	return true;
}


bool 
CCameraListener::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return true;
}


bool 
CCameraListener::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return true;
}

bool 
CCameraListener::keyPressed(const OIS::KeyEvent &arg)
{
	if (arg.key==OIS::KC_I)
	{
		//move forward
		m_fowardSpeed = -50.0f;
	}

	if (arg.key==OIS::KC_K)
	{
		//move backwards
		m_fowardSpeed = 50.0f;
	}

	return true;
}



bool 
CCameraListener::keyReleased(const OIS::KeyEvent &arg)
{
	if (arg.key == OIS::KC_I || arg.key == OIS::KC_K)
	{
		m_fowardSpeed = 0.0f;
	}


	return true;
}