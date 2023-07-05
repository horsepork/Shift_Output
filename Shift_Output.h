#include "Arduino.h"

class Shift_Output{
    public:
        Shift_Output(uint8_t _latchPin, uint8_t _clockPin, uint8_t _dataPin, uint16_t _numOutputs){
            latchPin = _latchPin;
            clockPin = _clockPin;
            dataPin = _dataPin;
            numOutputs = _numOutputs;
            numShiftRegisters = ceil(numOutputs / 8.0);
            outputArray = new uint8_t[numShiftRegisters];
            for(int i = 0; i < numShiftRegisters; i++){
                outputArray[i] = 0;
            }
        }
    
    private:
        uint8_t latchPin;
        uint8_t clockPin;
        uint8_t dataPin;
        uint8_t numShiftRegisters;
        uint16_t numOutputs;
        uint8_t *outputArray;
        uint32_t timer;
        uint16_t updateTime = 1000;
        bool debug = false;
    
    public:
        void begin(){
            pinMode(latchPin, OUTPUT);
            pinMode(clockPin, OUTPUT);
            pinMode(dataPin, OUTPUT);
            digitalWriteAllLow();
        }

        void digitalWrite(uint16_t index, bool state){
            if(index >= numOutputs){
                digitalWriteAllLow();
                if(!debug) return;
                Serial.print(index);
                Serial.println(" exceeds number of outputs");
            }
            digitalWrite(index / 8, index % 8);
        }

        void digitalWrite(uint16_t shiftRegisterIndex, uint8_t pinIndex, bool state){
            if(shiftRegisterIndex >= numShiftRegisters){
                digitalWriteAllLow();
                if(!debug) return;
                Serial.print(shiftRegisterIndex);
                Serial.println(" exceeds the number of shift registers");
            }
            if(pinIndex > 7){
                digitalWriteAllLow();
                if(!debug) return;
                Serial.print("Pin index of an individual shift register cannot exceed 7. You passed ");
                Serial.println(pinIndex);
            }
            outputArray[shiftRegisterIndex] |= power(2, pinIndex);
            pushOutputArray();
        }

        void digitalWriteShiftRegister(uint8_t shiftRegisterIndex, uint8_t outputValue){
            if(shiftRegisterIndex >= numShiftRegisters){
                digitalWriteAllLow();
                if(!debug) return;
                Serial.print(shiftRegisterIndex);
                Serial.println(" exceeds the number of shift registers");
            }
            outputArray[shiftRegisterIndex] = outputValue;
            pushOutputArray();
        }

        void digitalWriteAllLow(){
            for(int i = 0; i < numShiftRegisters; i++){
                outputArray[i] = 0;
            }
            pushOutputArray();
        }

        void update(){
            if(millis() - timer < updateTime) return;
            pushOutputArray();
        }

        void pushOutputArray(){
            for(int i = 0; i < numShiftRegisters; i++){
                shiftOut(dataPin, clockPin, LSBFIRST, outputArray[i]);
            }
            timer = millis();
        }

        void changeUpdateTime(uint16_t t){
            updateTime = t;
        }
    
    private:
        int power(int base, int exp){
            int val = 1;
            for(int i = 0; i < exp; i++){
                val *= base;
            }
            return val;
        }
};