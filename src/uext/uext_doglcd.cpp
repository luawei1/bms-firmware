/* LibreSolar Battery Management System firmware
 * Copyright (c) 2016-2019 Martin Jäger (www.libre.solar)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef UNIT_TEST

#include "config.h"

#ifdef DOGLCD_ENABLED     // otherwise don't compile code to reduce firmware size

#include "pcb.h"

#include "DogLCD.h"
#include "font_6x8.h"
#include "font_8x16.h"

#include "bms.h"
#include "data_objects.h"

SPI spi(PIN_UEXT_MOSI, NC, PIN_UEXT_SCK);
DogLCD lcd(spi, PIN_UEXT_SSEL, PIN_UEXT_RX, PIN_UEXT_TX); //  spi, cs, a0, reset

extern bool blinkOn;

extern BMS bms;
extern Serial serial;

extern int battery_voltage;
extern int battery_current;
extern int load_voltage;
extern int cell_voltages[15];      // max. number of cells
extern float temperatures[3];
extern float SOC;

void uext_init()
{
    lcd.init();
    lcd.view(VIEW_TOP);
}

void uext_process_1s()
{
    char str[20];

    int balancingStatus = bms.get_balancing_status();

    lcd.clear_screen();

    sprintf(str, "%.2fV", bms.pack_voltage()/1000.0);
    lcd.string(0,0,font_8x16, str);

    sprintf(str, "%.2fA", bms.pack_current()/1000.0);
    lcd.string(7*8,0,font_8x16, str);

    sprintf(str, "T:%.1f", bms.get_temp_degC(1));
    lcd.string(0,2,font_6x8, str);

    sprintf(str, "SOC:%.2f", bms.get_soc());
    lcd.string(6*7,2,font_6x8, str);

    sprintf(str, "Load: %.2fV", load_voltage/1000.0);
    lcd.string(0,3,font_6x8, str);

    for (int i = 0; i < NUM_CELLS_MAX; i++) {
        if (blinkOn || !(balancingStatus & (1 << i))) {
            sprintf(str, "%d:%.3fV", i+1, bms.cell_voltage(i+1)/1000.0);
            lcd.string((i % 2 == 0) ? 0 : 51, 4 + (i / 2), font_6x8, str);
        }
    }
}

#endif /* DOGLCD_ENABLED */

#endif /* UNIT_TEST */