
#include "edos.h"


OsRoutine *ed_os;

void edInit() {

    ed_os = (OsRoutine *)*(u32 *) 0x1A0;
    ed_os->memInitDmaCode(); 
}
