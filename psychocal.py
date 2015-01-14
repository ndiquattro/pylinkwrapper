# Define custom calibration display using Psychopy
from psychopy import event, sound, visual
import pylink

class psychocal(pylink.EyeLinkCustomDisplay):
	'''This inherits a default class from pylink then adds psychopy stim.'''
	
	def __init__(self, w, h, tracker, window):
		pylink.EyeLinkCustomDisplay.__init__(self)
		self.sres = (w, h)
		
		# Set up window
		self.window = window
		self.window.flip(clearBuffer = True)
		
		# Define stimuli
		self.backcolor = window.color
		if sum(window.color) != 0:
			tcolout = -(window.color)
		else:
			tcolout = -1
										
		self.targetout = visual.Circle(self.window, pos = (0, 0), radius = 10,
										fillColor = tcolout,
										lineColor = tcolout, units = 'pix')
										
		self.targetin = visual.Circle(self.window, pos = (0, 0), radius = 3,
										fillColor = window.color,
										lineColor = window.color,
										units = 'pix')
										
		# Set up sounds
		self.__target_beep__ = sound.Sound(800, secs = .1)
		self.__target_beep__done__ = sound.Sound(1200, secs = .1)
		self.__target_beep__error__ = sound.Sound(400, secs = .1)

		# Define tracker
		self.setTracker(tracker)

	def setTracker(self, tracker):
		''' Initial camera set-up for calibration '''
		
		self.tracker = tracker
		self.tracker_version = tracker.getTrackerVersion()
		if(self.tracker_version >=3):
			self.tracker.sendCommand("enable_search_limits=YES")
			self.tracker.sendCommand("track_search_limits=YES")
			self.tracker.sendCommand("autothreshold_click=YES")
			self.tracker.sendCommand("autothreshold_repeat=YES")
			self.tracker.sendCommand("enable_camera_position_detect=YES")

	def setup_cal_display(self):
		intxt = 'Please follow the Dot. Try not to anticipate its movements.'
		txtob = visual.TextStim(self.window, intxt, pos = (0, 100),
								color = -(self.backcolor), units = 'pix')
								
		txtob.draw()
		self.targetout.draw()
		self.window.flip()		

	def exit_cal_display(self):
		self.clear_cal_display()

	def record_abort_hide(self):
		pass

	def clear_cal_display(self):
		self.setup_cal_display()	

	def erase_cal_target(self):
		self.window.flip()

	def draw_cal_target(self, x, y):
		# Convert to psychopy coordinates
		x = x - (self.sres[0] / 2)
		y = -(y - (self.sres[1] / 2))

		# Set calibration target position
		self.targetout.pos = (x, y)
		self.targetin.pos = (x, y)
	
		# Display
		self.targetout.draw()
		self.targetin.draw()
		self.window.flip()
		
	def play_beep(self, beepid):
		if beepid == pylink.DC_TARG_BEEP or beepid == pylink.CAL_TARG_BEEP:
			self.__target_beep__.play()
		elif beepid == pylink.CAL_ERR_BEEP or beepid == pylink.DC_ERR_BEEP:
			self.__target_beep__error__.play()
		else:#	CAL_GOOD_BEEP or DC_GOOD_BEEP
			self.__target_beep__done__.play()
		
	def getColorFromIndex(self, colorindex):
		if colorindex   ==  pylink.CR_HAIR_COLOR: return (255,255,255,255)
		elif colorindex ==  pylink.PUPIL_HAIR_COLOR: return (255,255,255,255)
		elif colorindex ==  pylink.PUPIL_BOX_COLOR:        return (0,255,0,255)
		elif colorindex ==  pylink.SEARCH_LIMIT_BOX_COLOR: return (255,0,0,255)
		elif colorindex ==  pylink.MOUSE_CURSOR_COLOR:     return (255,0,0,255)
		else: return (0,0,0,0)

	def draw_line(self, x1, y1, x2, y2, colorindex):
		pass

	def draw_lozenge(self, x, y, width, height, colorindex):
		pass

	def get_mouse_state(self):
		pass

	def get_input_key(self):
# 		kp = event.waitKeys()
# 		char = kp[0][0]
# 		print(char)
# 		
# 		pylink_key = None
# 		if char == "escape":
# 			pylink_key = pylink.ESC_KEY
# 		elif char == "return":
# 			pylink_key = pylink.ENTER_KEY
# 		elif char == " ":
# 			pylink_key = ord(char)
# 		elif char == "c":
# 			pylink_key = ord(char)
# 		elif char == "v":
# 			pylink_key = ord(char)
# 		elif char == "a":
# 			pylink_key = ord(char)
# 		elif char == "page_up":
# 			pylink_key = pylink.PAGE_UP
# 		elif char == "page_down":
# 			pylink_key = pylink.PAGE_DOWN
# 		elif char == "-":
# 			pylink_key = ord(char)
# 		elif char == "=":
# 			pylink_key = ord(char)
# 		elif char == "up":
# 			pylink_key = pylink.CURS_UP
# 		elif char == "down":
# 			pylink_key = pylink.CURS_DOWN
# 		elif char == "left":
# 			pylink_key = pylink.CURS_LEFT
# 		elif char == "right":
# 			pylink_key = pylink.CURS_RIGHT
# 		else:
# 			print('Error!')
# 			return
# 			
# 		# Return Key pressed
# 		k = pylink.KeyInput(pylink_key, 0)
# 		return k
		pass
		
	def exit_image_display(self):
		self.clear_cal_display()
		
	def alert_printf(self,msg): 
		print "alert_printf"
		
	def setup_image_display(self, width, height):
		pass
		
	def image_title(self,  text): 
		pass
		
	def draw_image_line(self, width, line, totlines,buff):		
		pass        
					
	def set_image_palette(self, r,g,b): 
		pass
