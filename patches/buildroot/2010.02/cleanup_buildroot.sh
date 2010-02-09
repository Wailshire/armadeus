#!/bin/bash
#
# Things to do before patching Buildroot
#

# Removes packages/files added through patches:
rm -rf buildroot/package/games/quake-pak/
rm -rf buildroot/package/games/sdlquake/
rm -rf buildroot/package/gpm/
rm -rf buildroot/package/pygame/
rm -f  buildroot/package/proftpd/proftpd-1.3.2-fix-kernel-header-capability-version.patch
rm -rf buildroot/package/uucp/
rm -f  buildroot/package/multimedia/libsndfile/libsndfile-1.0.*-srconly.patch
rm -rf buildroot/package/multimedia/flite/
rm -rf buildroot/package/libcurl/
rm -f  buildroot/package/multimedia/mplayer/mplayer-1.0rc2-add_pld_asm_inst_for_non_armv5_arch.patch