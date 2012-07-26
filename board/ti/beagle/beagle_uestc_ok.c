/*
 * (C) Copyright 2004-2008
 * Texas Instruments, <www.ti.com>
 *
 * Author :
 *	Sunil Kumar <sunilsaini05@gmail.com>
 *	Shashi Ranjan <shashiranjanmca05@gmail.com>
 *
 * Derived from Beagle Board and 3430 SDP code by
 *	Richard Woodruff <r-woodruff2@ti.com>
 *	Syed Mohammed Khasim <khasim@ti.com>
 *
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <common.h>
#ifdef CONFIG_STATUS_LED
#include <status_led.h>
#endif
#include <twl4030.h>
#include <asm/io.h>
#include <asm/arch/mmc_host_def.h>
#include <asm/arch/mux.h>
#include <asm/arch/sys_proto.h>
#include <asm/gpio.h>
#include <asm/mach-types.h>
#ifdef CONFIG_USB_EHCI
#include <usb.h>
#include <asm/arch/clocks.h>
#include <asm/arch/clocks_omap3.h>
#include <asm/arch/ehci_omap3.h>
/* from drivers/usb/host/ehci-core.h */
extern struct ehci_hccr *hccr;
extern volatile struct ehci_hcor *hcor;
#endif
#include "beagle.h"
#include "uestc-logo.h"
#include <command.h>

#define pr_debug(fmt, args...) debug(fmt, ##args)

#define TWL4030_I2C_BUS			0
#define EXPANSION_EEPROM_I2C_BUS	1
#define EXPANSION_EEPROM_I2C_ADDRESS	0x50

#define TINCANTOOLS_ZIPPY		0x01000100
#define TINCANTOOLS_ZIPPY2		0x02000100
#define TINCANTOOLS_TRAINER		0x04000100
#define TINCANTOOLS_SHOWDOG		0x03000100
#define KBADC_BEAGLEFPGA		0x01000600
#define LW_BEAGLETOUCH			0x01000700
#define BRAINMUX_LCDOG			0x01000800
#define BRAINMUX_LCDOGTOUCH		0x02000800
#define BBTOYS_WIFI			0x01000B00
#define BBTOYS_VGA			0x02000B00
#define BBTOYS_LCD			0x03000B00
#define BEAGLE_NO_EEPROM		0xffffffff

/*************************12.21 modified by haolong***************/
#define RSLT_HEIGHT             1024
#define RSLT_WIDTH				768


DECLARE_GLOBAL_DATA_PTR;

static struct {
	unsigned int device_vendor;
	unsigned char revision;
	unsigned char content;
	char fab_revision[8];
	char env_var[16];
	char env_setting[64];
} expansion_config;

/*
 * Routine: board_init
 * Description: Early hardware init.
 */
int board_init(void)
{
	gpmc_init(); /* in SRAM or SDRAM, finish GPMC */
	/* board id for Linux */
	gd->bd->bi_arch_number = MACH_TYPE_OMAP3_BEAGLE;
	/* boot param addr */
	gd->bd->bi_boot_params = (OMAP34XX_SDRC_CS0 + 0x100);

#if defined(CONFIG_STATUS_LED) && defined(STATUS_LED_BOOT)
	status_led_set (STATUS_LED_BOOT, STATUS_LED_ON);
#endif

	return 0;
}
/**************************12.19 modified by haolong*************************/
void dss_init(void)
{
	unsigned int i;
	unsigned char pixel[3];
	int offset = 0;

	/* assuming a resolution of 1280x720 - draw logo into dss buffer */

	/* fill in the blank white bar at the top */
	for(i = 0; i<(((RSLT_WIDTH/2)-(beagle_height/2))*RSLT_HEIGHT)*2; i+=2) {
		*((unsigned short *)(0x80500000 + i + offset)) = 0xffff;
	}

	offset += i;

	/* Add the image data */
	for (i = 0; i < ((beagle_width*beagle_height)*2);) {
		HEADER_PIXEL(header_data, pixel);
		*((unsigned short *)(0x80500000 + i + offset)) =
			((((pixel[0])&0xf8) << 8) |
			(((pixel[1])&0xfc) << 3) |
			(((pixel[2])&0xf8) >> 3));
		i+=2;
	}

	offset += i;

	/* fill in the blank white bar at the bottom */
	for(i = 0; i<(RSLT_WIDTH - (offset/2/RSLT_HEIGHT))*RSLT_HEIGHT*2; i+=2) {
		*((unsigned short *)(0x80500000 + i + offset)) = 0xffff;
	}
	
//	*((uint *) 0x48310034) = 0xfefffedf;
//	*((uint *) 0x48310094) = 0x01000120;
//	*((uint *) 0x48004D44) = 0x0001b00c;
//	*((uint *) 0x48004E40) = 0x00001006;
//	*((uint *) 0x48004D00) = 0x00370037;
//	*((uint *) 0x48050C00) = 0x00000002;
//	*((uint *) 0x48050C04) = 0x0000001B;
//	*((uint *) 0x48050C08) = 0x00000040;
//	*((uint *) 0x48050C0C) = 0x00000000;
//	*((uint *) 0x48050C10) = 0x00000000;
//	*((uint *) 0x48050C14) = 0x00008000;
//	*((uint *) 0x48050C18) = 0x00000000;
//	*((uint *) 0x48050C1C) = 0x00008359;
//	*((uint *) 0x48050C20) = 0x0000020C;
//	*((uint *) 0x48050C24) = 0x00000000;
//	*((uint *) 0x48050C28) = 0x043F2631;
//	*((uint *) 0x48050C2C) = 0x00000024;
//	*((uint *) 0x48050C30) = 0x00000130;
//	*((uint *) 0x48050C34) = 0x00000198;
//	*((uint *) 0x48050C38) = 0x000001C0;
//	*((uint *) 0x48050C3C) = 0x0000006A;
//	*((uint *) 0x48050C40) = 0x0000005C;
//	*((uint *) 0x48050C44) = 0x00000000;
//	*((uint *) 0x48050C48) = 0x00000001;
//	*((uint *) 0x48050C4C) = 0x0000003F;
//	*((uint *) 0x48050C50) = 0x21F07C1F;
//	*((uint *) 0x48050C54) = 0x00000000;
//	*((uint *) 0x48050C58) = 0x00000015;
//	*((uint *) 0x48050C5C) = 0x00001400;
//	*((uint *) 0x48050C60) = 0x00000000;
//	*((uint *) 0x48050C64) = 0x069300F4;
//	*((uint *) 0x48050C68) = 0x0016020C;
//	*((uint *) 0x48050C6C) = 0x00060107;
//	*((uint *) 0x48050C70) = 0x008D034E;
//	*((uint *) 0x48050C74) = 0x000F0359;
//	*((uint *) 0x48050C78) = 0x01A00000;
//	*((uint *) 0x48050C7C) = 0x020501A0;
//	*((uint *) 0x48050C80) = 0x01AC0024;
//	*((uint *) 0x48050C84) = 0x020D01AC;
//	*((uint *) 0x48050C88) = 0x00000006;
//	*((uint *) 0x48050C8C) = 0x00000000;
//	*((uint *) 0x48050C90) = 0x03480079;
//	*((uint *) 0x48050C94) = 0x02040024;
//	*((uint *) 0x48050C98) = 0x00000000;
//	*((uint *) 0x48050C9C) = 0x00000000;
//	*((uint *) 0x48050CA0) = 0x0001008A;
//	*((uint *) 0x48050CA4) = 0x01AC0106;
//	*((uint *) 0x48050CA8) = 0x01060006;
//	*((uint *) 0x48050CAC) = 0x00000000;
//	*((uint *) 0x48050CB0) = 0x00140001;
//	*((uint *) 0x48050CB4) = 0x00010001;
//	*((uint *) 0x48050CB8) = 0x00FF0000;
//	*((uint *) 0x48050CBC) = 0x00000000;
//	*((uint *) 0x48050CC0) = 0x00000000;
//	*((uint *) 0x48050CC4) = 0x0000000D;
//	*((uint *) 0x48050CC8) = 0x00000000;
//	*((uint *) 0x48050010) = 0x00000001;
//	*((uint *) 0x48050040) = 0x00000078;
//	*((uint *) 0x48050044) = 0x00000000;
//	*((uint *) 0x48050048) = 0x00000000;
//	*((uint *) 0x48050050) = 0x00000000;
//	*((uint *) 0x48050058) = 0x00000000;
//	*((uint *) 0x48050410) = 0x00002015;
//	*((uint *) 0x48050414) = 0x00000001;
//	*((uint *) 0x48050444) = 0x00000004;
//	*((uint *) 0x4805044c) = 0xFFFFFFFF;
//	*((uint *) 0x48050450) = 0x00000000;
//	*((uint *) 0x48050454) = 0x00000000;
//	*((uint *) 0x48050458) = 0x00000000;
//	*((uint *) 0x48050464) = 0x0ff03f31;
//	*((uint *) 0x48050468) = 0x01400504;
//	*((uint *) 0x4805046c) = 0x00007028;
//	*((uint *) 0x48050470) = 0x00010002;
//	*((uint *) 0x48050478) = 0x00ef027f;
//	*((uint *) 0x4805047c) = 0x02cf04ff;
	*((uint *) 0x48050480) = 0x80500000;
	*((uint *) 0x48050484) = 0x80500000;
	*((uint *) 0x48050488) = 0x00000000;
	*((uint *) 0x4805048c) = 0x02cf04ff;
	*((uint *) 0x4805048c) = 0x02ff03ff;//--
	*((uint *) 0x480504a0) = 0x0000008d;
	*((uint *) 0x480504a4) = 0x03fc03bc;
	*((uint *) 0x480504a8) = 0x00000400;
	*((uint *) 0x480504ac) = 0x00000001;
	*((uint *) 0x480504b0) = 0x00000001;
	*((uint *) 0x480504b4) = 0x00000000;
	*((uint *) 0x480504b8) = 0x807ff000;
	udelay(1000);

	
	*((uint *) 0x48050440) = 0x0001836b;
	udelay(1000);
	*((uint *) 0x48050440) = 0x0001836b;
	udelay(1000);
	*((uint *) 0x48050440) = 0x0001836b;
	udelay(1000);
//	*((uint *) 0x48050440) = 0x0001836d;// enable digital output
//	udelay(1000);
//	*((uint *) 0x48050440) = 0x0001836d;
//	udelay(1000);
//	*((uint *) 0x48050440) = 0x0001836d;
//	udelay(1000);
}

/*
 * Routine: get_board_revision
 * Description: Detect if we are running on a Beagle revision Ax/Bx,
 *		C1/2/3, C4 or xM. This can be done by reading
 *		the level of GPIO173, GPIO172 and GPIO171. This should
 *		result in
 *		GPIO173, GPIO172, GPIO171: 1 1 1 => Ax/Bx
 *		GPIO173, GPIO172, GPIO171: 1 1 0 => C1/2/3
 *		GPIO173, GPIO172, GPIO171: 1 0 1 => C4
 *		GPIO173, GPIO172, GPIO171: 0 0 0 => xM
 */
int get_board_revision(void)
{
	int revision;

	if (!gpio_request(171, "") &&
	    !gpio_request(172, "") &&
	    !gpio_request(173, "")) {

		gpio_direction_input(171);
		gpio_direction_input(172);
		gpio_direction_input(173);

		revision = gpio_get_value(173) << 2 |
			   gpio_get_value(172) << 1 |
			   gpio_get_value(171);

		gpio_free(171);
		gpio_free(172);
		gpio_free(173);
	} else {
		printf("Error: unable to acquire board revision GPIOs\n");
		revision = -1;
	}

	return revision;
}

/*
 * Routine: get_expansion_id
 * Description: This function checks for expansion board by checking I2C
 *		bus 1 for the availability of an AT24C01B serial EEPROM.
 *		returns the device_vendor field from the EEPROM
 */
unsigned int get_expansion_id(void)
{
	i2c_set_bus_num(EXPANSION_EEPROM_I2C_BUS);

	/* return BEAGLE_NO_EEPROM if eeprom doesn't respond */
	if (i2c_probe(EXPANSION_EEPROM_I2C_ADDRESS) == 1) {
		i2c_set_bus_num(TWL4030_I2C_BUS);
		return BEAGLE_NO_EEPROM;
	}

	/* read configuration data */
	i2c_read(EXPANSION_EEPROM_I2C_ADDRESS, 0, 1, (u8 *)&expansion_config,
		 sizeof(expansion_config));

	i2c_set_bus_num(TWL4030_I2C_BUS);

	return expansion_config.device_vendor;
}

/*
 * Configure DSS to display background color on DVID
 * Configure VENC to display color bar on S-Video
 */
void beagle_display_init(void)
{
	omap3_dss_venc_config(&venc_config_std_tv, VENC_HEIGHT, VENC_WIDTH);
	switch (get_board_revision()) {
	case REVISION_AXBX:
	case REVISION_CX:
	case REVISION_C4:
		omap3_dss_panel_config(&dvid_cfg);
		break;
	case REVISION_XM_A:
	case REVISION_XM_B:
	case REVISION_XM_C:
	default:
		omap3_dss_panel_config(&dvid_cfg_xm);
		break;
	}

/***********************12.21 modified by haolong*************************/
	
/**********************************************************************/
}

/*
 * Routine: misc_init_r
 * Description: Configure board specific parts
 */
int misc_init_r(void)
{
	struct gpio *gpio5_base = (struct gpio *)OMAP34XX_GPIO5_BASE;
	struct gpio *gpio6_base = (struct gpio *)OMAP34XX_GPIO6_BASE;
	struct control_prog_io *prog_io_base = (struct control_prog_io *)OMAP34XX_CTRL_BASE;

	/* Enable i2c2 pullup resisters */
	writel(~(PRG_I2C2_PULLUPRESX), &prog_io_base->io1);

	switch (get_board_revision()) {
	case REVISION_AXBX:
		printf("Beagle Rev Ax/Bx\n");
		setenv("beaglerev", "AxBx");
		break;
	case REVISION_CX:
		printf("Beagle Rev C1/C2/C3\n");
		setenv("beaglerev", "Cx");
		MUX_BEAGLE_C();
		break;
	case REVISION_C4:
		printf("Beagle Rev C4\n");
		setenv("beaglerev", "C4");
		MUX_BEAGLE_C();
		/* Set VAUX2 to 1.8V for EHCI PHY */
		twl4030_pmrecv_vsel_cfg(TWL4030_PM_RECEIVER_VAUX2_DEDICATED,
					TWL4030_PM_RECEIVER_VAUX2_VSEL_18,
					TWL4030_PM_RECEIVER_VAUX2_DEV_GRP,
					TWL4030_PM_RECEIVER_DEV_GRP_P1);
		break;
	case REVISION_XM_A:
		printf("Beagle xM Rev A\n");
		setenv("beaglerev", "xMA");
		MUX_BEAGLE_XM();
		/* Set VAUX2 to 1.8V for EHCI PHY */
		twl4030_pmrecv_vsel_cfg(TWL4030_PM_RECEIVER_VAUX2_DEDICATED,
					TWL4030_PM_RECEIVER_VAUX2_VSEL_18,
					TWL4030_PM_RECEIVER_VAUX2_DEV_GRP,
					TWL4030_PM_RECEIVER_DEV_GRP_P1);
		break;
	case REVISION_XM_B:
		printf("Beagle xM Rev B\n");
		setenv("beaglerev", "xMB");
		MUX_BEAGLE_XM();
		/* Set VAUX2 to 1.8V for EHCI PHY */
		twl4030_pmrecv_vsel_cfg(TWL4030_PM_RECEIVER_VAUX2_DEDICATED,
					TWL4030_PM_RECEIVER_VAUX2_VSEL_18,
					TWL4030_PM_RECEIVER_VAUX2_DEV_GRP,
					TWL4030_PM_RECEIVER_DEV_GRP_P1);
		break;
	case REVISION_XM_C:
		printf("Beagle xM Rev C\n");
		setenv("beaglerev", "xMC");
		MUX_BEAGLE_XM();
		/* Set VAUX2 to 1.8V for EHCI PHY */
		twl4030_pmrecv_vsel_cfg(TWL4030_PM_RECEIVER_VAUX2_DEDICATED,
					TWL4030_PM_RECEIVER_VAUX2_VSEL_18,
					TWL4030_PM_RECEIVER_VAUX2_DEV_GRP,
					TWL4030_PM_RECEIVER_DEV_GRP_P1);
		break;
	default:
		printf("Beagle unknown 0x%02x\n", get_board_revision());
		MUX_BEAGLE_XM();
		/* Set VAUX2 to 1.8V for EHCI PHY */
		twl4030_pmrecv_vsel_cfg(TWL4030_PM_RECEIVER_VAUX2_DEDICATED,
					TWL4030_PM_RECEIVER_VAUX2_VSEL_18,
					TWL4030_PM_RECEIVER_VAUX2_DEV_GRP,
					TWL4030_PM_RECEIVER_DEV_GRP_P1);
	}

	switch (get_expansion_id()) {
	case TINCANTOOLS_ZIPPY:
		printf("Recognized Tincantools Zippy board (rev %d %s)\n",
			expansion_config.revision,
			expansion_config.fab_revision);
		MUX_TINCANTOOLS_ZIPPY();
		setenv("buddy", "zippy");
		break;
	case TINCANTOOLS_ZIPPY2:
		printf("Recognized Tincantools Zippy2 board (rev %d %s)\n",
			expansion_config.revision,
			expansion_config.fab_revision);
		MUX_TINCANTOOLS_ZIPPY();
		setenv("buddy", "zippy2");
		break;
	case TINCANTOOLS_TRAINER:
		printf("Recognized Tincantools Trainer board (rev %d %s)\n",
			expansion_config.revision,
			expansion_config.fab_revision);
		MUX_TINCANTOOLS_ZIPPY();
		MUX_TINCANTOOLS_TRAINER();
		setenv("buddy", "trainer");
		break;
	case TINCANTOOLS_SHOWDOG:
		printf("Recognized Tincantools Showdow board (rev %d %s)\n",
			expansion_config.revision,
			expansion_config.fab_revision);
		/* Place holder for DSS2 definition for showdog lcd */
		setenv("defaultdisplay", "showdoglcd");
		setenv("buddy", "showdog");
		break;
	case KBADC_BEAGLEFPGA:
		printf("Recognized KBADC Beagle FPGA board\n");
		MUX_KBADC_BEAGLEFPGA();
		setenv("buddy", "beaglefpga");
		break;
	case LW_BEAGLETOUCH:
		printf("Recognized Liquidware BeagleTouch board\n");
		setenv("buddy", "beagletouch");
		break;
	case BRAINMUX_LCDOG:
		printf("Recognized Brainmux LCDog board\n");
		setenv("buddy", "lcdog");
		break;
	case BRAINMUX_LCDOGTOUCH:
		printf("Recognized Brainmux LCDog Touch board\n");
		setenv("buddy", "lcdogtouch");
		break;
	case BBTOYS_WIFI:
		printf("Recognized BeagleBoardToys WiFi board\n");
		MUX_BBTOYS_WIFI()
		setenv("buddy", "bbtoys-wifi");
		break;;
	case BBTOYS_VGA:
		printf("Recognized BeagleBoardToys VGA board\n");
		break;;
	case BBTOYS_LCD:
		printf("Recognized BeagleBoardToys LCD board\n");
		break;;
	case BEAGLE_NO_EEPROM:
		printf("No EEPROM on expansion board\n");
		setenv("buddy", "none");
		break;
	default:
		printf("Unrecognized expansion board: %x\n",
			expansion_config.device_vendor);
		setenv("buddy", "unknown");
	}

	if (expansion_config.content == 1)
		setenv(expansion_config.env_var, expansion_config.env_setting);

	twl4030_power_init();
	switch (get_board_revision()) {
	case REVISION_XM_A:
	case REVISION_XM_B:
		twl4030_led_init(TWL4030_LED_LEDEN_LEDBON);
		break;
	default:
		twl4030_led_init(TWL4030_LED_LEDEN_LEDAON | TWL4030_LED_LEDEN_LEDBON);
		break;
	}

	/* Set GPIO states before they are made outputs */
	writel(GPIO23 | GPIO10 | GPIO8 | GPIO2 | GPIO1,
		&gpio6_base->setdataout);
	writel(GPIO31 | GPIO30 | GPIO29 | GPIO28 | GPIO22 | GPIO21 |
		GPIO15 | GPIO14 | GPIO13 | GPIO12, &gpio5_base->setdataout);

	/* Configure GPIOs to output */
	writel(~(GPIO23 | GPIO10 | GPIO8 | GPIO2 | GPIO1), &gpio6_base->oe);
	writel(~(GPIO31 | GPIO30 | GPIO29 | GPIO28 | GPIO22 | GPIO21 |
		GPIO15 | GPIO14 | GPIO13 | GPIO12), &gpio5_base->oe);
/*****************************11.12.26 modified by haolong***************************************/
//	dieid_num_r();
	beagle_display_init();
	omap3_dss_enable();
	dss_init();
	
	dieid_num_r();


	return 0;
}

/*
 * Routine: set_muxconf_regs
 * Description: Setting up the configuration Mux registers specific to the
 *		hardware. Many pins need to be moved from protect to primary
 *		mode.
 */
void set_muxconf_regs(void)
{
	MUX_BEAGLE();
}

#ifdef CONFIG_GENERIC_MMC
int board_mmc_init(bd_t *bis)
{
	omap_mmc_init(0);
	return 0;
}
#endif

#ifdef CONFIG_USB_EHCI

#define GPIO_PHY_RESET 147

/* Reset is needed otherwise the kernel-driver will throw an error. */
int ehci_hcd_stop(void)
{
	pr_debug("Resetting OMAP3 EHCI\n");
	gpio_set_value(GPIO_PHY_RESET, 0);
	writel(OMAP_UHH_SYSCONFIG_SOFTRESET, OMAP3_UHH_BASE + OMAP_UHH_SYSCONFIG);
	/* disable USB clocks */
	struct prcm *prcm_base = (struct prcm *)PRCM_BASE;
	sr32(&prcm_base->iclken_usbhost, 0, 1, 0);
	sr32(&prcm_base->fclken_usbhost, 0, 2, 0);
	sr32(&prcm_base->iclken3_core, 2, 1, 0);
	sr32(&prcm_base->fclken3_core, 2, 1, 0);
	return 0;
}

/* Call usb_stop() before starting the kernel */
void show_boot_progress(int val)
{
	if(val == 15)
		usb_stop();
}

/*
 * Initialize the OMAP3 EHCI controller and PHY on the BeagleBoard.
 * Based on "drivers/usb/host/ehci-omap.c" from Linux 2.6.37.
 * See there for additional Copyrights.
 */
int ehci_hcd_init(void)
{
	pr_debug("Initializing OMAP3 ECHI\n");

	/* Put the PHY in RESET */
	gpio_request(GPIO_PHY_RESET, "");
	gpio_direction_output(GPIO_PHY_RESET, 0);
	gpio_set_value(GPIO_PHY_RESET, 0);

	/* Hold the PHY in RESET for enough time till DIR is high */
	/* Refer: ISSUE1 */
	udelay(10);

	struct prcm *prcm_base = (struct prcm *)PRCM_BASE;
	/* Enable USBHOST_L3_ICLK (USBHOST_MICLK) */
	sr32(&prcm_base->iclken_usbhost, 0, 1, 1);
	/*
	 * Enable USBHOST_48M_FCLK (USBHOST_FCLK1)
	 * and USBHOST_120M_FCLK (USBHOST_FCLK2)
	 */
	sr32(&prcm_base->fclken_usbhost, 0, 2, 3);
	/* Enable USBTTL_ICLK */
	sr32(&prcm_base->iclken3_core, 2, 1, 1);
	/* Enable USBTTL_FCLK */
	sr32(&prcm_base->fclken3_core, 2, 1, 1);
	pr_debug("USB clocks enabled\n");

	/* perform TLL soft reset, and wait until reset is complete */
	writel(OMAP_USBTLL_SYSCONFIG_SOFTRESET,
		OMAP3_USBTLL_BASE + OMAP_USBTLL_SYSCONFIG);
	/* Wait for TLL reset to complete */
	while (!(readl(OMAP3_USBTLL_BASE + OMAP_USBTLL_SYSSTATUS)
			& OMAP_USBTLL_SYSSTATUS_RESETDONE));
	pr_debug("TLL reset done\n");

	writel(OMAP_USBTLL_SYSCONFIG_ENAWAKEUP |
		OMAP_USBTLL_SYSCONFIG_SIDLEMODE |
		OMAP_USBTLL_SYSCONFIG_CACTIVITY,
		OMAP3_USBTLL_BASE + OMAP_USBTLL_SYSCONFIG);

	/* Put UHH in NoIdle/NoStandby mode */
	writel(OMAP_UHH_SYSCONFIG_ENAWAKEUP
		| OMAP_UHH_SYSCONFIG_SIDLEMODE
		| OMAP_UHH_SYSCONFIG_CACTIVITY
		| OMAP_UHH_SYSCONFIG_MIDLEMODE,
		OMAP3_UHH_BASE + OMAP_UHH_SYSCONFIG);

	/* setup burst configurations */
	writel(OMAP_UHH_HOSTCONFIG_INCR4_BURST_EN
		| OMAP_UHH_HOSTCONFIG_INCR8_BURST_EN
		| OMAP_UHH_HOSTCONFIG_INCR16_BURST_EN,
		OMAP3_UHH_BASE + OMAP_UHH_HOSTCONFIG);

	/*
	 * Refer ISSUE1:
	 * Hold the PHY in RESET for enough time till
	 * PHY is settled and ready
	 */
	udelay(10);
	gpio_set_value(GPIO_PHY_RESET, 1);

	hccr = (struct ehci_hccr *)(OMAP3_EHCI_BASE);
	hcor = (struct ehci_hcor *)(OMAP3_EHCI_BASE + 0x10);

	pr_debug("OMAP3 EHCI init done\n");
	return 0;
}

#endif /* CONFIG_USB_EHCI */

/*
 * This command returns the status of the user button on beagle xM
 * Input - none
 * Returns - 	1 if button is held down
 *		0 if button is not held down
 */
int do_userbutton(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int     button = 0;
	int	gpio;

	/*
	 * pass address parameter as argv[0] (aka command name),
	 * and all remaining args
	 */
	switch (get_board_revision()) {
	case REVISION_AXBX:
	case REVISION_CX:
	case REVISION_C4:
		gpio = 7;
		break;
	case REVISION_XM_A:
	case REVISION_XM_B:
	case REVISION_XM_C:
	default:
		gpio = 4;
		break;
	}
	gpio_request(gpio, "");
	gpio_direction_input(gpio);
	printf("The user button is currently ");
	if (gpio_get_value(gpio))
	{
		button = 1;
		printf("PRESSED.\n");
	}
	else
	{
		button = 0;
		printf("NOT pressed.\n");
	}

	gpio_free(gpio);

	return !button;
}

/* -------------------------------------------------------------------- */

U_BOOT_CMD(
	userbutton, CONFIG_SYS_MAXARGS, 1,	do_userbutton,
	"Return the status of the BeagleBoard USER button",
	""
);
