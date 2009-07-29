#ifndef URUS_YARPCODEC_HPP
#define URUS_YARPCODEC_HPP

#include "<!comp.name()!>Struct.h"
#include "lib/YarpCodec.hpp"

namespace GenomYarp {
<?
for t in typesVect:
  typeName = t.identifier()
  ?>
template<> class YarpCodec<<!typeName!>> {
public:
static int encode(yarp::os::Bottle *b, const <!typeName!>& v);
static int decode(const yarp::os::Bottle *b, <!typeName!>& v,int it);
static int print (const <!typeName!>& v);
static void freeAllocatedMemory(<!typeName!> *v);
};
<?
?>
}

#endif
