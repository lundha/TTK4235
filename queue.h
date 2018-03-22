


void q_update_lights(); //OPPDATERER BESITLLINGSLYS
int q_get_order(int floor, int button_type); //HENTER ORDRE FRA ORDREMATRISEN
void q_printOrders(); //PRINTER ORDRE
void q_clear_all_orders(); //SLETTER ALLE ORDRE
int q_check_if_order_above(int floor); //SJEKKER OM DET ER EN ORDRE OVER GJELDENDE ETASJE
int q_check_if_order_below(int floor); //SJEKKER OM DET ER EN ORDRE UNDER GJELDENDE ETASJE
int q_prev_floor(int current_floor); //SJEKKER HVA SOM ER FORRIGE ETASJE
int q_check_if_stop(int current_floor, int direction); //SJEKKER OM DEN SKAL STOPPE
void q_clear_current_floor(int floor); //SLETTER BESTILLINGER I GJELDENDE ETASJE
void q_watch_buttons(); //TAR I MOT SIGNALER FRA ALLE KNAPPENE PÅ HEISBOKSEN
int q_check_orders(int current_floor); // GÅR GJENNOM BESTILLINGER
int q_check_orders2(int current_floor);
void q_update_floor_indicator(int current_floor); //OPPDATERER ETASJELYS
int q_down_row_empty(); //SJEKKER OM BESTILLINGER NEDOVER ER TOM
int q_down_down_empty(); //SJEKKER OM BESTILLILNGER OPPOVER ER TOM
