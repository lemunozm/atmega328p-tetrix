#include "driver/digit_led_display.h"
#include "lfsr.h"
#include "port.h"

#include <util/delay.h>
#include <stdlib.h>

#include <avr/io.h>
#define TABLE_WIDTH 8
#define TABLE_HEIGHT 32

#define FIGURE_SIZE 4

#define BLOCK_EMPTY 0
#define BLOCK_WALL 1

#define X_AXIS 0
#define Y_AXIS 1

typedef struct Tetrix
{
    uint8_t table [TABLE_HEIGHT + FIGURE_SIZE][TABLE_WIDTH];
    uint8_t figure_x[FIGURE_SIZE];
    uint8_t figure_y[FIGURE_SIZE];

}Tetrix;

void init_tetrix(Tetrix* tetrix)
{
    for(int y = 0; y < TABLE_HEIGHT + FIGURE_SIZE; y++)
    {
        for(int x = 0; x < TABLE_WIDTH; x++)
        {
            tetrix->table[y][x] = BLOCK_EMPTY;
        }
    }

    for(int i = 0; i < FIGURE_SIZE; i++)
    {
        tetrix->figure_x[i] = 0;
        tetrix->figure_y[i] = TABLE_HEIGHT;
    }
}

bool tetrix_end(Tetrix* tetrix)
{
    bool end = false;
    for(int x = 0; x < TABLE_WIDTH; x++)
    {
        end |= tetrix->table[TABLE_HEIGHT - 1][x];
    }

    return end;
}

void tetrix_create_figure(Tetrix* tetrix, uint16_t figure_seed)
{
    LFSR lfsr;
    init_lfsr(&lfsr, figure_seed);

    tetrix->figure_x[0] = FIGURE_SIZE;
    tetrix->figure_y[0] = TABLE_HEIGHT;

    int x_direction = (lfsr_generate_bounded(&lfsr, 2) == 1) ? 1 : -1;
    int x_displacement = 0;
    for(int i = 1; i < FIGURE_SIZE; i++)
    {
        int axis = lfsr_generate_bounded(&lfsr, 2);
        switch (axis)
        {
            case X_AXIS:
                tetrix->figure_x[i] = tetrix->figure_x[i - 1] + x_direction;
                tetrix->figure_y[i] = tetrix->figure_y[i - 1];
                x_displacement++;
                break;

            case Y_AXIS:
                tetrix->figure_x[i] = tetrix->figure_x[i - 1];
                tetrix->figure_y[i] = tetrix->figure_y[i - 1] + 1;
                break;
        }
    }

    //random x init coordinate (optional)
    int x_left = (x_direction == 1) ? FIGURE_SIZE : FIGURE_SIZE - x_displacement;
    int x_right = (x_direction == 1) ? FIGURE_SIZE + x_displacement : FIGURE_SIZE;
    int movement = lfsr_generate_bounded(&lfsr, TABLE_WIDTH - x_right + x_left) - x_left;

    for(int i = 0; i < FIGURE_SIZE; i++)
    {
        tetrix->figure_x[i] += movement;
    }
}

bool tetrix_move_figure(Tetrix* tetrix, int movement, int axis)
{
    uint8_t* coordinates = (axis == X_AXIS) ? tetrix->figure_x : tetrix->figure_y;
    uint8_t* fixed = (axis == X_AXIS) ? tetrix->figure_y : tetrix->figure_x;
    for(int i = 0; i < FIGURE_SIZE; i++)
    {
        int destination = coordinates[i] + movement;
        if(destination < 0 || (axis == X_AXIS && destination >= TABLE_WIDTH)
                           || (axis == Y_AXIS && destination >= TABLE_HEIGHT + FIGURE_SIZE))
        {
            return false;
        }

        for(int d = destination; d != coordinates[i]; d += -movement / abs(movement))
        {
            uint8_t value = (axis == X_AXIS) ? tetrix->table[fixed[i]][d] : tetrix->table[d][fixed[i]];
            if(value == BLOCK_WALL)
            {
                return false;
            }
        }
    }

    for(int i = 0; i < FIGURE_SIZE; i++)
    {
        coordinates[i] += movement;
    }

    return true;
}

void tetrix_table_update(Tetrix* tetrix)
{
    for(int i = 0; i < FIGURE_SIZE; i++)
    {
        tetrix->table[tetrix->figure_y[i]][tetrix->figure_x[i]] = BLOCK_WALL;
    }

    int current_row = 0;
    for(int y = 0; y < TABLE_HEIGHT; y++)
    {
        for(int x = 0; x < TABLE_WIDTH; x++)
        {
            tetrix->table[current_row][x] = tetrix->table[y][x];
        }

        bool remove_row = true;
        for(int x = 0; x < TABLE_WIDTH; x++)
        {
            remove_row &= tetrix->table[y][x] == BLOCK_WALL;
        }

        if(!remove_row)
        {
            current_row++;
        }
    }
}

void tetrix_display(Tetrix* tetrix, DigitLedDisplay* dld)
{
    for(int i = 0; i < FIGURE_SIZE; i++)
    {
        tetrix->table[tetrix->figure_y[i]][tetrix->figure_x[i]] = BLOCK_WALL;
    }

    for(int x = 0; x < TABLE_WIDTH; x++)
    {
        uint8_t values_to_display[4] = {0};
        for(int y = 0; y < TABLE_HEIGHT; y++)
        {
            uint8_t value = tetrix->table[y][x] << ((TABLE_WIDTH - 1) - (y % TABLE_WIDTH));
            values_to_display[y / TABLE_WIDTH] |= value;
        }
        dld_row(dld, x, values_to_display);
    }

    for(int i = 0; i < FIGURE_SIZE; i++)
    {
        tetrix->table[tetrix->figure_y[i]][tetrix->figure_x[i]] = BLOCK_EMPTY;
    }
}

int main(void)
{
    SPI spi;
    init_spi(&spi, D_PIN_10, UNUSED_PIN, D_PIN_9, D_PIN_8, SPI_MSB);

    DigitLedDisplay dld;
    init_dld(&dld, &spi, 4);
    dld_write(&dld, DLD_OP_INTENSITY, 1);
    dld_light(&dld, true);

    LFSR lfsr;
    init_lfsr(&lfsr, 0xACE1); //TODO: from entropy

    pin_mode(D_PIN_13, PIN_INPUT);
    pin_mode(D_PIN_12, PIN_INPUT);
    pin_mode(D_PIN_11, PIN_INPUT);

    while(true)
    {
        Tetrix tetrix;
        init_tetrix(&tetrix);
        while(!tetrix_end(&tetrix))
        {
            tetrix_create_figure(&tetrix, lfsr_generate(&lfsr));
            while(tetrix_move_figure(&tetrix, -1, Y_AXIS))
            {
                tetrix_display(&tetrix, &dld);
                int waiting = digital_pin_in(D_PIN_13) ? 1 : 25;
                int right_movement = 0;
                int left_movement = 0;
                for(int i = 0; i < waiting; i++)
                {
                    right_movement |= digital_pin_in(D_PIN_11);
                    left_movement |= digital_pin_in(D_PIN_12);
                    _delay_ms(10);
                }
                tetrix_move_figure(&tetrix, right_movement - left_movement, X_AXIS);
            }
            tetrix_table_update(&tetrix);
        }
    }
}

//DEBUG method:
//port_mode(PORT_0, PORT_OUTPUT);
//digital_port_out(PORT_0, 0);
