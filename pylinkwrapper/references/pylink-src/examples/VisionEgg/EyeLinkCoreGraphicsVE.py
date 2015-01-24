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
# $Date: 2007/08/29 18:48:22 $
# 
#


import pylink

from pygame import *
import time

import sys

import pygame.mixer
import pygame.event
from pygame.constants import *
import array
import pygame.image
import Image
import PIL.ImageDraw
import pygame.draw
import os
import os.path

script_home = os.path.dirname(sys.argv[0])
sys.argv="EB Session","EB Session"
from VisionEgg import *
VisionEgg.start_default_logging(); VisionEgg.watch_exceptions()
from VisionEgg.Core import *
from VisionEgg.MoreStimuli import *
from VisionEgg.Text import *
from VisionEgg.Textures import *







class EyeLinkCoreGraphicsVE(pylink.EyeLinkCustomDisplay):
	def __init__(self,screen,tracker):
		self.size=screen.size
		self.tracker=tracker
		if sys.byteorder == 'little':
			self.byteorder = 1
		else:
			self.byteorder = 0
		pylink.EyeLinkCustomDisplay.__init__(self)
		try:
	            	pygame.mixer.init()
			self.__target_beep__ = pygame.mixer.Sound(os.path.join(script_home,"caltargetbeep.wav"))
			self.__target_beep__done__ = pygame.mixer.Sound(os.path.join(script_home,"caltargetbeep.wav"))
			self.__target_beep__error__ = pygame.mixer.Sound(os.path.join(script_home,"caltargetbeep.wav"))
		except:
			self.__target_beep__ = None
			self.__target_beep__done__ = None
			self.__target_beep__error__ = None
	
		self.imagebuffer = array.array('L')
		self.pal = None	

		# Create viewport for calibration / DC 
		
		cal_screen = screen
		cal_screen.parameters.bgcolor = (1.0,1.0,1.0,1.0)			

		target = Target2D(size  = (10.0,10.0),
                  		color      = (0.0,0.0,0.0,1.0), # Set the target color (RGBA) black
                  		orientation = 0.0)

		self.cal_vp = Viewport(screen=cal_screen, stimuli=[target])		

		# Create viewport for camera image screen 
		text = Text(text="Eye Label",
            		color=(0.0,0.0,0.0), # alpha is ignored (set with max_alpha_param)
            		position=(cal_screen.size[0]/2,int(screen.size[1]*0.1)),
            		font_size=20,
            		anchor='center')

                     
		img =Image.new("RGBX",(int(screen.size[0]*0.75),int(screen.size[1]*0.75)))
		image = TextureStimulus(mipmaps_enabled=0,
			   texture=Texture(img),
			   size=(int(screen.size[0]*0.75),int(screen.size[1]*0.75)),
			   texture_min_filter=gl.GL_LINEAR,
			   position=(cal_screen.size[0]/2.0,cal_screen.size[1]/2.0),
			   anchor='center')

		self.image_vp = Viewport(screen=cal_screen, stimuli=[text,image])		
		
		self.width=cal_screen.size[0]
		self.height=cal_screen.size[1]
		
		
	def setup_cal_display (self):
		self.cal_vp.parameters.screen.clear()
		VisionEgg.Core.swap_buffers()
				
	def exit_cal_display(self): 
		self.clear_cal_display()
		
	def record_abort_hide(self):
		pass

	def clear_cal_display(self): 
		self.cal_vp.parameters.screen.clear()
		VisionEgg.Core.swap_buffers()
		
	def erase_cal_target(self):
		self.cal_vp.parameters.screen.clear()
		VisionEgg.Core.swap_buffers()
		
		
	def draw_cal_target(self, x, y): 
		self.cal_vp.parameters.stimuli[0].parameters.position=(x,y)			
		self.cal_vp.parameters.screen.clear()
		self.cal_vp.draw()
		VisionEgg.Core.swap_buffers()
		

	def play_beep(self,beepid):
		if beepid == pylink.DC_TARG_BEEP or beepid == pylink.CAL_TARG_BEEP:
			self.__target_beep__.play()
		elif beepid == pylink.CAL_ERR_BEEP or beepid == pylink.DC_ERR_BEEP:
			self.__target_beep__error__.play()
		else:#	CAL_GOOD_BEEP or DC_GOOD_BEEP
			self.__target_beep__done__.play()

	def translate_key_message(self,event):
		if event.type == KEYDOWN:
			if event.key == K_F1:  
				key = pylink.F1_KEY;
			elif event.key == K_F2:  
				key = pylink.F2_KEY;
			elif event.key == K_F3:  
				key = pylink.F3_KEY;
			elif event.key == K_F4:  
				key = pylink.F4_KEY;
			elif event.key == K_F5:  
				key = pylink.F5_KEY;
			elif event.key == K_F6:  
				key = pylink.F6_KEY;
			elif event.key == K_F7:  
				key = pylink.F7_KEY;
			elif event.key == K_8:  
				key = pylink.F8_KEY;
			elif event.key == K_F9:  
				key = pylink.F9_KEY;
			elif event.key == K_F10:  
				key = pylink.F10_KEY;
			elif event.key == K_PAGEUP:  
				key = pylink.PAGE_UP;
			elif event.key == K_PAGEDOWN:  
				key = pylink.PAGE_DOWN;
			elif event.key == K_UP:  
				key = pylink.CURS_UP;
			elif event.key == K_DOWN:  
				key = pylink.CURS_DOWN;
			elif event.key == K_LEFT:  
				key = pylink.CURS_LEFT;
			elif event.key == K_RIGHT:  
				key = pylink.CURS_RIGHT;
			elif event.key == K_BACKSPACE:  
				key = '\b';
			elif event.key == K_RETURN:  
				key = pylink.ENTER_KEY;
			elif event.key == K_ESCAPE:  
				key = pylink.ESC_KEY;
			elif event.key == K_TAB:  
				key = '\t';			
			else:
 				key = event.key;

			if key == pylink.JUNK_KEY:
				return 0
			return key	

		return 0

	def get_input_key(self):
		ky=[]
		for key in pygame.event.get([KEYDOWN]):
			try:
				tkey = self.translate_key_message(key)
				ky.append(pylink.KeyInput(tkey,key.mod))
			except Exception, err:
				print err
		return ky
	
	def get_mouse_state(self):
		pos = pygame.mouse.get_pos()
		state = pygame.mouse.get_pressed()
		return (pos,state[0])	
	
	def exit_image_display(self):
		self.image_vp.parameters.screen.clear()
		VisionEgg.Core.swap_buffers()
		
	def alert_printf(self,msg): 
		print "alert_printf"		
	
	def setup_image_display(self, width, height):
		self.img_size = (width,height)
		self.image_vp.parameters.screen.clear()
		
	def image_title(self, text): 
		self.image_vp.parameters.stimuli[0].parameters.text=text			
		
	def draw_image_line(self, width, line, totlines,buff):
                i =0
		while i <width:
			if buff[i]>=len(self.pal):
				buff[i]=len(self.pal)-1
			self.imagebuffer.append(self.pal[buff[i]&0x000000FF])
	                #self.imagebuffer.append(self.pal[buff[i]])
			i= i+1
		
		if line == totlines:
			img =Image.new("RGBX",self.img_size)
			img.fromstring(self.imagebuffer.tostring())
			img = img.resize(self.image_vp.parameters.stimuli[1].parameters.size)
			
			self.__img__=img
			self.draw_cross_hair()
			self.__img__=None
			self.image_vp.parameters.stimuli[1].texture_object.put_sub_image(img)			

			self.image_vp.parameters.screen.clear()
			self.image_vp.draw()
			
			VisionEgg.Core.swap_buffers()

			self.imagebuffer = array.array('l')
	
	
	def draw_lozenge(self,x,y,width,height,colorindex):
		if colorindex   ==  pylink.CR_HAIR_COLOR:          color = (255,255,255,255)
		elif colorindex ==  pylink.PUPIL_HAIR_COLOR:       color = (255,255,255,255)
		elif colorindex ==  pylink.PUPIL_BOX_COLOR:        color = (0,255,0,255)
		elif colorindex ==  pylink.SEARCH_LIMIT_BOX_COLOR: color = (255,0,0,255)
		elif colorindex ==  pylink.MOUSE_CURSOR_COLOR:     color = (255,0,0,255)
		else: color =(0,0,0,0)

		
		
		imr = self.__img__.size
		x=int((float(x)/float(self.img_size[0]))*imr[0])
		width=int((float(width)/float(self.img_size[0]))*imr[0])
		y=int((float(y)/float(self.img_size[1]))*imr[1])
		height=int((float(height)/float(self.img_size[1]))*imr[1])
		
		idraw = PIL.ImageDraw.Draw(self.__img__)
		if width>height:
			rad = height/2
			
			#draw the lines
			idraw.line([(x+rad,y),(x+width-rad,y)],fill=color)
			idraw.line([(x+rad,y+height),(x+width-rad,y+height)],fill=color)
			
			#draw semicircles			
			clip = (x,y,x+height,y+height)
			idraw.arc(clip,90,270,fill=color)
			
			
			clip = ((x+width-height),y,x+width,y+height)
			idraw.arc(clip,270,90,fill=color)
			
		else:
			rad = width/2

			#draw the lines
			idraw.line([(x,y+rad),(x,y+height-rad)],fill=color)
			idraw.line([(x+width,y+rad),(x+width,y+height-rad)],fill=color)

			#draw semicircles
			clip = (x,y,x+width,y+width)
			idraw.arc(clip,180,360,fill=color)
			
			
			clip = (x,y+height-width,x+width,y+height)
			idraw.arc(clip,360,180,fill=color)

			

	
	def draw_line(self,x1,y1,x2,y2,colorindex):
		imr = self.__img__.size
		x1=int((float(x1)/float(self.img_size[0]))*imr[0])
		x2=int((float(x2)/float(self.img_size[0]))*imr[0])
		y1=int((float(y1)/float(self.img_size[1]))*imr[1])
		y2=int((float(y2)/float(self.img_size[1]))*imr[1])
		idraw = PIL.ImageDraw.Draw(self.__img__)
		if colorindex   ==  pylink.CR_HAIR_COLOR:          color = (255,255,255,255)
		elif colorindex ==  pylink.PUPIL_HAIR_COLOR:       color = (255,255,255,255)
		elif colorindex ==  pylink.PUPIL_BOX_COLOR:        color = (0,255,0,255)
		elif colorindex ==  pylink.SEARCH_LIMIT_BOX_COLOR: color = (255,0,0,255)
		elif colorindex ==  pylink.MOUSE_CURSOR_COLOR:     color = (255,0,0,255)
		else: color =(0,0,0,0)
		idraw.line([(x1,y1),(x2,y2)], fill=color)
		
		
	

	def set_image_palette(self, r,g,b):
		self.imagebuffer = array.array('L')
		self.clear_cal_display()
		sz = len(r)
		i =0
		self.pal = []
		while i < sz:
			rf = int(b[i])
			gf = int(g[i])
			bf = int(r[i])
			if self.byteorder:
				self.pal.append((rf<<16) | (gf<<8) | (bf))
			else:
	                        self.pal.append((bf<<24) |  (gf<<16) | (rf<<8)) #for mac
			i = i+1

