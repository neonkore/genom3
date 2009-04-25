#include "<!comp.name()!>Error.h"

static const H2_ERROR const <!comp.name()!>H2errMsgs[] = <!upper(comp.name())!>_H2_ERR_MSGS;

int <!comp.name()!>RecordH2errMsgs(void)
{
  /* std errors from genom */
  genomRecordH2errMsgs();

  /* <!comp.name()!> errors */
  if (!h2recordErrMsgs("<!comp.name()!>RecordH2errMsgs", "<!comp.name()!>", M_<!comp.name()!>, 
		      sizeof(<!comp.name()!>H2errMsgs)/sizeof(H2_ERROR),
		      <!comp.name()!>H2errMsgs))
    return 0;

  /* others modules errros */
<?
for s in comp.importedComponents():
    write("  " + s + "RecordH2errMsgs();\n")
?>

  return 1;
}
