"""
Pylink Wrapper test experiment
N. DiQuattro - January 2015

This is a simple experiment where a circle appears randomly on the screen. It's
purpose is to provide examples of how to use the pylink wrapper with a psychopy
experiment.

There's help documentaiton available for each of the functions that show the
available parameters.
"""

# Import modules
from psychopy import visual
from psychopy import core, event
import numpy as np

import pylinkwrapper  # Here's the special one

# Window set-up
win = visual.Window(monitor='nickMon', units='deg', fullscr=True,
                    allowGUI=False, color=0)

# Initiate eye-tracker link and open EDF
tracker = pylinkwrapper.Connect(win, '1_test')

# Calibrate eye-tracker
tracker.calibrate()

# Stimulus
fix = visual.Circle(win, radius=1, pos=(0, 0), fillColor=[1, 0, 0],
                    lineColor=[1, 0, 0])

cfix = visual.Circle(win, radius=.15, fillColor=-1, lineColor=-1)

# Display stimulus 5 times
for t in range(5):
    # Find random coordinates and set them
    fx = np.random.randint(-10, 10)
    fy = np.random.randint(-10, 10)
    fix.setPos((fx, fy))

    # Eye tracker trial set-up
    stxt = 'Trial %d' % t
    tracker.set_status(stxt)  # Define status message that appears on eye-link
    #  display

    tracker.set_trialid()  # Sends trial start message for EDF
    tracker.send_message('Circle Trial')

    # Draw IA
    tracker.draw_ia(fx, fy, 1, 1, 5, 'circle')  # Draw interest area and box

    # Start recording
    tracker.record_on()

    # Draw and display circle
    cfix.draw()
    fix.draw()
    win.flip()

    # Wait for response 
    keyp = event.waitKeys()

    # Stop Recording
    tracker.record_off()

    # Send response key to EDF file
    tracker.send_var('response', keyp[0][0])

    # End trial for EDF
    tracker.set_trialresult()

    # ISI with fixation check
    cfix.draw()
    win.flip()
    tracker.fixcheck(2, 1, 'z')

# Retrieve EDF
tracker.end_experiment('C:\\edfs\\')  # Closes and retrieves EDF file to
# specified path
