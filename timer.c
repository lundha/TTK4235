#include <stdio.h>
#include <sys/time.h>
#include "timer.h"


double getWallTime(void)  //Antall sekunder siden 1.januar 1970
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}



double timer() {

    double start = getWallTime();

   do {


//UPDATES LIGHTS AND BUTTONS WHILE TIMER IS ON
q_update_lights();
q_watch_buttons();

   }  while ((getWallTime() - start) < 2);

    start = 0;

    return 1;
}
