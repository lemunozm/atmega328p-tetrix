#include "driver/digit_led_display.h"
#include "lfsr.h"
#include "port.h"

#include <util/delay.h>

#define TABLE_WIDTH 8
#define TABLE_HEIGHT 32
#define TABLE_HEIGHT_FIGURE 32

#define FIGURE_SIZE 5

#define BLOCK_EMPTY 0
#define BLOCK_WALL 0
#define BLOCK_FIGURE 0

typedef struct Tetrix
{
    uint8_t table [TABLE_HEIGHT + TABLE_HEIGHT_FIGURE][TABLE_WIDTH];
    uint8_t figure_x[FIGURE_SIZE];
    uint8_t figure_y[FIGURE_SIZE];

}Tetrix;

void init_tetrix(Tetrix* tetrix)
{
    for(int y = 0; y < TABLE_HEIGHT + TABLE_HEIGHT_FIGURE; y++)
    {
        for(int x = 0; x < TABLE_WIDTH; x++)
        {
            tetrix->table[y][x] = BLOCK_EMPTY;
        }
    }

    for(int i = 0; i < FIGURE_SIZE; i++)
    {
        tetrix->figure_x[i] = 0;
        tetrix->figure_y[i] = 0;
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

void tetrix_create_figure(Tetrix* tetrix, uint8_t figure_id)
{
    (void)tetrix; (void)figure_id;
}

bool tetrix_move_figure(Tetrix* tetrix, int x, int y)
{
    (void)tetrix; (void)x; (void)y;
    return false;
}

bool tetrix_table_update(Tetrix* tetrix)
{
    (void) tetrix;
    return 0;
}

void tetrix_display(Tetrix* tetrix, DigitLedDisplay* dld)
{
    //add figure in table
    for(int x = 0; x < TABLE_WIDTH; x++)
    {
        uint8_t values_to_dld[4] = {0};
        for(int y = 0; y < TABLE_HEIGHT + TABLE_HEIGHT_FIGURE; y++)
        {
            //put better figure here
            values_to_dld[y / 8] |= (tetrix->table[y][x] << (y % 8));
            dld_row(dld, x, values_to_dld);
        }
    }
    //remove figure from table
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
    init_lfsr(&lfsr, 0xACE1); //from entropy

    /*while(true)
    {
        Tetrix tetrix;
        init_tetrix(&tetrix);
        tetrix_create_figure(&tetrix, 1);
        while(!tetrix_end(&tetrix))
        {
            int x_movement = //read_user_input
            tetrix_move_figure(&tetrix, x_movement, 0);
            bool figure_moved = tetrix_move_figure(&tetrix, 0, -1);
            if(figure_moved)
            {
                tetrix_table_update(&tetrix);
                tetrix_create_figure(&tetrix, 1);
            }
            tetrix_display(&tetrix, &dld);
            _delay_ms(100);
        }
    }*/
}
