# pylinkwrapper
Wrapper for using pylink in psychopy

## Installation
First, download `pylinkwrapper.py` and `psychocal.py` placing them in the same directory as your psychopy experiment script

Then, include the following `import` command at the top of your experiment script: 
```python
from pylinkwrapper import eyeLinkFuns
```

Finally, initiate an instance of eyeLinkFuns providing EDF name and window
```python
win = visual.window(monitor = 'nickMon', fullScr = True, allowGUI = False, color = -1)
tracker = pylinkwrapper.eyeLinkFuns(win, '1_nd')
```

## Functions
