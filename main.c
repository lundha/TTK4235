#include "elev.h"
#include <stdio.h>
#include "channels.h"
#include "queue.h"
#include "fsm.h"

int main() {





    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    //git original version
    }
     printf("Press STOP button to stop elevator and exit program.\n");
    while(1){
 
      fsm();
}


}
