#ifndef URUS_YARPCODEC_HPP
#define URUS_YARPCODEC_HPP

#include "struct.hpp"
#include "gurus/cmpntYarpCodec.hpp"

namespace cmpnt{
<?
for t in typesVect:
  typeName = t.identifier()
  ?>
template<> class YarpCodec<<!typeName!>> {
public:
static int encode(yarp::os::Bottle *b, const <!typeName!>& v);
static int decode(const yarp::os::Bottle *b, <!typeName!>& v,int it);
static int print (const <!typeName!>& v);
};
<?
?>
}

#endif
