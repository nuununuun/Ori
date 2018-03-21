#include "InputManager.h"

USING_NS_CC;
using namespace std;

InputManager * InputManager::_instance = nullptr;

InputManager * InputManager::getInstance() {
	if (!_instance) _instance = new InputManager();
	return _instance;
}

void InputManager::initialize(Scene * scene) {
	this->autorelease();

	_mouseListener = EventListenerMouse::create();

	_mouseListener->onMouseDown = CC_CALLBACK_1(InputManager::onMouseDown, this);
	_mouseListener->onMouseMove = CC_CALLBACK_1(InputManager::onMouseMove, this);
	_mouseListener->onMouseScroll = CC_CALLBACK_1(InputManager::onMouseScroll, this);
	_mouseListener->onMouseUp = CC_CALLBACK_1(InputManager::onMouseUp, this);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_mouseListener, this);



	_keyboardListener = EventListenerKeyboard::create();

	_keyboardListener->onKeyPressed = CC_CALLBACK_2(InputManager::onKeyPressed, this);
	_keyboardListener->onKeyReleased = CC_CALLBACK_2(InputManager::onKeyReleased, this);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, this);


	
	_touchesListener = EventListenerTouchAllAtOnce::create();

	_touchesListener->onTouchesBegan = CC_CALLBACK_2(InputManager::onTouchesBegan, this);
	_touchesListener->onTouchesMoved = CC_CALLBACK_2(InputManager::onTouchesMoved, this);
	_touchesListener->onTouchesEnded = CC_CALLBACK_2(InputManager::onTouchesEnded, this);
	_touchesListener->onTouchesCancelled = CC_CALLBACK_2(InputManager::onTouchesCancelled, this);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchesListener, this);



	_mouseEvents["down"] = std::vector<std::function<void(EventMouse * e)> >();
	_mouseEvents["move"] = std::vector<std::function<void(EventMouse * e)> >();
	_mouseEvents["scroll"] = std::vector<std::function<void(EventMouse * e)> >();
	_mouseEvents["up"] = std::vector<std::function<void(EventMouse * e)> >();

	_keyboardEvents["pressed"] = std::vector<std::function<void(EventKeyboard::KeyCode code, Event * e)> >();
	_keyboardEvents["released"] = std::vector<std::function<void(EventKeyboard::KeyCode code, Event * e)> >();

	_touchesEvents["began"] = std::vector<std::function<void(const std::vector<cocos2d::Touch*>&, cocos2d::Event*)> >();
	_touchesEvents["moved"] = std::vector<std::function<void(const std::vector<cocos2d::Touch*>&, cocos2d::Event*)> >();
	_touchesEvents["ended"] = std::vector<std::function<void(const std::vector<cocos2d::Touch*>&, cocos2d::Event*)> >();
	_touchesEvents["cancelled"] = std::vector<std::function<void(const std::vector<cocos2d::Touch*>&, cocos2d::Event*)> >();

	scene->addChild(this);
}

void InputManager::setMouseEventCallback(const std::function<void(EventMouse*e)>& callback, const std::string & eventname) {
	_mouseEvents.at(eventname).push_back(callback);
}

void InputManager::setKeyboardEventCallback(const std::function<void(EventKeyboard::KeyCode code, Event * e)>& callback, const std::string & eventname) {
	_keyboardEvents.at(eventname).push_back(callback);
}

void InputManager::setTouchesEventCallback(const std::function<void(const vector<Touch*>&, Event*)>  &callback, const std::string &eventname) {
	_touchesEvents.at(eventname).push_back(callback);
}



void InputManager::onMouseDown(EventMouse * e) {
	for (auto &callback : _mouseEvents["down"]) callback(e);
}

void InputManager::onMouseMove(EventMouse * e) {
	for (auto &callback : _mouseEvents["move"]) callback(e);
}

void InputManager::onMouseScroll(EventMouse * e) {
	for (auto &callback : _mouseEvents["scroll"]) callback(e);
}

void InputManager::onMouseUp(EventMouse * e) {
	for (auto &callback : _mouseEvents["up"]) callback(e);
}



void InputManager::onKeyPressed(EventKeyboard::KeyCode code, Event * e) {
	for (auto &callback : _keyboardEvents["pressed"]) callback(code, e);
}

void InputManager::onKeyReleased(EventKeyboard::KeyCode code, Event * e) {
	for (auto &callback : _keyboardEvents["released"]) callback(code, e);
}



void InputManager::onTouchesBegan(const vector<Touch*>& t, cocos2d::Event* e) {
	for (auto &callback : _touchesEvents["began"]) callback(t, e);
}

void InputManager::onTouchesMoved(const vector<Touch*>& t, cocos2d::Event* e) {
	for (auto &callback : _touchesEvents["moved"]) callback(t, e);
}

void InputManager::onTouchesEnded(const vector<Touch*>& t, cocos2d::Event* e) {
	for (auto &callback : _touchesEvents["ended"]) callback(t, e);
}

void InputManager::onTouchesCancelled(const vector<Touch*>& t, cocos2d::Event* e) {
	for (auto &callback : _touchesEvents["cancelled"]) callback(t, e);
}