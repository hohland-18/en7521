RT28xx_MODE = AP

TARGET = LINUX

CHIPSET = 3390

#RT28xx_DIR = home directory of RT28xx source code
RT28xx_DIR = $(shell pwd)
RTMP_SRC_DIR = $(RT28xx_DIR)/RT$(CHIPSET)

#PLATFORM: Target platform
#PLATFORM = PPC405EXr
#PLATFORM = 5VT
#PLATFORM = PC
PLATFORM = TC3162U
#PLATFORM = IKANOS_V160
#PLATFORM = IKANOS_V180
#PLATFORM = SIGMA
#PLATFORM = SIGMA_8622
#PLATFORM = INIC
#PLATFORM = STAR
#PLATFORM = IXP
#PLATFORM = INF_TWINPASS
#PLATFORM = INF_DANUBE
#PLATFORM = INF_AR9
#PLATFORM = BRCM_6358
#PLATFORM = INF_AMAZON_SE
#PLATFORM = INF_AMAZON_SE_KERNEL_2_6
#PLATFORM = CAVM_OCTEON
#PLATFORM = CMPC
#PLATFORM = RALINK_2880
#PLATFORM = RALINK_3052
#PLATFORM = SMDK
#PLATFORM = RMI
#PLATFORM = RMI_64
#RELEASE Package
RELEASE = DPA

ifeq ($(PLATFORM),PPC405EXr)
ARCH := powerpc
export ARCH
#LINUX_SRC = /home/feng/My_WorkSpace/MIPS_CNF/lcf_002/src/custom/AMCC/linux-2.6.29-denx
LINUX_SRC = /home/Darren/Work/Customer/Foxconn/20090819_RT2890_BringUpFailed_Issue/PowerPC/Linux/linux-2.6.29-denx
CROSS_COMPILE = /opt/eldk/usr/bin/ppc_4xx-
endif

ifeq ($(PLATFORM),5VT)
LINUX_SRC = /project/stable/5vt/ralink-2860-sdk/linux-2.6.17
CROSS_COMPILE = /opt/crosstool/uClibc_v5te_le_gcc_4_1_1/bin/arm-linux-
endif

ifeq ($(PLATFORM),IKANOS_V160)
LINUX_SRC = /home/sample/projects/LX_2618_RG_5_3_00r4_SRC/linux-2.6.18
CROSS_COMPILE = mips-linux-
endif

ifeq ($(PLATFORM),IKANOS_V180)
LINUX_SRC = /home/sample/projects/LX_BSP_VX180_5_4_0r1_ALPHA_26DEC07/linux-2.6.18
CROSS_COMPILE = mips-linux-
endif

ifeq ($(PLATFORM),SIGMA)
LINUX_SRC = /root/sigma/smp86xx_kernel_source_2.7.172.0/linux-2.6.15
CROSS_COMPILE = /root/sigma/smp86xx_toolchain_2.7.172.0/build_mipsel_nofpu/staging_dir/bin/mipsel-linux-
endif

ifeq ($(PLATFORM),SIGMA_8622)
LINUX_SRC = /home/snowpin/armutils_2.5.120.1/build_arm/linux-2.4.22-em86xx
CROSS_COMPILE = /home/snowpin/armutils_2.5.120.1/toolchain/bin/arm-elf-
CROSS_COMPILE_INCLUDE = /home/snowpin/armutils_2.5.120.1/toolchain/lib/gcc-lib/arm-elf/2.95.3
endif

ifeq ($(PLATFORM),INIC)
UCOS_SRC = /opt/uCOS/iNIC_rt2880
CROSS_COMPILE = /usr/bin/mipsel-linux-
endif

ifeq ($(PLATFORM),STAR)
LINUX_SRC = /opt/star/kernel/linux-2.4.27-star
CROSS_COMPILE = /opt/star/tools/arm-linux/bin/arm-linux-
endif

ifeq ($(PLATFORM),RMI)
LINUX_SRC = /opt/rmi/1.7.0/linux/src/
CROSS_COMPILE = /opt/rmi/1.7.0/mipscross/nptl/bin/mips64-unknown-linux-gnu-
endif

ifeq ($(PLATFORM),RMI_64)
LINUX_SRC = /opt/rmi/1.7.0/linux_64/src/
CROSS_COMPILE = /opt/rmi/1.7.0/mipscross/nptl/bin/mips64-unknown-linux-gnu-
endif

ifeq ($(PLATFORM), RALINK_2880)
LINUX_SRC = /project/stable/RT288x/RT288x_SDK/source/linux-2.4.x
CROSS_COMPILE = /opt/buildroot-gdb/bin/mipsel-linux-
endif

ifeq ($(PLATFORM),RALINK_3052)
LINUX_SRC = /home/peter/ap_soc/SDK_3_3_0_0/RT288x_SDK/source/linux-2.6.21.x
CROSS_COMPILE = /opt/buildroot-gcc342/bin/mipsel-linux-uclibc-
endif

ifeq ($(PLATFORM),PC)
# Linux 2.6
LINUX_SRC = /lib/modules/$(shell uname -r)/build
# Linux 2.4 Change to your local setting
#LINUX_SRC = /usr/src/linux-2.4
LINUX_SRC_MODULE = /lib/modules/$(shell uname -r)/kernel/drivers/net/wireless/
CROSS_COMPILE = 
endif

ifeq ($(PLATFORM),IXP)
LINUX_SRC = /project/stable/Gmtek/snapgear-uclibc/linux-2.6.x
CROSS_COMPILE = arm-linux-
endif

ifeq ($(PLATFORM),INF_TWINPASS)
# Linux 2.6
#LINUX_SRC = /lib/modules/$(shell uname -r)/build
# Linux 2.4 Change to your local setting
LINUX_SRC = /project/stable/twinpass/release/2.0.1/source/kernel/opensource/linux-2.4.31/
CROSS_COMPILE = mips-linux-
endif

ifeq ($(PLATFORM),INF_DANUBE)
LINUX_SRC = /opt/danube/sdk/linux-2.6.16.x
CROSS_COMPILE = mips-linux-
ROOTDIR = /opt/danube/sdk
export ROOTDIR
endif

ifeq ($(PLATFORM),INF_AR9)
LINUX_SRC = /root/ar9/xR9_BSP1.2.2.0/source/kernel/opensource/linux-2.6.20/
CROSS_COMPILE = /root/ar9/ifx-lxdb26-1.0.2/gcc-3.4.4/toolchain-mips/bin/
endif

ifeq ($(PLATFORM),BRCM_6358)
LINUX_SRC = 
CROSS_COMPILE = 
endif

ifeq ($(PLATFORM),INF_AMAZON_SE)
# Linux 2.6
#LINUX_SRC = /lib/modules/$(shell uname -r)/build
# Linux 2.4 Change to your local setting
LINUX_SRC = /backup/ifx/3.6.2.2/source/kernel/opensource/linux-2.4.31
#CROSS_COMPILE = mips-linux-
#LINUX_SRC = /project/Infineon/3.6.2.2/source/kernel/opensource/linux-2.4.31
CROSS_COMPILE = /opt/uclibc-toolchain/ifx-lxdb-1-2-3-external/gcc-3.3.6/toolchain-mips/R0208V35/mips-linux-uclibc/bin/
endif

ifeq ($(PLATFORM),INF_AMAZON_SE_KERNEL_2_6)
# Linux 2.6
#LINUX_SRC = /lib/modules/$(shell uname -r)/build
# Linux 2.4 Change to your local setting
LINUX_SRC = /root/ASEBSP1.1.0.1/source/kernel/opensource/linux-2.6.20
CROSS_COMPILE = /opt/uclibc_toolchain/ifx-lxdb26-1.0.2/gcc-3.4.4/toolchain-mips/bin/
endif

ifeq ($(PLATFORM),ST)
LINUX_SRC = /opt/STM/STLinux-2.2/devkit/sources/kernel/linux0039
CROSS_COMPILE = /opt/STM/STLinux-2.2/devkit/sh4/bin/sh4-linux-
ARCH := sh
export ARCH
endif

ifeq ($(PLATFORM),CAVM_OCTEON)
OCTEON_ROOT = /usr/local/Cavium_Networks/OCTEON-SDK
LINUX_SRC = $(OCTEON_ROOT)/linux/kernel_2.6/linux
CROSS_COMPILE = mips64-octeon-linux-gnu-
endif


ifeq ($(PLATFORM),WINBOND)
LINUX_SRC =/root/W90N745-uClinux/uClinux-dist/linux-2.4.x
CROSS_COMPILE = arm-elf-
CROSS_COMPILE_INCLUDE=/usr/local/arm_tools/lib/gcc-lib/arm-elf/3.0
endif

ifeq ($(PLATFORM),CMPC)
LINUX_SRC = /opt/fvt_11N_SDK_0807/fvt131x_SDK_11n/linux-2.6.17
CROSS_COMPILE =
endif

ifeq ($(PLATFORM),SMDK)
LINUX_SRC = /home/bhushan/itcenter/may28/linux-2.6-samsung
CROSS_COMPILE = /usr/local/arm/4.2.2-eabi/usr/bin/arm-linux-
endif

ifeq ($(PLATFORM),TC3162U)
LINUX_SRC = $(KERNEL_DIR)
#CROSS_COMPILE = mips-linux-
endif

export RT28xx_DIR RT28xx_MODE LINUX_SRC CROSS_COMPILE CROSS_COMPILE_INCLUDE PLATFORM RELEASE CHIPSET RTMP_SRC_DIR LINUX_SRC_MODULE TARGET

all: build_tools $(TARGET)


build_tools:
	make -C tools
	$(RT28xx_DIR)/tools/bin2h

test:
	make -C tools test

UCOS:
	make -C os/ucos/ MODE=$(RT28xx_MODE)
	echo $(RT28xx_MODE)


LINUX:
ifneq (,$(findstring 2.4,$(LINUX_SRC)))
	cp -f os/linux/Makefile.4 $(RT28xx_DIR)/os/linux/Makefile
	make -C $(RT28xx_DIR)/os/linux/
ifeq ($(RT28xx_MODE),AP)
	cp -f $(RT28xx_DIR)/os/linux/rt$(CHIPSET)ap.o /tftpboot
ifeq ($(PLATFORM),INF_AMAZON_SE)
	cp -f /tftpboot/rt2870ap.o /backup/ifx/build/root_filesystem/lib/modules/2.4.31-Amazon_SE-3.6.2.2-R0416_Ralink/kernel/drivers/net
endif
ifeq ($(PLATFORM),INF_AMAZON_SE_KERNEL_2_6)
	cp -f /tftpboot/rt3370ap.ko /root/ASEBSP1.1.0.1/build/root_filesystem/lib/modules
endif
else	
ifeq ($(RT28xx_MODE),APSTA)
	cp -f $(RT28xx_DIR)/os/linux/rt$(CHIPSET)apsta.o /tftpboot
else
	cp -f $(RT28xx_DIR)/os/linux/rt$(CHIPSET)sta.o /tftpboot
endif	
endif	
else
	cp -f os/linux/Makefile.6 $(RT28xx_DIR)/os/linux/Makefile
	make  -C  $(LINUX_SRC) SUBDIRS=$(RT28xx_DIR)/os/linux modules
ifeq ($(RT28xx_MODE),AP)
#	cp -f $(RT28xx_DIR)/os/linux/rt$(CHIPSET)ap.ko /tftpboot
	$(CROSS_COMPILE)strip --strip-unneeded $(RT28xx_DIR)/os/linux/rt$(CHIPSET)ap.ko
ifeq ($(PLATFORM),INF_AMAZON_SE_KERNEL_2_6)
	cp -f /tftpboot/rt3370ap.ko /root/ASEBSP1.1.0.1/build/root_filesystem/lib/modules
endif

else	
ifeq ($(RT28xx_MODE),APSTA)
	cp -f $(RT28xx_DIR)/os/linux/rt$(CHIPSET)apsta.ko /tftpboot
else
	cp -f $(RT28xx_DIR)/os/linux/rt$(CHIPSET)sta.ko /tftpboot
endif	
endif
endif

release:
ifeq ($(TARGET), LINUX)
	make -C $(RT28xx_DIR)/striptool -f Makefile.release clean
	make -C $(RT28xx_DIR)/striptool -f Makefile.release
	striptool/striptool.out
endif

prerelease:
ifeq ($(CHIPSET), 2880)
	make -C $(RT28xx_DIR)/os/linux -f Makefile.release.2880 prerelease
else
	make -C $(RT28xx_DIR)/os/linux -f Makefile.release prerelease
endif
	cp $(RT28xx_DIR)/os/linux/Makefile.DPB $(RTMP_SRC_DIR)/os/linux/.
	cp $(RT28xx_DIR)/os/linux/Makefile.DPA $(RTMP_SRC_DIR)/os/linux/.
	cp $(RT28xx_DIR)/os/linux/Makefile.DPC $(RTMP_SRC_DIR)/os/linux/.
ifeq ($(RT28xx_MODE),STA)
	cp $(RT28xx_DIR)/os/linux/Makefile.DPD $(RTMP_SRC_DIR)/os/linux/.
	cp $(RT28xx_DIR)/os/linux/Makefile.DPO $(RTMP_SRC_DIR)/os/linux/.
endif	

clean:
ifeq ($(TARGET), LINUX)
ifneq (,$(findstring 2.4,$(LINUX_SRC)))
	cp -f os/linux/Makefile.4 os/linux/Makefile
else
	cp -f os/linux/Makefile.6 os/linux/Makefile
endif
	make -C os/linux clean
	rm -rf os/linux/Makefile
endif	
ifeq ($(TARGET), UCOS)
	make -C os/ucos clean MODE=$(RT28xx_MODE)
endif

uninstall:
ifeq ($(TARGET), LINUX)
ifneq (,$(findstring 2.4,$(LINUX_SRC)))
	make -C $(RT28xx_DIR)/os/linux -f Makefile.4 uninstall
else
	make -C $(RT28xx_DIR)/os/linux -f Makefile.6 uninstall
endif
endif

install:
ifeq ($(TARGET), LINUX)
ifneq (,$(findstring 2.4,$(LINUX_SRC)))
	make -C $(RT28xx_DIR)/os/linux -f Makefile.4 install
else
	make -C $(RT28xx_DIR)/os/linux -f Makefile.6 install
endif
endif

libwapi:
ifneq (,$(findstring 2.4,$(LINUX_SRC)))
	cp -f os/linux/Makefile.libwapi.4 $(RT28xx_DIR)/os/linux/Makefile
	make -C $(RT28xx_DIR)/os/linux/
else
	cp -f os/linux/Makefile.libwapi.6 $(RT28xx_DIR)/os/linux/Makefile	
	make -C  $(LINUX_SRC) SUBDIRS=$(RT28xx_DIR)/os/linux modules	
endif	
	
