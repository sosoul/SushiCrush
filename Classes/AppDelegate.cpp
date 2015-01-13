#include "AppDelegate.h"

#include "Common/ConfigService.h"
#include "Common/GameDB.h"
#include "Common/ModelService.h"
#include "Common/Resource.h"
#include "Controller/GameController.h"
#include "View/Scene/StartScene.h"

USING_NS_CC;

const Size kDesignResolutionSize = Size(640.0, 1138.0);
const Size kResourceResolution = Size(640.0, 1138.0);
// test frame size
const Size kFrameSize = Size(640.0, 960.0);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
	

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }

	// resolution adaption
	director->setContentScaleFactor(kResourceResolution.height / kDesignResolutionSize.height);
	glview->setFrameSize(kFrameSize.width, kFrameSize.height);
	glview->setDesignResolutionSize(kDesignResolutionSize.width, kDesignResolutionSize.height,
		ResolutionPolicy::NO_BORDER);
	
	std::vector<std::string> searchPath;
	searchPath.push_back("w640");
	searchPath.push_back("fonts");
	searchPath.push_back("config");
	searchPath.push_back("tiledMap");
	CCFileUtils::getInstance()->setSearchPaths(searchPath);

	initialize();
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
	auto scene = StartScene::create();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AppDelegate::initialize() {
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(s_resPlist);
	GameDB::getInstance()->init();
	DataBase::getInstance()->init();
	ModelService::getInstance()->init();
	ConfigService::getInstance()->init();
	GameController::getInstance()->init();
}
