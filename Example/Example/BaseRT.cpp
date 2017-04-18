#include "BaseRT.h"

bool BaseRT::Load(int nrt, int cf, int df, int w, int h) {
	this->number_RT = nrt;
	this->color_format = cf;
	this->depth_format = df;
	this->w = w;
	this->h = h;
	return LoadAPIRT();
}
