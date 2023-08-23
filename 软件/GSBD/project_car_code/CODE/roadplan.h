#ifndef _roadplan_h
#define _roadplan_h
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
//#include "zf_common_headfile.h"
#include "zf_common_headfile.h"
#define N 20   
#define M 20   
#define NcMax 15
//#define  float double
//#define int int8
struct data
{
	int16 map_read[N][2];    //有框最多20个！！！！！！！！！！！！！！！！
};
struct data calc_path(struct data datax,int num);
#endif