#############################################################
#
# u-boot
#
#############################################################
U-BOOT_VER:=1.1.3
U-BOOT_SOURCE:=u-boot-$(U-BOOT_VER).tar.bz2
U-BOOT_SITE:=http://osdn.dl.sourceforge.net/sourceforge/u-boot/
U-BOOT_DIR:=$(BUILD_DIR)/u-boot-$(U-BOOT_VER)

U-BOOT_PACKAGE_DIR:=$(ARMADEUS_PATH)/u-boot
U-BOOT_TARGET_NAME:=apm9328

$(DL_DIR)/$(U-BOOT_SOURCE):
	 $(WGET) -P $(DL_DIR) $(U-BOOT_SITE)/$(U-BOOT_SOURCE)

u-boot-source: $(DL_DIR)/$(U-BOOT_SOURCE)

$(U-BOOT_DIR)/.unpacked: $(DL_DIR)/$(U-BOOT_SOURCE)
	bzcat $(DL_DIR)/$(U-BOOT_SOURCE) | tar -C $(BUILD_DIR) -xvf -
	toolchain/patch-kernel.sh $(U-BOOT_DIR) $(U-BOOT_PACKAGE_DIR) *.patch
	touch $(U-BOOT_DIR)/.unpacked	

$(U-BOOT_DIR)/.configured: $(U-BOOT_DIR)/.unpacked
	make -C $(U-BOOT_DIR) CROSS_COMPILE=$(TARGET_CROSS) $(U-BOOT_TARGET_NAME)_config
ifneq ($(BR2_TARGET_ARMADEUS_SDRAM_SIZE),)
	$(SED) "s,^#define CFG_SDRAM_MBYTE_SYZE.*,#define CFG_SDRAM_MBYTE_SYZE $(BR2_TARGET_ARMADEUS_SDRAM_SIZE),g;" $(U-BOOT_DIR)/include/configs/$(U-BOOT_TARGET_NAME).h
endif
	touch $(U-BOOT_DIR)/.configured	

$(U-BOOT_DIR)/u-boot.bin: $(U-BOOT_DIR)/.configured
	make -C $(U-BOOT_DIR) CROSS_COMPILE=$(TARGET_CROSS)
	touch -c $(U-BOOT_DIR)/u-boot.bin

u-boot: $(U-BOOT_DIR)/u-boot.bin $(U-BOOT_DIR)/u-boot.brec
	cp $(U-BOOT_DIR)/u-boot.bin .
	cp $(U-BOOT_DIR)/u-boot.brec .
	ln -fs $(U-BOOT_DIR)/tools/mkimage $(STAGING_DIR)/bin/mkimage

u-boot-clean:
	make -C $(U-BOOT_DIR) clean
	rm $(U-BOOT_DIR)/.configured

u-boot-dirclean:
	rm -rf $(U-BOOT_DIR)

#############################################################
#
# Toplevel Makefile options
#
#############################################################
ifeq ($(strip $(BR2_TARGET_U-BOOT)),y)
TARGETS+=u-boot
endif
