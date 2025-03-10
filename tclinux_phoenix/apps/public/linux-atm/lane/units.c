/*
 * Unitlist
 *
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/linux-atm/lane/units.c#1 $
 *
 */

/* System includes */
#include <assert.h>
#include <string.h>

/* Local includes */
#include "units.h"
#include "mem.h"
#include "lane.h"
#include "load.h"
#include "dump.h"
#include "connect.h"
#include "timers.h"
#include "events.h"
#include "atm.h"

/* Type definitions */

/* Local function prototypes */

/* Data */
const Unit_t *unitlist[] = {
  &dump_unit,
  &mem_unit,
  &load_unit,
  &conn_unit,
  &main_unit,
  &timer_unit,
  &events_unit,
  &atm_unit,
  NULL
};

const unsigned int num_units = sizeof(unitlist)/sizeof(Unit_t *)-1;

/* Functions */
const Unit_t *
find_unit(const char *name)
{
  const Unit_t *tmp;
  unsigned int i;

  assert(name != NULL);
  for (i = 0; unitlist[i] != NULL; i++) {
    tmp = unitlist[i];
    assert (tmp->name != NULL);
    if (strcmp(tmp->name, name) == 0) {
      return tmp;
    }
  }
  return NULL;
}

