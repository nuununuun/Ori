#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include "cocos2d.h"

class InputManager : public cocos2d::Node {
public:
	static InputManager * getInstance();

	void initialize(cocos2d::Scene * scene);

	/**	
		down
		move
		scroll
		up
	**/
	void setMouseEventCallback(const std::function<void(cocos2d::EventMouse * e)> &callback, const std::string &eventname);

	/**	
		pressed
		released
	**/
	void setKeyboardEventCallback(const std::function<void(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event * e)> &callback, const std::string &eventname);

	/**
		began
		moved
		ended
		cancelled
	**/
	void setTouchesEventCallback(const std::function<void(const std::vector<cocos2d::Touch*>&, cocos2d::Event*)>  &callback, const std::string &eventname);

private:
	InputManager() {}
	virtual ~InputManager() {}

	// mouse event
	void onMouseDown(cocos2d::EventMouse * e);
	void onMouseMove(cocos2d::EventMouse * e);
	void onMouseScroll(cocos2d::EventMouse * e);
	void onMouseUp(cocos2d::EventMouse * e);

	// keyboard event
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event * e);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event * e);

	// touches event
	void onTouchesBegan(const std::vector<cocos2d::Touch*>& t, cocos2d::Event* e);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& t, cocos2d::Event* e);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& t, cocos2d::Event* e);
	void onTouchesCancelled(const std::vector<cocos2d::Touch*>& t, cocos2d::Event* e);

private:
	static InputManager * _instance;

	cocos2d::EventListenerMouse * _mouseListener;
	cocos2d::EventListenerKeyboard * _keyboardListener;
	cocos2d::EventListenerTouchAllAtOnce *_touchesListener;

	// event name, event list
	std::map<std::string, std::vector<std::function<void(cocos2d::EventMouse * e)> > > _mouseEvents;
	std::map<std::string, std::vector<std::function<void(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event * e)> > > _keyboardEvents;
	std::map<std::string, std::vector<std::function<void(const std::vector<cocos2d::Touch*>&, cocos2d::Event*)> > > _touchesEvents;

};

#endif