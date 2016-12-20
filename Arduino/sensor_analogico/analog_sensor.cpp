#include "analog_sensor.h"

/* analog_sensor */

analog_sensor::analog_sensor(unsigned short Pin) : pin(Pin)
{
    id = n_sensors;
    n_sensors++;
};

unsigned analog_sensor::n_sensors = 0;

void analog_sensor::read()
{
    readed = analogRead(pin);
    calc();
};

void analog_sensor::calc()
{
    value = readed;
    svalue = String(value);
};

const float *analog_sensor::broadcast()
{
    return &value;
};

const String *analog_sensor::sbroadcast()
{
    return &svalue;
};

unsigned analog_sensor::showID(){
    return id;
};

analog_sensor::operator int(){
    return pin;
}

/* lineal_sensor */

lineal_sensor::lineal_sensor(unsigned short Pin) : analog_sensor(Pin), n(0.0), m(1.0){};
lineal_sensor::lineal_sensor(unsigned short Pin, float M, float N) : analog_sensor(Pin), n(N), m(M){};

void lineal_sensor::calc()
{
    value = m * float(readed) + n;
    svalue = String(value, 3);
};

/* exp_sensor */

exp_sensor::exp_sensor(unsigned short Pin) : analog_sensor(Pin), a(1.0), b(1.0), c(0.0){};
exp_sensor::exp_sensor(unsigned short Pin, float A, float B) : analog_sensor(Pin), a(A), b(B), c(0.0){};
exp_sensor::exp_sensor(unsigned short Pin, float A, float B, float C) : analog_sensor(Pin), a(A), b(B), c(C){};

void exp_sensor::calc()
{
    value = (float)(b * exp(a * float(readed)) + c);
    svalue = String(value, 3);
};

/* log_sensor */

log_sensor::log_sensor(unsigned short Pin) : analog_sensor(Pin), a(1.0), b(0.0){};
log_sensor::log_sensor(unsigned short Pin, float A, float B) : analog_sensor(Pin), a(A), b(B){};

void log_sensor::calc()
{
    value = float((a * log(readed) + b));
    svalue = String(value, 3);
}

/* pol_sensor */

pol_sensor::pol_sensor(unsigned short Pin, unsigned short N, char *cff) : analog_sensor(Pin), n(N)
{
    coeff = new float[N + 1];
    bool ctrl = false;
    int i = N;

    for (char *ptr = cff; *ptr; ptr++)
    {
        if (ctrl == false)
        {
            sscanf(ptr, "%f", &coeff[i--]);
            ctrl = true;
        }
        else
        {
            if (*ptr == ',')
            {
                ctrl = false;
            }
        }
        if (i < 0)
        {
            break;
        }
    }
    if (i >= 0)
    {
        for (i; i >= 0; i--)
        {
            coeff[i] = 0;
        }
    }
}

void pol_sensor::calc()
{
    unsigned short ctrl = 0;
    value = 0.0;
    for (unsigned short i = 0; n > i; i++)
    {
        value += (float)(pow(readed, i) * coeff[i]);
    }
    svalue = String(value, 3);
}