#include <REGX52.H>

/*
舵机周期为20ms，一个周期的高电平持续时间影响转动角度
0.5ms——0度；
1.0ms——45度；
1.5ms——90度；
2.0ms—–135度；
2.5ms—–180度；
*/

//Electric Potential Time
unsigned int SteeringEngine_HEPT[]={65074,64613,64153,63692,63230};
unsigned int SteeringEngine_LEPT[]={47564,48025,48485,48946,49406};
