#include "util/bitmap.h"
#include "menu/menu_global.h"
#include "menu/menu.h"
#include "menu/menu_global.h"
#include "menu/menu_option.h"
#include "menu/option_level.h"
#include "util/funcs.h"
#include "util/keyboard.h"
#include "util/sound.h"
#include "util/token.h"
#include "util/tokenreader.h"
#include "globals.h"
#include "resource.h"
#include "init.h"
#include "configuration.h"
#include "music.h"

std::priority_queue<std::string> MenuGlobals::lastPlayed;

std::priority_queue<std::string> MenuGlobals::selectSound;

std::string MenuGlobals::level = "";

MenuGlobals::MenuGlobals(){
}

MenuGlobals::~MenuGlobals(){
}

void MenuGlobals::setMusic(const std::string &file){
	lastPlayed.push(file);
	if(Music::loadSong( Util::getDataPath() + file )){
		Music::pause();
		Music::play();
	}
}

const std::string MenuGlobals::currentMusic(){
	if(!lastPlayed.empty()){
		return lastPlayed.top();
	}
	else return std::string();
}

void MenuGlobals::popMusic(){
	if(!lastPlayed.empty()){
		lastPlayed.pop();
		Music::pause();
		Music::loadSong( Util::getDataPath() + lastPlayed.top() );
		Music::play();
	}
}

void MenuGlobals::setSelectSound(const std::string &file){
	selectSound.push(file);
}

const std::string MenuGlobals::currentSelectSound(){
	if(!selectSound.empty()){
		return selectSound.top();
	}
	else return std::string();
}

void MenuGlobals::playSelectSound() throw (LoadException){
	if (!selectSound.empty()){
	      try{
                  Sound * select = Resource::getSound(selectSound.top());
                  select->play();
	      } catch ( const LoadException & ex ) {
		      throw ex;
	      }
	}
}

void MenuGlobals::popSelectSound(){
	if(!selectSound.empty()){
		selectSound.pop();
	}
}

/*! game speed */
double MenuGlobals::getGameSpeed(){
	return Configuration::getGameSpeed();
}

/*! set speed */
void MenuGlobals::setGameSpeed(double s){
	Configuration::setGameSpeed( s );
}

/*! invincible */
bool MenuGlobals::getInvincible(){
	return Configuration::getInvincible();
}

/*! set invincible */
void MenuGlobals::setInvincible(bool i){
	Configuration::setInvincible( i );
}

/*! game fullscreen */
bool MenuGlobals::getFullscreen(){
	return Configuration::getFullscreen();
}

/*! set speed */
void MenuGlobals::setFullscreen(bool f){
	Configuration::setFullscreen( f );
}

/*! game lives */
int MenuGlobals::getLives(){
	return Configuration::getLives();
}

/*! set lives */
void MenuGlobals::setLives(int l){
	Configuration::setLives( l );
}

int MenuGlobals::getNpcBuddies(){
	return Configuration::getNpcBuddies();
}

void MenuGlobals::setNpcBuddies( int i ){
	Configuration::setNpcBuddies( i );
}

std::string MenuGlobals::doLevelMenu(const std::string dir){
    std::vector<std::string> possible = Util::getFiles( Util::getDataPath() + dir + "/", "*.txt" );
	int count = 0;
        for ( vector<string>::iterator it = possible.begin(); it != possible.end(); it++ ){
            string & path = *it;
            path.erase(0, Util::getDataPath().length() + 1);
	    count+=60;
        }
	
	if ( count > 250 ) count = 250;

	if ( possible.size() == 0 ){
		return "no-files";
	}
	
	try{
		Menu temp;
		temp.load(Util::getDataPath() + "menu/level_select.txt");
		temp.backboard.position.height = count;
	
		for ( unsigned int i = 0; i < possible.size(); i++ ){
			OptionLevel *opt = new OptionLevel(0);
			opt->setText(possible[i]);
			opt->setInfoText("Select a set of levels to play");
			temp.addOption(opt);
		}		
		// Run it
		temp.run();
	} catch (const TokenException & ex){
		Global::debug(0) << "There was a problem with the token. Error was:\n  " << ex.getReason() << endl;
		return "";
	} catch (const LoadException & ex){
		Global::debug(0) << "There was a problem loading the level select menu. Error was:\n  " << ex.getReason() << endl;
		return "";
        }
	// Now lets get the level or return
	std::string l = level;
	level = "";
      
      return l;
}

bool MenuGlobals::freeForAll(){
    return Configuration::getPlayMode() == Configuration::FreeForAll;
}

bool MenuGlobals::cooperative(){
    return Configuration::getPlayMode() == Configuration::Cooperative;
}

void MenuGlobals::setFreeForAll(){
    Configuration::setPlayMode(Configuration::FreeForAll);
}

void MenuGlobals::setCooperative(){
    Configuration::setPlayMode(Configuration::Cooperative);
}
