/* 
 * File:   Sensor.h
 * Author: Robin
 *
 * Created on 17 maart 2014, 14:39
 */

#ifndef SENSOR_H
#define	SENSOR_H

typedef struct{
    int ID; 
    int InOfUitSensor; //0 voor in, 1 voor uit
    int StoplichtID;
    int State;
}Sensor;

void event_AutoOp_Sensor(Kruispunt* v,KruispuntEvent e);
void event_AutoAf_Sensor(Kruispunt* v,KruispuntEvent e);

#endif	/* SENSOR_H */

