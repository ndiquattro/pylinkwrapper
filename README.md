# Pylink Wrapper
Wrapper for using pylink in psychopy

## Installation
First, download `pylinkwrapper.py` and `psychocal.py` placing them in the same directory as your psychopy experiment script.

Then, include the following `import` command at the top of your experiment script: 
```python
from pylinkwrapper import eyeLinkFuns
```

Finally, initiate an instance of eyeLinkFuns providing your window object and EDF filename.
```python
win = visual.window(monitor = 'nickMon', fullScr = True, allowGUI = False, color = -1)
tracker = pylinkwrapper.eyeLinkFuns(win, '1_nd')
```

## Functions
Here's a list of the functions currently in the wrapper. See each functions `help()` for explanation of parameters.
* `.calibrate()` : Runs the camera set-up procedure using the custom calibration defined in `psychocal.py`. The function automatically sets the background color to match that defined in your window object.
* `.setStatus()` : Defines the status message that appears on the eye-tracker software.
* `.setTrialID()` : Indicates the start of an experimental trial in the EDF. Sets TRIAL ID to 1.
* `.recordON()` : Starts recording to the EDF file only.
* `.recordOFF()` : Stops recording to the EDF file.
* `.drawIA()` : Defines a square interest area in the EDF and draws a corresponding colored box on the eye-tracker display.
* `.sendVAR()` : Sends key-value pairs to the EDF file as variables that can be included in dataviewer reports.
* `.setTrialResult()` : Indiciates the end of an experimental trial in the EDF. Sets TRIAL_RESULT to 0.
* `.endExperiment()` : Closes the EDF file and transfers it to the provided path on the display computer.
* `.fixCheck()` : Pauses experiment until fixation at the center of the screen has been held for a given amount of time.

## Example
An very simple experiment that shows how to use the above functions is provided in `eyetest.py`.
