
#include "elev.h"
#include <stdio.h>
#include "channels.h"
#include "queue.h"
#include "timer.h"

//declaring enum state_c that hold on the different states
typedef enum{
    INIT,
    IDLE,
    IDLE_INBETWEEN,
    MOVING,
    MOVING_INBETWEEN,
    OPEN_DOOR,
    EMERGENCY,
} state_c;

//declaring variables that are used
  int direction;
  int current_floor;
  int prev_floor;
  int prev_direction;
  int current_floor_inbetween;
  state_c state_current = INIT;

void fsm() {

if (elev_get_stop_signal() == 1) {
              state_current = EMERGENCY;
          }

    if(state_current == EMERGENCY){
        elev_set_stop_lamp(1);
    }
    else{
        elev_set_stop_lamp(0);
    }
      if (elev_get_floor_sensor_signal() != -1) {
        current_floor = elev_get_floor_sensor_signal();
      }



    q_watch_buttons();
    q_update_lights();

  switch(state_current)

  {

//INITIALIZES THE ELEVATOR TO A FLOOR
    case INIT:
        printf("INIT\n");
        direction = DIRN_UP;
        if (elev_get_floor_sensor_signal() != -1) {
        current_floor = elev_get_floor_sensor_signal();
      }

        prev_floor = q_prev_floor(current_floor);

        while(elev_get_floor_sensor_signal() == -1) {
              elev_set_motor_direction(direction);
        }
        elev_set_motor_direction(DIRN_STOP);
        state_current = IDLE; // setter state -> IDLE etter initialisering

//ELEVATOR IS ON A FLOOR, READY FOR ANY ORDER
    case IDLE:

      prev_floor = q_prev_floor(current_floor);


      //IF ORDERED FLOOR IS ON A DIFFERENT FLOOR THAN CURRENT FLOOR, WE MOVE TO STATE MOVING
      //IF ORDER IS ON CURRENT FLOOR, THE DOOR OPENS, IF 
      if(q_check_orders(current_floor)>= 0){
        state_current = MOVING;
      }
      else if(q_check_orders(current_floor) == -2){
        state_current = OPEN_DOOR;
      }
      else if(q_check_orders(current_floor) == -1){
        state_current = IDLE;
      }

      break;


//ELEVATOR IS MOVING AND DECIDES A DIRECTION TO MOVE DEPENDENT ON THE ORDERS
    case MOVING:


    if (elev_get_floor_sensor_signal() != -1) {
        current_floor = elev_get_floor_sensor_signal();
            printf("current floor: %d", current_floor);

      }

          q_update_floor_indicator(current_floor);
          //sjekker stopp knapp
          
          //SETS DIRECTION ACCORDING TO NEXT ORDER
          if(q_check_orders(current_floor) < current_floor) {
            elev_set_motor_direction(DIRN_DOWN);
            direction = DIRN_DOWN;
          }
          if (q_check_orders(current_floor) > current_floor) {
            elev_set_motor_direction(DIRN_UP);
            direction = DIRN_UP;
          }


          //PRIORITIZES WHICH FLOOR TO STOP AT DEPENDENT ON DIRECTION
          if (q_check_if_stop(current_floor, direction) == 1) {
                state_current = OPEN_DOOR;
                }
          else{
                state_current = MOVING;
                }

          break;

//ELEVATOR IS MOVING BETWEEN TWO FLOORS, THE ELEVATOR PROCESSES WHICH DIRECTION TO MOVE DEPENDENT ON ORDERS
    case MOVING_INBETWEEN: 


   if(q_check_orders(current_floor) < current_floor) {
            elev_set_motor_direction(DIRN_DOWN);
            direction = DIRN_DOWN;
          }
  if (q_check_orders(current_floor) > current_floor) {
            elev_set_motor_direction(DIRN_UP);
            direction = DIRN_UP;
          }
  else if (q_check_orders(current_floor) == current_floor) {
          if (direction == DIRN_UP) {
            elev_set_motor_direction(DIRN_DOWN);
            direction = DIRN_DOWN;
          }
          else {
            elev_set_motor_direction(DIRN_DOWN);
            direction = DIRN_UP;
          }

  }


  if (q_check_if_stop(current_floor, direction) == 1) {
                state_current = OPEN_DOOR;
                }
          else{
                state_current = MOVING;
                }
        break;

//ELEVATOR OPENS DOOR WHEN IT ARRIVES A FLOOR OR WHEN THE STOP BUTTON IS PRESSED AT A FLOOR
//STATE WHERE MOTOR IS SET TO 0
case OPEN_DOOR:



    printf("OPEN_DOOR\n");
    printf("CURRENT FLOOR %d\n",current_floor+1);


    q_clear_current_floor(current_floor);
    elev_set_motor_direction(DIRN_STOP);
    q_printOrders();

    if(current_floor != -1){
    elev_set_floor_indicator(current_floor);
}
    elev_set_door_open_lamp(1);
    while(timer() != 1){
    }
    elev_set_door_open_lamp(0);
    state_current = IDLE;


    break;

//IF THE STOP BUTTON IS PRESSED, WE ARE SENT TO EMERGENCY STATE WHICH CHECKS IF WE ARE ON A FLOOR OR BETWEEN TWO FLOORS
case EMERGENCY:
    //STOPS ELEVATOR
    elev_set_motor_direction(DIRN_STOP);
    //IF THE STOP BUTTON IS PRESSED DOWN, YOU ARE NOT ABLE TO ORDER ANY FLOORS
    while(elev_get_stop_signal() == 1){
        state_current = EMERGENCY;
    }

    q_clear_all_orders();
    q_printOrders();

    //IF THE ELEVATOR IS BETWEEN TWO FLOORS, CHANGE STATE TO IDLE_INBETWEEN, IF WE ARE ON A FLOOR, OPEN DOOR
    if(elev_get_floor_sensor_signal() == -1){
      state_current = IDLE_INBETWEEN;
    }
    else{
      state_current = OPEN_DOOR;
    }

    break;

//ELEVATOR IS STOPPED BETWEEN TWO FLOORS AND IS READY TO TAKE AN ORDER
case IDLE_INBETWEEN:
    q_watch_buttons();

   

    if(q_check_orders2(prev_floor)>= 0 || q_check_orders2(prev_floor) == prev_floor){
            
            if (direction == DIRN_UP) {
              current_floor = prev_floor + 1;
            }
            else {
              current_floor = prev_floor - 1;
            }
            state_current = MOVING_INBETWEEN;
          }

    break;
     }

}

