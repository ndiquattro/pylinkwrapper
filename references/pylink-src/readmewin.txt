Pre-requisites: 
-Install EyeLink Development Kit for windows: https://www.sr-support.com/forums/showthread.php?t=6
-Install Python2.7 and the compatible versions of Pygame packages.
-download the pylink source package pylink-src-universal.zip from https://www.sr-support.com/forums/showthread.php?t=14
then unzip it into the folder where the Dev Kit was installed (ex: C:\Program Files\SR Research\EyeLink\pylink-src). This should ensure that the relative paths to the include and lib folders are properly aligned. 

To Build and install pylink module in Windows:
-Take a backup of your existing pylink installation (under site-packages). The new installation will overwrite it.
-run a cmd shell as admin and cd into the pylink-src folder you just unzipped.
-The next step is to build the code and depends on which compiler is available. If you have Visual Studio 2008 then the default settings should be ok and you can move on to the next step. If you have Visual Studio 2010, you will need to do something like this in the same shell: 
>set VS90COMNTOOLS=%VS100COMNTOOLS% 
For other compilers please refer to the bottom of this guide: http://docs.python.org/install/index.html
-In the same shell as before, type command: "setup.py install"
-This should build and install the pylink module under python's site-packages folder.