# pylinkwrapper
Wrapper for using pylink in psychopy

## Installation
1. Download `pylinkwrapper.py` and `psychocal.py` placing them in the same directory as your psychopy experiment script

2. Include the following `import` command at the top of your experiment script: 
```python
from pylinkwrapper import eyeLinkFuns
```

3. Initiate an instance of eyeLinkFuns providing EDF name and window
```python
win = visual.window(monitor = 'nickMon', fullScr = True, allowGUI = False, color = -1)
tracker = pylinkwrapper.eyeLinkFuns(win, '1_nd')
```

## Functions
