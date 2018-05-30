#include "driver/digit_led_display.h"
#include "lfsr.h"
#include "port.h"

#include <util/delay.h>
#include <stdlib.h>

#define TABLE_WIDTH 8
#define TABLE_HEIGHT 32

#define FIGURE_SIZE 5

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
        tetrix->figure_x[i] = TABLE_HEIGHT;
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

//TODO:
// - por que necesito dividir con un bit mas en LFSR/MAX?
// - por que salen solo dos tipos de figuras?
// - anadir limites
//     - eje x bounded respecto a los limites de la tabla y la propia figura
//     - eje y solo hacia arriba unbounded
void tetrix_create_figure(Tetrix* tetrix, uint16_t figure_seed)
{
    LFSR lfsr;
    init_lfsr(&lfsr, figure_seed);

    tetrix->figure_x[0] = lfsr_generate(&lfsr) / (LFSR_MAX / 16); //0-8
    tetrix->figure_y[0] = TABLE_HEIGHT;

    port_mode(PORT_0, PORT_OUTPUT);
    digital_port_out(PORT_0, tetrix->figure_x[0]);

    for(int i = 1; i < FIGURE_SIZE; i++)
    {
        int direction = lfsr_generate(&lfsr) / (LFSR_MAX / 8); //0-4, 2 lowest bits
        uint8_t axis = (direction & 1) ? X_AXIS : Y_AXIS;
        uint8_t displacement = (direction & 1) ? 1 : -1;
        if(axis == X_AXIS)
        {
            tetrix->figure_x[i] = tetrix->figure_x[i - 1] + displacement;
            tetrix->figure_y[i] = tetrix->figure_y[i - 1];
        }
        else
        {
            tetrix->figure_x[i] = tetrix->figure_x[i - 1];
            tetrix->figure_y[i] = tetrix->figure_y[i - 1] + displacement;
        }
    }

}

bool tetrix_move_figure(Tetrix* tetrix, int movement, int axis)
{
    uint8_t* coordinates = (axis == X_AXIS) ? tetrix->figure_x : tetrix->figure_y;
    uint8_t* fixed = (axis == X_AXIS) ? tetrix->figure_y : tetrix->figure_x;
    for(int i = 0; i < FIGURE_SIZE; i++)
    {
        int8_t destination = coordinates[i] + movement;
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
            values_to_display[y / 8] |= (tetrix->table[y][x] << (7 - (y % 8)));
            dld_row(dld, x, values_to_display);
        }
    }

    for(int i = 0; i < FIGURE_SIZE; i++)
    {
        tetrix->table[tetrix->figure_y[i]][tetrix->figure_x[i]] = BLOCK_EMPTY;
    }
}

int main(void)
{
    SPI spi;
    init_spi(&spi, DIGITAL_13, UNUSED_PIN, DIGITAL_11, DIGITAL_10, SPI_MSB);

    DigitLedDisplay dld;
    init_dld(&dld, &spi, 4);
    dld_write(&dld, DLD_OP_INTENSITY, 1);
    dld_light(&dld, true);

    LFSR lfsr;
    init_lfsr(&lfsr, 0xACE1); //TODO: from entropy


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
                _delay_ms(100);
                int x_movement = 0; //TODO: read_user_input
                tetrix_move_figure(&tetrix, x_movement, X_AXIS);
            }
            tetrix_table_update(&tetrix);
        }
    }
}
