To upgrade your pylink to the new version (October 2004) you will have to do the following.

0. Take a backup of pylink and your script
1. copy the pylink directory in this distribution to your site-packages directory

2. At the begining of the program after the imports put something like
eyelink_tracker = EyeLink("100.1.1.1") 

2a. to use eyelink object on broadcast mode, use EyeLinkListener() instead of EyeLink


2b. EyeLink constructor takes an eyelink tracker address as the argument. If None is passed in as the argument, then the eyelink is opened as dummy mode.

3. In you code search for all occurance of EYELINK and replace it with getEYELINK()
The EYELINK is no longer constant. The old EYELINK is initialized with None, if you use the constant you will get None type not EyeLink.


Install Instructions
1. download and install eyelink software form http://www.sr-support.com/forums/showthread.php?t=6
2. download and install pygame for your python version from http://www.pygame.org. 
	Eg. for python2.3 download and install http://www.pygame.org/ftp/pygame-1.6.win32-py2.3.exe
	You may need to delete the sdl.dll that is packaged with pygame.

3. copy pylinkXX directory to your <python install path>\Lib\site-packages(Eg. D:\Python23\Lib\site-packages), 
	where XX is the version of your python(eg. 23 if your python version 2.3.x) and rename pylinkxx to pylink.
4. try running gcwindow_main.py

Examples:

gcwindow:(python implementation of gcwindow. See the Eyelink II documents for more datails)
	gcwindow_main.py
	gcwindow_trial.py

broadcast:(python implementation of broadcast. See the Eyelink II documents for more datails)
	broadcast.py

integration to vision egg:(Example of integration to other graphics environments using python. To run this example you will have to install visionegg. You may download vision egg from http://www.visionegg.org/) 
	VisionEgg\eye_gabor_2d.py
	VisionEgg\EyeLinkCoreGraphicsVE.py

pygame graphics for calibration/validation/camera setup:(Example of custom eyelink core graphics using pygame)
	pygame\gcwindow_main.py
	pygame\gcwindow_trial.py
	pygame\EyeLinkCoreGraphicsPyGame.py
