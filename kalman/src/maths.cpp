#include "maths.h"

#include <ros/ros.h>
#include <iostream>
#include <limits>
#include <numeric>
#include <cmath>


MovingAverage::MovingAverage(){
    count_inf_nan = 0;
}


void MovingAverage::movingAverage(float &value)
{
    if (values_vector.size() == size_average){values_vector.erase(values_vector.begin());}
       
    values_vector.push_back(value);
    moving_average = std::accumulate(values_vector.begin(), values_vector.end(), 0.f) / values_vector.size();
}

void MovingAverage::manageInfNanAndMovingAverage(float &value)
{  
   if ((value == std::numeric_limits<float>::infinity() || value == -std::numeric_limits<float>::infinity() || std::isnan(value)))
    {   
        count_inf_nan += 1;
        if(count_inf_nan < stop_inf_nan)
        {
            value = moving_average;
        }
        else
        {
            count_inf_nan = 0;
            moving_average = std::numeric_limits<float>::infinity();
            values_vector.clear();
        }
    }
    else
    {
        movingAverage(value);
        count_inf_nan = 0;
    }
}

Gauss::Gauss(float measure, float variance){
       x_begin = measure - 4*variance;
       x_end = measure + 4*variance;
       _variance = variance; _measure = measure;
}

float Gauss::piFunction(float x)
{
    float exp_power = (x - _measure)/ _variance;
    exp_power = -pow(exp_power, 2)/ 2;
    return std::exp(exp_power);
}

Trapeze::Trapeze(float measure,float variance,float slope_width){
         x_begin = measure - variance - slope_width;
         x_end = measure + variance + slope_width;
         _variance = _variance; _measure = measure; _slope_width = slope_width;
}

float Trapeze::piFunction(float x)
{   float result;
    if (_measure - _variance <= x && x <= _measure + _variance)
    {
        result = 1.;
    }
    else if (_measure - _variance - _slope_width < x && x < _measure - _variance)
    {
        result = x / _slope_width + (1 + (_variance - _measure) / _slope_width);
    }

    else if (_measure + _variance < x && x < _measure + _variance + _slope_width)
    {
        result = -x / _slope_width + 1 + (_measure + _variance) / _slope_width;
    }
    else
    {
        result = 0.;
    }
    return result;
}

PubInf::PubInf(int period)
{
    _period= period;
}

void PubInf::function(){
    int i = 0;
    while (true) {
        i ++;
    if (i ==_period ){value = std::numeric_limits<float>::infinity(); i =0;}
    else {value = static_cast<float>(rand());}
    }
}


