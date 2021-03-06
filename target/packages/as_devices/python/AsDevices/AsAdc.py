#! /usr/bin/python
# -*- coding: utf-8 -*-
#-----------------------------------------------------------------------------
# Name:     AsAdc.py
# Purpose:
# Author:   Fabien Marteau <fabien.marteau@armadeus.com>
# Created:  04/02/2011
#-----------------------------------------------------------------------------
#  Copyright (2011)  Armadeus Systems
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
# created :  04/02/2011
#-----------------------------------------------------------------------------
# Revision list :
#
# Date       By        Changes
#
#-----------------------------------------------------------------------------

__doc__ = "This class drive ARMadeus boards ADCs"
__version__ = "0.1"
__author__ = "Fabien Marteau <fabien.marteau@armadeus.com>"

import wrappers.AsAdc_wrap as wrapper

class AsAdcError(Exception):
    def __init__(self, value):
        self.value = value
    def __str__(self):
        return repr(self.value)

class AsAdc:
    """ Drive adc
    """
    def __init__(self, aAdcType, aDeviceNum, aVRef):
        try:
            self.__device = wrapper.adc_open(aAdcType, aDeviceNum, aVRef)
        except Exception, e:
            raise AsAdcError("Can't open adc port type "+str(aAdcType)+\
                                " num "+str(aDeviceNum))

    def __del__(self):
        try:
            wrapper.adc_close(self.__device)
        except Exception, e:
            pass

    def getValueInMillivolts(self, aChannel):
        """ Get value in millivolts
        """
        try:
            return wrapper.getValueInMillivolts(self.__device, aChannel)
        except Exception, e:
            raise AsAdcError(str(e))

