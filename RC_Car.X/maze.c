#include "maze.h"
#include "ir_sensor.h"
#include "line_follower.h"

void Maze_Run(void){
    uint8_t value = IR_Read();
    switch (value) {
        case IR_PATTERN_ALL_BLACK : 
            Line_Handler(IR_PATTERN_LEFT);
            break;
        default:
            Line_Handler(value);
    }
}