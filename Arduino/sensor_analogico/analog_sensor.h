/*

> Analog Sensors Lib. 

> Author: Mariano Fernández Abadía.
> Date: 2016-12-19.
> License: CC BY.
> Version: 0.1 Beta.

> Summary: This library gives a simple solution for read from analog sensors in Arduino developments.                           
If we know the analytic function which relates - approximately - the analog mesaure and the physical magnitude,            
this library supplies tools that return real values of these magnitudes.                                                                                                                                                                                   

> Resumen: Esta biblioteca ofrece una solución sencilla para la lectura de sensores analógicos en desarrollos Arduino.      
Si conocemos la función analítica que relaciona - aproximadamente - la medida analógica y la magnitud física,               
esta biblioteca suministra herramientas que devuelven valores reales de estas magnitudes.                                   

*/

#ifndef ANALOG_SENSOR
#define ANALOG_SENSOR

#include "arduino.h"

class analog_sensor
{
protected:
  unsigned short pin;
  unsigned readed;
  float value;
  String svalue;
  unsigned id;

public:
  analog_sensor(unsigned short Pin);
  operator int();
  void read();
  const float *broadcast();
  const String *sbroadcast();
  unsigned showID();

private:
  static unsigned n_sensors;
  virtual void calc();
};

class lineal_sensor : public analog_sensor //f(x) = mx + n
{
protected:
  float m;
  float n;

public:
  lineal_sensor(unsigned short Pin);
  lineal_sensor(unsigned short Pin, float M, float N);

private:
  void calc();
};

class exp_sensor : public analog_sensor //f(x) = b · e^ax + c
{
protected:
  float a;
  float b;
  float c;

public:
  exp_sensor(unsigned short Pin);
  exp_sensor(unsigned short Pin, float A, float B);
  exp_sensor(unsigned short Pin, float A, float B, float C);

private:
  void calc();
};

class log_sensor : public analog_sensor //f(x) = a · log(x) + b
{
protected:
  float a;
  float b;

public:
  log_sensor(unsigned short Pin);
  log_sensor(unsigned short Pin, float A, float B);

private:
  virtual void calc();
};

class pol_sensor : public analog_sensor // f(x) = ax^n + bx^(n-1) + ... + c
{
protected:
  unsigned short n; //Grade
  float *coeff;

public:
  pol_sensor(unsigned short Pin, unsigned short N, char *cff);

  /* cff format: "a.aaa, b.bbb, c.ccc..."
  *  cff examples:
  *  pol_sensor(2,3,"2.12,4,0.54,2.04") -> 2.12 · x^3 + 4 · x^2 + 0.54 · x + 2.04
  *  pol_sensor(3,2,"1.56") -> 1.56 · x^2
  *  pol_sensor(3,3,"1.2,5.6,0,8.09") -> 1.2 · x^3 + 5.6 · x^2 + 8.09
  */

private:
  void calc();
};

#endif