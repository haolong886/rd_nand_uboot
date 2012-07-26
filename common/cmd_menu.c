#include <common.h>
#include <command.h>
#include <mmc.h>

//static int curr_device = -1;

//int init_mmc()
//{
//	struct mmc *mmc;

//	if (curr_device < 0) {
//		if (get_mmc_num() > 0)
//			curr_device = 0;
//		else {
//			puts("No MMC device available\n");
//			return 1;
//		}
//	}

//	mmc = find_mmc_device(curr_device);

//	if (mmc) {
//		mmc_init(mmc);
//		print_mmcinfo(mmc);
//		return 0;
//	} else {
//		printf("no mmc device at slot %x\n", curr_device);
//		return 1;
//	}

//}

//int fat_load()
//{
//	
//}
void menu_shell(void);
void main_menu_usage(void);
//extern char awaitkey(unsigned long delay, int* error_p);
static char awaitkey(unsigned long delay, int* error_p)
{
	int i;
	char c;
	if (delay == -1) 
	{
		while (1) 
		{
   			if (tstc()) /* we got a key press */
   			return getc();
  		}
 	}
 	else 
 	{        
  		for (i = 0; i < delay; i++) 
  		{
   			if (tstc()) /* we got a key press */
   			return getc();
   			udelay (10*1000);
  		}
 	}
 	if (error_p)
 	*error_p = -1;
 	return 0;
}

int do_menu (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	menu_shell();
	return 0;
}

void menu_shell(void)
{
	char cmd_buf[200], size[20];
	char c;
	while(1)
	{
		main_menu_usage();
		c = awaitkey(-1, NULL);
		printf("you select %c\n", c);
		switch(c)
		{
			case '1':
			{
				int i = 1;
				strcpy(cmd_buf, "mmcinfo;");
				run_command(cmd_buf, 0);
				while(i)
				{
					printf("[1]flash MLO from mmc\n");
					printf("[2]flash UBOOT from mmc\n");
					printf("[3]flash KERNEL from mmc\n");
					printf("[4]flash CRAMFS(ROOTFS) from mmc\n");
					printf("[5]flash YAFFS2(USERFS) from mmc\n");
					printf("[6]boot kernel\n");
					printf("[0]exit this menu\n");
					c = awaitkey(-1, NULL);
					switch(c)
					{
						case '1':
						{
							printf("flashing ML0\n");
							//add code to check if file existting
							sprintf(cmd_buf, "nandecc hw; \
											 nand erase %x %x; \
											 fatload mmc 0:1 0x82000000 rd_nand_mlo;",
											 simple_strtoul(getenv("mlo_offset"), NULL, 10),
											 simple_strtoul(getenv("mlo_partition_size"), NULL, 10));
							printf("cmd_buf = %s\n", cmd_buf);
							run_command(cmd_buf, 0);
							sprintf(size, "0x%s", getenv("filesize"));
							sprintf(cmd_buf, "nand write 0x82000000 %x %x; \
											  nandecc sw", 
									simple_strtoul(getenv("mlo_offset"), NULL, 10),
									simple_strtoul(size, NULL, 10));
							
							printf("cmd_buf = %s\n", cmd_buf);
							run_command(cmd_buf, 0);	
							printf("flash completed\n");
							break;
						}
						case '2':
						{
							printf("flashing UBOOT\n");
							//add code to check if file existting
							sprintf(cmd_buf, "nandecc sw; \
											  nand erase %x %x; \
											 fatload mmc 0:1 0x82000000 rd_nand_u-boot.bin;",
											 simple_strtoul(getenv("uboot_offset"), NULL, 10),
											 simple_strtoul(getenv("uboot_partition_size"), NULL, 10));
							printf("cmd_buf = %s\n", cmd_buf);
							run_command(cmd_buf, 0);
							sprintf(size, "0x%s", getenv("filesize"));
							sprintf(cmd_buf, "nand write 0x82000000 %x %x", 
									simple_strtoul(getenv("uboot_offset"), NULL, 10),
									simple_strtoul(size, NULL, 10));
							printf("cmd_buf = %s\n", cmd_buf);
							run_command(cmd_buf, 0);	
//							strcpy(cmd_buf, "fatload mmc 0:1 0x82000000 u-boot.bin");
//							run_command(cmd_buf, 0);
							break;

						}
						case '3':
						{
							printf("flashing KERNEL\n");
							//add code to check if file existting
							sprintf(cmd_buf, "nandecc sw; \
											 nand erase %x %x; \
											 fatload mmc 0:1 0x82000000 rd_nand_uImage;",
											 simple_strtoul(getenv("kernel_offset"), NULL, 10),
											 simple_strtoul(getenv("kernel_partition_size"), NULL, 10));
							printf("cmd_buf = %s\n", cmd_buf);
							run_command(cmd_buf, 0);
							sprintf(size, "0x%s", getenv("filesize"));
							sprintf(cmd_buf, "nand write 0x82000000 %x %x", 
									simple_strtoul(getenv("kernel_offset"), NULL, 10),
									simple_strtoul(size, NULL, 10));
							
							printf("cmd_buf = %s\n", cmd_buf);
							run_command(cmd_buf, 0);	
							break;

						}
						case '4':
						{
							printf("flashing ROOTFS\n");
							//add code to check if file existting
							sprintf(cmd_buf, "nandecc hw; \
											 nand erase %x %x; \
											 fatload mmc 0:1 0x82000000 rd_nand_rootfs_cramfs;",
											 simple_strtoul(getenv("rootfs_offset"), NULL, 10),
											 simple_strtoul(getenv("rootfs_partition_size"), NULL, 10));
							printf("cmd_buf = %s\n", cmd_buf);
							run_command(cmd_buf, 0);
							sprintf(size, "0x%s", getenv("filesize"));
							sprintf(cmd_buf, "nand write 0x82000000 %x %x; \
											  nandecc sw", 
									simple_strtoul(getenv("rootfs_offset"), NULL, 10),
									simple_strtoul(size, NULL, 10));
							
							printf("cmd_buf = %s\n", cmd_buf);
							run_command(cmd_buf, 0);	
							break;

						}
						case '5':
						{
							printf("flashing USRFS\n");
							//add code to check if file existting
							sprintf(cmd_buf, "nandecc hw; \
											 nand erase %x %x; \
											 fatload mmc 0:1 0x82000000 rd_nand_usrfs_yaffs;",
											 simple_strtoul(getenv("usrfs_offset"), NULL, 10),
											 simple_strtoul(getenv("usrfs_partition_size"), NULL, 10));
							printf("cmd_buf = %s\n", cmd_buf);
							run_command(cmd_buf, 0);
							sprintf(size, "0x%s", getenv("filesize"));
							sprintf(cmd_buf, "nand write.yaffs 0x82000000 %x %x; \
											  nandecc sw", 
									simple_strtoul(getenv("usrfs_offset"), NULL, 10),
									simple_strtoul(size, NULL, 10));
							
							printf("cmd_buf = %s\n", cmd_buf);
							run_command(cmd_buf, 0);	
							break;

						}
						case '6':
						{
							printf("booting the kernel\n");
							strcpy(cmd_buf, "boot");
							run_command(cmd_buf, 0);
							break;
						}
						case '9':
						{
							printf("flashing ML0\n");
							//add code to check if file existting
							sprintf(cmd_buf, "nandecc hw; \
											 nand erase %x %x; \
											 fatload mmc 0:1 0x82000000 rd_nand_mlo;",
											 simple_strtoul(getenv("mlo_offset"), NULL, 10),
											 simple_strtoul(getenv("mlo_partition_size"), NULL, 10));
							run_command(cmd_buf, 0);
							sprintf(size, "0x%s", getenv("filesize"));
							sprintf(cmd_buf, "nand write 0x82000000 %x %x; \
											  nandecc sw", 
									simple_strtoul(getenv("mlo_offset"), NULL, 10),
									simple_strtoul(size, NULL, 10));
							run_command(cmd_buf, 0);	
							
							printf("flashing UBOOT\n");
							//add code to check if file existting
							sprintf(cmd_buf, "nandecc sw; \
											  nand erase %x %x; \
											 fatload mmc 0:1 0x82000000 rd_nand_u-boot.bin;",
											 simple_strtoul(getenv("uboot_offset"), NULL, 10),
											 simple_strtoul(getenv("uboot_partition_size"), NULL, 10));
							printf("cmd_buf = %s\n", cmd_buf);
							run_command(cmd_buf, 0);
							sprintf(size, "0x%s", getenv("filesize"));
							sprintf(cmd_buf, "nand write 0x82000000 %x %x", 
									simple_strtoul(getenv("uboot_offset"), NULL, 10),
									simple_strtoul(size, NULL, 10));
							printf("cmd_buf = %s\n", cmd_buf);
							run_command(cmd_buf, 0);
							
							printf("flashing KERNEL\n");
							//add code to check if file existting
							sprintf(cmd_buf, "nandecc sw; \
											 nand erase %x %x; \
											 fatload mmc 0:1 0x82000000 rd_nand_uImage;",
											 simple_strtoul(getenv("kernel_offset"), NULL, 10),
											 simple_strtoul(getenv("kernel_partition_size"), NULL, 10));
							printf("cmd_buf = %s\n", cmd_buf);
							run_command(cmd_buf, 0);
							sprintf(size, "0x%s", getenv("filesize"));
							sprintf(cmd_buf, "nand write 0x82000000 %x %x", 
									simple_strtoul(getenv("kernel_offset"), NULL, 10),
									simple_strtoul(size, NULL, 10));
							
							printf("cmd_buf = %s\n", cmd_buf);
							run_command(cmd_buf, 0);	
							
							printf("flashing ROOTFS\n");
							sprintf(cmd_buf, "nandecc hw; \
											 nand erase %x %x; \
											 fatload mmc 0:1 0x82000000 rd_nand_rootfs_cramfs;",
											 simple_strtoul(getenv("rootfs_offset"), NULL, 10),
											 simple_strtoul(getenv("rootfs_partition_size"), NULL, 10));
							printf("cmd_buf = %s\n", cmd_buf);
							run_command(cmd_buf, 0);
							sprintf(size, "0x%s", getenv("filesize"));
							sprintf(cmd_buf, "nand write 0x82000000 %x %x; \
											  nandecc sw", 
									simple_strtoul(getenv("rootfs_offset"), NULL, 10),
									simple_strtoul(size, NULL, 10));
							
							printf("cmd_buf = %s\n", cmd_buf);
							run_command(cmd_buf, 0);
							
							printf("flashing USRFS\n");
							//add code to check if file existting
							sprintf(cmd_buf, "nandecc hw; \
											 nand erase %x %x; \
											 fatload mmc 0:1 0x82000000 rd_nand_usrfs_yaffs;",
											 simple_strtoul(getenv("usrfs_offset"), NULL, 10),
											 simple_strtoul(getenv("usrfs_partition_size"), NULL, 10));
							printf("cmd_buf = %s\n", cmd_buf);
							run_command(cmd_buf, 0);
							sprintf(size, "0x%s", getenv("filesize"));
							sprintf(cmd_buf, "nand write.yaffs 0x82000000 %x %x; \
											  nandecc sw", 
									simple_strtoul(getenv("usrfs_offset"), NULL, 10),
									simple_strtoul(size, NULL, 10));
							
							printf("cmd_buf = %s\n", cmd_buf);
							run_command(cmd_buf, 0);	
							break;
						}
						case '0':
						{
							i--;
							break;
						}
					}
				}
				break;
			}
			case '2':
			{
				int i = 1;
				strcpy(cmd_buf, "run tftpargs; \
								 usb start;");
				run_command(cmd_buf, 0);
				while(i)
				{
					printf("[1]flash MLO from tftp\n");
					printf("[2]flash UBOOT from tftp\n");
					printf("[3]flash KERNEL from tftp\n");
					printf("[4]flash CRAMFS(ROOTFS) from tftp\n");
					printf("[5]flash YAFFS2(USERFS) from tftp\n");
					printf("[9]flash all radar stuff\n");
					printf("[0]exit this menu\n");
					c = awaitkey(-1, NULL);
					switch(c)
					{
						case '1':
							strcpy(cmd_buf, "echo Reading MLO from tftp...; \
											tftp MLO; \
											echo Flashing MLO to NAND;");
							run_command(cmd_buf, 0);
							break;
						case '2':
							
							strcpy(cmd_buf, "echo Reading UBOOT from tftp...; \
											tftp u-boot.bin; \
											echo Flashing UBOOT to NAND;");
							run_command(cmd_buf, 0);
							break;
						case '3':
							
							strcpy(cmd_buf, "echo Reading KERNEL from tftp...; \
											tftp uImage; \
											echo Flashing KERNEL to NAND;");
							run_command(cmd_buf, 0);
							break;
						case '4':
							strcpy(cmd_buf, "echo Reading ROOTFS from tftp...; \
											tftp rootfs.cramfs; \
											echo Flashing ROOTFS to NAND;");
							run_command(cmd_buf, 0);
							break;
						case '5':
							strcpy(cmd_buf, "echo Reading USRFS from tftp...; \
											tftp usrfs.yaffs; \
											echo Flashing USRFS to NAND;");
							run_command(cmd_buf, 0);
							break;
						case '0':
							i--;
							break;
					}
				}
				
				strcpy(cmd_buf, "dcache on;");
				run_command(cmd_buf, 0);
				break;
			}
			case '3':
                        {
                                printf("this selection has not completed\n");
                                break;
                        }
			case '4':
                        {
                                printf("this selection has not completed\n");
                                break;
                        }
			case '5':
                        {
                                printf("this selection has not completed\n");
                                break;
                        }
			case '6':
                        {
				strcpy(cmd_buf, "setenv ipaddr 192.168.1.253; setenv serverip 192.168.1.188; setenv usbethaddr ce:69:73:ab:d6:75; usb reset; tftp 0x80300000 uImage; bootm 0x80300000");
				run_command(cmd_buf, 0);
                                break;
                        }
			case '7':
                        {
                                printf("this selection has not completed\n");
                                break;
                        }
			case '8':
                        {
                                printf("this selection has not completed\n");
                                break;
                        }
			case '0':
                        {
                                return;
                                break;
                        }
			case '9':
			{
				strcpy(cmd_buf, "boot");
                run_command(cmd_buf, 0);
				break;
			}
			
		}
	}
}

void main_menu_usage(void)
{
	printf("[1] flash from mmc\n");
	printf("[2] flash from tftp\n");
	printf("[9] boot kernel\n");
	printf("[0] exit this menu\n");
	printf("Enter your selection: ");
}


U_BOOT_CMD(menu, 3, 0, do_menu,
        "menu - display a menu, to select the items to execute cmd",
        "-display a menu, to select the items to execute cmd\n"
	"add by haolong");
