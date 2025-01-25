/****************************************************************************
 * Copyright (C) 2020 by Fabrice Muller                                     *
 *                                                                          *
 * This file is useful for ESP32 Design course.                             *
 *                                                                          *
 ****************************************************************************/

/**
 * @file lab5_main.c
 * @author Fabrice Muller
 * @date 21 Oct. 2020
 * @brief File containing the lab5 of Part 3.
 *
 * @see https://github.com/fmuller-pns/esp32-vscode-project-template
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_log.h"
#include "driver/gpio.h"

/* FreeRTOS.org includes. */
#include "freertos/FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "my_helper_fct.h"





/*Create Tasks*/
void vTaskTimer(void *parameters);


TaskHandle_t = H;


void app_main(void){






}

void vTaskTimer(void *parameters){
    TickType_t xLastWakeup;
    xLastWakeup = xTaskGetTickCount();
    TickType_t period ms = toTick(100UL);
    for(;;){
       COMPUTE_IN_TIME_MS(10);
       xTaskToGive(H);
       

    }






}
