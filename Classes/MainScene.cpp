#include "MainScene.h"

#include "InputManager.h"

USING_NS_CC;
using namespace std;

bool MainScene::init() {
    auto bg = LayerColor::create(Color4B::WHITE);
    this->addChild(bg);

	Vec2 center = Director::getInstance()->getVisibleSize() / 2;
    
    int size = 200;
    
    _paper = Paper::create(size);
    _paper->setPosition(center);
    this->addChild(_paper);
    
    InputManager::getInstance()->initialize(this);
    InputManager::getInstance()->setTouchesEventCallback(CC_CALLBACK_2(MainScene::onTouchesBegan, this), "began");
    InputManager::getInstance()->setTouchesEventCallback(CC_CALLBACK_2(MainScene::onTouchesMoved, this), "moved");
    InputManager::getInstance()->setTouchesEventCallback(CC_CALLBACK_2(MainScene::onTouchesEnded, this), "ended");
    
	return true;
}

void MainScene::onTouchesBegan(const vector<Touch*> &touches, cocos2d::Event *e) {
    
    for (auto &t : touches) {
		_touchId = t->getID();

		_startPoint = _paper->convertToNodeSpace(t->getLocation());
		_endPoint = _startPoint;

		_paper->_debug->clear();
		_paper->_debug->drawDot(_startPoint, 4, Color4F::GREEN);
        
        _paper->selectPolygon(_startPoint);
    }
    
}

void MainScene::onTouchesMoved(const vector<Touch*> &touches, cocos2d::Event *e) {

    for (auto &t : touches) {
		if (_touchId == t->getID()) {

			_endPoint = _paper->convertToNodeSpace(t->getLocation());

			_paper->_debug->clear();
            if (_paper->_selectMode != 0) {
                
                Vec2 from = ((_startPoint - _endPoint).getPerp()) * 10000.0f + (_startPoint + _endPoint) /2;
                Vec2 to = ((_startPoint - _endPoint).getRPerp()) * 10000.0f + (_startPoint + _endPoint) / 2;
                _paper->_debug->drawSegment(from, to, 1, Color4F::BLACK);
                
                _paper->_debug->drawDot(_startPoint, 4, Color4F::GREEN);
                _paper->_debug->drawDot(_endPoint, 4, Color4F::BLUE);
                
                if (_paper->_selectMode == 1) {
                    _paper->_debug->drawDot(*_paper->_selectedVertex, 4, Color4F::ORANGE);
                    
                } else if (_paper->_selectMode == 2) {
                    _paper->_debug->drawSegment(*(*_paper->_selectedEdge).from, *(*_paper->_selectedEdge).to, 2, Color4F::GREEN);
                }
            }
		}
    }
}

void MainScene::onTouchesEnded(const vector<Touch*> &touches, cocos2d::Event *e) {
    for (auto &t : touches) {
		if (_touchId == t->getID()) {
			_touchId = -1;
			
			_endPoint = _paper->convertToNodeSpace(t->getLocation());
            
            if (_paper->_selectMode != 0) {
                Vec2 from = ((_startPoint - _endPoint).getPerp()) * 10000.0f + (_startPoint + _endPoint) /2;
                Vec2 to = ((_startPoint - _endPoint).getRPerp()) * 10000.0f + (_startPoint + _endPoint) / 2;
                
                _paper->splitPolygon(from, to);
            }
		}
    }
}
