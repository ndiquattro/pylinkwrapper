# Pylink wrapper for Psychopy
import os
import pylink
from . import psychocal
import time
import re
from psychopy.tools.monitorunittools import deg2pix, pix2deg
from psychopy import event


class Connect(object):
    """
    Provides functions for interacting with the EyeLink via Pylink.

    :param window: Psychopy window object.
    :param edfname: Desired name of the EDF file.
    :type edfname: str
    """

    def __init__(self, window, edfname, address="100.1.1.1"):
        # Pull out monitor info
        self.sres = window.size
        self.scenter = [self.sres[0] / 2.0, self.sres[1] / 2.0]
        self.win = window

        # Make filename
        fname = os.path.splitext(edfname)[0]  # strip away extension if present
        assert re.match(r'\w+$', fname), 'Name must only include A-Z, 0-9, or _'
        assert len(fname) <= 8, 'Name must be <= 8 characters.'

        self.edfname = fname + '.edf'

        # Initialize connection with eye-tracker
        try:
            self.tracker = pylink.EyeLink(address)
            self.realconnect = True
        except RuntimeError:
            self.tracker = pylink.EyeLink(None)
            self.realconnect = False

        # Check which eye is being recorded
        self.eye_used = self.tracker.eyeAvailable()

        # Make pylink accessible
        self.pylink = pylink

        # Open EDF
        #Close any open file first
        if self.tracker.getTrackerStatus() == pylink.EL_RECORDING:
            self.tracker.closeDataFile()
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

        fstxt = 'LEFT,RIGHT,GAZE,AREA,GAZERES,STATUS,HTARGET,INPUT'
        self.tracker.sendCommand('file_sample_data = ' + fstxt)

        # Set display coords for dataviewer
        disptxt = 'DISPLAY_COORDS 0 0 {} {}'.format(*self.sres)
        self.tracker.sendMessage(disptxt)

    def calibrate(self, cnum=13, paval=1000):
        """
        Calibrates eye-tracker using psychopy stimuli.

        :param cnum: Number of points to use for calibration. Options are 3, 5,
                     9, 13.
        :type cnum: int
        :param paval: Pacing of calibration, i.e. how long you have to fixate
                      each target in milliseconds.
        :type paval: int
        """

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
            print('*' * 150)
            print('Calibration Mode')
            print('*' * 150)
            pylink.openGraphicsEx(genv)

            # Calibrate
            self.tracker.doTrackerSetup(self.sres[0], self.sres[1])
        else:
            genv.dummynote()

    def set_status(self, message):
        """
        Sets status message to appear while recording.

        :param message: Text object to send, must be < 80 char
        :type message: str
        """
        msg = "record_status_message '{}'".format(message)
        self.tracker.sendCommand(msg)

    def set_trialid(self, idval=1):
        """
        Sends message that indicates start of trial in EDF.

        :param idval: Values to set TRIALID.
        """

        tid = 'TRIALID {}'.format(idval)
        self.tracker.sendMessage(tid)

    def record_on(self, sendlink=False):
        """
        Starts recording. Waits 50ms to allow eyelink to prepare.

        :param sendlink: Toggle for sending eye data over the link to the
                         display computer during recording.
        :type sendlink: bool
        """

        self.tracker.sendCommand('set_idle_mode')
        time.sleep(.05)
        if sendlink:
            self.tracker.startRecording(1, 1, 1, 1)
        else:
            self.tracker.startRecording(1, 1, 0, 0)

    def record_off(self):
        """
        Stops recording.
        """
        self.tracker.stopRecording()

    def draw_ia(self, x, y, size, index, color, name):
        """
        Draws square interest area in EDF and a corresponding filled box on
        eye-tracker display. Must be called after :py:func:`set_trialid` for
        interest areas to appear in the EDF.

        :param x: X coordinate in degrees visual angle for center of check area.
        :type x: float or int
        :param y: Y coordinate in degrees visual angle for center of check area.
        :type y: float or int
        :param size: length of one edge of square in degrees visual angle.
        :type size: float or int
        :param index: number to assign interest area in EDF
        :type index: int
        :param color: color of box drawn on eye-tracker display (0 - 15)
        :type color: int
        :param name: Name of interest area in EDF
        :type name: str
        """

        # Convert units to eyelink space
        elx, ely = self.convert_coords(x, y)
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

    def send_var(self, name, value):
        """
        Sends a trial variable to the EDF file.

        :param name: Name of variable.
        :type name: str
        :param value: Value of variable.
        :type value: float, str, or int
        """

        # Make string
        varmsg = '!V TRIAL_VAR {} {}'.format(name, value)

        # Send message
        self.tracker.sendMessage(varmsg)

    def set_trialresult(self, rval=0, scrcol=0):
        """
        Sends trial result to indiciate trial end in EDF and clears screen on
        EyeLink Display.

        :param rval: Value to set for TRIAL_RESULT.
        :type rval: float, str, or int
        :param scrcol: Color to clear screen to. Defaults to black.
        :type scrcol: int
        """

        trmsg = 'TRIAL_RESULT {}'.format(rval)
        cscmd = 'clear_screen {}'.format(scrcol)

        self.tracker.sendMessage(trmsg)
        self.tracker.sendCommand(cscmd)

    def end_experiment(self, spath):
        """
        Closes and transfers the EDF file.

        :param spath: Absolute file path of where to save the EDF file.
        :type spath: str
        """

        # Rest the eyetracker
        self.tracker.setOfflineMode()
        time.sleep(.5)

        # Generate file path
        fpath = os.path.join(spath, self.edfname)

        # Close the file and transfer it to Display PC
        self.tracker.closeDataFile()
        time.sleep(1)
        assert os.path.isdir(spath), 'EDF destination directory does not exist.'
        self.tracker.receiveDataFile(self.edfname, fpath)
        self.tracker.close()

    def fix_check(self, size, ftime, button):
        """
        Checks that fixation is maintained for certain time.

        :param size: Length of one side of box in degrees visual angle.
        :type size: float or int
        :param ftime: Length of time to check for fixation in seconds.
        :type ftime: float
        :param button: Key to press to recalibrate eye-tracker.
        :type button: char
        """

        # Calculate Fix check borders
        size = deg2pix(size, self.win.monitor) / 2.0

        xbdr = [self.scenter[0] - size, self.scenter[0] + size]
        ybdr = [self.scenter[1] - size, self.scenter[1] + size]

        # Set status message & Draw box
        self.set_status('Fixation Check')
        bxmsg = 'draw_box {} {} {} {} 1'.format(xbdr[0], ybdr[0], xbdr[1],
                                                ybdr[1])
        self.tracker.sendCommand(bxmsg)

        # Begin recording
        self.tracker.startRecording(0, 0, 1, 1)

        # Begin polling
        fixtime = time.clock()
        while self.realconnect:  # only start check loop if real connection

            # Check for recalibration button
            keys = event.getKeys(button)
            if keys:
                self.tracker.stopRecording()
                self.calibrate()
                break

            gaze = self.get_gaze()

            # Are we in the box?
            if xbdr[0] < gaze[0] < xbdr[1] and ybdr[0] < gaze[1] < ybdr[1]:
                # Have we been in the box long enough?
                if (time.clock() - fixtime) > ftime:
                    self.tracker.stopRecording()
                    break
            else:
                # Reset clock if not in box
                fixtime = time.clock()

    def send_message(self, txt):
        """
        Sends a message to the tracker that is recorded in the EDF.

        :param txt: Message to send.
        :type txt: str
        """

        # Send message
        self.tracker.sendMessage(txt)

    def send_command(self, cmd):
        """
        Sends a command to the Eyelink.

        :param cmd: Command to send.
        :type cmd: str
        """

        # Send Command
        self.tracker.sendCommand(cmd)

    def draw_text(self, msg):
        """
        Draws text on eye-tracker screen.

        :param msg: Text to draw.
        :type msg: str
        """

        # Figure out center
        x = self.scenter[0]

        # Send message
        txt = '"{}"'.format(msg)
        self.tracker.drawText(txt, (x, 50))

    def get_gaze(self):
        """
        Gets current gaze position of eye. Must be called between
        :py:func:`record_on` and :py:func:`record_off`. Sendlink must be set to
        True as well.

        :return: list of coordinates in the form of [x, y].
        """

        if self.realconnect:
            # Grab latest sample
            sample = self.tracker.getNewestSample()

            # Extract gaze coordinates
            if self.eye_used == 65535:
                gaze = sample.getRightEye().getGaze()
            else:
                gaze = sample.getLeftEye().getGaze()

            return gaze

    def convert_coords(self, x, y, to='eyelink'):
        """
        Converts from degrees visual angle units to EyeLink Pixel units.

        :param x: X coordinate in visual angle.
        :type x: float or int
        :param y: Y coordinate in viusal angle.
        :type y: float or int
        :param to: Direction of conversion. Options: 'eyelink' or 'psychopy'.
        :return: Two values in order x, y
        """
        if to == 'eyelink':
            # Convert coordinates to Eyelink space
            elx = deg2pix(x, self.win.monitor) + self.scenter[0]
            ely = -(deg2pix(y, self.win.monitor) - self.scenter[1])

        elif to == 'psychopy':
            elx = pix2deg(x - self.scenter[0], self.win.monitor)
            ely = pix2deg(-(y - self.scenter[1]), self.win.monitor)

        return [elx, ely]

    def sac_detect(self, x, y, radius):
        """
        Checks if current gaze position is outside a circular interest area.

        :param x: X coordinate in degrees visual angle for center of circle IA.
        :type x: float or int
        :param y: Y coordinate in degrees visual angle for center of circle IA.
        :type y: float or int
        :param radius: Radius of detection circle in degrees visual angle.
        :type radius: float or int
        :return: List of whether saccade was detected and the gaze coordinates
        at time of detection. In the form of [bool, [x, y]].
        :rtype: bool, list
        """

        # Convert coordinates to Eyelink space
        elx, ely = self.convert_coords(x, y)
        elsr = deg2pix(radius, self.win.monitor)

        if self.realconnect:
            # Get current gaze position
            gaze = self.get_gaze()

            # Calculate distance of gaze from circle center
            gdist = ((gaze[0] - elx) ** 2) + ((gaze[1] - ely) ** 2)

            # Compare to radius
            outcirc = gdist > (elsr ** 2)

            # Convert gaze to psychopy units
            gaze = self.convert_coords(gaze[0], gaze[1], to='psychopy')

            return outcirc, gaze
