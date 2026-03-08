#ifndef __PID_H
#define __PID_H

int error_get(int *LL,int *L,int *R,int *RR);
float PID_get(PID_TypeDef*pid, float sensor, float dt);


#endif
