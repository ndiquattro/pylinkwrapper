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
# $Date: 2007/08/29 18:48:20 $
# 
#



import pygame
import pygame.mixer
import pygame.event
import pygame.image
import pygame.draw
import pygame.mouse
import array

import Image
import pylink
from pygame.constants import *
import os.path
import sys


script_home = os.path.dirname(sys.argv[0])


	
	
class EyeLinkCoreGraphicsPyGame(pylink.EyeLinkCustomDisplay):
	def __init__(self,w,h, tracker):
		pylink.EyeLinkCustomDisplay.__init__(self)
		pygame.init()
		pygame.display.init()
		pygame.mixer.init()
		pygame.display.set_mode((w, h), pygame.FULLSCREEN |pygame.DOUBLEBUF |pygame.RLEACCEL|pygame.HWSURFACE ,32)
		self.__target_beep__ = pygame.mixer.Sound(os.path.join(script_home,"caltargetbeep.wav"))
		self.__target_beep__done__ = pygame.mixer.Sound(os.path.join(script_home,"caltargetbeep.wav"))
		self.__target_beep__error__ = pygame.mixer.Sound(os.path.join(script_home,"caltargetbeep.wav"))
		pygame.mouse.set_visible(False)
		self.imagebuffer = array.array('l')
		self.pal = None	
		self.size = (0,0)
                
		if(not pygame.font.get_init()):
			pygame.font.init()
		self.fnt = pygame.font.Font(os.path.join(script_home,"cour.ttf"),20)
		self.fnt.set_bold(1)
		self.setTracker(tracker)
		self.last_mouse_state = -1
		
	
    
    
	def setTracker(self, tracker):
		self.tracker = tracker
		self.tracker_version = tracker.getTrackerVersion()
		if(self.tracker_version >=3):
			self.tracker.sendCommand("enable_search_limits=YES")
			self.tracker.sendCommand("track_search_limits=YES")
			self.tracker.sendCommand("autothreshold_click=YES")
			self.tracker.sendCommand("autothreshold_repeat=YES")
			self.tracker.sendCommand("enable_camera_position_detect=YES")
		
	


	def setup_cal_display (self):
		surf = pygame.display.get_surface()
		surf.fill((255,255,255,255))
		pygame.display.flip()
		surf.fill((255,255,255,255)) 
		
		
	def exit_cal_display(self): 
		self.clear_cal_display()
		
	def record_abort_hide(self):
		pass

	def clear_cal_display(self): 
		surf = pygame.display.get_surface()
		surf.fill((255,255,255,255))
		pygame.display.flip()	
		surf.fill((255,255,255,255))
		
	def erase_cal_target(self):
		surf = pygame.display.get_surface()
		surf.fill((255,255,255,255))
		
		
	def draw_cal_target(self, x, y): 
		outsz=10
		insz=4
		surf = pygame.display.get_surface()
		rect = pygame.Rect(x-outsz,y-outsz,outsz*2,outsz*2)
		pygame.draw.ellipse(surf,(0,0,0), rect)	
		rect = pygame.Rect(x-insz,y-insz,insz*2,insz*2)
		pygame.draw.ellipse(surf,(255,255,255), rect)	
		pygame.display.flip()
		
	def play_beep(self,beepid):
		if beepid == pylink.DC_TARG_BEEP or beepid == pylink.CAL_TARG_BEEP:
			self.__target_beep__.play()
		elif beepid == pylink.CAL_ERR_BEEP or beepid == pylink.DC_ERR_BEEP:
			self.__target_beep__error__.play()
		else:#	CAL_GOOD_BEEP or DC_GOOD_BEEP
			self.__target_beep__done__.play()
		
	
	def getColorFromIndex(self,colorindex):
		if colorindex   ==  pylink.CR_HAIR_COLOR:          return (255,255,255,255)
		elif colorindex ==  pylink.PUPIL_HAIR_COLOR:       return (255,255,255,255)
		elif colorindex ==  pylink.PUPIL_BOX_COLOR:        return (0,255,0,255)
		elif colorindex ==  pylink.SEARCH_LIMIT_BOX_COLOR: return (255,0,0,255)
		elif colorindex ==  pylink.MOUSE_CURSOR_COLOR:     return (255,0,0,255)
		else: return (0,0,0,0)
		
	
		
		
	def draw_line(self,x1,y1,x2,y2,colorindex):
		color = self.getColorFromIndex(colorindex)
		imr = self.__img__.get_rect()
		
		
		x1=int((float(x1)/float(self.size[0]))*imr.w)
		x2=int((float(x2)/float(self.size[0]))*imr.w)
		y1=int((float(y1)/float(self.size[1]))*imr.h)
		y2=int((float(y2)/float(self.size[1]))*imr.h)
		pygame.draw.line(self.__img__,color,(x1,y1),(x2,y2))
		
		
		
	
	def draw_lozenge(self,x,y,width,height,colorindex):
		
		ci = 100
		color = self.getColorFromIndex(colorindex)
		
		#guide
		#self.draw_line(x,y,x+width,y,ci) #hor
		#self.draw_line(x,y+height,x+width,y+height,ci) #hor
		#self.draw_line(x,y,x,y+height,ci) #ver
		#self.draw_line(x+width,y,x+width,y+height,ci) #ver
		
		
		imr = self.__img__.get_rect()
		x=int((float(x)/float(self.size[0]))*imr.w)
		width=int((float(width)/float(self.size[0]))*imr.w)
		y=int((float(y)/float(self.size[1]))*imr.h)
		height=int((float(height)/float(self.size[1]))*imr.h)
		
		 
		if width>height:
			rad = height/2
			
			#draw the lines
			pygame.draw.line(self.__img__,color,(x+rad,y),(x+width-rad,y))
			pygame.draw.line(self.__img__,color,(x+rad,y+height),(x+width-rad,y+height))
			
			#draw semicircles
			
			pos = (x+rad,y+rad)
			clip = (x,y,rad,rad*2)
			self.__img__.set_clip(clip)
			pygame.draw.circle(self.__img__,color,pos,rad,1)
			self.__img__.set_clip(imr)
			
			pos = ((x+width)-rad,y+rad)
			clip = ((x+width)-rad,y,rad,rad*2)
			self.__img__.set_clip(clip)
			pygame.draw.circle(self.__img__,color,pos,rad,1)
			self.__img__.set_clip(imr)
		else:
			rad = width/2

			#draw the lines
			pygame.draw.line(self.__img__,color,(x,y+rad),(x,y+height-rad))
			pygame.draw.line(self.__img__,color,(x+width,y+rad),(x+width,y+height-rad))

			#draw semicircles
			if rad ==0: 
				return #cannot draw sthe circle with 0 radius
			pos = (x+rad,y+rad)
			clip = (x,y,rad*2,rad)
			self.__img__.set_clip(clip)
			pygame.draw.circle(self.__img__,color,pos,rad,1)
			self.__img__.set_clip(imr)

			pos = (x+rad,y+height-rad)
			clip = (x,y+height-rad,rad*2,rad)
			self.__img__.set_clip(clip)
			pygame.draw.circle(self.__img__,color,pos,rad,1)
			self.__img__.set_clip(imr)

			

	def get_mouse_state(self):
		pos = pygame.mouse.get_pos()
		state = pygame.mouse.get_pressed()
		return (pos,state[0])
	def get_input_key(self):
		ky=[]
		v = pygame.event.get()
		for key in v:
			if key.type != KEYDOWN:
				continue
			keycode = key.key
			if keycode == K_F1:  keycode = pylink.F1_KEY
			elif keycode ==  K_F2:  keycode = pylink.F2_KEY
			elif keycode ==   K_F3:  keycode = pylink.F3_KEY
			elif keycode ==   K_F4:  keycode = pylink.F4_KEY
			elif keycode ==   K_F5:  keycode = pylink.F5_KEY
			elif keycode ==   K_F6:  keycode = pylink.F6_KEY
			elif keycode ==   K_F7:  keycode = pylink.F7_KEY
			elif keycode ==   K_F8:  keycode = pylink.F8_KEY
			elif keycode ==   K_F9:  keycode = pylink.F9_KEY
			elif keycode ==   K_F10: keycode = pylink.F10_KEY

			elif keycode ==   K_PAGEUP: keycode = pylink.PAGE_UP
			elif keycode ==   K_PAGEDOWN:  keycode = pylink.PAGE_DOWN
			elif keycode ==   K_UP:    keycode = pylink.CURS_UP
			elif keycode ==   K_DOWN:  keycode = pylink.CURS_DOWN
			elif keycode ==   K_LEFT:  keycode = pylink.CURS_LEFT
			elif keycode ==   K_RIGHT: keycode = pylink.CURS_RIGHT

			elif keycode ==   K_BACKSPACE:    keycode = ord('\b')
			elif keycode ==   K_RETURN:  keycode = pylink.ENTER_KEY
			elif keycode ==   K_ESCAPE:  keycode = pylink.ESC_KEY
			elif keycode ==   K_TAB:     keycode = ord('\t')
  			elif(keycode==pylink.JUNK_KEY): keycode= 0
  			
  			
  			
			ky.append(pylink.KeyInput(keycode,key.mod))
		return ky
		
	def exit_image_display(self):
		self.clear_cal_display()
		
	def alert_printf(self,msg): 
		print "alert_printf"
		
		
		
	
	def setup_image_display(self, width, height):
		self.size = (width,height)
		self.clear_cal_display()
		self.last_mouse_state = -1
		
	def image_title(self,  text): 
		text = text 
		sz = self.fnt.size(text[0])
		txt = self.fnt.render(text,len(text),(0,0,0,255), (255,255,255,255))
		surf = pygame.display.get_surface()
		imgsz=(self.size[0]*3,self.size[1]*3)
		topleft = ((surf.get_rect().w-imgsz[0])/2,(surf.get_rect().h-imgsz[1])/2)
		imsz=(topleft[0],topleft[1]+imgsz[1]+10)
		surf.blit(txt, imsz)
		pygame.display.flip()
		surf.blit(txt, imsz)
		
	def draw_image_line(self, width, line, totlines,buff):		
		#print "draw_image_line", len(buff)
		i =0
		while i <width:
			self.imagebuffer.append(self.pal[buff[i]])
			i= i+1
				
		
		if line == totlines:
			imgsz = (self.size[0]*3,self.size[1]*3)
			bufferv = self.imagebuffer.tostring()
			img =Image.new("RGBX",self.size)
			img.fromstring(bufferv)
			img = img.resize(imgsz)
			
			
			img = pygame.image.fromstring(img.tostring(),imgsz,"RGBX");
			
			self.__img__ = img
			self.draw_cross_hair()
			self.__img__ = None
			surf = pygame.display.get_surface()
			surf.blit(img,((surf.get_rect().w-imgsz[0])/2,(surf.get_rect().h-imgsz[1])/2))
			pygame.display.flip()
			surf.blit(img,((surf.get_rect().w-imgsz[0])/2,(surf.get_rect().h-imgsz[1])/2)) # draw on the back buffer too
			self.imagebuffer = array.array('l')
			
					
		
	def set_image_palette(self, r,g,b): 
		self.imagebuffer = array.array('l')
		self.clear_cal_display()
		sz = len(r)
		i =0
		self.pal = []
		while i < sz:
			rf = int(b[i])
			gf = int(g[i])
			bf = int(r[i])
			#self.pal.append((rf<<16) |  (gf<<8) | (bf)) #for windows
                        self.pal.append((bf<<24) |  (gf<<16) | (rf<<8)) #for mac
			i = i+1