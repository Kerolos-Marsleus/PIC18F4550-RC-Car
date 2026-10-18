#include "maze.h"
#include "ir_sensor.h"
#include "line_follower.h"

void Maze_Run(uint8_t speed){
    uint8_t value = IR_Read();
    switch (value) {
        case IR_PATTERN_ALL_BLACK : 
            Line_Handler(IR_PATTERN_LEFT,speed);
            break;
        default:
            Line_Handler(value,speed);
    }
}