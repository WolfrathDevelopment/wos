#include <types.h>
#include <io/bus.h>
#include <io/console.h>
#include <io/mmio.h>
#include <drivers/drivers.h>

#define VGA_COLOR_WHITE     (0x0F)
#define VGA_COLOR_BLACK     (0x00)

#define VGA_MAX_ROWS        (25)
#define VGA_MAX_COLS        (80)
#define VGA_MAX_CHARS       (VGA_MAX_ROWS * VGA_MAX_COLS)
#define VGA_BUFFER_LEN      (VGA_BUFFER_LEN * sizeof(uint16_t))

static int16_t console_cursor_location = 0;

static inline uint16_t console_desc_for_char(char c)
{
    uint8_t attr = (VGA_COLOR_BLACK << 4) | (VGA_COLOR_WHITE & 0x0F);
    return (attr << 8) | c;
}

static void set_cursor_location(int32_t x, int32_t y)
{
}

static void console_scroll()
{

}

static void console_update()
{
    // No negative values
    console_cursor_location = (console_cursor_location < 0) ? 0 : console_cursor_location;
    if(console_cursor_location >= VGA_MAX_CHARS)
    {
        console_scroll();
    } 

    write_io_bus_port(IO_BUS_VGA_CONTROL_REG, IO_BUS_HIGH_BYTE);
    write_io_bus_port(IO_BUS_VGA_DATA_REG, console_cursor_location >> 8);

    write_io_bus_port(IO_BUS_VGA_CONTROL_REG, IO_BUS_LOW_BYTE);
    write_io_bus_port(IO_BUS_VGA_DATA_REG, console_cursor_location & 0xFF);
}

static void console_clear()
{
    uint16_t empty_char = console_desc_for_char(' ');
    uint32_t offset = 0;
    for(; offset < VGA_BUFFER_LEN; offset += sizeof(uint16_t))
    {
        OsRc rc = RC_SUCCESS;
        rc = mmio_write_bytes(MmioConsole, &empty_char, offset, sizeof(uint16_t));
        if(rc != RC_SUCCESS)
        {
            TRACE_RC(rc);
            PANIC("Unexpected RC during console init");
        }
    }
}

void console_init()
{
    console_clear();
    console_update(); 
}

void console_notify_key_pressed(uint16_t scancode)
{

}

void printf(char* fmt, ... )
{

}
