#include <math.h>

#define Vdc Input (0)
#define Vga Input (1)
#define Vgb Input (2)
#define Vgc Input (3)
#define Isa Input (4)
#define Isb Input (5)
#define Isc Input (6)
#define control_enable Input (7)
#define Vdc_ref Input (8)
#define Qref Input (9)
#define Vdc2 Input (10)
#define Vdc3 Input (11)
#define I1 Input (12)
#define I2 Input (13)

#define fsw     ParamRealData(0,0)    
#define Ts      ParamRealData(1,0)   
#define fdsp    ParamRealData(2,0)  
#define Kpouter ParamRealData(3,0)  
#define Kiouter ParamRealData(4,0)  
#define Kp_pll  ParamRealData(5,0)
#define Ki_pll  ParamRealData(6,0)
#define Kp_res  ParamRealData(7,0)
#define Ki_res  ParamRealData(8,0)
#define Kiq     ParamRealData(9,0)
#define Kpouter_eq ParamRealData(10,0)
#define Kiouter_eq ParamRealData(11,0)

#define PRD  (fdsp/fsw)/2           						 // COntador Up e Down, PRD = (fdsp/fsw)/2 
#define PRD_div2  PRD/2              						 // PRD_div2 = PRD/2;
#define pi    3.141592653589793   
#define wn    2*pi*fn                 						 //Frequência angular fundamental 
#define N     300               						   
//...............Variáveis do Controle da tensão do link cc
float psat = 10e3;
float pot_act_sat = 0;

//...............Variáveis do Controle de corrente do inversor
float Ir = 30;
float Pc = 0;
float Qc = 0;
float Vpwm_norm_a = 0;
float Vpwm_norm_b = 0;
float Vpwm_norm_c = 0;
float vmin = 0;
float vmax = 0; 
float vsa_svpwm = 0;
float vsb_svpwm = 0;
float vsc_svpwm = 0;
float dutya = 0;
float dutyb = 0;
float dutyc = 0;
int counti = 0;
//

typedef struct {
	float Xref;
	float Xm;
	float erro;
	float erro_ant;
	float inte;
	float inte_ant;
	float duty;
	float piout;
	float piout_ant;
	float piout_sat;
	float erropi;
	float erropi_ant ;
	float dif;
} sPI;

#define PI_default {0,0,0,0,0,0,0,0,0,0,0,0,0}

sPI PIvdc = PI_default;
sPI PIvdceq2 = PI_default;
sPI PIvdceq3 = PI_default;
sPI PIpll = PI_default;
sPI PIq   = PI_default;

typedef struct {
float Xref;
float Xm;
float erro;
float erro_ant;
float erro_ant2;
float res;
float res_ant;
float res_ant2;
float pr_out;
double c1;
double c2;
double c3;
double c4;
} sPR;

#define PR_default  {0,0,0,0,0,0,0,0,0,0.00002777574703951879,-0.00002777574703951879,-1.999561366949691,1.000000000000000}

sPR PRf_alfa = PR_default;
sPR PRf_beta = PR_default;

typedef struct {
	float x;
	float y;
	float W;
	float b0;
	float b1;
	float b2;
	float b3;
	float a1;
	float a2;
	float V_sogi;
	float V_sogi1;
	float V_sogi2;
	float V_sogi_q;
	float V_sogi_q1;
	float V_sogi_q2;
	float Vm;
	float Vm1;
	float Vm2;
	float K_damp;
	float freq_res;
} sSOGI;

#define SOGI_default {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1.414213562373095,60}

sSOGI SOGI1 = SOGI_default;
sSOGI SOGI2 = SOGI_default;

typedef struct{
float a;
float b;
float c;
} sABC;

#define ABC_default {0,0,0}

sABC Vabc = ABC_default;
sABC Vpwm_abc = ABC_default;
sABC Isabc = ABC_default;

typedef struct{
float alfa;
float beta;
} sAlfaBeta;

#define AlfaBeta_default {0,0}

sAlfaBeta Valfabeta = AlfaBeta_default;
sAlfaBeta Vpwm_alfabeta = AlfaBeta_default;
sAlfaBeta Ialfabeta = AlfaBeta_default;
sAlfaBeta Isalfabeta = AlfaBeta_default;

typedef struct{
float d;
float q;
} sDQ;

#define dq_default {0,0}

sDQ Vdq = dq_default;

typedef struct{
float Valfa_in;
float Vbeta_in;
float angle;
float angle_ant;
float pi_out_ant;
float wf;
} spll;

#define pll_default {0,0,0,0,0,376.99111843077}

spll PLL = pll_default;

typedef struct{
float array[300];
float x;
float y;
float y_ant;
int j;
} sMAV;
 
#define  MAV_default {{0},0,0,0,0}             
sMAV MAVP = MAV_default;
sMAV MAVQ = MAV_default;
sMAV MAVV = MAV_default;

typedef struct{
float x;
float x_ant;
float y;
float y_ant;
double c0;
double c1;
} sFilter;
 
#define FILTER_DEFAULTS_2_5_HZ {0,0,0,0,0.00087266462599716480,0.00087266462599716480} //2.5Hz       
#define FILTER_DEFAULTS_20HZ   {0,0,0,0,0.00698131700797731836,0.00698131700797731836} //20Hz     
sFilter filVdc = FILTER_DEFAULTS_20HZ;
sFilter filVdc2 = FILTER_DEFAULTS_20HZ;
sFilter filVdc3 = FILTER_DEFAULTS_20HZ;

typedef struct{
float final;
float final_ant;
float atual;
float in;
float delta;
int flag;
int flag2;
float inc;
} Ramp;

#define QRamp_default {0,0,0,0,0,0,0,0.12} 
Ramp QRamp = QRamp_default;

//................Parametros do PWM
int count = 0;
int inc = 1;
int S1 = 0;
int S2 = 0;
int S3 = 0;
int S4 = 0;
int S5 = 0;
int S6 = 0;

//funções
//Filtro segunda ordem

void First_order_filter(sFilter *filt)
{
	filt->y = filt->x*filt->c0  + (1-filt->c1)*filt->y_ant;
	filt->x_ant  = filt->x;
	filt->y_ant  = filt->y;
}