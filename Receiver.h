#ifndef RAYTRACING_RECEIVER_H
#define RAYTRACING_RECEIVER_H


#include "Object.h"

class Receiver : public Object {
public:
	Receiver() = default;
	Receiver(double x, double y);
};


#endif //RAYTRACING_RECEIVER_H
