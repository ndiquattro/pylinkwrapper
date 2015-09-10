# -*- coding: CP1251 -*-
#
# Copyright (c) 1996-2005, SR Research Ltd., All Rights Reserved
#
#
# For use by SR Research licencees only. Redistribution and use in source
# and binary forms, with or without modification, are NOT permitted.
#
#
#
# Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in
# the documentation and/or other materials provided with the distribution.
#
# Neither name of SR Research Ltd nor the name of contributors may be used
# to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
# IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# $Date: 2009/11/02 16:14:37 $
# 
#


"""Performing research with eye-tracking equipment typically requires a long-term investment in software
tools to collect, process, and analyze data. Much of this involves real-time data collection, saccadic
analysis, calibration routines, and so on.
The EyeLink® eye-tracking system is designed to implement most of the required software base for data
collection and conversion. It is most powerful when used with the Ethernet link interface, which allows
remote control of data collection and real-time data transfer. The PyLink toolkit includes Pylink module,
which implements all core EyeLink functions and classes for EyeLink connection and the eyelink graphics,
such as the display of camera image, calibration, validation, and drift correct. The EyeLink graphics is
currently implemented using Simple Direct Media Layer (SDL: www.libsdl.org).


The Pylink library contains a set of classes and functions, which are used to program experiments on many
different platforms, such as MS-DOS, Windows, Linux, and the Macintosh. Some programming standards,
such as placement of messages in the EDF file by your experiment, and the use of special data types, have
been implemented to allow portability of the development kit across platforms. The standard messages
allow general analysis tools such as EDF2ASC converter or EyeLink Data Viewer to process your EDF files.


"""

from constants import * # import everything from constant


from pylink import inRealTimeMode
from pylink import flushGetkeyQueue
from pylink import beginRealTimeMode
from pylink import currentTime
from pylink import currentUsec
from pylink import currentDoubleUsec
from pylink import endRealTimeMode
from pylink import pumpDelay
from pylink import msecDelay
from pylink import alert
from pylink import enableExtendedRealtime

try:
	from pylink import setCalibrationColors
	from pylink import setTargetSize
	from pylink import setCalibrationSounds
	from pylink import setDriftCorrectSounds
	from pylink import setCameraPosition
	from pylink import getDisplayInformation
	from pylink import openGraphics
	from pylink import closeGraphics
	from pylink import resetBackground
	from pylink import disableCustomBackgroundOnImageMode
	from pylink import setCalibrationAnimationTarget
	from pylink import enableExternalCalibrationDevice
except:
	#means no_sdl defined
	pass
from pylink import openCustomGraphicsInternal
from pylink import bitmapSave
from pylink import sendMessageToFile
from pylink import openMessageFile
from pylink import closeMessageFile


from tracker import DisplayInfo
from tracker import EndBlinkEvent
from tracker import StartBlinkEvent
from tracker import StartNonBlinkEvent
from tracker import FixUpdateEvent
from tracker import StartFixationEvent
from tracker import EndFixationEvent
from tracker import StartSaccadeEvent
from tracker import EndSaccadeEvent
from tracker import EyeLinkAddress
from tracker import EyelinkMessage
from tracker import EyeLinkCustomDisplay
from tracker import KeyInput
from tracker import ILinkData
from tracker import IOEvent
from tracker import ButtonEvent
from tracker import MessageEvent
from tracker import Sample



from eyelink import EyeLinkListener
from eyelink import EyeLink
from eyelink import getEYELINK
from eyelink import openGraphicsEx



import version 
__version__ = "%d.%d.%d.%d"%(version.vernum)

