##
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
# $Date: 2007/08/29 18:48:21 $
# 
#



import math
import constants


## The EyeLinkAddress class is used to hold addresses to EyeLink nodes. An instance of EyeLinkAddress class
#  can be initialized with the class constructor:
#   	
#	\code
# 	EyeLinkAddress(ip = (100,1,1,1), port = 4000)
#	\endcode
#
#  where <i>ip</i> is a four-item tuple containing the IP address of the EyeLink node and <i>port</i> is the port number of
#  the connection.
#
#  For example,
#	\code
#	myAddress = EyeLinkAddress((100, 1, 1, 1), 4000)
#	\endcode
class EyeLinkAddress:
	## Constructor
	# @param ip optional ipaddress in tuple form. eg. if the ip address is 192.168.25.48, the tuple form is (192,168,25,48)
	# The default ip address is 100.1.1.1
	# @param port optional port value as integer. The default value is 4000.
	def __init__(self, ip = (100,1,1,1), port = 4000):
		self.ip = ip
		self.port = port
	
	## Returns the IP address of the EyeLink node.
	#  @return
	#	A four-item tuple (integer) containing the IP address of the EyeLink node.
	def getIP(self):
		return self.ip

	## Returns the port number of the EyeLink node.
	#  @return
	#	An integer for the port number of the connection.
	def getPort(self):
		return self.port
		
	def __str__(self):
		return "%d.%d.%d.%d:%d"%(self.ip[0],self.ip[1],self.ip[2],self.ip[3],self.port)


## EyelinkMessage class, derived from EyeLinkAddress class, is used to send and receive messages between
#  EyeLink nodes. Instances of this class are commonly used as the return values of the \c nodeReceive() and
#  \c getNode() methods of the EyeLinkListener or EyeLink class. An instance of EyelinkMessage class can be
#  initialized with the class constructor:
#
#  	\code
#	EyelinkMessage(ip = (100,1,1,1), port = 4000, msg = "")
#  	\endcode
#
#  where <i>ip</i> is a four-item tuple containing the IP address of the EyeLink node, <i>port</i> is the port number of the
#  connection, <i>msg</i> is the message to be sent to or received from the node.
#  
#  For example,
#	\code
#	myMessage = EyelinkMessage((100, 1, 1, 1), 4000, "test")
#	\endcode
class EyelinkMessage(EyeLinkAddress):
	## Constructor
	# @param ip optional ipaddress in tuple form. eg. if the ip address is 192.168.25.48, the tuple form is (192,168,25,48)
	# The default ip address is 100.1.1.1
	# @param port optional port value as integer. The default value is 4000.
	# @param msg  text message.
	def __init__(self,  ip = (100,1,1,1), port = 4000, msg = ""):
		EyeLinkAddress.__init__(self,ip,port)
		self.msg = msg

	def __str__(self):
		return "%s -- %s"%(EyeLinkAddress.__str__(self), self.msg)

	## Returns the message to be sent to or received from the node.
	#  @return
	#	Text message.
	def getText():
		return self.msg
		

## Sample data for left and right eye.
#
#  The \c getRightEye() or \c getLeftEye() functions returns an instance of SampleData class, which contains the current 
#  sample position (raw, HREF, or gaze) and pupil size information of the desired eye.  The following methods can 
#  be used to retrieve the attributes of an instance of the SampleData class.  
#
#  For example, the x gaze position of the left eye for a given sample can be retrieved as:
#
#	\code
#	newSample = EYELINK.getFloatData()
#	gaze = newSample. getLeftEye().getGaze()
#	left_eye_gaze_x = gaze[0]
#	\endcode
#
#  If certain property information not sent for this sample, the value \c MISSING_DATA (or \c 0, depending on the field) 
#  will be returned, and the corresponding bit in the flags field will be zero (see eye_data.h for a list of bits).  
#  Data may be missing because of the tracker configuration (set by commands sent at the start of the experiment, 
#  from the Set Options screen of the %EyeLink II tracker, or from the default configuration set by the DATA.INI file 
#  for the %EyeLink I tracker).  Eye position data may also be set to \c MISSING_VALUE during a blink.	
class SampleData:
	def __init__(self,px, py,hx, hy,pa,gx, gy):
		self.__pupil__ =(px, py) 
		self.__headref__ = (hx, hy) 
		self.__area__ = pa
		self.__gaze__ = (gx,gy)

	## Display gaze position (in pixel coordinates set by the
	#  \c screen_pixel_coords command). The first and second
	#  item of the tuple store the x- and y- coordinate gaze
	#  position respectively.
	#  @return
	#	Two-item tuple in the format of (float, float).
	def getGaze(self):
		return self.__gaze__

	## HREF angular coordinates. The first and second items of the
	#  tuple are for the x and y coordinates, respectively.
	#  @return
	#	Two-item tuple in the format of (float, float).
	def getHREF(self):
		return self.__headref__
		
	## Camera x, y of pupil center. The first and second items of
	#  the tuple store pupil center in the x- and y- coordinate
	#  respectively.		
	#  @return
	#	Two-item tuple in the format of (float, float).
	def getRawPupil(self):
		return self.__pupil__

	## Pupil size (in arbitrary units, area or diameter as selected).
	#  @return
	#	Float.
	def getPupilSize(self):
		return self.__area__
			

	def __str__(self):
		return " %d %d %d %d %d %d %d "%(self.__pupil__[0],self.__pupil__[1], self.__headref__[0],self.__headref__[1], self.__area__, self.__gaze__[0],self.__gaze__[1])
	

## The EyeLink toolkit library defines special data classes that allow the same programming calls to be used on 
#  different platforms such as MS-DOS, Windows, and Macintosh.  You will need to know these classes to read the 
#  examples and to write your own experiments.  In this documentation, the common data classes are: Sample class, 
#  Eye Event Classes, MessageEvent Class, and IOEvent Class.  You only need to read this section if you are 
#  planning to use real-time link data for gaze-contingent displays or gaze-controlled interfaces, or to use data 
#  playback. 
#
#  The %EyeLink tracker measures eye position 250 or 500 times per second depending on the tracking mode you are 
#  working with, and computes true gaze position on the display using the head camera data.  This data is stored 
#  in the EDF file, and made available through the link in as little as 3 milliseconds after a physical eye movement.
#
#  Samples can be read from the link by \c getFloatData() or \c getNewestSample() method of the EyeLink/EyeLinkLisenter 
#  class.  These functions can return instances of Sample class.  For example, 
#
#	\code
#	newSample = EYELINK.getFloatData()
#	\endcode
#
#  The following methods can be used to retrieve properties of a Sample class instance.  For example, the time of 
#  the sample can be retrieved as \c newSample.getTime().  Please note that all methods for the Sample class do not 
#  take a parameter whereas the return values are noted.
class Sample:
	def __init__(self, time, type, flags, px, py, hx, hy, pa, gx, gy, rx, ry, status, input, buttons, htype, hdata):
		self.__time__ = time
		self.__type__ = type
		self.__flags__ = flags
		if(flags & 0x8000):
			self.__leftData__ = SampleData(px[0], py[0],hx[0], hy[0],pa[0],gx[0], gy[0])
		else:
			self.__leftData__ = None
		if(flags & 0x4000):
			self.__rightData__= SampleData(px[1], py[1],hx[1], hy[1],pa[1],gx[1], gy[1])
		else:
			self.__rightData__ = None
		self.__ppd__ =  (rx, ry)
		self.__status__ = status  
		self.__input__ = input  
		self.__buttons__ = buttons   
		self.__htype__ = htype 
	 	self.__hdata__ = hdata

	## Convenient method to clone a sample
	#
	def initFromSample(self,sample):
		self.__time__     = sample.__time__      
		self.__type__     = sample.__type__      
		self.__flags__    = sample.__flags__    
		self.__leftData__ = sample.__leftData__ 
		self.__rightData__= sample.__rightData__ 
		self.__ppd__      = sample.__ppd__      
		self.__status__   = sample.__status__   
		self.__input__    = sample.__input__    
		self.__buttons__  = sample.__buttons__  
		self.__htype__    = sample.__htype__    
	 	self.__hdata__    = sample.__hdata__   
	
	## \c 1 if the sample contains the left eye data; \c 0 if not.
	#  @return
	#   	Integer.
	def isLeftSample(self):
		return (self.__leftData__ != None)

	## \c 1 if the sample contains the right eye data; \c 0 if not.
	#  @return
	#	Integer.
	def isRightSample(self):
	 	return (self.__rightData__ != None)
	
	## \c 1 if the sample contains data from both eyes; \c 0 if not.
	#  @return
	#	Integer.
	def isBinocular(self):
	 	return (self.__leftData__ != None and self.__rightData__ != None)

	## Timestamp when camera imaged eye (in milliseconds since %EyeLink tracker was activated).
	#  @return
	#  	Long integer.
	def getTime(self): 
		return self.__time__
	
	## Always \c SAMPLE_TYPE.
	#  @return
	#  	Integer.
	def getType(self):
		return self.__type__
		
	## Angular resolution at current gaze position in screen pixels
	#  per visual degree. The first item of the tuple stores the x-coordinate
	#  resolution and the second item of the tuple
	#  stores the y-coordinate resolution.
	#  @return
	#	Two-item tuple in the format of (float, float).
	def getPPD(self):
		return self.__ppd__
		
	## Error and status flags (only useful for %EyeLink II, report CR
	#  status and tracking error). See eye_data.h for useful bits.
	#  @return
	#	Integer.
	def getStatus(self):
		return self.__status__	

	## Data from input port(s).
	#  @return
	#  	Integer.
	def getInput(self):
		return self.__input__
	
	## Bits indicating what types of data are present, and for which eye(s). See eye_data.h for useful bits.
	#  @return
	# 	Integer.
	def getFlags(self):
		return self.__flags__

	## Button input data: high 8 bits indicate changes from last
	#  sample, low 8 bits indicate current state of buttons 8 (MSB) to 1 (LSB).
	#  @return
	# 	Integer.
	def getButtons(self):
		return self.__buttons__	

	## Returns the sample data information from the desired eye.
	#  @return
	#	Instance of sample data class.
	def getRightEye(self):
		return self.__rightData__

	## Returns the sample data information from the desired eye.
	#  @return
	#	Instance of sample data class.
	def getLeftEye(self):
		return self.__leftData__

	## Returns the href data
	#
	def getHData(self):
		return self.__hdata__

	## Returns Eye data status.
	#  @return
	#  \arg 2 if both left and right eye data are present
	#  \arg 1 if right eye data present
	#  \arg 0 if left eye data present
	def getEye(self): 		
		if (self.__leftData__ is not None and self.__rightData__ is not None):
			return 2
		elif self.__leftData__ is not None :
			return 0
		else:
			return 1

	## Target Distance
	#  @return
	#	Float.	
	def getTargetDistance(self):
		return self.__hdata__[2]	

	## Target X
	#  @return
	#	Float.		
	def getTargetX(self):
		return self.__hdata__[0]

	## Target Y
	#  @return
	#	Float.		
	def getTargetY(self):
		return self.__hdata__[1]

	## Target Flags
	#  @return
	#	Float.		
	def getTargetFlags(self):
		return self.__hdata__[3]
			
	def __str__(self):
	 	rv= "Sample: %i %d %d %i %i %i %i "%(self.__time__, self.__ppd__[0],self.__ppd__[1], self.__status__, self.__input__, self.__buttons__, self.__htype__)
	 	rv += self.__leftData__.__str__()
	 	rv += self.__rightData__.__str__()
	 	return rv

		 	 
def newSample(time, flags, px, py, hx, hy, pa, gx, gy, rx, ry, status, input, buttons, htype, hdata):
	return Sample(time,constants.SAMPLE_TYPE, flags, px, py, hx, hy, pa, gx, gy, rx, ry, status, input, buttons, htype, hdata)
	 	 
		 	 
## The %EyeLink tracker simplifies data analysis by detecting important changes in the sample data and 
#  placing corresponding events into the data stream.  These include eye-data events (blinks, saccades, 
#  and fixations), button events, input-port events, and messages.  Several classes have been created to 
#  holds eye event data (start/end of fixation, start/end of saccade, start/end of blink, fixation update) 
#  information.  Start events contain only the start time, and optionally the start eye or gaze position.  
#  End events contain the start and end time, plus summary data on saccades and fixations.
#
#  It is important to remember that data sent over the link does not arrive in strict time sequence.  
#  Typically, eye events (such as \c STARTSACC and \c ENDFIX) arrive up to 32 milliseconds after the corresponding 
#  samples, and messages and buttons may arrive before a sample with the same time code.  This differs from 
#  the order seen in an ASC file, where the events and samples have been sorted into a consistent order by 
#  their timestamps.
#
#  The \c LOST_DATA_EVENT is a new event, introduced for %EyeLink tracker version 2.1 and later, and produced 
#  within the DLL to mark the location of lost data.   It is possible that data may be lost, either during 
#  recording with real-time data enabled, or during playback.  This might happen because of a lost link packet 
#  or because data was not read fast enough (data is stored in a large queue that can hold 2 to 10 seconds 
#  of data, and once it is full the oldest data is discarded to make room for new data).  This event has no 
#  data or time associated with it.
#
#  Event data returned by the \c getFloatData() method the EyeLink class.
#
#  For example,
#
#	\code
#	newEvent = EYELINK.getFloatData()
#	\endcode
#
#  Right now, the developer kit implements the following eye events:
#
#  <table>
#  <tr><td class="indexkey">Constant Name</td><td class="indexkey" align="center">Value</td><td class="indexkey">Description</td></tr>
#  <tr><td>STARTBLINK</td><td align="center">3</td><td>Pupil disappeared (time only)</td></tr>
#  <tr><td>ENDBLINK</td><td align="center">4</td><td>Pupil reappeared (duration data)</td></tr>
#  <tr><td>STARTSACC</td><td align="center">5</td><td>Start of saccade (with time only)</td></tr>
#  <tr><td>ENDSACC</td><td align="center">6</td><td>End of saccade (with summary data)</td></tr>
#  <tr><td>STARTFIX</td><td align="center">7</td><td>Start of fixation (with time only)</td></tr>
#  <tr><td>ENDFIX</td><td align="center">8</td><td>End of fixation (with summary data)</td></tr>
#  <tr><td>FIXUPDATE</td><td align="center">9</td><td>Update within fixation (summary data for interval)</td></tr>
#  <tr><td>MESSAGEEVENT</td><td align="center">24</td><td>User-definable text (IMESSAGE structure)</td></tr>
#  <tr><td>BUTTONEVENT</td><td align="center">25</td><td>Button state change (IOEVENT structure)</td></tr>
#  <tr><td>INPUTEVENT</td><td align="center">28</td><td>Change of input port (IOEVENT structure)</td></tr>
#  <tr><td>SAMPLE_TYPE</td><td align="center">200</td><td>Event flags gap in data stream</td></tr>
#  </table>
#
#  Please note that due to the tracker configuration, some of the property information returned may be a missing value 
#  \c MISSING_DATA (or \c 0, depending on the field).  So make sure you check for the validity of the data before 
#  trying to use them.   To do the tracker configuration, the user can use the \c setLinkEventFilter() and 
#  \c setLinkEventData() methods of the EyeLink class to send commands at the start of the experiment or modify 
#  the DATA.INI file on the tracker PC.
#
#  When both eyes are being tracked, left and right eye events are produced.  The eye from which data 
#  was produced can be retrieved by the \c getEye() method. 
class EyeEvent:
	def __init__(self, time, type, eye, read, sttime):
		self.__time__ = time
		self.__type__ = type
		self.__eye__ = eye
		self.__read__ = read
		self.__sttime__ = sttime
		self.__status__ = 0

	## Timestamp of the sample causing event (in milliseconds since %EyeLink tracker was activated).
	#  @return
	#	Long integer.
	def getTime(self):
		return self.__time__

	## The event code.
	#  @return
	#	Integer.
	def getType(self):
		return self.__type__

	## Which eye produced the event: \c 0 (\c LEFT_EYE) or \c 1 (\c RIGHT_EYE).
	#  @return
	#	Integer.
	def getEye(self):
		return self.__eye__

	## Bits indicating which data fields contain valid data (see
	#  eye_data.h file for details of the bits information).
	#  @return
	#	Integer.
	def getRead(self):
		return self.__read__

	## Timestamp of the first sample of the event.
	#  @return
	# 	Long integer.
	def getStartTime(self):
		return self.__sttime__

	## Errors and warnings of the event.
	#	
	def getStatus(self):
		return self.__status__
		
		
## Class to represent Start Blink event.
#  There are no direct properties for this interface.  All properties are inherited from EyeEvent.
class StartBlinkEvent(EyeEvent):
	def __init__(self, time, type, eye, read, sttime):
		EyeEvent.__init__(self, time, type, eye, read, sttime)		


def newStartBlinkEvent(time, type, eye, read, sttime):
	return StartBlinkEvent(time, type, eye, read, sttime)
		

## This interface is never used as is. FixUpdateEvent, StartFixationEvent and 
#  StartSaccadeEvent types inherit this.
#  This also inherits all properties from EyeEvent.
class StartNonBlinkEvent(EyeEvent):
	def __init__(self, time, type, eye, read, sttime, gstx, gsty, hstx, hsty, svel, supd_x, supd_y):
		EyeEvent.__init__(self, time, type, eye, read, sttime)
		self.__startGaze__ = (gstx, gsty)
		self.__startHref__ = (hstx, hsty)
		self.__startVelocity__ = svel
		self.__startUpd__  = (supd_x, supd_y)
		
	## Gaze position at the start of the event (in pixel coordinates
	#  set by the \c screen_pixel_coords command). The first and
	#  second items of the tuple store the x- and y- gaze position
	#  respectively.
	#  @return
	#	Two-item tuple in the format of (float, float).
	def getStartGaze(self):
		return self.__startGaze__
	
	## HEADREF position at the start of the event. The first and
	#  second items of the tuple store the x- and y- HREF data
	#  respectively.
	#  @return
	#	Two-item tuple in the format of (float, float).
	def getStartHREF(self):
		return self.__startHref__
		
	## Gaze velocity at the start of the event (in visual degrees per second).
	#  @return
	#	Float.
	def getStartVelocity(self):
		return self.__startVelocity__
		
	## Angular resolution at the start of the event (in screen pixels
	#  per visual degree, PPD). The first item of the tuple stores
	#  the x-coordinate PPD resolution and the second item of the
	#  tuple stores the y-coordinate PPD resolution.
	#  @return
	#	Two-item tuple in the format of (float, float).
	def getStartPPD(self):
		return self.__startUpd__


## Class to represent Start Saccade event.
#  There are no direct properties for this interface.  All properties are inherited from StartNonBlinkEvent.	
class StartSaccadeEvent(StartNonBlinkEvent):
	def __init__(self, time, type, eye, read, sttime, gstx, gsty, hstx, hsty, svel, supd_x, supd_y):
		StartNonBlinkEvent.__init__(self, time, type, eye, read, sttime, gstx, gsty, hstx, hsty, svel, supd_x, supd_y)


def newStartSaccadeEvent(time, type, eye, read, sttime, gstx, gsty, hstx, hsty, svel, supd_x, supd_y):
	return StartSaccadeEvent(time, type, eye, read, sttime, gstx, gsty, hstx, hsty, svel, supd_x, supd_y)
	

## Class to represent Start Fixation event.
#  This also inherits all properties from StartNonBlinkEvent.
class StartFixationEvent(StartNonBlinkEvent):
	def __init__(self, time, type, eye, read, sttime, gstx, gsty, hstx, hsty, sta, svel, supd_x, supd_y):
		StartNonBlinkEvent.__init__(self, time, type, eye, read, sttime, gstx, gsty, hstx, hsty, svel, supd_x, supd_y)
		self.__sta__  = sta
		
	## Pupil size (in arbitrary units, area or diameter as selected) at the start of a fixation interval.
	#  @return
	#	Float.
	def getStartPupilSize(self):
		return self.__sta__


def newStartFixationEvent(time, type, eye, read, sttime, gstx, gsty, hstx, hsty, sta, svel, supd_x, supd_y):
	return StartFixationEvent(time, type, eye, read, sttime, gstx, gsty, hstx, hsty, sta, svel, supd_x, supd_y)
	

## Class to represent End Blink event. This also contains the Start Blink data.
#  This also inherits all properties from EyeEvent.
class EndBlinkEvent(EyeEvent):
	def __init__(self, time, type, eye, read, sttime, entime):
		EyeEvent.__init__(self, time, type, eye, read, sttime)
		self.__entime__ = entime

	## Timestamp of the last sample of the blink.
	#  @return
	#  	Long integer.
	def getEndTime(self):
		return self.__entime__
		
		
def newEndBlinkEvent(time, type, eye, read, sttime, entime):
	return EndBlinkEvent(time, type, eye, read, sttime, entime)
	

## This interface is never used as is. EndFixationEvent, EndSaccadeEvent and FixUpdateEvent
#  types inherit this.
#
#  Please note that the \c getStartTime() and \c getEndTime() methods of the event class are the 
#  timestamps of the first and last samples in the event.  To compute duration, subtract 
#  these two values and add 4 msec (even in 500 Hz tracking modes, the internal parser of 
#  %EyeLink II quantizes event times to 4 milliseconds). 
#
#  Peak velocity returned by \c getPeakVelocity() for fixations is usually corrupted by terminal 
#  segments of the preceding and following saccades. 
#
#  The \c getStartPPD() and \c getEndPPD() methods contain the angular resolution at the start and 
#  end of the saccade or fixation.  The average of the start and end angular resolution can 
#  be used to compute the size of saccades in degrees.  This Python code would compute the 
#  true magnitude of a saccade from an \c ENDSACC event in the following way: 
#
#	\code
#	newEvent = EYELINK.getFloatData()
#
#	if isinstance(newEvent, EndFixationEvent):
#		Start_Gaze = newEvent.getStartGaze()
#		Start_PPD = newEvent.getStartPPD()
#		End_Gaze = newEvent.getEndGaze()
#		End_PPD = newEvent. getEndPPD()
#
#	dx = (End_Gaze[0] - Start_Gaze[0]) / ((End_PPD[0] + Start_PPD[0])/2.0);
#	dy = (End_Gaze[1] - Start_Gaze[1]) / ((End_PPD[1] + Start_PPD[1])/2.0); 
#	dist = math.sqrt(dx*dx + dy*dy);
#	\endcode
#
#  Please note that the average velocity for saccades may be larger than the saccade magnitude 
#  divided by its duration because of overshoots and returns.   
class EndNonBlinkEvent:
	def __init__(self, entime, genx, geny, henx, heny, evel, avel, pvel, eupd_x, eupd_y):
		self.__endTime__ = entime
		self.__endGaze__ = (genx, geny)
		self.__endHref__ = (henx, heny)
		self.__endVelocity__ = evel
		self.__avgVelocity__ = avel
		self.__peakVelocity__ = pvel
		self.__endUpd__  = (eupd_x, eupd_y)

	## Timestamp of the last sample of the event.
	#  @return
	#	Long integer.
	def getEndTime(self):
		return self.__endTime__

	## Gaze position at the end of the event (in pixel coordinates 
	#  set by the \c screen_pixel_coords command).  The first and 
	#  second items of the returned tuple store the x- and y- gaze position 
	#  respectively.
	#  @return
	#	Two-item tuple in the format of (float, float).
	def getEndGaze(self):
		return self.__endGaze__

	## HEADREF position at the end of the event.  The first and 
	#  second items of the returned tuple store the x- and y- coordinate HREF data 
	#  respectively.
	#  @return
	#	Two-item tuple in the format of (float, float).
	def getEndHREF(self):
		return self.__endHref__

	## Gaze velocity at the end of the event (in visual degrees per second).
	#  @return
	#	Float.
	def getEndVelocity(self):
		return self.__endVelocity__

	## Average gaze velocity during event (in visual degrees per second).		
	#  @return
	#	Float.
	def getAverageVelocity(self):
		return self.__avgVelocity__

	## Peak gaze velocity during event (in visual degrees per second).	
	#  @return
	# 	Float.
	def getPeakVelocity(self):
		return self.__peakVelocity__

	## Angular resolution at the end of the event (in screen pixels per visual degree).
	#  @return
	#	Two-item tuple in the format of (float, float).
	def getEndPPD(self):
		return self.__endUpd__


def getAngle(xavgres,yavgres,x1,y1, x2, y2):
		if x1 == constants.MISSING_DATA or y1 == constants.MISSING_DATA or x2 == constants.MISSING_DATA or y2 == constants.MISSING_DATA\
		or(x1==x2 and y1==y2):
			return constants.MISSING_DATA
		x = (x1 -x2)/xavgres
		y = (y1 -y2)/yavgres
	        rv= 57.296*math.atan2(y,0-x);
	        
	        
		#make sure the angle is between -180 to +180 So the calculation are simpler to do        
		rv = math.fmod(rv,360)
		if rv >180:
			rv = rv-360
		elif rv < -180:
			rv = 360+rv
	        
	        
	        
	        return rv


## Class to represent End Saccade event. This also contains the Start Saccade data.
#  This also inherits all properties from StartSaccadeEvent and EndNonBlinkEvent.
class EndSaccadeEvent(StartSaccadeEvent, EndNonBlinkEvent):
	def __init__(self, time, type, eye, read, sttime, gstx, gsty, hstx, hsty, svel, supd_x, supd_y, entime, genx, geny, henx, heny, evel, avel, pvel, eupd_x, eupd_y):
		StartSaccadeEvent.__init__(self, time, type, eye, read, sttime, gstx, gsty, hstx, hsty, svel, supd_x, supd_y)
		EndNonBlinkEvent.__init__(self, entime, genx, geny, henx, heny, evel, avel, pvel, eupd_x, eupd_y)
	
	## Returns the amplitude between the start and end of the event.
	#  @return Amplitude between the start and end of the event.
	def getAmplitude(self):
		ppdx = (self.getStartPPD()[0]+self.getEndPPD()[0])/2
		ppdy = (self.getStartPPD()[1]+self.getEndPPD()[1])/2
		x = (self.getEndGaze()[0] -self.getStartGaze()[0])/ppdx
		y = (self.getEndGaze()[1] -self.getStartGaze()[1])/ppdy
		return (x,y)
	
	## Returns the angle between the start and end of the event.
	#  @return Angle between the start and end of the event.
	def getAngle(self):
		ppdx = (self.getStartPPD()[0]+self.getEndPPD()[0])/2
		ppdy = (self.getStartPPD()[1]+self.getEndPPD()[1])/2
#		x = (self.getStartGaze()[0] -self.getEndGaze()[0])/ppdx
#		y = (self.getStartGaze()[1] -self.getEndGaze()[1])/ppdy
#	        rv= 57.296*math.atan2(y,0-x);
		return getAngle(ppdx,ppdy,self.getStartGaze()[0],self.getStartGaze()[1] ,self.getEndGaze()[0],self.getEndGaze()[1])
#	        return rv
    
	
def newEndSaccadeEvent(time, type, eye, read, sttime, gstx, gsty, hstx, hsty, svel, supd_x, supd_y, entime, genx, geny, henx, heny, evel, avel, pvel, eupd_x, eupd_y):
	return EndSaccadeEvent(time, type, eye, read, sttime, gstx, gsty, hstx, hsty, svel, supd_x, supd_y, entime, genx, geny, henx, heny, evel, avel, pvel, eupd_x, eupd_y)


## Class to represent End Fixation event. This also contains the Start Fixation data.
#  This also inherits all properties from StartFixationEvent and EndNonBlinkEvent.
class EndFixationEvent(StartFixationEvent, EndNonBlinkEvent):
	def __init__(self, time, type, eye, read, sttime, gstx, gsty, hstx, hsty, sta, svel, supd_x, supd_y, entime, genx, geny, gavx, gavy, henx, heny, havx, havy, ena, ava, evel, avel, pvel, eupd_x, eupd_y):
		StartFixationEvent.__init__(self, time, type, eye, read, sttime, gstx, gsty, hstx, hsty, sta, svel, supd_x, supd_y)
		EndNonBlinkEvent.__init__(self, entime, genx, geny, henx, heny, evel, avel, pvel, eupd_x, eupd_y)
		self.__avgGaze__=(gavx, gavy)
		self.__avgHref__=(havx, havy)
		self.__ena__=ena
		self.__ava__=ava

	## The average gaze position during the fixation period (in
	#  pixel coordinates set by the \c screen_pixel_coords command).
	#  @return
	#	Two-item tuple in the format of (float, float).
	def getAverageGaze(self):
		return self.__avgGaze__

	## Average HEADREF position during the fixation period.
	#  @return
	#	Two-item tuple in the format of (float, float).
	def getAverageHREF(self):
		return self.__avgHref__	

	## Average pupil size (in arbitrary units, area or diameter as selected) during a fixation.
	#  @return
	#	Float.
	def getAveragePupilSize(self):
		return self.__ava__
	
	## Pupil size (in arbitrary units, area or diameter as selected) at the end of a fixation interval.
	#  @return
	#	Float.
	def getEndPupilSize(self):
		return self.__ena__


def newEndFixationEvent(time, type, eye, read, sttime, gstx, gsty, hstx, hsty, sta, svel, supd_x, supd_y, entime, genx, geny, gavx, gavy, henx, heny, havx, havy, ena, ava, evel, avel, pvel, eupd_x, eupd_y):
	return EndFixationEvent(time, type, eye, read, sttime, gstx, gsty, hstx, hsty, sta, svel, supd_x, supd_y, entime, genx, geny, gavx, gavy, henx, heny, havx, havy, ena, ava, evel, avel, pvel, eupd_x, eupd_y)


## Class to represent the Fix Update event.
#  This also inherits all properties from StartNonBlinkEvent and EndNonBlinkEvent.
class FixUpdateEvent(StartNonBlinkEvent, EndNonBlinkEvent):
	def __init__(self, time, type, eye, read, sttime, gstx, gsty, hstx, hsty, sta, svel, supd_x, supd_y, entime, genx, geny, gavx, gavy, henx, heny, havx, havy, ena, ava, evel, avel, pvel, eupd_x, eupd_y):
		StartNonBlinkEvent.__init__(self, time, type, eye, read, sttime, gstx, gsty, hstx, hsty, svel, supd_x, supd_y)
		EndNonBlinkEvent.__init__(self, entime, genx, geny, henx, heny, evel, avel, pvel, eupd_x, eupd_y)
		self.__avgGaze__=(gavx, gavy)
		self.__avgHref__=(havx, havy)
		self.__ena__=ena
		self.__ava__=ava
		self.__sta__  = sta
	
	## Pupil size (in arbitrary units, area or diameter as selected) at the start of a fixation interval.
	#  @return
	#	Float.
	def getStartPupilSize(self):
		return self.__sta__
	
	## The average gaze position during the fixation period (in
	#  pixel coordinates set by the \c screen_pixel_coords command).
	#  @return
	#	Two-item tuple in the format of (float, float).
	def getAverageGaze(self):
		return self.__avgGaze__
				
	## Average HEADREF position during the fixation period.
	#  @return
	#	Two-item tuple in the format of (float, float).
	def getAverageHREF(self):
		return self.__avgHref__	

	## Average pupil size (in arbitrary units, area or diameter as selected) during a fixation.
	#  @return
	#	Float.
	def getAveragePupilSize(self):
		return self.__ava__
	
	## Pupil size (in arbitrary units, area or diameter as selected) at the end of a fixation interval.
	#  @return
	#	Float.
	def getEndPupilSize(self):
		return self.__ena__

	def __str__(self):
	 	rv= "FixationUpdate: %i %i %f %f"%(self.__time__,self.getEndTime()-self.getStartTime(),self.getAverageGaze()[0], self.getAverageGaze()[1])
	 	return rv

		
def newFixUpdateEvent(time, type, eye, read, sttime, gstx, gsty, hstx, hsty, sta, svel, supd_x, supd_y, entime, genx, geny, gavx, gavy, henx, heny, havx, havy, ena, ava, evel, avel, pvel, eupd_x, eupd_y):
	return FixUpdateEvent(time, type, eye, read, sttime, gstx, gsty, hstx, hsty, sta, svel, supd_x, supd_y, entime, genx, geny, gavx, gavy, henx, heny, havx, havy, ena, ava, evel, avel, pvel, eupd_x, eupd_y)
	

## A message event is created by your experiment program and placed in the EDF file.  It 
#  is possible to enable the sending of these messages back through the link, although 
#  there is rarely a reason to do this.  The MessageEvent class is designed to hold 
#  information on %EyeLink message events retrieved from the link.  Please note that all 
#  methods for the MessageEvent class do not take a parameter.
#
#  Returned by \c getFloatData() whenever there is a Message Event.
class MessageEvent:
	def __init__(self, time, type, msg):
		self.__time__ = time;
		self.__type__ = type
		self.__text__ = msg
	
	## Timestamp of the sample causing event (when camera imaged eye, 
	#  in milliseconds since %EyeLink tracker was activated).
	#  @return
	#	Long integer.
	def getTime(self):
		return self.__time__

	## The event code. This should be \c MESSAGEEVENT (i.e., 24).
	#  @return
	#	Integer.
	def getType(self):
		return self.__type__

	## Message contents (max length 255 characters).
	#  @return
	#	String.
	def getText(self):
		return self.__text__	
	
	
def newMessageEvent(time, type, text):
	return MessageEvent(time, type, text)	
	

## IOEvent class is used to handle \c BUTTONEVENT and \c INPUTEVENT types, which report changes 
#  in button status or in the input port data.  The \c getTime() method records the timestamp 
#  of the eye-data sample where the change occurred, although the event itself is usually 
#  sent before that sample.  Button events from the link are rarely used; monitoring buttons 
#  with one of \c readKeybutton(), \c lastButtonPress(), or \c buttonStates() of the EyeLink class 
#  methods is preferable, since these can report button states at any time, not just during 
#  recording.
#
#  Returned by \c getFloatData() whenever there is an IOEvent.
class IOEvent:
	def __init__(self, iotype, time, data):
		self.__time__ = time
		self.__iotype__ = iotype
		self.__data__ = data

	## Timestamp of the sample causing event (in milliseconds since %EyeLink tracker was activated).	
	#  @return
	#	Long integer.
	def getTime(self):
		return self.__time__

	## The event code. This should be \c BUTTONEVENT (i.e., 25) or \c INPUTEVENT (i.e., 28).
	#  @return
	#  	Integer.
	def getType(self):
		return self.__iotype__

	## Coded event data.
	#  @return
	#	Long integer.
	def getData(self):
		return self.__data__

	def __str__(self):
		return str(self.__time__)+"\t IOEvent type="+str(self.__iotype__)+"\tdata="+str(self.__data__)


## A ButtonEvent class, derived from the IOEvent class, is created to handle specifics of 
#  button events.  For this class, in addition to the generic IOEvent class methods, 
#  two additional methods can be used for instances of the ButtonEvent class.
#
#  Returned by \c getFloatData() whenever there is a Button Event.
class ButtonEvent(IOEvent):
	def __init__(self,time,data):
		IOEvent.__init__(self,25,time,data)
		self.__activeButtons__=list()
		self.__states__=list()
		
		b  = data>>8
		bs = 255 & data
		abuttons = [1,2,3,4,5,6,7,8]
		for i in abuttons:
			if (b&1) != 0:
				self.__activeButtons__.append(i)	
				self.__states__.append(bs&1)	
			b = b>>1
 			bs = bs >>1

	## A list of buttons pressed or released.
	#  @return
	#	List of integers.
	def getButtons(self):
		if len(self.__activeButtons__) == 0:
			return [-1]
		return self.__activeButtons__
		
	## The button state (\c 1 for pressed or \c 0 for released).
	#  @return
	#	List of integers.
	def getStates(self):
		if len(self.__states__) == 0:
			return [-1]
		return self.__states__
	
	def __str__(self):
		return str(self.__time__)+"\t Button Event"+str(self.getType())+"\t"+str(self.getButtons())+str(self.__states__)
		

def newIOEvent(btype,time, data):
#	print "New IOEvent ",str(BUTTONEVENT), str(btype)
	if(int(btype) == constants.BUTTONEVENT):
		return ButtonEvent(time, data)
	return IOEvent(btype, time,data)


## The DisplayInfo class is used to contain information on display configurations, including 
#  width, height, color bits, and refresh rate. The current display configuration can be 
#  retrieved by the \c getDisplayInformation() function of the pylink module. 
#
#  For example:
#  
#  	\code
#   	from pylink import *
#  
#  	// Code to open the display
#  	currentDisplay = getDisplayInformation();
#  
#  	print "Current display settings: ", currentDisplay.width, currentDisplay.height, \
#  					currentDisplay.bits, currentDisplay.refresh
#   	\endcode
#

#  The DisplayInfo has the following four attributes.
#	
#  <table>
#  <tr><td>width</td><td>Integer</td><td>Display width in screen pixels.</td></tr>
#  <tr><td>height</td><td>Integer</td><td>Display height in screen pixels.</td></tr>
#  <tr><td>bits</td><td>Integer</td><td>Color resolution, in bits per pixel, of the display device (for example: 4 bits for 16 colors, 8 bits for 256 colors, or 16 bits for 65,536 colors).</td></tr>
#  <tr><td>refresh</td><td>Float</td><td>Refresh rate.</td></tr>
#  </table>
#  
class DisplayInfo:	
	def __init__(self, width, height, bits, refresh):
           self.width = width
           self.height = height
           self.bits = bits
           self.refresh = refresh

	## @var width 
	#  Display width in screen pixels.
	
	## @var height 
	#  Display height in screen pixels.
	
	## @var bits 
	#  Color resolution, in bits per pixel, of the display device (for example: 4 bits for 16 colors, 8 bits for 256 colors, or 16 bits for 65,536 colors).
	
	## @var refresh 
	#  Refresh rate.           

def newDisplayInfo(width, height, bits, refresh):
	return DisplayInfo(width,height,bits, refresh)


## Class to represent tracker status information such as time stamps, flags, tracker addresses and so on.
#
#  A valid reference to this object can be obtained by calling the function \c EYELINK.getTrackerInfo().
class ILinkData:
	def __init__(self):
		self._time  = None
		self._samrate= None
		self._samdiv= None 
		self._prescaler= None 
		self._vprescaler= None 
		self._pprescaler= None 
		self._hprescaler= None 
		self._sample_data= None 
		self._event_data= None
		self._event_types= None
		self._in_sample_block= None
		self._in_event_block= None
		self._have_left_eye= None
		self._have_right_eye= None
		self._last_data_gap_types= None
		self._last_data_buffer_type= None
		self._last_data_buffer_size= None
		self._control_read= None
		self._first_in_block= None
		self._last_data_item_time= None
		self._last_data_item_type= None
		self._last_data_item_contents= None
		self._block_number= None
		self._block_sample= None
		self._block_event= None
		self._last_resx= None
		self._last_resy= None
		self._last_pupil= None
		self._last_status= None
		self._queued_samples= None
		self._queued_events= None
		self._queue_size= None
		self._queue_free= None
		self._last_rcve_time= None
		self._samples_on= None
		self._events_on= None
		self._packet_flags= None
		self._link_flags= None
		self._state_flags= None
		self._link_dstatus= None
		self._link_pendcmd= None
		self._polling_remotes= None
		self._poll_responses= None
		self._reserved= None
		self._our_name= None
		self._eye_name= None
		self._nodes= None
		self._last_data_item= None
		self._our_address= None
		self._eye_address= None
		self._ebroadcast_address= None
		self._rbroadcast_address= None

	## Time of last control event.
	#  Equivalent field in ILINKDATA "C": Time.
	def getTime(self):          
		return self._time

	## 10*sample rate (\c 0 if no samples, \c 1 if nonconstant).
	#  Equivalent field in ILINKDATA "C": samrate.
	def getSampleRate(self):    
		return self._samrate
	
	## Sample "divisor" (min msec between samples).
	#  Equivalent field in ILINKDATA "C": samdiv.
	def getSampleDivisor(self): 
		return self._samdiv
	
	## Amount to divide gaze x,y,res by.
	#  Equivalent field in ILINKDATA "C": prescaler.
	def getPrescaler(self):     
		return self._prescaler
		
	## Amount to divide velocity by.
	#  Equivalent field in ILINKDATA "C": vprescaler.
	def getVelocityPrescaler(self):
		return self._vprescaler
	
	## Pupil prescale (\c 1 if area, greater if diameter).
	#  Equivalent field in ILINKDATA "C": pprescaler.
	def getPupilPrescaler(self):
		return self._pprescaler
	
	## Head-distance prescale (to mm).
	#  Equivalent field in ILINKDATA "C": hprescaler.
	def getHeadDistencePrescaler(self):
		return self._hprescaler
	
	## \c 0 if off, else all flags.
	#  Equivalent field in ILINKDATA "C": sample_data.
	def getSampleDataFlags(self):
		return self._sample_data
	
	## \c 0 if off, else all flags.
	#  Equivalent field in ILINKDATA "C": event_data.
	def getEventDataFlags(self):
		return self._event_data
	
	## \c 0 if off, else event-type flags.
	#  Equivalent field in ILINKDATA "C": event_types.
	def getEventTypeFlags(self):
		return self._event_types
	
	## Set if in block with samples.
	#  Equivalent field in ILINKDATA "C": in_sample_block.
	def isInBlockWithSamples(self):
		return self._in_sample_block
	
	## Set if in block with events.
	#  Equivalent field in ILINKDATA "C": in_event_block.
	def isInBlockWithEvents(self):
		return self._in_event_block
	
	## Set if any left-eye data expected.
	#  Equivalent field in ILINKDATA "C": have_left_eye.
	def haveLeftEye(self):
		return self._have_left_eye
	
	## Set if any right-eye data expected.
	#  Equivalent field in ILINKDATA "C": have_right_eye.
	def haveRightEye(self):
		return self._have_right_eye
	
	## Flags what we lost before last item.
	#  Equivalent field in ILINKDATA "C": last_data_gap_types.
	def getLostDataTypes(self):
		return self._last_data_gap_types
	
	## Buffer-type code.
	#  Equivalent field in ILINKDATA "C": last_data_buffer_type.
	def getLastBufferType(self):
		return self._last_data_buffer_type
	
	## Buffer size of last item.
	#  Equivalent field in ILINKDATA "C": last_data_buffer_size.
	def getLastBufferSize(self):
		return self._last_data_buffer_size
	
	## Set if control event read with last data.
	#  Equivalent field in ILINKDATA "C": control_read.
	def isControlEvent(self):
		return self._control_read
	
	## Set if control event started new block.
	#  Equivalent field in ILINKDATA "C": first_in_block.
	def isNewBlock(self):
		return self._first_in_block
	
	## Time field of item.
	#  Equivalent field in ILINKDATA "C": last_data_item_time.
	def getLastItemTimeStamp(self):
		return self._last_data_item_time
	
	## Type: \c 100 = sample, \c 0 = none, else event type.
	#  Equivalent field in ILINKDATA "C": last_data_item_type.
	def getLastItemType(self):
		return self._last_data_item_type
	
	## Content: &lt;read&gt; (\c IEVENT), &lt;flags&gt; (\c ISAMPLE).
	#  Equivalent field in ILINKDATA "C": last_data_item_contents.
	def getLastItemContent(self):
		return self._last_data_item_contents
	
	## Block in file.
	#  Equivalent field in ILINKDATA "C": block_number.
	def getBlockNumber(self):
		return self._block_number
	
	## Samples read in block so far.
	#  Equivalent field in ILINKDATA "C": block_sample.
	def getSamplesInBlock(self):
		return self._block_sample
	
	## Events (excl. control read in block).
	#  Equivalent field in ILINKDATA "C": block_event.
	def getEventsInBlock(self):
		return self._block_event
	
	## Updated by samples only. 
	#  Equivalent field in ILINKDATA "C": last_resx.
	def getLastResX(self):
		return self._last_resx
	
	## Updated by samples only.
	#  Equivalent field in ILINKDATA "C": last_resy.
	def getLastResY(self):
		return self._last_resy
	
	## Updated by samples only.
	#  Equivalent field in ILINKDATA "C": last_pupil.
	def getLastPupil(self):
		return self._last_pupil
	
	## Updated by samples, events.
	#  Equivalent field in ILINKDATA "C": last_status.
	def getLastItemStatus(self):
		return self._last_status
	
	## Number of items in queue.
	#  Equivalent field in ILINKDATA "C": queued_samples.
	def getSampleQueueLength(self):
		return self._queued_samples
	
	## Includes control events.
	#  Equivalent field in ILINKDATA "C": queued_events.
	def getEventQueueLength(self):
		return self._queued_events
	
	## Total queue buffer size.
	#  Equivalent field in ILINKDATA "C": queue_size.
	def getQueueSize(self):
		return self._queue_size
	
	## Unused bytes in queue.
	#  Equivalent field in ILINKDATA "C": queue_free.
	def getFreeQueueLength(self):
		return self._queue_free
	
	## Time tracker last sent packet.
	#  Equivalent field in ILINKDATA "C": last_rcve_time.
	def getLastReceiveTime(self):
		return self._last_rcve_time
	
	## Data type rcve enable (switch).
	#  Equivalent field in ILINKDATA "C": samples_on.
	def isSamplesEnabled(self):
		return self._samples_on
	
	## Data type rcve enable (switch).
	#  Equivalent field in ILINKDATA "C": events_on.
	def isEventsEnabled(self):
		return self._events_on
	
	## Status flags from data packet.
	#  Equivalent field in ILINKDATA "C": packet_flags.
	def getPacketFlags(self):
		return self._packet_flags
	
	## Status flags from link packet header.
	#  Equivalent field in ILINKDATA "C": link_flags.
	def getLinkFlags(self):
		return self._link_flags
	
	## Tracker error state flags.
	#  Equivalent field in ILINKDATA "C": state_flags.
	def getStateFlags(self):
		return self._state_flags
	
	## Tracker data output state.
	#  Equivalent field in ILINKDATA "C": link_dstatus.
	def getTrackerDataOutputState(self):
		return self._link_dstatus
	
	## Tracker commands pending.
	#  Equivalent field in ILINKDATA "C": link_pendcmd.
	def getPendingCommands(self):
		return self._link_pendcmd
	
	## \c 1 if polling remotes, else polling trackers.
	#  Equivalent field in ILINKDATA "C": polling_remotes.
	def isPoolingRemote(self):
		return self._polling_remotes
	
	## Total nodes responding to polling.
	#  Equivalent field in ILINKDATA "C": poll_responses.
	def getPoolResponse(self):
		return self._poll_responses
	
	## \c 0 for %EyeLink I or original %EyeLink API DLL.
	#  Equivalent field in ILINKDATA "C": reserved.
	def getReserved(self):
		return self._reserved
	
	## A name for our machine.
	#  Equivalent field in ILINKDATA "C": our_name.
	def getName(self):
		return self._our_name
	
	## Name of tracker connected to.
	#  Equivalent field in ILINKDATA "C": eye_name.
	def getTrackerName(self):
		return self._eye_name
	
	## Data on nodes.
	#  Equivalent field in ILINKDATA "C": nodes.
	def getNodes(self):
		return self._nodes
	
	## Buffer containing last item.
	#  Equivalent field in ILINKDATA "C": last_data_item.
	def getLastItem(self):
		return self._last_data_item
	
	## Address of our machine.
	#  Equivalent field in ILINKDATA "C": our_address.
	def getAddress(self):
		return self._our_address
	
	## Address of the connected tracker.
	#  Equivalent field in ILINKDATA "C": eye_address.
	def getTrackerAddress(self):
		return self._eye_address
	
	## Broadcast address for eye trackers.
	#  Equivalent field in ILINKDATA "C": ebroadcast_address.
	def getTrackerBroadcastAddress(self):
		return self._ebroadcast_address
	
	## Broadcast address for remotes 
	#  Equivalent field in ILINKDATA "C": rbroadcast_address.
	def getRemoteBroadcastAddress(self):
		return self._rbroadcast_address


def _updatePerTrialDynamic(dt,x):
	(dt._samrate,dt._samdiv,dt._prescaler,dt._vprescaler,dt._pprescaler,dt._hprescaler,dt._sample_data,dt._event_data,dt._event_types )=x


def _updateBooleanFlags(dt, x):
	(dt._in_sample_block,dt._in_event_block,dt._have_left_eye,dt._have_right_eye,dt._samples_on,dt._events_on,dt._control_read,dt._first_in_block)=x


def _updateTimeStamps(dt, x):
	(dt._time,dt._last_data_item_time,dt._last_rcve_time)=x


def _updateStatusFlags(dt, x):
	(dt._last_status,dt._packet_flags,dt._link_flags,dt._state_flags,dt._link_dstatus,dt._link_pendcmd)=x


def _updateTypeFlags(dt,x):
	(dt._last_data_gap_types,dt._last_data_buffer_type,dt._last_data_item_type)=x


def _updateSampleEventData(dt, x, pl, ldi):
	(dt._last_data_buffer_size,dt._last_data_item_contents,dt._block_number,dt._block_sample,dt._block_event,dt._last_resx,dt._last_resy,dt._queued_samples,dt._queued_events,dt._queue_size,dt._queue_free) =x 
	dt._last_pupil= pl
	dt._last_data_item=ldi
	
	
def _updateNetworkData(dt, x, nodes):
	(dt._polling_remotes, dt._poll_responses)= x
	dt._nodes= nodes


def _updateAddresses(dt, nm, ad):
	(dt._our_name,dt._eye_name)= nm
	(dt._eye_address, dt._our_address,dt._ebroadcast_address,dt._rbroadcast_address)=ad


def _updateReserved(dt, r):
	dt._reserved=r


def drawCalTarget(x,y):
	if(constants.EYELINK != None):
		constants.EYELINK.drawCalTarget((x,y))
		

## This represents a key input. This is used with EyeLinkCustomDisplay to notify the eyelink_core.dll 
#  that a key input is available.
class KeyInput:
	def __init__(self,key,mod=0,state=0):
		self.__key__ = key
		self.__state__ =state
		self.__type__= 0x1
		self.__mod__ = mod
		


from pylink import request_cross_hair_draw
## EyeLinkCustomDisplay is an abstract class, that one would implement to present calibration/validation/drift correction targets and
#  camera images.  In addition, EyeLinkCustomDisplay can also play target beeps and display error messages.
#
#  To use custom display do the following.
#	- 1. Implement EyeLinkCustomDisplay
#	- 2. Create an instance of the custom diplay object
#	- 3. Use pylink.openGraphicsEx to let pylink know to use the custom display object
#
#  \b Example:
#  @code
#  	genv = EyeLinkCoreGraphicsPyGame(800,600,eyelinktracker)
#	openGraphicsEx(genv)
#  @endcode
#  \link customdisplayexample Code to implement EyeLinkCustomDisplay using pygame.
class EyeLinkCustomDisplay:
	## Constructor takes no parameters
	#
	def __init__(self):
		pass
	## Internal function to update the mage size. 
	#  The size set by this function is used to draw cross hair when \c draw_cross_hair() is called
	#  This function should \b not be overrided and should \b not be called other than the 
	#  display mechanism.
	#
	#  @param width Width of the image.
	#  @param height Height of the image.
	def __updateimgsize__(self,width,height):
		self.__imgwidth__ = width
		self.__imgheight__ = height

	## This function is called to setup calibration/validation display. 
	#  This will be called just before we enter into the calibration or validataion or drift correction mode.	
	#  Any allocation per calibration or validation drift corretion can be done here.
	#  Also, it is normal to clear the display in this call. 
	def setup_cal_display(self): 
		print "%s.setup_cal_display not implemented"%(self.__class__)
		print self.setup_cal_display.__doc__

	## This function is called just before exiting calibration/validation display.
	#  Any resource allocation done in \c setup_cal_display() can be cleared. 
	def exit_cal_display(self): 
		print "%s.exit_cal_display not implemented"%(self.__class__)
		print self.exit_cal_display.__doc__
	
	## This function is called if abort of record. It is used to hide display from subject.	
	def record_abort_hide(self): 
		print "%s.record_abort_hide not implemented"%(self.__class__)
		print self.record_abort_hide.__doc__

	## This function is used to setup image display. 
	#  It takes expected image size of the source image. This may be called repeatedly for same display. If this fails,
	#  It should return \c 1 if success and \c 0 otherwise.	
	#
	#  @param width Width of the incoming image.
	#  @param height Height of the incoming image.
	#  @return \c 1 if success, \c 0 otherwise.
	def setup_image_display(self, width, height): 
		print "%s.setup_image_display not implemented"%(self.__class__)
		print self.setup_image_display.__doc__

	## This function displays the camera image title. 
	#  This is called whenever the title changes
	#
	#  @param title title change
	def image_title(self, title): 
		print "%s.setup_cal_display not implemented"%(self.__class__)
		print self.image_title.__doc__

	## This function is used to display an image line. This function is called with an
	#  array of bytes containing picture colors. The byte on pixels are just palette
	#  indexes. This index should be used against the palette created on the call to
	#  \c set_image_palette_hook(). The image is given line by line from top to bottom.
	#  It may be efficient to collect one full image and do a full blit of the entire
	#  image.
	# 
	#  @code
	#	i =0
	#	imline = self.imagebuffer[line-1]
	#	while i <width:
	#			imline[i] = self.pal[buff[i]]
	#	i= i+1	
	#  @endcode
	def draw_image_line(self, width, line, totlines,buff): 
		print "%s.draw_image_line not implemented"%(self.__class__)
		print self.draw_image_line.__doc__

	## This function is called to setup the image palettes. The function is called 
	#  with a set of RGB colors to set up for next image.
	#  @code
	#	self.pal = []
	#	while i < sz:
	#		rf = int(b[i])
	#		gf = int(g[i])
	#		bf = int(r[i])
	#		self.pal.append((rf<<16) | (gf<<8) | (bf))
	#		i = i+1	
	#  @endcode
	def set_image_palette(self, red, green, blue): 
		print "%s.set_image_palette not implemented"%(self.__class__)
		print self.set_image_palette.__doc__

	## Called to end the image display.		
	def exit_image_display(self): 
		print "%s.exit_image_display not implemented"%(self.__class__)
		print self.exit_image_display.__doc__
	
	## Called to clear the calibration display.
	def clear_cal_display(self): 
	
		print "%s.clear_cal_display not implemented"%(self.__class__)
		print self.clear_cal_display.__doc__

	## Called to erase the calibration or validation target.
	#  Erase the target drawn by the previous call to \c draw_cal_target().
	def erase_cal_target(self): 
		print "%s.erase_cal_target not implemented"%(self.__class__)
		print self.erase_cal_target.__doc__
		
		
	## Called to draw the calibration or validation target.		
	#  Draw a target at x,y and display it.
	#  
	#  @remark x and y values are relative to the active \c screen_pixel_coords command.
	#  @param x X location to draw the target.
	#  @param y Y location to draw the target. 
	def draw_cal_target(self, x, y): 
		print "%s.draw_cal_target not implemented"%(self.__class__)
		print self.draw_cal_target.__doc__
		
	## Called to play target beeps.
	# 
	#  @param beepid Id of the beep to be played.
	# 	Possible values for beepid are:
	#	\arg CAL_ERR_BEEP   =-1
	#	\arg DC_ERR_BEEP    =-2
	#	\arg CAL_GOOD_BEEP  = 0
	#	\arg CAL_TARG_BEEP  = 1
	#	\arg DC_GOOD_BEEP   = 2
	#	\arg DC_TARG_BEEP   = 3
	def play_beep(self, beepid):
		print "%s.play_beep not implemented"%(self.__class__)
		print self.play_beep.__doc__

	## This function should return list of KeyInput.
	#  If there are not keys, return an empty list or None.
	def get_input_key(self): 
		print "%s.get_input_key not implemented"%(self.__class__)
		print self.get_input_key.__doc__

	## Called to notify any error message to display or print.
	def alert_printf(self,msg): 
		print "%s.alert_printf not implemented"%(self.__class__)
		print self.alert_printf.__doc__
	
	## Called to draw the cross hair, in response to the call to \c draw_cross_hair().
	#  This function should draw a line from (x1,y1) to (x2,y2). The x and y values
	#  are relative to the width and height of the image, given at \c setup_image_display().
	#
	#  @param x1 Starting x position.
	#  @param y1 Starting y position.
	#  @param x2 Ending x position.
	#  @param y2 Ending y position.
	#  @param colorindex Color id of the line.
	#  	Possible value for colorindex are:
	#	\arg CR_HAIR_COLOR=1
	#	\arg PUPIL_HAIR_COLOR=2
	#	\arg PUPIL_BOX_COLOR=3
	#	\arg SEARCH_LIMIT_BOX_COLOR=4
	#	\arg MOUSE_CURSOR_COLOR=5
	def draw_line(self,x1,y1,x2,y2,colorindex):
		print "%s.draw_line not implemented"%(self.__class__)
		print self.draw_line.__doc__
	
	## Called to draw the cross hair, in response to the call to \c draw_cross_hair().
	#  This function should draw an lozenge bounded by the box (x,y),(width,height). The x and y values
	#  are relative to the width and height of the image, given at \c setup_image_display().
	# 
	#  @remark This function is not used at the moment.
	#  @param x Starting x position.
	#  @param y Starting y position.
	#  @param width  bounding width
	#  @param height bounding height
	#  @param colorindex Color id of the ellipse.
	# 	Possible value for colorindex are:
	#	\arg CR_HAIR_COLOR=1
	#	\arg PUPIL_HAIR_COLOR=2
	#	\arg PUPIL_BOX_COLOR=3
	#	\arg SEARCH_LIMIT_BOX_COLOR=4
	#	\arg MOUSE_CURSOR_COLOR=5
	def draw_lozenge(self,x,y,width,height,colorindex):
		print "%s.draw_lozenge not implemented"%(self.__class__)
		print self.draw_lozenge.__doc__
	
	## Called to get the mouse location.  
	#  This function should return the mouse location and the state at the time of call. ((x,y),state).
	#  At the moment we only care if the mouse is clicked or not. So, if clicked the state = 1, 0 otherwise.
	#  This function is only useful for EyeLink1000.
	def get_mouse_state(self):
		print "%s.get_mouse_state not implemented"%(self.__class__)
		print self.get_mouse_state.__doc__
	
	
	## User call this function to request draw cross hair.
	#  After completion of filling in the camera image, call this function to draw the
	#  cross hair on the camera image.
	def draw_cross_hair(self):
		request_cross_hair_draw(self)
