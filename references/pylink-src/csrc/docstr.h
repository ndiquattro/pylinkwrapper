static char  alert_doc[]=
	"alert(message)\n"
	"This method is used to give a notification to the user when an error occurs.\n"
	"Parameters\n"
	"<message>: Text message to be displayed.\n"
	"Return Value:\n"
	"None\n"
	"Remarks:\n"
	"This function does not allow printf formatting as in c. However you can do a formatted string argument in python.\n"
	"This is equivalent to the C API void alert_printf(char *fmt, ...);\n";


static char  beginRealTimeMode_doc[]=
"beginRealTimeMode(delay)\n"
"Sets the application priority and cleans up pending Windows activity to place the application in realtime\n"
"mode. This could take up to 100 milliseconds, depending on the operation system, to set the application\n"
"priority.\n"
"Parameters\n"
"<delay> an integer, used to set the minimum time this function takes, so that this function can act as\n"
"a useful delay.\n"
"Return Value\n"
"None\n"
"This function is equivalent to the C API void begin_realtime_mode(UINT32 delay);\n";

static char  closeGraphics_doc[]=
"closeGraphics()\n"
"Notifies the eyelink_core_graphics to close or release the graphics.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"None\n"
"This is equivalent to the C API void close_expt_graphics(void);\n"
"This function should not be used with custom graphics \n";

static char  currentDoubleUsec_doc[]=
"currentDoubleTime()\n"
"Returns the current microsecond time (as a double type) since the initialization\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"A float data for the current microsecond time since the initialization\n"
"Remarks:\n"
"Same as currentUsec except, this function can return large microseconds.\n"
"return up to 2147483648 microseconds starting from initialization.\n"
"up to 36028797018963968 microseconds.\n"
"This is equivalent to the C API double current_double_usec(void);\n";


static char  currentTime_doc[]=
"currentTime()\n"
"Returns the current millisecond time since the initialization of the EyeLink library.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"Long integer for the current millisecond time since the initialization of the EyeLink library\n"
"This function is equivalent to the C API UINT32 current_time(void);\n";


static char  currentUsec_doc[]=
"currentUsec()\n"
"Returns the current microsecond time since the initialization of the EyeLink library.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"Long integer for the current microsecond time since the initialization of the EyeLink library\n"
"This is equivalent to the C API UINT32 current_usec(void);\n";


static char  endRealTimeMode_doc[]=
"endRealTimeMode()\n"
"Returns the application to a priority slightly above normal, to end realtime mode. This function should\n"
"execute rapidly, but there is the possibility that Windows will allow other tasks to run after this call, causing\n"
"delays of 1-20 milliseconds.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"None\n"
"This function is equivalent to the C API void end_realtime_mode(void);\n";

static char  flushGetkeyQueue_doc[]=
"flushGetkeyQueue()\n"
"Initializes the key queue used by getkey(). It may be called at any time to get rid any of old keys from the\n"
"queue.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"None\n"
"This is equivalent to the C API void flush_getkey_queue(void);\n";




static char  getDisplayInformation_doc[]=
"getDisplayInformation()\n"
"Returns the display configuration.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"Instance of DisplayInfo class. The width, height, bits, and refresh rate of the display can be accessed\n"
"from the returned value.\n"
"For example.\n"
"display = getDisplayInformation()\n"
"print display.width, display.height, display.bits, display.refresh\n";



static char  msecDelay_doc[]=
"msecDelay(delay)\n"
"Does a unblocked delay using currentTime().\n"
"Parameters\n"
"<delay>: an integer for number of milliseconds to delay.\n"
"Return Value\n"
"None\n"
"This is equivalent to the C API void msec_delay(UINT32 n);\n";


static char  openGraphics_doc[]=
"openGraphics(dimension, bits);\n"
"Opens the graphics if the display mode is not set. If the display mode is already set, uses the existing\n"
"display mode.\n"
"Parameters\n"
"<dimension>: two-item tuple of display containing width and height information.\n"
"<bits>: color bits.\n"
"Return Value\n"
"None or run-time error.\n"
"This is equivalent to the SDL version C API INT16 init_expt_graphics(SDL_Surface * s, DISPLAYINFO *info).\n";


static char  pumpDelay_doc[]=
"pumpDelay(delay)\n"
"During calls to msecDelay(), Windows is not able to handle messages. One result of this is that windows\n"
"may not appear. This is the preferred delay function when accurate timing is not needed. It calls\n"
"pumpMessages() until the last 20 milliseconds of the delay, allowing Windows to function properly. In rare\n"
"cases, the delay may be longer than expected. It does not process modeless dialog box messages.\n"
"Parameters\n"
"<delay>: an integer, which sets number of milliseconds to delay.\n"
"Return Value\n"
"None\n"
"Use the This is equivalent to the C API void pump_delay(UINT32 delay);\n";

static char  setCalibrationSounds_doc[]=
"setCalibrationSounds(target, good, error)\n"
"Selects the sounds to be played during do_tracker_setup(), including calibration, validation and drift\n"
"correction. These events are the display or movement of the target, successful conclusion of calibration or\n"
"good validation, and failure or interruption of calibration or validation.\n"
"Note: If no sound card is installed, the sounds are produced as beeps from the PC speaker. Otherwise,\n"
"sounds can be selected by passing a string. If the string is  (empty), the default sounds are played. If the\n"
"string is off, no sound will be played for that event. Otherwise, the string should be the name of a .WAV\n"
"file to play.\n"
"Parameters\n"
"<target>: Sets sound to play when target moves;\n"
"<good>: Sets sound to play on successful operation;\n"
"<error>: Sets sound to play on failure or interruption.\n"
"Return Value\n"
"None\n"
"This function is equivalent to the C API void set_cal_sounds(char *target, char *good, char *error);\n";



static char  setCalibrationColors_doc[]=
"setCalibrationColors(foreground_color, background_color)\n"
"Passes the colors of the display background and fixation target to the eyelink_core_graphics library. During\n"
"calibration, camera image display, and drift correction, the display background should match the brightness\n"
"of the experimental stimuli as closely as possible, in order to maximize tracking accuracy. This function\n"
"passes the colors of the display background and fixation target to the eyelink_core_graphics library. This\n"
"also prevents flickering of the display at the beginning and end of drift correction.\n"
"Parameters\n"
"<foreground_color>: color for foreground calibration target.\n"
"<background_color>: color for foreground calibration background. Both colors must be a threeinteger\n"
"(from 0 to 255) tuple encoding the red, blue, and green color component.\n"
"Return Value\n"
"None\n"
"This is equivalent to the C API void set_calibration_colors(SDL_Color *fg, SDL_Color *bg);\n"
"Example:\n"
"setCalibrationColors((0, 0, 0), (255, 255, 255))\n"
"This sets the calibration target in black and calibration background in white.\n";


static char  setCameraPosition_doc[]=
"setCameraPosition(left, top, right, bottom)\n"
"Sets the camera position on the display computer. Moves the top left hand corner of the camera position\n"
"to new location.\n"
"Parameters\n"
"<left>: x-coord of upper-left corner of the camera image window;\n"
"<top>: y-coord of upper-left corner of the camera image window;\n"
"<right>: x-coord of lower-right corner of the camera image window;\n"
"<bottom>: y-coord of lower-right corner of the camera image window.\n"
"Return Value\n"
"None\n";


static char  setDriftCorrectSounds_doc[]=
"setDriftCorrectSounds(target, good, setup)\n"
"Selects the sounds to be played during doDriftCorrect(). These events are the display or movement of\n"
"the target, successful conclusion of drift correction, and pressing the ESC key to start the Setup menu.\n"
"Note:\n"
"If no sound card is installed, the sounds are produced as beeps from the PC speaker. Otherwise, sounds\n"
"can be selected by passing a string. If the string is  (empty), the default sounds are played. If the string\n"
"is off, no sound will be played for that event. Otherwise, the string should be the name of a .WAV file to\n"
"play.\n"
"Parameters\n"
"<target>: Sets sound to play when target moves;\n"
"<good>: Sets sound to play on successful operation;\n"
"<setup>: Sets sound to play on ESC key pressed.\n"
"Return Value\n"
"None\n"
"This function is equivalent to the C API void set_dcorr_sounds(char *target, char *good, char *setup);\n";

static char  setTargetSize_doc[]=
"setTargetSize(diameter, holesize)\n"
"The standard calibration and drift correction target is a disk (for peripheral delectability) with a central\n"
"hole target (for accurate fixation). The sizes of these features may be set with this function.\n"
"Parameters\n"
"<diameter>: Size of outer disk, in pixels.\n"
"<holesize>: Size of central feature, in pixels. If holesize is 0, no central feature will be drawn. The\n"
"disk is drawn in the calibration foreground color, and the hole is drawn in the calibration background\n"
"color.\n"
"Return Value\n"
"None\n"
"This function is equivalent to the C API void set_target_size(UINT16 diameter, UINT16 holesize);\n";




static char  abort_doc[]=
"abort()\n"
"Places EyeLink tracker in off-line (idle) mode.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"0 if mode switch begun, else link error\n"
"Remarks\n"
"Use before attempting to draw graphics on the tracker display, transferring files, or closing link. Always\n"
"call waitForModeReady() afterwards to ensure tracker has finished the mode transition. This function\n"
"pair is implemented by the EyeLink toolkit library function setOfflineMode().\n"
"This function is equivalent to the C API INT16 eyelink_abort(void);\n";

static char  acceptTrigger_doc[]=
"acceptTrigger()\n"
"Triggers the EyeLink tracker to accept a fixation on a target, similar to the ‘Enter’ key or spacebar on the\n"
"tracker.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"NO_REPLY if drift correction not completed yet\n"
"OK_RESULT (0) if success\n"
"ABORT_REPLY (27) if ‘Esc’ key aborted operation\n"
"-1 if operation failed\n"
"1 if poor calibration or excessive validation error.\n"
"This function is equivalent to the C API INT16 eyelink_accept_trigger(void);\n";

static char  applyDriftCorrect_doc[]=
"applyDriftCorrect()\n"
"Applies the results of the last drift correction. This is not done automatically after a drift correction, allowing\n"
"the message returned by getCalibrationMessage() to be examined first.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"0 if command sent ok, else link error.\n"
"This function is equivalent to the C API INT16 eyelink_apply_driftcorr(void);\n";

static char  breakPressed_doc[]=
"breakPressed()\n"
"Tests if the program is being interrupted. You should break out of loops immediately if this function does\n"
"not return 0, if getkey() return TERMINATE_KEY, or if isConnected() method of the class returns 0.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"1 if CTRL-C is pressed, terminalBreak() was called, or the program has been terminated with ALT-F4;\n"
"0 otherwise\n"
"Remarks\n"
"Under Windows XP, this call will not work in realtime mode at all, and will take several seconds to\n"
"respond if graphics are being drawn continuously. This function works well in realtime mode under\n"
"Windows 2000.\n"
"This function is equivalent to the C API INT16 break_pressed(void);\n";



static char  broadcastOpen_doc[]=
"broadcastOpen()\n"
"Allows a third computer to listen in on a session between the eye tracker and a controlling remote machine.\n"
"This allows it to receive data during recording and playback, and to monitor the eye tracker mode. The\n"
"local computer will not be able to send commands to the eye tracker, but may be able to send messages or\n"
"request the tracker time.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"0 if successful.\n"
"LINK_INITIALIZE_FAILED if link could not be established.\n"
"CONNECT_TIMEOUT_FAILED if tracker did not respond.\n"
"WRONG_LINK_VERSION if the versions of the EyeLink library and tracker are incompatible\n"
"Remarks\n"
"This may not function properly, if there are more than one Ethernet cards installed.\n"
"This function is equivalent to the C API INT16 eyelink_broadcast_open(void);\n";

static char close_doc[]=
"close()\n"
"Sends a disconnect message to the EyeLink tracker.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"0 if successful, otherwise linkerror.\n"
"This function is equivalent to the C API INT16 eyelink_close(int send_msg); with send_msg parameter 1.\n";

static char  closeDataFile_doc[]=
"closeDataFile()\n"
"Closes any currently opened EDF file on the EyeLink tracker computer’s hard disk. This may take several\n"
"seconds to complete.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"0 if command executed successfully else error code.\n"
"This function is equivalent to the C API int close_data_file(void);\n";

static char  commandResult_doc[]=
"commandResult()\n"
"Check for and retrieves the numeric result code sent by the tracker from the last command.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"NO_REPLY if no reply to last command\n"
"OK_RESULT (0) if OK\n"
"Other error codes represent tracker execution error.\n"
"This function is equivalent to the C API INT16 eyelink_command_result(void);\n";

static char  dataSwitch_doc[]=
"dataSwitch(flag)\n"
"Sets what data from tracker will be accepted and placed in queue. Note: This does not start the tracker\n"
"recording, and so can be used with broadcastOpen(). It also does not clear old data from the queue.\n"
"Parameters\n"
"<flags> bitwise OR of the following flags:\n"
"RECORD_LINK_SAMPLES - send samples on link\n"
"RECORD_LINK_EVENTS - send events on link\n"
"Return Value\n"
"0 if no error, else link error code\n"
"This function is equivalent to the C API INT16 eyelink_data_switch(UINT16 flags);\n";

static char  doDriftCorrect_doc[]=
"doDriftCorrect(x, y, draw, allow_setup)\n"
"Performs a drift correction before a trial.\n"
"Parameters:\n"
"<x, y>: Position (in pixels) of drift correction target.\n"
"<draw>: If 1, the drift correction will clear the screen to the target background color, draw the target,\n"
"and clear the screen again when the drift correction is done. If 0, the fixation target must be drawn by\n"
"the user.\n"
"<allow_setup>: if 1, accesses Setup menu before returning, else aborts drift correction.\n"
"Return value\n"
"0 if successful, 27 if ‘Esc’ key was pressed to enter Setup menu or abort\n"
"This is equivalent to the C API int do_drift_correct(int x, int y, int draw, int allow_setup);\n";

static char  doTrackerSetup_doc[]=
"doTrackerSetup(position)\n"
"Switches the EyeLink tracker to the Setup menu, from which camera setup, calibration, validation, drift\n"
"correction, and configuration may be performed. Pressing the ‘Esc’ key on the tracker keyboard will exit the\n"
"Setup menu and return from this function. Calling exitCalibration() from an event handler will cause any\n"
"call to doTrackerSetup() in progress to return immediately.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"None\n"
"This is equivalent to the C API int do_tracker_setup(void);";


static char  drawCalTarget_doc[]=
"drawCaltarget()\n"
"Allow the normal calibration target drawing to proceed at different locations.\n"
"Parameters\n"
"Position: A tuple in the format of (x, y), passing along the position of drift correction target. X and y\n"
"are in screen pixels.\n"
"Return Value\n"
"None\n"
"This is equivalent to the C API INT16 CALLTYPE set_draw_cal_target_hook(INT16 (CALLBACK *\n"
"erase_cal_target_hook)(HDC hdc), INT16 options);\n";

static char  echo_key_doc[]=
"echo_key()\n"
"Checks for Windows keystroke events and dispatches messages; similar to getkey(), but also sends\n"
"keystroke to tracker.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"0 if no key pressed, else key code TERMINATE_KEY if CTRL-C held down or program has been\n"
"terminated.\n"
"Remarks\n"
"Warning: Under Windows XP, this call will not work in realtime mode at all, and will take several\n"
"seconds to respond if graphics are being drawn continuously. This function works well in realtime\n"
"mode under Windows 2000.\n"
"This function is equivalent to the C API unsigned echo_key(void);\n";


static char  escapePressed_doc[]=
"escapePressed()\n"
"This function tests if the ESC key is held down, and is usually used to break out of nested loops.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"1 if ESC key held down 0 if not\n"
"Remarks\n"
"Under Windows XP, this call will not work in realtime mode at all, and will take several seconds t\n"
"o respond if graphics are being drawn continuously. This function works well in realtime mode under\n"
"Windows 2000.\n"
"This function is equivalent to the C API INT16 escape_pressed(void);\n";


static char  exitCalibration_doc[]=
"exitCalibration()\n"
"This function should be called from a message or event handler if an ongoing call to doDriftCorrect() or\n"
"doTrackerSetup() should return immediately.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"None.\n"
"This function is equivalent to the C API void exit_calibration(void);\n";

static char  eyeAvailable_doc[]=
"eyeAvailable()\n"
"After calling the waitForBlockStart() method, or after at least one sample or eye event has been read, this\n"
"function can be used to check which eyes data is available for.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"LEFT_EYE (0) if left eye data\n"
"RIGHT_EYE (1) if right eye data\n"
"BINOCULAR (2) if both left and right eye data\n"
"-1 if no eye data is available\n"
"This is equivalent to the C API INT16 eyelink_eye_available(void);\n";

static char  flushKeybuttons_doc[]=
"flushKeyButtons(enable_buttons)\n"
"Causes the EyeLink tracker and the EyeLink library to flush any stored button or key events. This should be\n"
"used before a trial to get rid of old button responses. The <enable_buttons> argument controls whether\n"
"the EyeLink library will store button press and release events. It always stores tracker key events. Even if\n"
"disabled, the last button pressed and button flag bits are updated.\n"
"Parameters\n"
"Sets to 0 to monitor last button press only, 1 to queue button events.\n"
"Return Value\n"
"Always 0\n"
"This is equivalent to the C API INT16 eyelink_flush_keybuttons(INT16 enable_buttons);\n";


static char  getButtonStates_doc[]=
"getButtonStates()\n"
"Returns a flag word with bits set to indicate which tracker buttons are currently pressed. This is button 1\n"
"for the LSB, up to button 16 for the MSB. Note: Buttons above 8 are not realized on the EyeLink tracker.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"Flag bits for buttons currently pressed.\n"
"This function is equivalent to the C API UINT16 eyelink_button_states(void);\n";

static char  getCalibrationMessage_doc[]=
"getCalibrationMessage()\n"
"Returns text associated with result of last calibration, validation, or drift correction. This usually specifies\n"
"errors or other statistics.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"Message string associated with result of last calibration, validation, or drift correction.\n"
"This function is equivalent to the C API INT16 eyelink_cal_message(char *msg);\n";


static char  getCalibrationResult_doc[]=
"getCalibrationResult()\n"
"Checks for a numeric result code returned by calibration, validation, or drift correction.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"NO_REPLY if drift correction not completed yet\n"
"OK_RESULT (0) if success\n"
"ABORT_REPLY (27) if ‘Esc’ key aborted operation\n"
"-1 if operation failed\n"
"1 if poor calibration or excessive validation error.\n"
"This function is equivalent to the C API INT16 eyelink_cal_result(void);\n";

static char  getCurrentMode_doc[]=
"getCurrentMode()\n"
"This function tests the current tracker mode, and returns a set of flags based of what the mode is doing.\n"
"The most useful flag using the EyeLink experiment toolkit is IN_USER_MENU to test if the EyeLink Abort\n"
"menu has been activated.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"Set of bit flags that mark mode function:\n"
"IN_DISCONNECT_MODE if disconnected\n"
"IN_IDLE_MODE if off-line (Idle mode)\n"
"IN_SETUP_MODE if in Setup-menu related mode\n"
"IN_RECORD_MODE if tracking is in progress\n"
"IN_PLAYBACK_MODE if currently playing back data\n"
"IN_TARGET_MODE if in mode that requires a fixation target\n"
"IN_DRIFTCORR_MODE if in drift-correction\n"
"IN_IMAGE_MODE if displaying grayscale camera image\n"
"IN_USER_MENU if displaying Abort or user-defined menu\n"
"This is equivalent to the C API INT16 eyelink_current_mode(void);\n";


static char  getDataCount_doc[]=
"getDataCount(samples, events)\n"
"Counts total items in queue: samples, events, or both.\n"
"Parameters\n"
"<samples>: if non-zero count the samples.\n"
"<events>: if non-zero count the events..\n"
"Return Value\n"
"Total number of samples and events is in the queue.\n"
"This function is equivalent to the C API INT16 eyelink_data_count(INT16 samples, INT16 events);\n";



static char  getEventDataFlags_doc[]=
"getEventDataFlags()\n"
"Returns the event data content flags.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"Possible return values are a set of the following bit flags:\n"
"+-------------------------------------------------------------+\n"
"|Constant Name   |Value  |Description                         |\n"
"+-------------------------------------------------------------+\n"
"|EVENT_VELOCITY  |0x8000 |Has velocity data                   |\n"
"|EVENT_PUPILSIZE |0x4000 |Has pupil size data                 |\n"
"|EVENT_GAZERES   |0x2000 |Has gaze resolution                 |\n"
"|EVENT_STATUS    |0x1000 |Has status flags                    |\n"
"|EVENT_GAZEXY    |0x0400 |Has gaze x, y position              |\n"
"|EVENT_HREFXY    |0x0200 |Has head-ref x, y position          |\n"
"|EVENT_PUPILXY   |0x0100 |Has pupil x, y position             |\n"
"|FIX_AVG_ONLY    |0x0008 |Only average data to fixation events|\n"
"|START_TIME_ONLY |0x0004 |Only start-time in start events     |\n"
"|PARSEDBY_GAZE   |0x00C0 |Events were generated by GAZE data  |\n"
"|PARSEDBY_HREF   |0x0080 |Events were generated by HREF data  |\n"
"|PARSEDBY_PUPIL  |0x0040 |Events were generated by PUPIL data |\n"
"+-------------------------------------------------------------+\n"
"\n"
"This is equivalent to the C API UINT16 eyelink_event_data_flags(void);\n";




static char   getEventTypeFlags_doc[]=
"getEventTypeFlags()\n"
"After at least one button or eye event has been read, can be used to check what type of events will be\n"
"available.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"Possible return values are a set of the following bit flags:\n"
"Constant Name Value Description\n"
"LEFTEYE_EVENTS 0x8000 Has left eye events\n"
"RIGHTEYE_EVENTS 0x4000 Has right eye events\n"
"BLINK_EVENTS 0x2000 Has blink events\n"
"FIXATION_EVENTS 0x1000 Has fixation events\n"
"FIXUPDATE_EVENTS 0x0800 Has fixation updates\n"
"SACCADE_EVENTS 0x0400 Has saccade events\n"
"MESSAGE_EVENTS 0x0200 Has message events\n"
"BUTTON_EVENTS 0x0040 Has button events\n"
"INPUT_EVENTS 0x0020 Has input port events\n"
"This is equivalent to the C API UINT16 eyelink_event_type_flags(void);\n";

static char   getFloatData_doc[]=
"getFloatData()\n"
"Reads data of a specific type returned by getNextData. If this function called multiple times without calling\n"
"getNextData(), the same data is returned.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"None if no data available. Otherwise, a valid data is returned. The returned data type can be:\n"
"1. Sample\n"
"2. StartBlinkEvent\n"
"3. EndBlinkEvent\n"
"4. StartSacadeEvent\n"
"5. EndSacadeEvent\n"
"6. StartFixationEvent\n"
"7. EndFixationEvent\n"
"8. FixUpdateEvent\n"
"9. IOEvent\n"
"10. MessageEvent\n"
"This function is equivalent to the C API INT16 eyelink_get_next_data(void *buf);\n";

static char getkeyEx_doc[] =
"getKeyEx()\n"
"Returns the key pressed.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"None if no key pressed, else key code. TERMINATE_KEY if CTRL-C held down or program has been\n"
"terminated.\n"
"Remarks\n"
"Warning: This function processes and dispatches any waiting messages. This will allow Windows to\n"
"perform disk access and negates the purpose of realtime mode. Usually these delays will be only a few\n"
"milliseconds, but delays over 20 milliseconds have been observed. You may wish to call\n"
"escapePressed() or breakPressed() in recording loops instead of getkey() if timing is critical, for\n"
"example in a gaze-contingent display. Under Windows XP, these calls will not work in realtime mode at\n"
"all (although these do work under Windows 2000). Under Windows 95/98/Me, realtime performance is\n"
"impossible even with this strategy.\n"
"Some useful keys are:\n"
"CURS_UP\n"
"CURS_DOWN\n"
"CURS_LEFT\n"
"CURS_RIGHT\n"
"ESC_KEY\n"
"ENTER_KEY\n"
"TERMINATE_KEY\n"
"JUNK_KEY\n"
"This function is equivalent to the C API unsigned getkey(void);\n";


static char   getkey_doc[]=
"getKey()\n"
"Returns the key pressed.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"0 if no key pressed, else key code. TERMINATE_KEY if CTRL-C held down or program has been\n"
"terminated.\n"
"Remarks\n"
"Warning: This function processes and dispatches any waiting messages. This will allow Windows to\n"
"perform disk access and negates the purpose of realtime mode. Usually these delays will be only a few\n"
"milliseconds, but delays over 20 milliseconds have been observed. You may wish to call\n"
"escapePressed() or breakPressed() in recording loops instead of getkey() if timing is critical, for\n"
"example in a gaze-contingent display. Under Windows XP, these calls will not work in realtime mode at\n"
"all (although these do work under Windows 2000). Under Windows 95/98/Me, realtime performance is\n"
"impossible even with this strategy.\n"
"Some useful keys are:\n"
"CURS_UP\n"
"CURS_DOWN\n"
"CURS_LEFT\n"
"CURS_RIGHT\n"
"ESC_KEY\n"
"ENTER_KEY\n"
"TERMINATE_KEY\n"
"JUNK_KEY\n"
"This function is equivalent to the C API unsigned getkey(void);\n";

static char   getLastButtonPress_doc[]=
"getLastButtonPress()\n"
"Reads the number of the last button detected by the EyeLink tracker. This is 0 if no buttons were pressed\n"
"since the last call, or since the buttons were flushed. If a pointer to a variable is supplied the eye-tracker\n"
"timestamp of the button may be read. This could be used to see if a new button has been pressed since the\n"
"last read. If multiple buttons were pressed since the last call, only the last button is reported.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"Two-item tuple, recording the button last pressed (0 if no button pressed since last read) and the time\n"
"of the button press.\n"
"This function is equivalent to the C API UINT16 eyelink_last_button_press(UINT32 *time);\n";

static char   getLastData_doc[]=
"getLastData()\n"
"Gets an integer (unconverted) copy of the last/newest link data (sample or event) seen by\n"
"getNextData().\n"
"Parameters\n"
"None\n"
"Return Value\n"
"Object of type Sample or Event.\n"
"This function is equivalent to the C API INT16 eyelink_get_last_data(void *buf);\n";

static char   getLastMessage_doc[]=
"getLastMessage()\n"
"Returns text associated with last command response: may have error message.\n"
"Parameters:\n"
"None.\n"
"Return Value:\n"
"Text associated with last command response or None.\n"
"This is equivalent to the C API INT16 eyelink_last_message(char *buf);\n";

static char   getModeData_doc[]=
"getModeData()\n"
"After calling waitForBlockStart(), or after at least one sample or eye event has been read, returns EyeLink\n"
"II extended mode data.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"A five-item tuple holding (in the following order):\n"
"· eye information (LEFT_EYE if left eye data, RIGHT_EYE if right eye data, BINOCULAR if both left\n"
"and right eye data, -1 if no eye data is available),\n"
"· sampling rate (samples per second),\n"
"· CR mode flag (0 if pupil-only mode, else pupil-CR mode),\n"
"· filter level applied to file samples (0=off, 1=std, 2=extra),\n"
"· filter level applied to link and analog output samples (0=off, 1=std, 2=extra).\n"
"This function is equivalent to the C API INT16 eyelink2_mode_data(INT16 *sample_rate, INT16\n"
"*crmode, INT16 *file_filter, INT16 *link_filter);\n";

static char   getNewestSample_doc[]=
"getNewestSample()\n"
"Check if a new sample has arrived from the link. This is the latest sample, not the oldest sample that is\n"
"read by getNextData(), and is intended to drive gaze cursors and gaze-contingent displays.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"None if there is no sample, instance of Sample type otherwise.\n"
"This function is equivalent to the C API INT16 CALLTYPE eyelink_newest_sample(void FARTYPE\n"
"*buf);\n";


static char   getNextData_doc[]=
"getNextData()\n"
"Fetches next data item from link buffer.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"0 if no data, SAMPLE_TYPE (200) if sample, else event type. Possible return values are,\n"
"+-----------------------------------------------------------------------+\n"
"|Constant Name |Value |Description                                      |\n"
"+-----------------------------------------------------------------------+\n"
"|STARTBLINK    |3     |Pupil disappeared, time only                     |\n"
"|ENDBLINK      |4     |Pupil reappeared (duration data)                 |\n"
"|STARTSACC     |5     |Start of saccade (with time only)                |\n"
"|ENDSACC       |6     |End of saccade (with summary data)               |\n"
"|STARTFIX      |7     |Start of fixation (with time only)               |\n"
"|ENDFIX        |8     |End of fixation (with summary data)              |\n"
"|FIXUPDATE     |9     |Update within fixation, summary data for interval|\n"
"|MESSAGEEVENT  |24    |User-definable text (IMESSAGE structure)         |\n"
"|BUTTONEVENT   |25    |Button state change (IOEVENT structure)          |\n"
"|INPUTEVENT    |28    |Change of input port (IOEVENT structure)         |\n"
"|SAMPLE_TYPE   |200   |Event flags gap in data stream                   |\n"
"+-----------------------------------------------------------------------+\n"
"This function is equivalent to the C API INT16 eyelink_get_next_data(void *buf);\n";


static char   getNode_doc[]=
"getNode(response)\n"
"Reads the responses returned by other trackers or remotes in response to pollTrackers() or pollRemotes().\n"
"It can also read the tracker broadcast address and remote broadcast addresses.\n"
"Parameters\n"
"<resp>\n"
"Nmber of responses to read: 0 gets our data, 1 get first response, 2 gets the second response, etc. -1\n"
"to read the tracker broadcast address. -2 to read remote broadcast addresses.\n"
"Return Value\n"
"If successful, an instance of EyelinkMessage class returned.\n"
"This function is equivalent to the C API INT16 eyelink_get_node(INT16 resp, void *data);\n";


static char   getPositionScalar_doc[]=
"getPositionScalar()\n"
"Returns the divisor used to convert integer eye data to floating point data.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"Integer for the divisor (usually 10)\n"
"This function is equivalent to the C API INT16 eyelink_position_prescaler(void);\n";

static char   getRecordingStatus_doc[]=
"getRecordingStatus()\n"
"Checks if we are in Abort menu after recording stopped and returns trial exit code. Call this function on\n"
"leaving a trial. It checks if the EyeLink tracker is displaying the Abort menu, and handles it if required. The\n"
"return value from this function should be returned as the trial result code.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"TRIAL_OK if no error\n"
"REPEAT_TRIAL, SKIP_TRIAL, ABORT_EXPT if Abort menu activated.\n"
"This function is equivalent to the C API INT16 check_record_exit(void);\n";


static char   getSample_doc[]=
"getSample()\n"
"Gets an integer (unconverted) sample from end of queue, discards any events encountered.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"Object of type Sample\n"
"This is equivalent to the C API INT16 eyelink_get_sample(void *sample);\n";

static char   getSampleDataFlags_doc[]=
"getSampleDataFlag()\n"
"After calling waitForBlockStart(), or after at least one sample or eye event has been read, returns sample\n"
"data content flag (0 if not in sample block).\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"Possible return values are a set of the following bit flags:\n"
"+-----------------------------------------------------------------------+\n"
"|Constant Name    |Value  |Description                                  |\n"
"+-----------------------------------------------------------------------+\n"
"|SAMPLE_LEFT      |0x8000 |Data for left eye                            |\n"
"|SAMPLE_RIGHT     |0x4000 |Data for right eye                           |\n"
"|SAMPLE_TIMESTAMP |0x2000 |always for link, used to compress files      |\n"
"|SAMPLE_PUPILXY   |0x1000 |pupil x,y pair                               |\n"
"|SAMPLE_HREFXY    |0x0800 |head-referenced x,y pair                     |\n"
"|SAMPLE_GAZEXY    |0x0400 |gaze x,y pair                                |\n"
"|SAMPLE_GAZERES   |0x0200 |gaze res (x,y pixels per degree) pair        |\n"
"|SAMPLE_PUPILSIZE |0x0100 |pupil size                                   |\n"
"|SAMPLE_STATUS    |0x0080 |error flags                                  |\n"
"|SAMPLE_INPUTS    |0x0040 |input data port                              |\n"
"|SAMPLE_BUTTONS   |0x0020 |button state: LSBy state, MSBy changes       |\n"
"|SAMPLE_HEADPOS   |0x0010 |head-position: byte tells # words            |\n"
"|SAMPLE_TAGGED    |0x0008 |reserved variable-length tagged              |\n"
"|SAMPLE_UTAGGED   |0x0004 |user-defineabe variable-length tagged        |\n"
"+-----------------------------------------------------------------------+\n"
"This function is equivalent to the C API INT16 eyelink_sample_data_flags(void);\n";

static char   getTargetPositionAndState_doc[]=
"getTargetPositionAndState()\n"
"Returns the current target position and state.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"A three-item tuple holding (in the following order):\n"
"· the target visibility (0 if visible),\n"
"· x position of the target,\n"
"· and y position of the target.\n"
"This function is equivalent to the C API INT16 eyelink_target_check(INT16 *x, INT16 *y);\n";

static char   getTrackerInfo_doc[]=
"getTrackerInfo()\n"
"Returns the current tracker information.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"An instance of the ILinkData class (see section 3.12).\n";

static char   getTrackerMode_doc[]=
"getTrackerMode()\n"
"Returns raw EyeLink mode numbers.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"Raw EyeLink mode, -1 if link disconnected.\n"
"+-------------------------------------------+\n"
"|Constant Name         |Value               |\n"
"+-------------------------------------------+\n"
"|EL_IDLE_MODE          |1                   |\n"
"|EL_IMAGE_MODE         |2                   |\n"
"|EL_SETUP_MENU_MODE    |3                   |\n"
"|EL_USER_MENU_1        |5                   |\n"
"|EL_USER_MENU_2        |6                   |\n"
"|EL_USER_MENU_3        |7                   |\n"
"|EL_OPTIONS_MENU_MODE  |8                   |\n"
"|EL_OUTPUT_MENU_MODE   |9                   |\n"
"|EL_DEMO_MENU_MODE     |10                  |\n"
"|EL_CALIBRATE_MODE     |11                  |\n"
"|EL_VALIDATE_MODE      |12                  |\n"
"|EL_DRIFT_CORR_MODE    |13                  |\n"
"|EL_RECORD_MODE        |14                  |\n"
"|USER_MENU_NUMBER(mode)|((mode)-4)          |\n"
"+-------------------------------------------+\n"
"This function is equivalent to the C API INT16 eyelink_tracker_mode(void);\n";

static char   getTrackerVersion_doc[]=
"getTrackerVersion()\n"
"After connection, determines if the connected tracker is an EyeLink I or II. Use getTrackerVersionString to get the string\n"
"value.\n"
"Parameters:\n"
"None\n"
"Return Values:\n"
"The returned value is a number (0 if not connected, 1 for EyeLink I, 2 for EyeLink II).\n"
"This is equivalent to the C API INT16 eyelink_get_tracker_version(char *c);\n";

static char   getTrackerVersionString_doc[]=
"getTrackerVersionString()\n"
"After connection, determines if the connected tracker is an EyeLink I or II (use getTrackerVersion) to get\n"
"number value.\n"
"Parameters:\n"
"None\n"
"Return Value:\n"
"A string indicating EyeLink tracker version.\n"
"This is equivalent to the C API INT16 eyelink_get_tracker_version(char *c);\n";


static char   inSetup_doc[]=
"inSetup()\n"
"Checks if tracker is still in a Setup menu activity (includes camera image view, calibration, and validation).\n"
"Used to terminate the subject setup loop.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"0 if no longer in setup mode.\n"
"This function is equivalent to the C API INT16 eyelink_in_setup(void);\n";

static char   isConnected_doc[]=
"isConnected()\n"
"Checks whether the connection to the tracker is alive.\n"
"Parameters:\n"
"None\n"
"Return Values:\n"
"Returns 0 if link closed, -1 if simulating connection, 1 for normal connection, 2 for broadcast connection.\n"
"This is equivalent to the C API INT16 eyelink_is_connected(void);\n";

static char   isInDataBlock_doc[]=
"isInDataBlock(samples, events)\n"
"Checks to see if framing events read from queue indicate that the data is in a block containing samples,\n"
"events, or both.\n"
"Parameters\n"
"<samples>: if non-zero, check if in a block with samples.\n"
"<events>: if non-zero, check if in a block with events.\n"
"Return Value\n"
"0 if no data of either masked type is being sent.\n"
"Remarks\n"
"The first item in queue may not be a block start even, so this should be used in a loop while discarding\n"
"items using eyelink_get_next_data(NULL). NOTE: this function did not work reliably in versions of the\n"
"DLL before v2.0 (did not detect end of blocks).\n"
"This function is equivalent to the C API INT16 eyelink_in_data_block(INT16 samples, INT16 events);\n";

static char   isRecording_doc[]=
"isRecording()\n"
"Check if we are recording: if not, report an error. Call this function while recording. It will return true if\n"
"recording is still in progress, otherwise it will throw an exception. It will also handle the EyeLink Abort\n"
"menu. Any errors returned by this function should be returned by the trial function. On error, this will\n"
"disable realtime mode and restore the heuristic.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"TRIAL_OK (0) if no error\n"
"REPEAT_TRIAL, SKIP_TRIAL, ABORT_EXPT, TRIAL_ERROR if recording aborted.\n"
"This function is equivalent to the C API int check_recording(void);\n";

static char   nodeReceive_doc[]=
"nodeReceive()\n"
"Checks for and gets the last packet received, stores the data and the node address sent from. Note: Data\n"
"can only be read once, and is overwritten if a new packet arrives before the last packet has been read.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"An instance of EyelinkMessage class is returned, if successful.\n"
"This function is equivalent to the C API INT16 eyelink_node_receive(ELINKADDR node, void *data);\n";

static char   nodeRequestTime_doc[]=
"nodeRequestTime(address)\n"
"Sends a request the connected eye tracker to return its current time. Note: The time reply can be read\n"
"with getTrackerTime().\n"
"Parameters\n"
"<address>: text IP address (for example, “100.1.1.1”) for a specific tracker.\n"
"Return Value\n"
"0 if no error, else link error code\n"
"This function is equivalent to the C API UINT32 eyelink_node_request_time(ELINKADDR node);\n";

static char   nodeSend_doc[]=
"nodeSend(address, data, length)\n"
"Sends a given data to the given node.\n"
"Parameters\n"
"<addr>: the address of the node;\n"
"<data>: Pointer to buffer containing data to send;\n"
"<length>: Number of bytes of data\n"
"Return Value\n"
"0 if successful, otherwise link error.\n"
"This function is equivalent to the C API INT16 eyelink_node_send(ELINKADDR node, void *data,\n"
"UINT16 dsize);\n";


static char   nodeSendMessage_doc[]=
"nodeSendMessage(address, message)\n"
"Sends a text message the connected eye tracker. The text will be added to the EDF file. Note: If the link\n"
"is initialized but not connected to a tracker, the message will be sent to the tracker set by setAddress() of\n"
"the pylink module.\n"
"Parameters\n"
"<address>: Address of a specific tracker.\n"
"<message>: Text to send to the tracker.\n"
"Return Value\n"
"0 if no error, else link error code\n"
"This function is equivalent to the C API INT16 eyelink_node_send_message(ELINKADDR node, char\n"
"*msg);\n";





static char   open_doc[]=
"open(eyelink_address = “100.1.1.1”, busytest=0)\n"
"Opens connection to single tracker. If no parameters are given, it tries to open connection to the default\n"
"host (100.1.1.1).\n"
"Parameters\n"
"<eyelink_address>: text IP address of the host PC (the default value is, “100.1.1.1”);\n"
"<busytest>: if non-zero the call to eyelink_open_node will not disconnect an existing connection.\n"
"Return Value\n"
"Returns None. Throws Runtime error exception if it cannot open the connection.\n"
"This is equivalent to the C API INT16 eyelink_open_node(ELINKADDR node, INT16 busytest);\n";



static char   openDataFile_doc[]=
"openDataFile(name)\n"
"Opens a new EDF file on the EyeLink tracker computer’s hard disk. By calling this function will close any\n"
"currently opened file. This may take several seconds to complete. The file name should be formatted for\n"
"MS-DOS, usually 8 or less characters with only 0-9, A-Z, and ‘_’ allowed.\n"
"Parameters\n"
"<name>: Name of eye tracker file, 8 characters or less.\n"
"Return Value\n"
"0 if file was opened successfully else error code.\n"
"This function is equivalent to the C API int open_data_file(char *name);\n";



static char   openNode_doc[]=
"openNode(eyelink_address , busytest)\n"
"Allows the computer to connect to tracker, where the tracker is on the same network.\n"
"Parameters:\n"
"<eyelink_address>: text IP address of the host PC (the default value is, “100.1.1.1”);\n"
"<busytest>: if non-zero the call to openNode will not disconnect an existing connection.\n"
"Return Values:\n"
"None. Throws Runtime Exception if it connects to the remote host.\n"
"This is equivalent to the C API INT16 eyelink_open_node(ELINKADDR node, INT16 busytest); with\n"
"node parameter converted from text to ELINKADDR.\n";



static char   pollRemotes_doc[]=
"pollRemotes()\n"
"Asks all non-tracker computers (with EyeLink software running) on the network to send their names and\n"
"node address.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"0 if successful, otherwise link error.\n"
"This function is equivalent to the C API INT16 eyelink_poll_ remotes(void);\n";



static char   pollResponses_doc[]=
"pollResponses()\n"
"Returns the count of node addresses received so far following the call of pollRemotes() or pollTrackers().\n"
"Note: You should allow about 100 milliseconds for all nodes to respond. Up to 4 node responses are saved.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"Number of nodes responded. 0 if no responses.\n"
"This function is equivalent to the C API INT16 eyelink_poll_responses(void);\n";



static char   pollTrackers_doc[]=
"pollTrackers()\n"
"Asks all trackers (with EyeLink software running) on the network to send their names and node address.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"0 if successful, otherwise link error.\n"
"This function is equivalent to the C API INT16 eyelink_poll_trackers(void);\n";



static char   pumpMessages_doc[]=
"pumpMessages()\n"
"Forces the graphical environment to process any pending key or mouse events.\n"
"Parameter:\n"
"None\n"
"Return Value:\n"
"None\n"
"This function is equivalent to the C API INT16 message_pump(HWND dialog_hook).\n";



static char   quietMode_doc[]=
"quietMode(mode)\n"
"Controls the level of control an application has over the tracker.\n"
"Parameters\n"
"<mode>: 0 to allow all communication; 1 to block commands (allows only key presses, messages, and\n"
"time or variable read requests); 2 to disable all commands, requests and messages; -1 to just return\n"
"current setting\n"
"Return Value\n"
"Returns the previous mode settings.\n"
"This function is equivalent to the C API INT16 eyelink_quiet_mode(INT16 mode);\n";



static char   readKeyButton_doc[]=
"readKeyButton()\n"
"Reads any queued key or button events from tracker.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"A five-item tuple, recording (in the following order):\n"
"· Key character if key press/release/repeat, KB_BUTTON (0xFF00) if button press or release,\n"
"· Button number or key modifier (Shift, Alt and Ctrl key states),\n"
"· Key or button change (KB_PRESS, KB_RELEASE, or KB_REPEAT),\n"
"· Key scan code,\n"
"· Tracker time of the key or button change.\n"
"This function is equivalent to the C API UINT16 eyelink_read_keybutton(INT16 *mods,INT16 *state,\n"
"UINT16 *kcode, UINT32 *time);\n";



static char   readKeyQueue_doc[]=
"readKeyQueue()\n"
"Read keys from the key queue. It is similar to getkey(), but does not process Windows messages. This can\n"
"be used to build key-message handlers in languages other than C.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"0 if no key pressed\n"
"JUNK_KEY (1) if untranslatable key\n"
"TERMINATE_KEY (0x7FFF) if CTRL-C is pressed, terminal_break() was called, or the program has been\n"
"terminated with ALT-F4.\n"
"or code of key if any key pressed.\n"
"This function is equivalent to the C API UINT16 read_getkey_queue(void);\n";



static char   readReply_doc[]=
"readReply()\n"
"Returns text with reply to last read request.\n"
"Parameters:\n"
"None.\n"
"Return Value:\n"
"String to contain text or None.\n"
"This is equivalent to the C API INT16 eyelink_read_reply(char *buf);\n";


static char   readRequest_doc[]=
"readRequest(text)\n"
"Sends a text variable name whose value is read and returned by the tracker as a text string.\n"
"Parameters:\n"
"<text>: String with message to send.\n"
"Return Value:\n"
"0 if success, otherwise link error code.\n"
"Remarks\n"
"If the link is initialized but not connected to a tracker, the message will be sent to the tracker set by\n"
"setAddress(). However, these requests will be ignored by tracker versions older than EyeLink I v2.1\n"
"and EyeLink II v1.1.\n"
"This is equivalent to the C API INT16 eyelink_read_request(char *text);\n";



static char   receiveDataFile_doc[]=
"receiveDataFile(src, dest)\n"
"This receives a data file from the EyeLink tracker PC. Source file name and destination file name should be\n"
"given.\n"
"Parameters\n"
"<Src>: Name of eye tracker file (including extension).\n"
"<Dest>: Name of local file to write to (including extension).\n"
"Return Value\n"
"Size of file if successful,\n"
"Otherwise Runtime Exception is raised.\n"
"This function is equivalent to the C API int receive_data_file(char *src, char *dest, int is_path);\n";


static char   reset_doc[]=
"reset()\n"
"Resets the link data system if it is listening in on a broadcast data session. This does not shut down the\n"
"link, which remains available for other operations.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"0 if successful, otherwise linkerror.\n"
"This function is equivalent to the C API INT16 eyelink_close(int send_msg); with send_msg parameter 0.\n";



static char   resetData_doc[]=
"resetData()\n"
"Prepares link buffers to receive new data.\n"
"Parameters\n"
"<clear>: If nonzero, removes old data from buffer.\n"
"Return Value\n"
"Always 0.\n"
"This function is equivalent to the C API INT16 eyelink_reset_data(INT16 clear);\n";



static char   sendCommand_doc[]=
"sendCommand(command_text)\n"
"Sends the given command to connected eyelink tracker and returns the command result.\n"
"Parameters:\n"
"<command_text>: text command to be sent. It does not support printf() kind of formatting.\n"
"Return Values:\n"
"Command result. If there is any problem sending the command, a runtime exception is raised.\n"
"This is equivalent to the C API int eyecmd_printf(char *fmt, ...); without any formatting.\n";



static char   sendKeybutton_doc[]=
"sendKeybutton(code, mods, state)\n"
"Sends a key or button event to tracker. Only key events are handled for remote control.\n"
"Parameters\n"
"<code>: key character, or KB_BUTTON (0xFF00) if sending button event\n"
"<mods>: button number, or key modifier (Shift, Alt and Ctrl key states).\n"
"<state>: key or button change (KB_PRESS or KB_RELEASE)\n"
"Return Value\n"
"0 if ok, else send link error.\n"
"This function is equivalent to the C API INT16 eyelink_send_keybutton(UINT16 code, UINT16 mods,\n"
"INT16 state);\n";



static char   sendMessage_doc[]=
"sendMessage(message_text)\n"
"Sends the given message to the connected eyelink tracker. The message will be written to the eyelink\n"
"tracker.\n"
"Parameters:\n"
"<message_text>: text message to be sent. It does not support printf() kind of formatting.\n"
"Return Values:\n"
"If there is any problem sending the message, a runtime exception is raised.\n"
"This is equivalent to the C API int eyecmd_printf(char *fmt, ...);\n";



static char   setAddress_doc[]=
"setAddress(text_IP_address)\n"
"Sets the IP address used for connection to the EyeLink tracker. This is set to “100.1.1.1” in the DLL, but\n"
"may need to be changed for some network configurations. This must be set before attempting to open a\n"
"connection to the tracker.\n"
"A\n"
"“broadcast” address (“255.255.255.255”) may be used if the tracker address is not known—this will work\n"
"only if a single Ethernet card is installed, or if DLL version 2.1 or higher, and the latest tracker software\n"
"versions (EyeLink I v2.1 or higher, and EyeLink II v1.1 or higher) are installed.\n"
"Parameters:\n"
"<text_IP_address>: Pointer to a string containing a “dotted” 4-digit IP address;\n"
"Return Value:\n"
"0 if success, -1 if could not parse address string\n"
"This is equivalent to the C API INT16 set_eyelink_address(char *addr);\n";



static char   setName_doc[]=
"SetName(name)\n"
"Sets the node name of this computer (up to 35 characters).\n"
"Parameters\n"
"<name>: String to become new name.\n"
"Return Value\n"
"None.\n"
"This function is equivalent to the C API INT16 eyelink_set_name(char *name);\n";



static char   setOfflineMode_doc[]=
"setOfflineMode()\n"
"Places EyeLink tracker in off-line (idle) mode. Wait till the tracker has finished the mode transition.\n"
"Parameters:\n"
"None\n"
"Return Values:\n"
"None\n"
"This is equivalent to the C API INT16 set_offline_mode(void);\n";



static char   startData_doc[]=
"startData(flags, lock)\n"
"Switches tracker to Record mode, enables data types for recording to EDF file or sending to link. These\n"
"types are set with a bit wise OR of these flags:\n"
"+-----------------------------------------------------------------------+\n"
"|Constant Name       |Value |Description                                |\n"
"+-----------------------------------------------------------------------+\n"
"|RECORD_FILE_SAMPLES |1     |Enables sample recording to EDF file       |\n"
"|RECORD_FILE_EVENTS  |2     |Enables event recording to EDF file        |\n"
"|RECORD_LINK_SAMPLES |4     |Enables sending samples to the link        |\n"
"|RECORD_LINK_EVENTS  |8     |Enables sending events to the link         |\n"
"+-----------------------------------------------------------------------+\n"
"Parameters\n"
"<flags>: Bitwise OR of flags to control what data is recorded. If 0, recording will be stopped.\n"
"<lock>: If nonzero, prevents ‘Esc’ key from ending recording.\n"
"Return Value\n"
"0 if command sent ok, else link error.\n"
"Remarks\n"
"If <lock> is nonzero, the recording may only be terminated through stopRecording() or stopData()\n"
"method of the EyeLinkListener class, or by the Abort menu (‘Ctrl’’Alt’’A’ keys on the eye tracker). If\n"
"zero, the tracker ‘Esc’ key may be used to halt recording.\n"
"This function is equivalent to the C API INT16 eyelink_data_start(UINT16 flags, INT16 lock);\n";


static char   startDriftCorrect_doc[]=
"startDriftCorrect(x, y)\n"
"Sets the position of the drift correction target, and switches the tracker to drift-correction mode. Should be\n"
"followed by a call to waitForModeReady() method.\n"
"Parameters\n"
"<x>: x position of the target.\n"
"<y>: y position of the target.\n"
"Return Value\n"
"0 if command sent ok, else link error.\n"
"This function is equivalent to the C API INT16 eyelink_driftcorr_start(INT16 x, INT16 y);\n";

static char   startPlayBack_doc[]=
"startPlayBack()\n"
"Flushes data from queue and starts data playback. An EDF file must be open and have at least one\n"
"recorded trial. Use waitForData() method to wait for data: this will time out if the playback failed. Playback\n"
"begins from start of file or from just after the end of the next-but-last recording block. Link data is\n"
"determined by file contents, not by link sample and event settings.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"0 if command sent ok, else link error.\n"
"This function is equivalent to the C API INT16 eyelink_playback_start(void);\n";



static char   startRecording_doc[]=
"startRecording(File_samples, File_events, Link_samples, Link_events)\n"
"Starts the EyeLink tracker recording, sets up link for data reception if enabled.\n"
"Parameters:\n"
"<File_samples>: If 1, writes samples to EDF file. If 0, disables sample recording\n"
".<File_events>: If 1, writes events to EDF file. If 0, disables event recording.\n"
"<Link_samples>: If 1, sends samples through link. If 0, disables link sample access.\n"
"<Link_events>: If 1, sends events through link. If 0, disables link event access.\n"
"Return Values:\n"
"0 if successful, else trial return code\n"
"This is equivalent to the C API INT16 start_recording(INT16 file_samples, INT16 file_events, INT16\n"
"link_samples, INT16 link_events);\n"
"Note:\n"
"Recording may take 10 to 30 milliseconds to begin from this command. The function also waits until at\n"
"least one of all requested link data types have been received. If the return value is not zero, return the\n"
"result as the trial result code.\n";



static char   startSetup_doc[]=
"startSetup()\n"
"Switches the EyeLink tracker to the setup menu, for calibration, validation, and camera setup. Should be\n"
"followed by a call to waitForModeReady().\n"
"Parameters:\n"
"None\n"
"Return Value:\n"
"0 if command send OK.\n"
"This is equivalent to the C API INT16 eyelink_start_setup(void);\n";


static char   stopData_doc[]=
"StopData()\n"
"Places tracker in idle (off-line) mode, does not flush data from queue.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"0 if command sent ok, else link error.\n"
"Remark:\n"
"Should be followed by a call to waitForModeReady() method.\n"
"This function is equivalent to the C API INT16 eyelink_data_stop(void);\n";


static char   stopPlayBack_doc[]=
"StopPlayBack()\n"
"Stops playback if in progress. Flushes any data in queue.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"None.\n"
"This function is equivalent to the C API INT16 eyelink_playback_stop(void);\n";



static char   stopRecording_doc[]=
"stopRecording()\n"
"Stops recording, resets EyeLink data mode. Call 50 to 100 msec after an event occurs that ends the trial.\n"
"This function waits for mode switch before returning.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"None\n"
"This is equivalent to the C API void stop_recording(void);\n";



static char   terminalBreak_doc[]=
"terminalBreak(assert)\n"
"This function can be called in an event handler to signal that the program is terminating. Calling this\n"
"function with an argument of 1 will cause breakPressed() to return 1, and getkey() to return\n"
"TERMINATE_KEY. These functions can be re-enabled by calling terminalBreak() with an argument of 0.\n"
"Parameters\n"
"<Assert>: 1 to signal a program break, 0 to reset break.\n"
"Return Value\n"
"None.\n"
"This function is equivalent to the C API void terminal_break(INT16 assert);\n";



static char   trackerTime_doc[]=
"trackerTime()\n"
"Returns the current tracker time (in milliseconds) since the tracker application started.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"An integer data for the current tracker time (in milliseconds) since tracker initialization.\n"
"This is equivalent to the C API UINT32 eyelink_tracker_time();\n";



static char   trackerTimeOffset_doc[]=
"trackerTimeOffset()\n"
"Returns the time difference between the tracker time and display pc time.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"An integer data for the time difference (in milliseconds) between the tracker time and display pc time.\n"
"This is equivalent to the C API UINT32 eyelink_time_offset();\n";


static char   trackerTimeUsec_doc[]=
"trackerTimeUsec()\n"
"Returns the current tracker time (in microseconds) since the tracker application started.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"A double precision data for the current tracker time (in microseconds) since tracker initialization.\n"
"This is equivalent to the C API UINT32 eyelink_tracker_time();\n";


static char   trackerTimeUsecOffset_doc[]=
"trackerTimeUsecOffset()\n"
"Returns the time difference between the tracker time and display pc time.\n"
"Parameters\n"
"None.\n"
"Return Value\n"
"A double precision data for the time difference (in microseconds) between the tracker time and display\n"
"pc time.\n"
"This is equivalent to the C API double eyelink_time_usec_offset();\n";



static char   userMenuSelection_doc[]=
"userMenuSelection()\n"
"Checks for a user-menu selection, clears response for next call.\n"
"Parameters\n"
"None\n"
"Return Value\n"
"0 if no selection made since last call, else code of selection\n"
"This function is equivalent to the C API INT16 eyelink_user_menu_selection(void);\n";


static char   waitForBlockStart_doc[]=
"waitForBlockStart(tiemout, samples, events)\n"
"Reads and discards events in data queue until in a recording block. Waits for up to <timeout> milliseconds\n"
"for a block containing samples, events, or both to be opened. Items in the queue are discarded until the\n"
"block start events are found and processed. This function will fail if both samples and events are selected\n"
"but only one of link samples and events were enabled by startRecording().\n"
"Parameters\n"
"<timeout>: time in milliseconds to wait.\n"
"<samples>: if non-zero, check if in a block with samples.\n"
"<events>: if non-zero, check if in a block with events..\n"
"Return Value\n"
"0 if time expired without any data of masked types available.\n"
"Remarks\n"
"This function did not work in versions previous to 2.0.\n"
"This function is equivalent to the C API INT16 eyelink_wait_for_block_start(UINT32 maxwait,INT16\n"
"samples, INT16 events);\n";




static char   waitForData_doc[]=
"waitForData(maxwait, samples, events)\n"
"Waits for data to be received from the eye tracker. Can wait for an event, a sample, or either. Typically\n"
"used after record start to check if data is being sent.\n"
"Parameters\n"
"<maxwait>: time in milliseconds to wait for data.\n"
"<samples>: if 1, return when first sample available.\n"
"<events>: if 1, return when first event available.\n"
"Return Value\n"
"1 if data is available; 0 if timed out.\n"
"This function is equivalent to the C API INT16 eyelink_wait_for_data (UINT32 maxwait, INT16 samples,\n"
"INT16 events);\n";



static char   waitForModeReady_doc[]=
"waitForModeReady(maxwait)\n"
"After a mode-change command is given to the EyeLink tracker, an additional 5 to 30 milliseconds may be\n"
"needed to complete mode setup. Call this function after mode change functions.\n"
"Parameters\n"
"<maxwait>: Maximum milliseconds to wait for the mode to change.\n"
"Return Value\n"
"0 if mode switching is done, else still waiting.\n"
"Remarks\n"
"If it does not return 0, assume a tracker error has occurred.\n"
"This function is equivalent to the C API INT16 eyelink_wait_for_mode_ready(UINT32 maxwait);\n";



static char request_cross_hair_draw_doc [] = "DOC UNDONE";
static char  inRealTimeMode_doc   []= "DOC UNDONE";
static char  enableExtendedRealtime_doc[]= "DOC UNDONE";
static char  resetBackground_doc[]= "DOC UNDONE";
static char  disableCustomBackgroundOnImageMode_doc[]= "DOC UNDONE";
static char  openCustomGraphicsInternal_doc[]= "DOC UNDONE";
static char  sendMessageToFile_doc[]= "DOC UNDONE";
static char  openMessageFile_doc[]= "DOC UNDONE";
static char  closeMessageFile_doc[]= "DOC UNDONE";

static char  dummy_open_doc[]= "DOC UNDONE";
static char  sendTimedCommand_doc[]= "DOC UNDONE";
static char  sendTimedCommandEx_doc[]= "DOC UNDONE";
static char  getLastButtonStates_doc[]= "DOC UNDONE";
static char  requestFileRead_doc[]= "DOC UNDONE";
static char  key_message_pump_doc[]= "DOC UNDONE";
static char  endFileTransfer_doc[]= "DOC UNDONE";
static char  requestFileBlock_doc[]= "DOC UNDONE";
static char  buttonStates_doc[]= "DOC UNDONE";
static char  imageModeDisplay_doc[]= "DOC UNDONE";
static char  targetModeDisplay_doc[]= "DOC UNDONE";
static char  _getDataStatus_doc[]= "DOC UNDONE";
static char  requestTime_doc[]= "DOC UNDONE";
static char  readTime_doc[]= "DOC UNDONE";
static char  _drawCalTarget_doc[]= "DOC UNDONE";
static char  getImageCrossHairData_doc[]= "DOC UNDONE";