#############################################################
#
# Armadeus Test Suite
#
#############################################################

ARMADEUS-TESTSUITE_VER:=1.1
ARMADEUS-TESTSUITE_SOURCE:=$(TOPDIR)/../target/test/
ARMADEUS-TESTSUITE_DIR:=$(BUILD_DIR)/armadeus-testsuite-$(ARMADEUS-TESTSUITE_VER)
ARMADEUS-TESTSUITE_CAT:=zcat
ARMADEUS-TESTSUITE_BINARY:=test_env.sh
ARMADEUS-TESTSUITE_TARGET_DIR:=$(TARGET_DIR)/usr/bin/testsuite
ARMADEUS-TESTSUITE_TARGET_BINARY:=$(ARMADEUS-TESTSUITE_TARGET_DIR)/$(ARMADEUS-TESTSUITE_BINARY)
ARMADEUS-TESTSUITE_WISHBONE_BUTTON_DIR:=$(TOPDIR)/../target/linux/modules/fpga/wishbone_example/wb_button/

$(ARMADEUS-TESTSUITE_DIR)/.unpacked:
	mkdir -p $(ARMADEUS-TESTSUITE_DIR)/
	cd $(ARMADEUS-TESTSUITE_SOURCE); tar ch --exclude=.svn --exclude=.git . | tar x -C $(ARMADEUS-TESTSUITE_DIR)/
	touch $@

$(ARMADEUS-TESTSUITE_TARGET_BINARY): $(ARMADEUS-TESTSUITE_DIR)/.unpacked
	$(ARMADEUS-TESTSUITE_SOURCE)/install.sh $(ARMADEUS-TESTSUITE_TARGET_DIR)
	make -C $(ARMADEUS-TESTSUITE_WISHBONE_BUTTON_DIR) test
	install -D $(ARMADEUS-TESTSUITE_WISHBONE_BUTTON_DIR)/testbutton $(ARMADEUS-TESTSUITE_TARGET_DIR)
	touch $@

armadeus-testsuite: $(ARMADEUS-TESTSUITE_TARGET_BINARY)

armadeus-testsuite-clean:
	rm -rf $(ARMADEUS-TESTSUITE_TARGET_DIR)

armadeus-testsuite-dirclean:
	rm -rf $(ARMADEUS-TESTSUITE_DIR)

#############################################################
#
# Toplevel Makefile options
#
#############################################################
ifeq ($(strip $(BR2_PACKAGE_ARMADEUS_TESTSUITE)),y)
TARGETS+=armadeus-testsuite
endif
