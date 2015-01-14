#!/usr/bin/env python
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
# $Date: 2007/08/29 18:48:21 $
# 
#
"""Perspective-distorted sinusoidal grating in gaussian window"""

from pylink import *
from VisionEgg import *
from VisionEgg.Core import *
from VisionEgg.Gratings import *
from VisionEgg.SphereMap import *
from VisionEgg.Text import *
from VisionEgg.Textures import *
import VisionEgg.ParameterTypes as ve_types
import math, os
import pygame
import OpenGL.GL as gl

from EyeLinkCoreGraphicsVE import EyeLinkCoreGraphicsVE

import VisionEgg.GUI # Could import in beginning, but no need if not using GUI




VisionEgg.config.VISIONEGG_FULLSCREEN =1
window = VisionEgg.GUI.GraphicsConfigurationWindow()
window.mainloop() # All this does is adjust VisionEgg.config
if not window.clicked_ok:
	sys.exit() # User wants to quit
VisionEgg.Configuration.save_settings()
VisionEgg.config.VISIONEGG_GUI_INIT =0



tracker = EyeLink()
if tracker == None:
	print "Error: EyeLink is not connected!";
	sys.exit()

screen = get_default_screen()

genv = EyeLinkCoreGraphicsVE(screen,tracker)
openGraphicsEx(genv)

tracker.sendCommand("screen_pixel_coords =  0 0 %d %d" %(VisionEgg.config.VISIONEGG_SCREEN_W,VisionEgg.config.VISIONEGG_SCREEN_H))

tracker_software_ver = 0
eyelink_ver = getEYELINK().getTrackerVersion()
if eyelink_ver == 3:
	tvstr = getEYELINK().getTrackerVersionString()
	vindex = tvstr.find("EYELINK CL")
	tracker_software_ver = int(float(tvstr[(vindex + len("EYELINK CL")):].strip()))
	

if eyelink_ver>=2:
	getEYELINK().sendCommand("select_parser_configuration 0")
	if eyelink_ver == 2: #turn off scenelink camera stuff
		getEYELINK().sendCommand("scene_camera_gazemap = NO")
else:
	getEYELINK().sendCommand("saccade_velocity_threshold = 35")
	getEYELINK().sendCommand("saccade_acceleration_threshold = 9500")
	
# set EDF file contents 
getEYELINK().sendCommand("file_event_filter = LEFT,RIGHT,FIXATION,SACCADE,BLINK,MESSAGE,BUTTON")
if tracker_software_ver>=4:
	getEYELINK().sendCommand("file_sample_data  = LEFT,RIGHT,GAZE,AREA,GAZERES,STATUS,HTARGET")
else:
	getEYELINK().sendCommand("file_sample_data  = LEFT,RIGHT,GAZE,AREA,GAZERES,STATUS")

# set link data (used for gaze cursor) 
getEYELINK().sendCommand("link_event_filter = LEFT,RIGHT,FIXATION,SACCADE,BLINK,BUTTON")
if tracker_software_ver>=4:
	getEYELINK().sendCommand("link_sample_data  = LEFT,RIGHT,GAZE,GAZERES,AREA,STATUS,HTARGET")
else:
	getEYELINK().sendCommand("link_sample_data  = LEFT,RIGHT,GAZE,GAZERES,AREA,STATUS")
	
	
tracker.doTrackerSetup()
#pylink changes




#def get_mouse_position():
#    # convert to OpenGL coordinates
#    (x,y) = pygame.mouse.get_pos()
#    y = screen.size[1]-y
#    return x,y

def get_mouse_position():
	dt = tracker.getNewestSample() # check for new sample update
	if(dt != None):
		# Gets the gaze position of the latest sample,
		if dt.isRightSample():
			gaze_position = dt.getRightEye().getGaze()
			return gaze_position[0],screen.size[1]-gaze_position[1]
		elif dt.isLeftSample():
			gaze_position = dt.getLeftEye().getGaze()
			return gaze_position[0],screen.size[1]-gaze_position[1]
	return 0,screen.size[1]-0
		











mask = Mask2D(function='gaussian',   # also supports 'circle'
              radius_parameter=25,   # sigma for gaussian, radius for circle (units: num_samples)
              num_samples=(512,512)) # this many texture elements in mask (covers whole size specified below)

grating_stimulus = SinGrating2D(mask             = mask,
                                position         = ( screen.size[0]/2.0, screen.size[1]/2.0 ),
                                size             = ( 800 , 800 ),
                                spatial_freq     = 10.0 / screen.size[0], # units of cycles/pixel
                                temporal_freq_hz = 1.0,
                                num_samples      = 1024,
                                orientation      = 45.0 )

text_color = (0.0,0.0,1.0) # RGB ( blue)
xpos = 10.0
yspace = 5
text_params = {'anchor':'lowerleft','color':text_color,'font_size':20}

text_stimuli = []
ypos = 0
text_stimuli.append( Text( text = "Numeric keypad changes grating orientation.",
                           position=(xpos,ypos),**text_params))
ypos += text_stimuli[-1].parameters.size[1] + yspace

tf_text = Text(text = "'t/T' changes TF (now %.2f hz)"%(grating_stimulus.parameters.temporal_freq_hz),
               position=(xpos,ypos),**text_params)
text_stimuli.append( tf_text )
ypos += text_stimuli[-1].parameters.size[1] + yspace

text_stimuli.append( Text( text = "'-' shrinks window, '+' grows window (slow)",
                           position=(xpos,ypos),**text_params))
ypos += text_stimuli[-1].parameters.size[1] + yspace

sf_text = Text(text = "'s/S' changes SF (now %.3f cycles/pixel = %.1f pixels/cycle)"%(grating_stimulus.parameters.spatial_freq,1.0/grating_stimulus.parameters.spatial_freq),
               position=(xpos,ypos),**text_params)
text_stimuli.append( sf_text )
ypos += text_stimuli[-1].parameters.size[1] + yspace

text_stimuli.append( Text( text = "Mouse moves gabor, press Esc to quit",
                   position=(xpos,ypos),**text_params))
ypos += text_stimuli[-1].parameters.size[1] + yspace

text_stimuli.append( Text( text = "Demonstration of mouse controlled gabor.",
                           position=(xpos,ypos),**text_params))

viewport = Viewport(screen=screen,
                    stimuli=[grating_stimulus] + text_stimuli)

quit_now = False
shift_key = False
frame_timer = FrameTimer()




#The following does drift correction at the begin of each trial
while 1:
	# Does drift correction and handles the re-do camera setup situations
	error=tracker.doDriftCorrect(screen.size[0]/2,screen.size[1]/2,1,1)
	if error != 27: 
		break;
	else:
		tracker.doTrackerSetup();
	

error = tracker.startRecording(1,1,1,1)
	

	
	
while not quit_now:
    for event in pygame.event.get():
        if event.type == pygame.locals.QUIT:
            quit_now = True
        elif event.type == pygame.locals.KEYUP:
            if event.key in [pygame.locals.K_LSHIFT,pygame.locals.K_RSHIFT]:
                shift_key = False
        elif event.type == pygame.locals.KEYDOWN:
            if event.key == pygame.locals.K_ESCAPE:
                quit_now = True
            elif event.key in [pygame.locals.K_LSHIFT,pygame.locals.K_RSHIFT]:
                shift_key = True
            elif event.key == pygame.locals.K_KP1:
                grating_stimulus.parameters.orientation = 225.0
            elif event.key == pygame.locals.K_KP2:
                grating_stimulus.parameters.orientation = 270.0
            elif event.key == pygame.locals.K_KP3:
                grating_stimulus.parameters.orientation = 315.0
            elif event.key == pygame.locals.K_KP6:
                grating_stimulus.parameters.orientation = 0.0
            elif event.key == pygame.locals.K_KP9:
                grating_stimulus.parameters.orientation = 45.0
            elif event.key == pygame.locals.K_KP8:
                grating_stimulus.parameters.orientation = 90.0
            elif event.key == pygame.locals.K_KP7:
                grating_stimulus.parameters.orientation = 135.0
            elif event.key == pygame.locals.K_KP4:
                grating_stimulus.parameters.orientation = 180.0
            elif event.key == pygame.locals.K_s:
                if shift_key:
                    grating_stimulus.parameters.spatial_freq *= (1.0/1.5)
                else:
                    grating_stimulus.parameters.spatial_freq *= 1.5
                sf_text.parameters.text = "'s/S' changes SF (now %.3f cycles per pixel = %.1f pixels per cycle)"%(grating_stimulus.parameters.spatial_freq,1.0/grating_stimulus.parameters.spatial_freq)
            elif event.key == pygame.locals.K_t:
                if shift_key:
                    grating_stimulus.parameters.temporal_freq_hz *= (1.0/1.5)
                else:
                    grating_stimulus.parameters.temporal_freq_hz *= 1.5
                tf_text.parameters.text = "'t/T' changes TF (now %.2f hz)"%(grating_stimulus.parameters.temporal_freq_hz)
            elif event.key == pygame.locals.K_MINUS:
                old_params = grating_stimulus.parameters.mask.constant_parameters
                new_radius = old_params.radius_parameter * 0.8
                new_mask = Mask2D(function=old_params.function,
                                  radius_parameter=old_params.radius_parameter*0.8,
                                  num_samples=old_params.num_samples)
                grating_stimulus.parameters.mask = new_mask
            elif event.key == pygame.locals.K_EQUALS:
                old_params = grating_stimulus.parameters.mask.constant_parameters
                new_radius = old_params.radius_parameter * 0.8
                new_mask = Mask2D(function=old_params.function,
                                  radius_parameter=old_params.radius_parameter/0.8,
                                  num_samples=old_params.num_samples)
                grating_stimulus.parameters.mask = new_mask

    screen.clear()
    x,y = get_mouse_position()
    grating_stimulus.parameters.position = x,y
    viewport.draw()
    swap_buffers()
    frame_timer.tick()
tracker.stopRecording()

frame_timer.log_histogram()
