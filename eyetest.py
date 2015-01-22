'''
Pylink Wrapper test experiment
N. DiQuattro - January 2015

This is a simple experiment where a circle appears randomly on the screen. It's
purpose is to provide examples of how to use the pylink wrapper with a psychopy
experiment.
'''

# Import modules
from psychopy import visual
from psychopy import core, event
import numpy as np

from pylinkwrapper import eyeLinkFuns  # Here's the special one

# Window set-up
win = visual.Window(monitor = 'nickMon', units = 'deg', fullscr = True,
                    allowGUI = False, color = 0)

# Initiate eye-tracker link and open EDF
tracker = eyeLinkFuns(win, '1_test') 

# Calibrate eye-tracker
tracker.calibrate()

# Stimulus
fix = visual.Circle(win, radius = 1, pos = (0, 0), fillColor = [1, 0, 0],
                    lineColor = [1, 0, 0])
                    
cfix = visual.Circle(win, radius = .15, fillColor = -1, lineColor = -1)

# Display stimulus 5 times
for t in range(5):

    # Find random coordinates and set them
    fx = np.random.randint(-10, 10)
    fy = np.random.randint(-10, 10)
    fix.setPos((fx, fy))
   
    # Eye tracker trial set-up
    stxt = 'Trial %d' % t
    tracker.setStatus(stxt)  #  Define status message that appears on eye-link
                             #  display
                             
    tracker.setTrialID()  # Sends trial start message for EDF
    tracker.sendMessage('Circle Trial')
    
    # Draw IA
    tracker.drawIA(fx, fy, 1, 1, 5, 'circle')  # Draw interest area and box
     
    # Start recording
    tracker.recordON()

    # Draw and display circle
    cfix.draw()
    fix.draw()
    win.flip()
   
    # Wait for response 
    keyp = event.waitKeys()
    
    # Stop Recording
    tracker.recordOFF()
    
    # Send response key to EDF file
    tracker.sendVar('response', keyp[0][0])

    # End trial for EDF
    tracker.setTrialResult()
    
    # ISI with fixation check
    cfix.draw()
    win.flip()
    tracker.fixCheck(2, 1, 'z')

# Retrieve EDF
tracker.endExperiment('C:\\edfs\\')  # Closes and retrieves EDF file to
                                     # specified path