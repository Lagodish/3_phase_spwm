#include <Arduino.h>
float errmeasure = 10; // разброс измерения
float errestimate = 10;  // разброс оценки
float q = 0.1;  // скорость изменения значений

float currentestimate = 0.0;
float lastestimate = 0.0;
float kalmangain = 0.0;


// функция фильтрации
float filter(int value) {
  kalmangain = errestimate / (errestimate + errmeasure);
  currentestimate = lastestimate + kalmangain * (value - lastestimate);
  errestimate =  (1.0 - kalmangain) * errestimate + fabs(lastestimate - currentestimate) * q;
  lastestimate = currentestimate;
  return currentestimate;
}