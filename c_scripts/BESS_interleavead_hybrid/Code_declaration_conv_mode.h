#include <math.h>

#define mode Input (0)

#define fsw     ParamRealData(0,0)    
#define Ts      ParamRealData(1,0)   
#define fdsp    ParamRealData(2,0)  

#define PRD  (fdsp/fsw)/2           						 // COntador Up e Down, PRD = (fdsp/fsw)/2 
#define PRD_div2  PRD/2              						 // PRD_div2 = PRD/2;
#define pi    3.141592653589793

int count = 0;
int inc = 1;

int t1 = 0;
int t2 = 0;
int b1 = 0;
int b2 = 0;