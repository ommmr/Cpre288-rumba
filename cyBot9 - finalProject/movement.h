#include "lcd.h"
#include "timer.h"
#include "open_interface.h"

void moveforward(oi_t *sensor, int millimeters);
void movebackward(oi_t *sensor_data, int millimeters);
void turncounterclockwise(oi_t *sensor, int ang);
void turnclockwise(oi_t *sensor_data, int ang);
void avoidright(oi_t * sensor_data);
void avoidleft(oi_t * sensor_data);
void command(int dist, int angle, oi_t *sensor_data);
void playsong(void);

