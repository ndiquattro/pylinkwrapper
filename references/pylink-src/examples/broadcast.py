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
# $Date: 2009/06/30 17:41:36 $
# 
#


from pylink import *
from pygame import *
import time
import sys


tracker_pixel_left=0
tracker_pixel_top=0
tracker_pixel_right=800
tracker_pixel_bottom=600


surf = None
def track2local_x(x):
	global tracker_pixel_left, tracker_pixel_top,tracker_pixel_right,tracker_pixel_bottom
	print tracker_pixel_right , " " , tracker_pixel_bottom
	surf = surf = display.get_surface()
	return  (x - tracker_pixel_left) * surf.get_rect().w /(tracker_pixel_right - tracker_pixel_left + 1)


def track2local_y(y):
	global tracker_pixel_left, tracker_pixel_top,tracker_pixel_right,tracker_pixel_bottom
	print tracker_pixel_right , " " , tracker_pixel_bottom
	surf = display.get_surface()
	return (y - tracker_pixel_top) * surf.get_rect().h /(tracker_pixel_bottom - tracker_pixel_top + 1)




class BroadcastEyelink(EyeLinkListener):
	def __init__(self):
		EyeLinkListener.__init__(self)
	
	def drawCalTarget(self, pos):
		x = track2local_x(pos[0])
		y = track2local_y(pos[1])
		print "orig " , pos ," new " , (x,y)
		EyeLinkListener.drawCalTarget(self,(x,y))
		
		
		
class GazeCursor:
	def __init__(self, tracker, surf, fg=None):
		self.tracker = tracker
		self.surf = surf
		self.fgbm = fg
		self.cursorsize = (100,100)
		self.color = (255,0,0,0)
		self.back = (255,255,255,255)
		self.backcursor = None;
		self.srcrct = None
		self.prevrct = None
		self.oldv = None
		self.initCursor()
		
		
	def initCursor(self):
		self.cursor = Surface(self.cursorsize)
		self.cursor.fill(self.back)
		draw.circle(self.cursor,self.color, (self.cursorsize[0]/2, self.cursorsize[1]/2) ,12)
		
		
	def setCursorSize(self, size):
		self.cursorsize =size
		self.initCursor()
	
	def setCursorColor(self, color):
		self.color = color
		self.initCursor()
	
	def setCursorBackground(self, color):
		self.back = color
		self.initCursor()
		
	def updateCursor(self,loc):
		if(self.fgbm != None):
			srcrct = self.cursor.get_rect().move(loc[0], loc[1])
			self.cursor.blit(self.fgbm,(0,0), srcrct)
	

	def drawCursor(self, loc):
		v = (loc[0]-self.cursorsize[0]/2, loc[1]-self.cursorsize[1]/2)
		if(self.oldv != None and self.oldv == v):
			return
		self.oldv = v
		if(self.backcursor != None): #copy the current backcursor to the surface and get a new backup
			if(self.prevrct != None):	
				self.surf.blit(self.pbackcursor, (self.prevrct.x, self.prevrct.y))

			self.pbackcursor.blit(self.backcursor,(0,0))
			self.pbackcursor.blit(self.backcursor,(0,0))
			self.prevrct = self.srcrct.move(0,0) #make a copy	
			self.srcrct.x = v[0]
			self.srcrct.y = v[1]
			self.backcursor.blit(self.surf, (0,0),self.srcrct)

		else: # create a new backcursor and copy the new back cursor
			self.backcursor = Surface(self.cursorsize)
			self.pbackcursor = Surface(self.cursorsize)
			self.backcursor.fill((0,255,0,255))
			self.srcrct = self.cursor.get_rect().move(0,0)
			self.srcrct.x = v[0]
			self.srcrct.y = v[1]
			self.backcursor.blit(self.surf, (0,0), self.srcrct)
			self.backcursor.blit(self.surf, (0,0), self.srcrct)

		self.updateCursor(v)
		self.surf.blit(self.cursor, v)
		display.flip()
		
		
		
		
	
def previewTrackerConnection(tracker):
	print "previewTrackerConnection"
	idata = tracker.getTrackerInfo()  # access link status info 
	tracker.requestTime()         # force tracker to send status and time  - not implemented in pylink
	t = currentTime()
	while(currentTime()-t < 500):   # wait for response 
		tt = tracker.readTime()   # will be nonzero if reply 
		if(tt != 0):		# extract connection state  
			if(idata._link_flags & LINK_BROADCAST):
				return LINK_BROADCAST;
			if(idata._link_flags & LINK_CONNECTED):
				return LINK_CONNECTED;
			else:
				return 0;
		tracker.pumpMessages();             # keep Windows happy 
		if(tracker.breakPressed()): 
			return 1;  # stop if program terminated 
	return -1  # failed (timed out) 




def waitForConnection(tracker):
	print "waiting for connection "
	first_pass = 1   # draw display only after first failure 

	while(1):	# loop till a connection happens     
			# check if tracker is connected 
		i = previewTrackerConnection(tracker)
		if(i == -1):
			alert("Cannot find tracker")
			return -1
		elif(i > 0):
			print " we have a connection"
			return 0  # we have a connection! 

		if(first_pass):   #If not, draw title screen 
			pass

		i = tracker.getkey()      #check for exit 
		if(i==ESC_KEY or i==TERMINATE_KEY):
			return 1

		msecDelay(5000);   #go to background, don't flood the tracker 
   
   

BACKGROUND = (0,0,0,255)   
def record_mode_display(tracker):
	surf = display.get_surface()
	surf.fill(BACKGROUND)
	display.flip()
	surf.fill(BACKGROUND)
	drawgc(tracker, surf)


				
				


def drawgc(tracker, surf):

	cursor = GazeCursor(tracker, surf)
	cursor.setCursorBackground(BACKGROUND)
	tracker.resetData()
	tracker.dataSwitch(RECORD_LINK_SAMPLES | RECORD_LINK_EVENTS);
  
	startTime = currentTime()
	tracker.flushKeybuttons(0)
	buttons =0
	while(tracker.isConnected() and  tracker.getTrackerMode() == EL_RECORD_MODE):
		if(tracker.breakPressed()):
			return ABORT_EXPT
		elif(tracker.escapePressed()):
			return SKIP_TRIAL
		error = tracker.isRecording()
		if(error!=0):
			return error

		buttons = tracker.getLastButtonPress()
		if(buttons != (0,0)):
			break;
		dt = tracker.getNewestSample()
		if(dt != None):
			if(dt.isRightSample()):
				v = dt.getRightEye().getGaze()
			else:
				v = dt.getLeftEye().getGaze()

			cursor.drawCursor(v)
	return tracker.getRecordingStatus()
        
        

def read_tracker_pixel_coords():
	
	if(not tracker.isConnected()or tracker.breakPressed()):
		return 1;    # not connected 
	tracker.readRequest("screen_pixel_coords");
	t = currentTime();
	while(currentTime()-t < 500):
		try:
			buf = tracker.readReply()
			global tracker_pixel_left, tracker_pixel_top,tracker_pixel_right,tracker_pixel_bottom
			(x, y,w,h)= buf.split(",")
			tracker_pixel_left 	= int(x)
			tracker_pixel_top	= int(y)
			tracker_pixel_right	= int(w)
			tracker_pixel_bottom	= int(h)
			
			print buf
			print x, y, w, h
			return 0;
		except:
			pass
		tracker.pumpMessages()  # keep Windows happy 
		if(not tracker.isConnected() or tracker.breakPressed()):
			return 1
	print "Cannot detect screen_pixel_coords using ", tracker_pixel_right, tracker_pixel_bottom
	return -1  # timed out 


can_read_pixel_coords = 1
def track_mode_loop(tracker):
  oldmode = -1;  # to force initial mode setup 

  while(tracker.isConnected()):
      mode = tracker.getTrackerMode()
      
      key = tracker.getkey()

      if(key==27 or tracker.breakPressed() or not tracker.isConnected()):
      	return
      
      elif(key):                  	# echo to tracker 
        tracker.sendKeybutton(key,0,KB_PRESS)

      if(mode == oldmode):
      	continue
      print "current mode is " , mode
      if(mode == EL_RECORD_MODE):        # Record mode: show gaze cursor 
            record_mode_display(tracker)
            
      elif(mode ==  EL_IMAGE_MODE):     # IMAGE NOT AVAILABLE IN BROADCAST 
	    pass            

      elif(mode ==  EL_SETUP_MENU_MODE):  # setup menu: just blank display 
                                    # read gaze coords in case changed 
      	    if(tracker.isConnected() and can_read_pixel_coords):
               read_tracker_pixel_coords()

      elif(mode ==  EL_CALIBRATE_MODE):    # show calibration targets 
      	    print "cal if"
            if(tracker.isConnected() and can_read_pixel_coords):
               print "reading pixel cords"
               read_tracker_pixel_coords()
               try:
               	tracker.targetModeDisplay();
               except:
               	pass
      elif(mode ==  EL_VALIDATE_MODE or mode ==  EL_DRIFT_CORR_MODE):
      	try:
      		tracker.targetModeDisplay();
      	except:
      		pass
      elif(mode ==  EL_OPTIONS_MENU_MODE):  # no change in visibility 
            pass
      else:                    # any other mode: transparent key (black) 
            pass
      oldmode = mode




display.init()
display.set_mode((800, 600), FULLSCREEN |DOUBLEBUF |RLEACCEL |DOUBLEBUF ,32)
surf = display.get_surface()

openGraphics()
tracker = BroadcastEyelink()

tracker.setName("broadcast")
waitForConnection(tracker)	
tracker.broadcastOpen()
tracker.sendCommand("link_event_filter = LEFT,RIGHT,FIXATION,SACCADE,BLINK,MESSAGE,BUTTON")
tracker.sendCommand("link_sample_data  = LEFT,RIGHT,GAZE,GAZERES,AREA,STATUS")
track_mode_loop(tracker)





































































