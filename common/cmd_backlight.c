#include <common.h>
#include <command.h>
#include <i2c.h>
#define BACKLIGHT_BUS 2
#define BACKLIGHT_ADDR 0xd
#define BACKLIGHT_LEVEL (0x55 << 4)
#define TWL4030_I2C_BUS 0
int do_backlight(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int i=1;
	uchar level[2] = {5, 80};
	while(i--)
	{
		i2c_set_bus_num(BACKLIGHT_BUS);
	//	i2c_read_standard_byte(BACKLIGHT_ADDR, &read_data);
	//	printf("read from i2c = %d\n", read_data);
		i2c_write_standard_byte(BACKLIGHT_ADDR, level);
	//	i2c_read_standard_byte(BACKLIGHT_ADDR, &read_data);
	//	printf("read from i2c = %d\n", read_data);
		i2c_set_bus_num(TWL4030_I2C_BUS);
	}
}
U_BOOT_CMD(backlight, 3, 0, do_backlight,
        "backlight - display a menu, to select the items to execute cmd",
        "-display a menu, to select the items to execute cmd\n"
	"add by haolong");

