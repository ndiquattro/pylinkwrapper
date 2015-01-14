# Pylink wrapper for Psychopy
import pylink
import psychocal
import time
from psychopy.tools.monitorunittools import deg2pix
from psychopy import event

class eyeLinkFuns(object):
    """Provides functions for interacting with Pylink."""

    def __init__(self, window, edfname):
        # Pull out monitor info
        self.sres = window.size
        self.win = window
        
        # Make filename
        self.edfname = edfname + '.edf'
        
        # Initialize connection with eye-tracker
        self.tracker = pylink.EyeLink()
        
        # Make pylink accessible
        self.pylink = pylink
        
        # Open EDF
        self.tracker.openDataFile(self.edfname)		
        pylink.flushGetkeyQueue() 
        self.tracker.setOfflineMode()

        # Set content of edf file
        eftxt = 'LEFT,RIGHT,FIXATION,SACCADE,BLINK,MESSAGE,BUTTON,INPUT'
        self.tracker.sendCommand('file_event_filter = ' + eftxt)
    
        lftxt = 'LEFT,RIGHT,FIXATION,SACCADE,BLINK,BUTTON'
        self.tracker.sendCommand('link_event_filter = ' + lftxt)
        
        lstxt = 'LEFT,RIGHT,GAZE,GAZERES,AREA,STATUS,HTARGET'
        self.tracker.sendCommand('link_sample_data = ' + lstxt)
        
        fstxt ='LEFT,RIGHT,GAZE,AREA,GAZERES,STATUS,HTARGET,INPUT'
        self.tracker.sendCommand('file_sample_data = ' + fstxt)
        
        # Set display coords for dataviewer
        disptxt = 'DISPLAY_COORDS 0 0 {} {}'.format(*self.sres)
        self.tracker.sendMessage(disptxt)

    def calibrate(self):
        '''Calibrates eye-tracker using psychopy stimuli.
        
        Parameters
            bgcolor -- background color of calibration display
            tcolout -- color of outer circle calibration target
            tcolin  -- color of inner circle calibration target
        '''
        
        # Generate custom calibration stimuli
        genv = psychocal.psychocal(self.sres[0], self.sres[1], self.tracker,
                                   self.win)
        pylink.openGraphicsEx(genv)
        
        # Calibrate and close window
        self.tracker.doTrackerSetup(self.sres[0], self.sres[1])
        
    def setStatus(self, message):
        '''Sets status message to appear while recording.
        
        Parameters
            message -- Text object to send, must < 80 char
        '''
        
        msg = "record_status_message '%s'" % message
        self.tracker.sendCommand(msg)
        
    def setTrialID(self):
        '''Sends message that indicates start of trial in EDF.'''
        
        self.tracker.sendMessage('TRIALID 1')
        
    def recordON(self):
        '''Starts recording, waits 50ms to allow eyelink to prepare.'''
        
        self.tracker.sendCommand('set_idle_mode')
        time.sleep(.05)
        self.tracker.startRecording(1, 1, 0, 0)

    def recordOFF(self):
        '''Stops recording.'''
        
        self.tracker.stopRecording()
        
    def drawIA(self, x, y, size, index, color, name):
        '''Draws square interest area in EDF and a  corresponding filled box on
           eye-tracker display.
        
        Parameters
            x, y  -- x and y coordinates in degrees visual angle
            size  -- length of one edge of square in degrees visual angle
            index -- number to assign interest area in EDF
            color -- color of box drawn on eye-tracker display (0 - 15)
            name  -- string to name interest area in EDF
        '''
            
        # Convert units to eyelink space
        elx = deg2pix(x, self.win.monitor) + (self.sres[0] / 2.0)
        ely = -(deg2pix(y, self.win.monitor) - (self.sres[1] / 2.0))
        elsz = deg2pix(size, self.win.monitor) / 2.0
    
        # Make top left / bottom right coordinates for square
        tplf = map(round, [elx - elsz, ely - elsz])
        btrh = map(round, [elx + elsz, ely + elsz])
    
        # Construct command strings
        flist = [index, name, color] + tplf + btrh
        iamsg = '!V IAREA RECTANGLE {0} {3} {4} {5} {6} {1}'.format(*flist)
        bxmsg = 'draw_filled_box {3} {4} {5} {6} {2}'.format(*flist)

        # Send commands
        self.tracker.sendMessage(iamsg)
        self.tracker.sendCommand(bxmsg)
        
    def sendVar(self, name, value):
        '''Sends trial variable to EDF file.
        
        Parameters
            name  -- string for name of variable.
            value -- string or number for variable value
        '''
        
        # Make string
        varmsg = '!V TRIAL_VAR {} {}'.format(name, value)
    
        # Send message
        self.tracker.sendMessage(varmsg)
        
    def setTrialResult(self):
        '''Sends trial result to indiciate trial end in EDF.'''
        
        self.tracker.sendMessage('TRIAL_RESULT 0')
        self.tracker.sendCommand('clear_screen 0')
        
    def endExperiment(self, spath):
        '''Closes and transfers the EDF file.
        
        Parameters
            spath -- File path of where to save EDF file. Include trailing \
        '''
        
        # File transfer and cleanup!
        self.tracker.setOfflineMode()
        time.sleep(.5)
        
        # Generate file path
        fpath = spath + self.edfname
        
        #Close the file and transfer it to Display PC
        self.tracker.closeDataFile()
        self.tracker.receiveDataFile(self.edfname, fpath)
        self.tracker.close()
        
    def fixcheck(self, size, ftime, button):
        '''Checks that fixation is maintained for certain time.
        
        Parameters
            size   -- length of one side of box in degrees visual angle
            ftime  -- length of time to check for fixation in seconds
            button -- key to press to recalibrate eye-tracker
        '''
        
        # Calculate Fix check borders
        cenX = self.sres[0] / 2.0
        cenY = self.sres[1] / 2.0
        size = deg2pix(size, self.win.monitor) / 2.0
        
        xbdr = [cenX - size, cenX + size]
        ybdr = [cenY - size, cenY + size]

        # Set status message
        self.setStatus('Fixation Check')
        
        # Begin recording
        self.tracker.startRecording(0, 0, 1, 1)
        
        # Check which eye is being recorded
        eye_used = self.tracker.eyeAvailable()
        RIGHT_EYE = 1
        LEFT_EYE = 0
        
        # Begin polling
        keys = []
        fixtime = time.clock()
        while True:

            # Check for recalibration button
            keys = event.getKeys(button)
            if keys:
                self.tracker.stopRecording()
                self.calibrate() 
            
            # Grab latest sample
            sample = self.tracker.getNewestSample()
            
            # Extract gaze coordinates
            if eye_used == RIGHT_EYE:
                gaze = sample.getRightEye().getGaze()
            else:
                gaze = sample.getLeftEye().getGaze()
                
            # Are we in the box?
            if xbdr[0] < gaze[0] < xbdr[1] and ybdr[0] < gaze[1] < ybdr[1]:
                # Have we been in the box long enough?
                if (time.clock() - fixtime) > ftime:
                    self.tracker.stopRecording()
                    break
            else:
                # Reset clock if not in box
                fixtime = time.clock()
                
        