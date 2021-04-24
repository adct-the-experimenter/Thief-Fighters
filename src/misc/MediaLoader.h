#ifndef MEDIA_LOADER_H
#define MEDIA_LOADER_H

#include "media.h"

class MediaLoader
{
public:
	
	MediaLoader();
	
	bool loadMedia();
	
	void freeMedia();

};

#endif
