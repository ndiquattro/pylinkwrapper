Installation
=========================================
1. `Download <https://github.com/ndiquattro/pylinkwrapper/releases>`_ and place the pylinkwrapper folder somewhere you can easily reach in the file system.
2. Add the folder to Psychopy's python path as detailed here: http://www.psychopy.org/recipes/addCustomModules.html

That's it! Here's a quick demo of getting connected to the Eyelink::

    import pylinkwrapper

    win = visual.window(monitor='nickMon', fullScr=True, allowGUI=False, color=-1)
    tracker = pylinkwrapper.connect(win, '1_nd')

Check out the documentation for the :doc:`connector` class to see what functions are available.