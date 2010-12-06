#include <HUD.h>
#include "TypeConverter.h"
#include "GameApp.h"
#include "functions.h"
#include <HoldingSlotWindow.h>
#include "GameChar.h"

HUD::HUD():
	curDisplayedWeapon(NULL),
	curDisplayedSpell(NULL)
{
	using namespace TypeConverter;
	loadLayout("HUD.layout");
	//getting some elements
	currentSpell = getWindow("currentspell");
	currentWeapon = getWindow("currentweapon");
	//hotkeyWindow[10];

	healthBar = getWindow("hpbar");
	manaBar = getWindow("manabar");

	healthBarContainer = getWindow("hpbar");
	manaBarContainer = getWindow("manabar");

	textDisplayWindow = getWindow("TextOut");

	manaBar->setInheritsTooltipText(true);
	healthBar->setInheritsTooltipText(true);

	hpBarWidth = healthBar->getWidth();
	manaBarWidth = manaBar->getWidth();

	compass = getWindow("compass");
	for(unsigned int i = 0;i<10;i++)
	{	
		CEGUI::String windowName = "hotkey"+cegui_str(i);
		CEGUI::Window *cur = getWindow(windowName);
		//subscribe
		cur->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&HUD::eventHotkeyClicked, this));
		
		cur->setID(i);
		hotkeyWindow[i] = cur;
		hotkey[i] = NULL;
	}
	//addToRoot();
	createCompass();
}

HUD::~HUD()
{
	destroyLayout();
}




void HUD::setDebugText(Ogre::String str,bool add)
{
	
	CEGUI::String test = "";
	if(add)
		test += textDisplayWindow->getText();
	test += str.c_str();


	textDisplayWindow->setText(test);
}

void HUD::updateStatBars(Ogre::Real hp, Ogre::Real mp, int maxHP, int maxMP)
{
	using namespace TypeConverter;

	Ogre::Real manaFactor = mp/float(maxMP);
	Ogre::Real hpFactor = hp/float(maxHP);

	CEGUI::UDim newHPWidth = hpBarWidth;
	newHPWidth.d_scale *= hpFactor;

	CEGUI::UDim newMPWidth = manaBarWidth;
	newMPWidth.d_scale *= manaFactor;

	manaBar->setWidth(newMPWidth);
	healthBar->setWidth(newHPWidth);

	
	CEGUI::String text = cegui_str(Math::Floor(mp))+"/"+cegui_str(maxMP);
	manaBarContainer->setTooltipText(text.c_str());
	

	text = cegui_str(Math::Floor(hp))+"/"+cegui_str(maxHP);
	healthBarContainer->setTooltipText(text.c_str());

}

void HUD::setCompassOrientation(Ogre::Quaternion orient)
{
	compNeedleNode->setOrientation(orient);
}

void HUD::createCompass()
{
	GameApp *app = GameApp::getSingletonPtr();

	

	compSceneMgr	= app->getRoot()->createSceneManager(ST_GENERIC);
	compNeedleNode	= compSceneMgr->getRootSceneNode()->createChildSceneNode();
	Entity *ent		= compSceneMgr->createEntity("compassent","CompassNeedle.mesh");

	SceneNode *meshNode = compNeedleNode->createChildSceneNode();
	meshNode->attachObject(ent);

	compassCam = compSceneMgr->createCamera("compasscam");

	unsigned int width = compass->getPixelSize().d_width;
	unsigned int height = compass->getPixelSize().d_height;
	
	TexturePtr renderTex = TextureManager::getSingleton().createManual( "CompassRttTex", 
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, 
		width, height, 0, PF_R8G8B8A8, TU_RENDERTARGET );

	
	
	compassCam->setAspectRatio(Ogre::Real(width) / Ogre::Real(height));
	compassCam->setPosition(0,1.3,0);
	
	compassCam->pitch(Degree(-90));
	compassCam->setNearClipDistance(0.01);
	
	Ogre::RenderTarget *rttTex = renderTex->getBuffer()->getRenderTarget();
	Viewport *v = rttTex->addViewport(compassCam);

	v->setClearEveryFrame(true);
	v->setBackgroundColour(ColourValue(0,0,0,0));
	
    

    MaterialPtr mat = MaterialManager::getSingleton().create("CompassRttMat",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	TextureUnitState* t = mat->getTechnique(0)->getPass(0)->createTextureUnitState("CompassRttTex");
    
  
	t->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
	
	

	CEGUI::Texture *ceguiTex = &app->getCEGUIRenderer()->createTexture(renderTex);

	CEGUI::String imageSetName = "compassimgset";
	
	CEGUI::Imageset* textureImageSet = &CEGUI::ImagesetManager::getSingletonPtr()->create(imageSetName, *ceguiTex);

	CEGUI::Size size = ceguiTex->getSize();
	
		

	textureImageSet->defineImage("compassneedle", CEGUI::Point(0.0f, 0.0f), CEGUI::Size(size.d_width, size.d_height), CEGUI::Point(0.0f,0.0f));
	

	compass->setProperty("Image", "set:"+imageSetName+" image:compassneedle");
}


void HUD::setDisplayedWeapon(gamedata_item* item)
{
	if(item == curDisplayedWeapon)
		return;

	curDisplayedWeapon = item;
	if(item)
		applyImageToWindow(currentWeapon,item->inv_image.c_str());
	else
		applyImageToWindow(currentWeapon,"");

}

void HUD::setDisplayedSpell(gamedata_spell* spell)
{
	if(spell == curDisplayedSpell)
		return;

	curDisplayedSpell = spell;
	if(spell)
		applyImageToWindow(currentSpell,spell->inv_image.c_str());
	else
		applyImageToWindow(currentSpell,"");

}

bool HUD::eventHotkeyClicked(const CEGUI::EventArgs& e)
{
	const CEGUI::MouseEventArgs& me = 
		static_cast<const CEGUI::MouseEventArgs&>(e);
	unsigned int hotkeyIndex = me.window->getID();
	assert(hotkeyIndex < 10);


	GameApp *app = GameApp::getSingletonPtr();
	GameChar *player = app->getPlayer();

	bool shouldPlaySound = false;

	
   
	UniKey curKey(me.button,me.sysKeys);// = convertCeguiMouseButtonToOis(me.button);	
	/*if(me.sysKeys & CEGUI::Control)
	{	
		curKey.setModifier(OIS::Keyboard::Ctrl);
	}
	else if(me.sysKeys & CEGUI::Alt)
	{
		curKey.setModifier(OIS::Keyboard::Alt);
	}
	else if(me.sysKeys & CEGUI::Shift)
	{
		curKey.setModifier(OIS::Keyboard::Shift);
	}*/
	gamedata_hotkeyable *holding = NULL;

	
	if(app->curHolding->hasItem() && !app->curHolding->hasInventory())
    {
		holding = app->curHolding->getItem().item;
    }
    else if(app->curHolding->hasSpell())
    {
		holding = app->curHolding->getSpell();
    }
	
    if(app->key_is(curKey,keyInvTakeOne))
    {
        if(holding)
        {
            setHotkey(hotkeyIndex,holding);
            shouldPlaySound = true;
        }
    }
    else if(app->key_is(curKey,keyInvUse))
    {
        //löschen
        if(getHotkey(hotkeyIndex))
        {
            setHotkey(hotkeyIndex,0);
            shouldPlaySound = true;
        }
    }
    

	if(shouldPlaySound)
	{		
		app->playPlayerSFX(app->clickSnd);
	}
	return true;
}

bool HUD::eventWeaponClicked(const CEGUI::EventArgs& e)
{
	return true;
}

bool HUD::eventSpellClicked(const CEGUI::EventArgs& e)
{
	return true;
}

void HUD::setHotkey(unsigned int index, gamedata_hotkeyable *item)
{
	assert(index<10);

	CEGUI::Window *field = hotkeyWindow[index];

    if(item)
    {
        //bildchen
		applyImageToWindow(field,item->inv_image.c_str());		
		field->setTooltipText(item->name.c_str());
		
    }
    else
    {
        field->setProperty("Image", "False");		
		field->setTooltipText("");
    }
    hotkey[index] = item;
}

bool HUD::useHotkey(unsigned int index)
{
    assert(index < 10);

    if(!hotkey[index])
        return false;

	GameApp *app = GameApp::getSingletonPtr();

    //check if the player actually has the item
    bool playerHas = false;
    if(hotkey[index]->type == gtItem)
        playerHas = app->getPlayer()->hasItem(static_cast<gamedata_item*>(hotkey[index]));
    else
        playerHas = app->getPlayer()->hasSpell(static_cast<gamedata_spell*>(hotkey[index]));

    if(playerHas)
        return hotkey[index]->onHotkey(app->getPlayer());
    else
        //player does not have the item
        return false;
}
