#ifndef _paintown_effect_h
#define _paintown_effect_h

#include "object_nonattack.h"

#include <string>
#include "util/load_exception.h"
#include "util/network/network.h"

class Bitmap;

extern const int ALLIANCE_NONE;

namespace Paintown{

class Animation;

class Effect: public ObjectNonAttack {
public:
	Effect( int alliance = ALLIANCE_NONE );
	Effect( const Effect & ef );
	Effect( const char * _filename, int alliance = ALLIANCE_NONE ) throw( LoadException );
	
	virtual int getWidth() const;
	virtual int getHeight() const;
	virtual bool isCollidable( Object * obj );
	virtual bool isGettable();

	virtual void act( std::vector< Object * > * others, World * world, std::vector< Object * > * add );
	virtual void draw( Bitmap * work, int rel_x, int rel_y );
	virtual Object * copy();
	
	virtual Network::Message getCreateMessage();

	virtual ~Effect();

protected:

	Animation * image;

};

}

#endif