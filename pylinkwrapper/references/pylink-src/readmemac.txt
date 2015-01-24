Install Instructions
1. download and install eyelink software form https://www.sr-support.com/forums/showthread.php?t=15
2. download and install pygame for your python version from http://www.pygame.org or http://pythonmac.org/packages/. 
	Eg. for python2.3 download and install http://pythonmac.org/packages/pygame-1.7.0-py2.3-macosx10.3.zip
3. copy pylinkX.X directory to your <python site-packages> and rename it to pylink, where X.X is the version of your python. 
	if your python version is 2.3 copy pylink2.3 directory to /System/Library/Frameworks/Python.framework/Versions/Current/lib/python2.3/site-packages and rename the pylink2.3 directory to pylink.
	
4. try running gcwindow_main.py (Run it using either pythonw or PythonLauncher.app)

Examples:

gcwindow:(python implementation of gcwindow. See the Eyelink II documents for more datails)
	gcwindow_main.py
	gcwindow_trial.py

broadcast:(python implementation of broadcast. See the Eyelink II documents for more datails)
	broadcast.py

integration to vision egg:(Example of integration to other graphics environments using python. To run this example you will have to install visionegg. You may download vision egg from http://www.visionegg.org/. Follow the instructions given at http://www.visionegg.org/install-macosx-details.html to install VisionEgg.)
	VisionEgg\eye_gabor_2d.py
	VisionEgg\EyeLinkCoreGraphicsVE.py

pygame graphics for calibration/validation/camera setup:(Example of custom eyelink core graphics using pygame)
	pygame\gcwindow_main.py
	pygame\gcwindow_trial.py
	pygame\EyeLinkCoreGraphicsPyGame.py
