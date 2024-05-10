#ifndef TOP_H
#define TOP_H
#include "initiator.h"
#include "target.h"

SC_MODULE(Top)
{
Initiator *initiator;
Target *target;
SC_CTOR(Top)
{
initiator=new Initiator("initiator");
target=new Target("target");
initiator->socket.bind(target->socket);
}
};

#endif
