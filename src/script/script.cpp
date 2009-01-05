#ifdef HAVE_PYTHON
#include "modules/python.h"
#endif

#include "script.h"
#include "globals.h"
#include "character.h"
#include "object/character.h"
#include <string>

using namespace std;

namespace Script{

    Engine * Engine::theEngine = NULL;

    Engine::Engine(){
        if (theEngine != NULL){
            delete theEngine;
        }
        theEngine = this;
    }

    Engine::~Engine(){
    }

    Engine * Engine::getEngine(){
        return theEngine;
    }

    Engine * getEngine(){
        return Engine::getEngine();
    }

    /* sets the global engine */
    void newEngine(const string & name, const string & path){
#ifdef HAVE_PYTHON
        if (name == "python"){
            new PythonEngine(path);
            return;
        }
#endif
        if (name == "none"){
            new NoEngine();
            return;
        }
        Global::debug(0) << "*Warning* no such engine '" << name << "' for path " << path << endl;
        new NoEngine();
        // throw NoSuchEngine(name);
    }

    NoEngine::NoEngine():Engine(){
    }

    void NoEngine::init(){
    }

    void NoEngine::shutdown(){
    }

    void NoEngine::createWorld(const World & world){
    }

    void NoEngine::destroyWorld(const World & world){
    }
            
    void NoEngine::tick(){
    }
            
    void * NoEngine::createCharacter(Character * character){
        return NULL;
    }

    void NoEngine::destroyCharacter(void * handle){
    }

    void NoEngine::objectTick(void * obj){
    }
            
    void NoEngine::objectTakeDamage(void * who, void * handle, int damage){
    }

    NoEngine::~NoEngine(){
    }

    typedef ::Object ObjectObject;
    Object::Object(ObjectObject * const guy):
    handle(NULL),
    guy(guy){
    }
        
    void Object::takeDamage(Object * him, int damage){
        void * himHandle = NULL;
        if (him != NULL){
            himHandle = him->getHandle();
        }
        getEngine()->objectTakeDamage(handle, himHandle, damage);
    }
    
    void Object::tick(){
        getEngine()->objectTick(handle);
    }

    Object::~Object(){
    }

    typedef ::Character ObjectCharacter;
    Character::Character(ObjectCharacter * const guy):
    Object(guy){
        handle = getEngine()->createCharacter(this);
    }

    Character::~Character(){
        getEngine()->destroyCharacter(handle);
    }
}
