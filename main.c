#include "driver/digit_led_display.h"

#include <util/delay.h>

#define TABLE_WIDTH 8
#define TABLE_HEIGHT 32
#define TABLE_HEIGHT_FIGURE 32

#define BLOCK_EMPTY 0
#define BLOCK_WALL 0
#define BLOCK_FIGURE 0

typedef struct Table
{
    uint8_t data [TABLE_HEIGHT + TABLE_HEIGHT_FIGURE][TABLE_WIDTH];
    uint8_t top;

}Table;

void init_table(Table* table)
{
    for(int y = 0; y < TABLE_HEIGHT + TABLE_HEIGHT_FIGURE; y++)
    {
        for(int x = 0; x < TABLE_WIDTH; x++)
        {
            table->data[y][x] = BLOCK_EMPTY;
        }
    }
    table->top = 0;
}

void table_add_figure(Table* table, uint8_t figure_id)
{
    (void)table; (void)figure_id;
}

bool table_update(Table* table)
{
    bool figure_down = true;
    for(int y = 1; y < TABLE_HEIGHT + TABLE_HEIGHT_FIGURE; y++)
    {
        uint8_t* line_down = table->data[y - 1];
        uint8_t* line_up = table->data[y];
        bool figure_to_down = false;
        for(int x = 0; x < TABLE_WIDTH; x++)
        {
            if(line_up[x] == BLOCK_FIGURE)
            {
                 figure_to_down &= line_down[x] == BLOCK_FIGURE;
            }
        }

        if(figure_to_down)
        {
            figure_down = false;
            for(int x = 0; x < TABLE_WIDTH; x++)
            {
                if(line_up[x] == BLOCK_FIGURE)
                {
                    line_down[x] = BLOCK_FIGURE;
                    line_up[x] = BLOCK_EMPTY;
                }
            }
        }
    }

    for(int y = 0; y < TABLE_HEIGHT; y++)
    {
        uint8_t* line = table->data[y];
        uint8_t delete_line = 0;
        for(int x = 0; x < TABLE_WIDTH; x++)
        {
            if(line[x] == BLOCK_FIGURE)
            {
                line[x] = BLOCK_WALL;
            }
            if(line[x] == BLOCK_WALL)
            {
                delete_line++;
            }
        }

        if(delete_line)
        {
            //TODO
        }
    }

    return figure_down;
}

void table_move(Table* table, int movement)
{
    (void)table; (void)movement;
}

void table_display(Table* table, DigitLedDisplay* dld)
{
    for(int x = 0; x < TABLE_WIDTH; x++)
    {
        uint8_t values_to_dld[4] = {0};
        for(int y = 0; y < TABLE_HEIGHT + TABLE_HEIGHT_FIGURE; y++)
        {
            values_to_dld[y / 8] |= (table->data[y][x] << (y % 8));
            dld_row(dld, x, values_to_dld);
        }
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

    while(true)
    {
        Table table;
        init_table(&table);
        table_add_figure(&table, 1);
        while(table.top < TABLE_WIDTH - 1)
        {
            //read_user_input
            table_move(&table, 1);
            bool figure_down = table_update(&table);
            if(figure_down)
            {
                table_add_figure(&table, 1);
            }
            table_display(&table, &dld);
            _delay_ms(100);
        }
    }
}
