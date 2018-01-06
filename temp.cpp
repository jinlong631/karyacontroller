#include "config_pins.h"
#include "common.h"
#include "temp.h"

uint32_t next_temp;
uint16_t ctemp = 0;
double Setpoint, Input, Output;
int wait_for_temp = 0;

#if defined(temp_pin) && (defined(__AVR__) || defined(ESP8266)) 
#include <PID_v1.h>



//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, 2, 5, 1, DIRECT);

void set_temp(float set){
  Setpoint=set;
}
void init_temp()
{
  //initialize the variables we're linked to

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  next_temp = micros();
  Setpoint=0;

}

float read_temp(int32_t temp){
  
for (int j = 1; j < NUMTEMPS; j++) {
              if (pgm_read_word(&(temptable[j][0])) > temp) {
                // Thermistor table is already in 14.2 fixed point
                // Linear interpolating temperature value
                // y = ((x - x₀)y₁ + (x₁-x)y₀ ) / (x₁ - x₀)
                // y = temp
                // x = ADC reading
                // x₀= temptable[j-1][0]
                // x₁= temptable[j][0]
                // y₀= temptable[j-1][1]
                // y₁= temptable[j][1]
                // y =
                // Wikipedia's example linear interpolation formula.
                temp = (
                //     ((x - x₀)y₁
                  ((uint32_t)temp - pgm_read_word(&(temptable[j-1][0]))) * pgm_read_word(&(temptable[j][1]))
                //                 +
                  +
                //                   (x₁-x)
                  (pgm_read_word(&(temptable[j][0])) - (uint32_t)temp)
                //                         y₀ )
                  * pgm_read_word(&(temptable[j-1][1])))
                //                              /
                  /
                //                                (x₁ - x₀)
                  (pgm_read_word(&(temptable[j][0])) - pgm_read_word(&(temptable[j-1][0])));
                return float(temp)/4.0;
              }
            }
  return 0;
}

        
void temp_loop(uint32_t cm)
{
  if ((next_temp < cm) && (cm - next_temp < 1000000)) {
    next_temp = cm + 1000000; // each half second
    ctemp = (ctemp + analogRead(temp_pin) *3) /4;
    Input =  read_temp(ctemp);
    #ifdef heater_pin
      if (wait_for_temp ) zprintf(PSTR("Temp:%f PID:%f\n"), ff(Input),ff(Output));
      myPID.Compute();
      analogWrite(heater_pin, Output);
    #endif
  }
}
int temp_achieved() {
  return fabs(Input-Setpoint)<10;
}
#endif

#else

int temp_achieved() {
  return 1;
}
void set_temp(float set){
}
void init_temp()
{
}
void temp_loop(uint32_t cm)
{
}
#endif
