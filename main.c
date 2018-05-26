#include "driver/digit_led_display.h"

#include <util/delay.h>

#define TABLE_WIDTH 8
#define TABLE_HEIGHT 32
#define TABLE_HEIGHT_FIGURE 32

typedef struct Table
{
    uint8_t data [TABLE_WIDTH * (TABLE_HEIGHT + TABLE_HEIGHT_FIGURE)];
    uint8_t top;
}Table;

void init_table(Table* table)
{
    (void)table;
}

void table_add_figure(Table* table, uint8_t figure_id)
{
    (void)table; (void)figure_id;
}

bool table_down_line(Table* table)
{
    (void)table;
    return 0;
}

void table_move(Table* table, int movement)
{
    (void)table; (void)movement;
}

void table_display(Table* table, DigitLedDisplay* dld)
{
    (void) table; (void)dld;
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
            bool nothing_to_go_down = table_down_line(&table);
            if(nothing_to_go_down)
            {
                table_add_figure(&table, 1);
            }
            table_display(&table, &dld);
            _delay_ms(100);
        }
    }
}
