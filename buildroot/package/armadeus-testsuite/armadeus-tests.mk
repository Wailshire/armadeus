#############################################################
#
# Armadeus Test Suite
#
#############################################################

ARMADEUS-TESTSUITE_VER:=1.0
ARMADEUS-TESTSUITE_SOURCE:=$(BUILD_DIR)/../../target/test/
ARMADEUS-TESTSUITE_DIR:=$(BUILD_DIR)/armadeus-testsuite-$(ARMADEUS-TESTSUITE_VER)
ARMADEUS-TESTSUITE_CAT:=zcat
ARMADEUS-TESTSUITE_BINARY:=test_env.sh
ARMADEUS-TESTSUITE_TARGET_DIR:=/usr/bin/test
ARMADEUS-TESTSUITE_TARGET_BINARY:=usr/bin/test/$(ARMADEUS-TESTSUITE_BINARY)

$(ARMADEUS-TESTSUITE_DIR)/.unpacked:
	mkdir -p $(ARMADEUS-TESTSUITE_DIR)/
	cp -r $(ARMADEUS-TESTSUITE_SOURCE)/* $(ARMADEUS-TESTSUITE_DIR)/
	touch $@

$(TARGET_DIR)/$(ARMADEUS-TESTSUITE_TARGET_BINARY): $(ARMADEUS-TESTSUITE_DIR)/.unpacked
	mkdir -p $(TARGET_DIR)/$(ARMADEUS-TESTSUITE_TARGET_DIR)
	install -D $(ARMADEUS-TESTSUITE_DIR)/*.sh $(TARGET_DIR)/$(ARMADEUS-TESTSUITE_TARGET_DIR)
	touch $@

armadeus-testsuite: $(TARGET_DIR)/$(ARMADEUS-TESTSUITE_TARGET_BINARY)

armadeus-testsuite-clean:
	rm -rf $(TARGET_DIR)/$(ARMADEUS-TESTSUITE_TARGET_DIR)

armadeus-testsuite-dirclean:
	rm -rf $(ARMADEUS-TESTSUITE_DIR)

#############################################################
#
# Toplevel Makefile options
#
#############################################################
ifeq ($(strip $(BR2_PACKAGE_ARMADEUS-TESTSUITE)),y)
TARGETS+=armadeus-testsuite
endif