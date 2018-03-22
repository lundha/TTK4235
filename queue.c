//queue
#include "channels.h"
#include "elev.h"
#include "io.h"
#include <stdio.h>
#include "queue.h"


static int orders[4][3] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
};//opp ned command


//SETS ORDER LAMP ON
void q_set_order_lamp (int floor, int button_type){
    if(orders[floor][button_type] == 1){
        elev_set_button_lamp(button_type, floor, 1);
    }

}

//RETURNS VALUE ON ORDER orders[floor][button]
int q_get_order(int floor, int button_type){
    return orders[floor][button_type];
}

//PRINTS ORDER MATRIX
void q_printOrders(){
    for (int i=0; i < 4; i++) {
            printf("\n");
        for (int y=0; y < 3; y++) {
            printf("%d", orders[i][y]);
        }
    }
}




//DELETES ALL ORDERS, SETS ALL VALUES TO 0 IN orders[4][3]
void q_clear_all_orders() {
    for (int floor = 1; floor<4; floor++) {
      elev_set_button_lamp(1,floor,0);
      orders[floor][1] = 0;
    }
    for (int floor = 0; floor<3; floor++) {
                elev_set_button_lamp(0, floor, 0);
                orders[floor][0] = 0;
    }
    for (int floor = 0; floor<4; floor++) {
                elev_set_button_lamp(2, floor, 0);
                orders[floor][2] = 0;
    }
}


//CHECKS IF THE ELEVATOR SHOULD STOP ACCORDING TO DIRECTION AND OTHER ORDERS
int q_check_if_stop(int current_floor, int direction) {

      if(direction == DIRN_UP){
        if(orders[3][1] == 1 || orders[3][2] == 1){
          if(q_check_if_order_above(current_floor) != 1){

            if(elev_get_floor_sensor_signal() == 3){
            return 1;
          }
        }
      }
        if(orders[1][0] == 1 || orders[1][2] == 1){
            if(elev_get_floor_sensor_signal() == 1){
          return 1;
        }
      }
      if(orders[2][0] == 1 || orders[2][2] == 1){
            if(elev_get_floor_sensor_signal() == 2){
          return 1;
        }
      }
        if (q_up_row_empty() && orders[3][1] == 0) {

        if (orders[1][1] == 1) {
            if(elev_get_floor_sensor_signal() == 2){
            return 1;
          }
        }

        if (orders[2][1] == 1) {
            if(elev_get_floor_sensor_signal() == 1){
            return 1;
          }
        }

    }
    }

      if(direction == DIRN_DOWN){
        if(orders[0][0] == 1 || orders[0][2] == 1){
              if(elev_get_floor_sensor_signal() == 0){
                     return 1;
             // }
            }
          }

        if((orders[2][1] || orders[2][2])){
            if(elev_get_floor_sensor_signal() == 2){
          return 1;
        }
      }

        if((orders[1][1] || orders[1][2])){
            if(elev_get_floor_sensor_signal() == 1){
          return 1;
        }
      }
        if (q_down_row_empty() && orders[0][0] == 0) {

        if (orders[2][0] == 1) {
            if(elev_get_floor_sensor_signal() == 2){
            return 1;
          }
        }

        if (orders[1][0] == 1) {
            if(elev_get_floor_sensor_signal() == 1){
            return 1;
          }
      }
    }
    }
 return 0;
}


//CLEARS FLOOR IN orders[4][3] AND LIGHT ON CURRENT FLOOR
void q_clear_current_floor(int floor){
  orders[floor][0] = 0;
  orders[floor][1] = 0;
  orders[floor][2] = 0;

  for(int buttons = 0; buttons<3; buttons++){
    orders[floor][buttons] = 0;
  }

  if(floor != 0){
    elev_set_button_lamp(1,floor, 0);
  }
  if(floor != 3){
    elev_set_button_lamp(0,floor, 0);
  }
  elev_set_button_lamp(2,floor, 0);
}

//SETS FLOOR INDICATOR ON IF THE ELEVATOR IS ON A FLOOR
void q_update_floor_indicator(int current_floor){
  if(current_floor != -1){
          elev_set_floor_indicator(current_floor);
          }
}



//REMEMBERS LAST FLOOR 
int q_prev_floor(int current_floor){
  if(current_floor == -1){
      //Do nothing
  }
  else{

    return current_floor;
  }
}


//


//DECIDES DIRECTION DEPENDENT ON FLOOR ORDERED
int q_check_orders(int current_floor) {
  if(current_floor != floor){
      for(int floor = 3; floor>=0; floor--){
        if(orders[floor][1] == 1 ||  orders[floor][2] == 1){
          return floor;
        }
      }

      for(int floor = 0; floor<4; floor++){

        if(orders[floor][0] == 1 || orders[floor][2] == 1){
          return floor;
        }
      }
}

      else{
        for(int buttons = 0; buttons <3; buttons++) {
        if(orders[current_floor][buttons] == 1){
          printf("Trykket i samme etasje");
          return -2;

        }
        }
      }
      return -1;
}

//IF AN ORDER BUTTON IS PRESSED, THE LIGHT WILL TURN ON
void q_update_lights(){
   for (int floor = 1; floor<4; floor++) {
      if(orders[floor][1] == 1){
        elev_set_button_lamp(1,floor,1);
      }
    }
    for (int floor = 0; floor<3; floor++) {
      if(orders[floor][0] == 1){
        elev_set_button_lamp(0,floor,1);
       }
    }
    for (int floor = 0; floor<4; floor++) {
      if(orders[floor][2] == 1){
        elev_set_button_lamp(2,floor,1);
      }
    }
}

//SETS BUTTONS IN orders[4][3] TO 1 IF A BUTTON IS PRESSED
void q_watch_buttons() {
          for(int floor = 0; floor<3; floor++) {
            if(elev_get_button_signal(0,floor)){
              orders[floor][0]=1;
            }
          }
          for(int floor = 1; floor<4; floor++) {
            if(elev_get_button_signal(1,floor)){
              orders[floor][1]=1;

            }
          }
          for(int floor = 0; floor<4; floor++) {
            if(elev_get_button_signal(2,floor)){
              orders[floor][2]=1;
            }
          }
}


//RETURNS 1 IF THERE ARE ORDERS ABOVE CURRENT_FLOOR
int q_check_if_order_above(int floor){
    for(int x = floor; x<4; x++){
        if(orders[x][0] == 1){
          printf("det er ordre over\n");
            return 1;
        }
        if(orders[x][2] == 1 && x != 3){
          printf("det er ordre over\n");
            return 1;
        }
    }
return 0;
}

//RETURNS 1 IF THERE ARE ORDERS BELOW CURRENT_FLOOR
int q_check_if_order_below(int floor){

    for(int x = floor; x>=0; x--){
        if(orders[x][1] == 1){
            return 1;
            printf("det er ordre under\n");
        }
        if(orders[x][2] == 1 && x != 0){
          printf("det er ordre under\n");
            return 1;
    }
  }
return 0;
}


//CHECK ORDERS 2 USED IN IDLE INBETWEEN
int q_check_orders2(int current_floor) {


      for(int floor = 3; floor>=0; floor--){
        if(orders[floor][1] == 1 ||  orders[floor][2] == 1){
          return floor;
        }
      }

      for(int floor = 0; floor<4; floor++){

        if(orders[floor][0] == 1 || orders[floor][2] == 1){
          return floor;
        }
      }

      return -1;
}


int q_down_row_empty(){
      for(int x = 0; x<4; x++){
          if(orders[x][1] == 1){
              return 0;
          }
      }
      return 1;
  }
  int q_up_row_empty(){
      for(int x = 0; x<4; x++){
          if(orders[x][0] == 1){
              return 0;
          }
      }
      return 1;
  }
