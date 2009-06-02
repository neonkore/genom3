<?
# error related functions
def createErrorList():
  l = []
  for s in servicesMap:
    service = s.data()
    for e in service.errorMessages():
	l.append(e)
  for t in comp.tasksMap():
    for e in t.data().errorMessages():
	l.append(e)
  return set(l)
errorList = createErrorList();
?>
#ifndef <!upperCompName!>_ERROR_HPP
#define <!upperCompName!>_ERROR_HPP

#include "userCodels.h"

std::string errorString(int err)
{
  switch(err) {
<?
for e in errorList: ?>
    case ERROR_<!e!>: return "<!e!>";
<?
?>
    default: return "";
  }
  return "";
}

#endif

