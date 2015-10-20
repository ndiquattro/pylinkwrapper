# Pylink wrapper for Psychopy
import pylink
import psychocal
import time
from psychopy.tools.monitorunittools import deg2pix
from psychopy import event

class connect(object):
    """Provides functions for interacting with the EyeLink via Pylink.
    
        Parameters
            window  -- Psychopy window object that the experiment uses
            edfname -- Desired name of the EDF file
    """

    def __init__(self, window, edfname):
        # Pull out monitor info
        self.sres = window.size
        self.win = window
        
        # Make filename
        self.edfname = edfname + '.edf'
        
        # Initialize connection with eye-tracker
        try:
            self.tracker = pylink.EyeLink()
            self.realconnect = True
        except:
            self.tracker = pylink.EyeLink(None)
            self.realconnect = False
            
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

    def calibrate(self, cnum = 13, paval = 1000):
        '''Calibrates eye-tracker using psychopy stimuli.
        
        Parameters
            cnum  -- Number of points to use for calibration. Default is 
                     13.
                     Options: 3, 5, 9, 13
                     
            paval -- Pacing of calibraiton, i.e., how long you have to      
                     fixate each target.
        '''
        
        # Generate custom calibration stimuli
        genv = psychocal.psychocal(self.sres[0], self.sres[1],
                                    self.tracker, self.win)
                                    
        if self.realconnect:
            # Set calibration type
            calst = 'HV{}'.format(cnum)
            self.tracker.setCalibrationType(calst)
            
            # Set calibraiton pacing
            self.tracker.setAutoCalibrationPacing(paval)
            
            # Execute custom calibration display
            pylink.openGraphicsEx(genv)
            
            # Calibrate
            self.tracker.doTrackerSetup(self.sres[0], self.sres[1])
        else:
            genv.dummynote()
        
    def setStatus(self, message):
        """
        Sets status message to appear while recording.

        :param message: Text object to send, must be < 80 char
        :type message: str
        """
        msg = "record_status_message '{}'".format(message)
        self.tracker.sendCommand(msg)
        
    def setTrialID(self, idval = 1):
        '''Sends message that indicates start of trial in EDF.
        
        Parameters
            idval = Values to set TRIALID. Defaults to 1
        '''
        
        tid = 'TRIALID {}'.format(idval)
        self.tracker.sendMessage(tid)
        
    def recordON(self, sendlink = False):
        '''Starts recording, waits 50ms to allow eyelink to prepare.
        
        Parameters
            sendlink -- Toggle for sending eye data over the link to the       
                        display computer during recording. Defaults to False
        '''
        
        self.tracker.sendCommand('set_idle_mode')
        time.sleep(.05)
        if sendlink:
            self.tracker.startRecording(1, 1, 1, 1)
        else:
            self.tracker.startRecording(1, 1, 0, 0)

    def recordOFF(self):
        '''Stops recording.'''
        
        self.tracker.stopRecording()
        
    def drawIA(self, x, y, size, index, color, name):
        '''Draws square interest area in EDF and a corresponding filled box on
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
            name  -- string for name of variable
            value -- string or number for variable value
        '''
        
        # Make string
        varmsg = '!V TRIAL_VAR {} {}'.format(name, value)
    
        # Send message
        self.tracker.sendMessage(varmsg)
        
    def setTrialResult(self, rval = 0, scrcol = 0):
        '''Sends trial result to indiciate trial end in EDF and clears    
           screen on EyeLink Display.
           
        Parameters
            rval   -- Value to set for TRIAL_RESULT. Defaults to 0
            scrcol -- Color to clear screen to. Defaults to 0 (black)
        '''
        
        trmsg = 'TRIAL_RESULT {}'.format(rval)
        cscmd = 'clear_screen {}'.format(scrcol)
        
        self.tracker.sendMessage(trmsg)
        self.tracker.sendCommand(cscmd)
        
    def endExperiment(self, spath):
        '''Closes and transfers the EDF file.
        
        Parameters
            spath -- File path of where to save EDF file. Include trailing 
                     slash
        '''
        
        # File transfer and cleanup!
        self.tracker.setOfflineMode()
        time.sleep(.5)
        
        # Generate file path
        fpath = spath + self.edfname
        
        #Close the file and transfer it to Display PC
        self.tracker.closeDataFile()
        time.sleep(1)
        self.tracker.receiveDataFile(self.edfname, fpath)
        self.tracker.close()
        
    def fixCheck(self, size, ftime, button):
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

        # Set status message & Draw box
        self.setStatus('Fixation Check')
        bxmsg = 'draw_box {} {} {} {} 1'.format(xbdr[0], ybdr[0], xbdr[1],
                                                ybdr[1])
        self.tracker.sendCommand(bxmsg)
        
        # Begin recording
        self.tracker.startRecording(0, 0, 1, 1)
        
        # Check which eye is being recorded
        eye_used = self.tracker.eyeAvailable()
        RIGHT_EYE = 1
        LEFT_EYE = 0
        
        # Begin polling
        keys = []
        fixtime = time.clock()
        while self.realconnect:  # only start check loop if real connection

            # Check for recalibration button
            keys = event.getKeys(button)
            if keys:
                self.tracker.stopRecording()
                self.calibrate()
                break 
            
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
                
    def sendMessage(self, txt):
        '''Sends a message to the tracker that is recorded in the EDF
        
        Parameters
            txt -- String that contains the message
        '''
        
        # Send message
        self.tracker.sendMessage(txt)
        
    def sendCommand(self, cmd):
        '''Sends a command to EyeLink
        
        Parameters
            cmd -- String that contains the command
        '''
        
        # Send Command
        self.tracker.sendCommand(cmd)
        
    def drawText(self, msg):
        '''Draws text on eye-tracker screen
        
        Parameters
            msg -- String that contains text.
        '''
        # Figure out center
        x = self.sres[0] / 2
        
        # Send message
        txt = '"{}"'.format(msg)
        self.tracker.drawText(text, (x, 50))