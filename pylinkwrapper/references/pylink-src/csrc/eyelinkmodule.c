/*
 * Copyright (c) 1996-2005, SR Research Ltd., All Rights Reserved
 *
 *
 * For use by SR Research licencees only. Redistribution and use in source
 * and binary forms, with or without modification, are NOT permitted.
 *
 *
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in
 * the documentation and/or other materials provided with the distribution.
 *
 * Neither name of SR Research Ltd nor the name of contributors may be used
 * to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
 * IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * $Date: 2009/06/30 17:40:28 $
 */


#include <Python.h>
//#include <sdl_expt.h>
#ifdef NO_SDL
#include <core_expt.h>
#else
#include <sdl_expt.h>
#endif

#include "eyelinkmodule.h"
#include "docstr.h"
/*!
\class pylink::EyeLinkCBind
C Implementation of EyeLinkListener.
*/

staticforward PyTypeObject EyelinkTrackerType;
static PyObject *newSample = NULL;
static PyObject *newStartBlinkEvent = NULL;
static PyObject *newEndBlinkEvent = NULL;
static PyObject *newStartSacadeEvent = NULL;
static PyObject *newEndSacadeEvent = NULL;
static PyObject *newStartFixationEvent = NULL;
static PyObject *newEndFixationEvent = NULL;
static PyObject *newIOEvent = NULL;
static PyObject *newMessageEvent = NULL;
static PyObject *newFixUpdateEvent = NULL;
static PyObject *newDisplayInfo = NULL;
static PyObject *EyelinkMessage = NULL;
static PyObject *trackerModule = NULL;

static PyObject *updatePerTrialDynamic	= NULL;
static PyObject *updateBooleanFlags		= NULL;
static PyObject *updateTimeStamps		= NULL;
static PyObject *updateStatusFlags		= NULL;
static PyObject *updateTypeFlags		= NULL;
static PyObject *updateSampleEventData	= NULL;
static PyObject *updateNetworkData		= NULL;
static PyObject *updateAddresses		= NULL;
static PyObject *updateReserved			= NULL;
static PyObject *drawCalTarget			= NULL;

#ifndef WIN32
inline INT32 eyelink_in_realtime_mode(void)
{
    return in_realtime_mode();
}
#else
char * getMultiByte(char *text,int len);
#define eyelink_in_realtime_mode in_realtime_mode // for automatic generation of code using macro
#endif






/*! \fn def pylink::EyeLinkCBind::broadcastOpen
	Allows a third computer to listen in on a session between the eye tracker and a controlling
	remote machine.  This allows it to receive data during recording and playback, and to monitor
	the eye tracker mode.  The local computer will not be able to send commands to the eye tracker,
	but may be able to send messages or request the tracker time.

	@remarks This may not function properly, if there are more than one Ethernet cards installed.
	This function is equivalent to the C API
	\code
	INT16  eyelink_broadcast_open(void);
	\endcode
	@return \c 0 if successful.\n
			\c LINK_INITIALIZE_FAILED if link could not be established.\n
			\c CONNECT_TIMEOUT_FAILED if tracker did not respond.\n
			\c WRONG_LINK_VERSION if the versions of the EyeLink library and tracker are incompatible.
 */
SHORTVOIDFCN(broadcast_open)
/*! \fn def pylink::EyeLinkCBind::dummy_open
	Sets the EyeLink library to simulate an eyetracker connection.  Functions will return
	plausible values, but no data.

	@remarks The function \c isConnected() will return \c -1 to indicate a simulated connection.
 */
SHORTVOIDFCN(dummy_open)
/*! \fn def pylink::EyeLinkCBind::isConnected
	Checks whether the connection to the tracker is alive.
	@remarks This is equivalent to the C API
	\code
	INT16 eyelink_is_connected(void);
	\endcode
	@return \c 0 if link closed.\n
			\c -1 if simulating connection.\n
			\c 1 for normal connection.\n
			\c 2 for broadcast connection.
 */
SHORTVOIDFCN(is_connected)
/*! \fn def pylink::EyeLinkCBind::pollTrackers
	Asks all trackers (with EyeLink software running) on the network to send their names and node
	address.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_poll_trackers(void);
	\endcode
	@return \c 0 if successful, otherwise link error.
 */
SHORTVOIDFCN(poll_trackers)
/*! \fn def pylink::EyeLinkCBind::pollRemotes
	Asks all non-tracker computers (with EyeLink software running) on the network to send their
	names and node address.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_poll_ remotes(void);
	\endcode
	@return \c 0 if successful, otherwise link error.
 */
SHORTVOIDFCN(poll_remotes)
/*! \fn def pylink::EyeLinkCBind::pollResponses
	Returns the count of node addresses received so far following the call of \c pollRemotes() or
	\c pollTrackers().

	@remarks You should allow about 100 milliseconds for all nodes to respond.
	Up to 4 node responses are saved.  This function is equivalent to the C API
	\code
	INT16 eyelink_poll_responses(void);
	\endcode
	@return Number of nodes responded.  \c 0 if no responses.
 */
SHORTVOIDFCN(poll_responses)
/*! \fn def pylink::EyeLinkCBind::commandResult
 	Check for and retrieves the numeric result code sent by the tracker from the last command.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_command_result(void);
	\endcode
	@return \c NO_REPLY if no reply to last command.\n
			<tt>OK_RESULT (0)</tt> if OK.\n
			Other error codes represent tracker execution error.
 */
SHORTVOIDFCN(command_result)
/*! \fn def pylink::EyeLinkCBind::abort
 	Places EyeLink tracker in off-line (idle) mode.

	@remarks Use before attempting to draw graphics on the tracker display, transferring files,
	or closing link. Always call \c waitForModeReady() afterwards to ensure tracker has finished
	the mode transition. This function pair is implemented by the EyeLink toolkit library
	function \c setOfflineMode().
	This function is equivalent to the C API
	\code
	INT16 eyelink_abort(void);
	\endcode
	@return \c 0 if mode switch begun, else link error.
 */
SHORTVOIDFCN(abort)
/*! \fn def pylink::EyeLinkCBind::startSetup
 	Switches the EyeLink tracker to the setup menu, for calibration, validation, and camera setup.
 	Should be followed by a call to \c waitForModeReady().

	@remarks This is equivalent to the C API
	\code
	INT16 eyelink_start_setup(void);
	\endcode
	@return \c 0 if command send fine.
 */
SHORTVOIDFCN(start_setup)
/*! \fn def pylink::EyeLinkCBind::inSetup
	Checks if tracker is still in a Setup menu activity (includes camera image view, calibration,
	and validation).  Used to terminate the subject setup loop.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_in_setup(void);
	\endcode
	@return \c 0 if no longer in setup mode.
 */
SHORTVOIDFCN(in_setup)
/*! \fn def pylink::EyeLinkCBind::acceptTrigger
	Triggers the EyeLink tracker to accept a fixation on a target, similar to the 'Enter' key or
	spacebar on the tracker.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_accept_trigger(void);
	\endcode
	@return \c NO_REPLY if drift correction not completed yet.\n
			<tt>OK_RESULT (0)</tt> if success.\n
			<tt>ABORT_REPLY (27)</tt> if 'ESC' key aborted operation.\n
			\c -1 if operation failed.\n
			\c 1 if poor calibration or excessive validation error.
 */
SHORTVOIDFCN(accept_trigger)
/*! \fn def pylink::EyeLinkCBind::getCalibrationResult
	Checks for a numeric result code returned by calibration, validation, or drift correction.

	@remarks This function is equivalent to the C API
	\code
	INT16  eyelink_cal_result(void);
	\endcode
	@return \c NO_REPLY if drift correction not completed yet.\n
			<tt>OK_RESULT (0)</tt> if success.\n
			<tt>ABORT_REPLY (27)</tt> if 'ESC' key aborted operation.\n
			\c -1 if operation failed.\n
			\c 1 if poor calibration or excessive validation error.
 */
SHORTVOIDFCN(cal_result)
/*! \fn def pylink::EyeLinkCBind::applyDriftCorrect
	Applies the results of the last drift correction. This is not done automatically after a
	drift correction, allowing the message returned by \c getCalibrationMessage() to be examined first.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_apply_driftcorr(void);
	\endcode
	@return \c 0 if command sent fine, else link error.
 */
SHORTVOIDFCN(apply_driftcorr)
/*! \fn def pylink::EyeLinkCBind::getTrackerMode
	Returns raw EyeLink mode numbers.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_tracker_mode(void);
	\endcode
	@return Raw EyeLink mode, \c -1 if link disconnected.
	<table>
	<tr><td class="indexkey">Constant</td><td class="indexkey" align="center">Value</td></tr>
  	<tt>
  	<tr><td>EL_IDLE_MODE</td><td>1</td></tr>
	<tr><td>EL_IMAGE_MODE</td><td>2</td></tr>
	<tr><td>EL_SETUP_MENU_MODE</td><td>3</td></tr>
	<tr><td>EL_USER_MENU_1</td><td>5</td></tr>
	<tr><td>EL_USER_MENU_2</td><td>6</td></tr>
	<tr><td>EL_USER_MENU_3</td><td>7</td></tr>
	<tr><td>EL_OPTIONS_MENU_MODE</td><td>8</td></tr>
	<tr><td>EL_OUTPUT_MENU_MODE</td><td>9</td></tr>
	<tr><td>EL_DEMO_MENU_MODE</td><td>10</td></tr>
	<tr><td>EL_CALIBRATE_MODE</td><td>11</td></tr>
	<tr><td>EL_VALIDATE_MODE</td><td>12</td></tr>
	<tr><td>EL_DRIFT_CORR_MODE</td><td>13</td></tr>
	<tr><td>EL_RECORD_MODE</td><td>14</td></tr>
	</tt>
	</table>

	<tt>USER_MENU_NUMBER(mode) ((mode)-4)</tt>
 */
SHORTVOIDFCN(tracker_mode)
/*! \fn def pylink::EyeLinkCBind::userMenuSelection
	Checks for a user-menu selection, clears response for next call.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_user_menu_selection(void);
	\endcode
	@return \c 0 if no selection made since last call, else code of selection.
 */
SHORTVOIDFCN(user_menu_selection)
/*! \fn def pylink::EyeLinkCBind::eyeAvailable
	After calling the \c waitForBlockStart() method, or after at least one sample or eye event
	has been read, this function can be used to check which eyes data is available for.

	@remarks This is equivalent to the C API
	\code
	INT16 eyelink_eye_available(void);
	\endcode
	@return <tt>LEFT_EYE (0)</tt> if left eye data.\n
			<tt>RIGHT_EYE (1)</tt> if right eye data.\n
			<tt>BINOCULAR (2)</tt> if both left and right eye data.\n
			\c -1 if no eye data is available.
 */
SHORTVOIDFCN(eye_available)
/*! \fn def pylink::EyeLinkCBind::getSampleDataFlags
	After calling \c waitForBlockStart(), or after at least one sample or eye event has been
	read, returns sample data content flag (\c 0 if not in sample block).

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_sample_data_flags(void);
	\endcode
	@return Possible return values are a set of the following bit flags:
	<table>
	<tr><td class="indexkey">Constant Name</td><td class="indexkey" align="center">Value</td><td class="indexkey">Description</td></tr>
	<tr><td>\c SAMPLE_LEFT</td><td>\c 0x8000</td><td>Data for left eye</td></tr>
	<tr><td>\c SAMPLE_RIGHT</td><td>\c 0x4000</td><td>Data for right eye</td></tr>
	<tr><td>\c SAMPLE_TIMESTAMP</td><td>\c 0x2000</td><td>always for link, used to compress files</td></tr>
	<tr><td>\c SAMPLE_PUPILXY</td><td>\c 0x1000</td><td>pupil x,y pair</td></tr>
	<tr><td>\c SAMPLE_HREFXY</td><td>\c 0x0800</td><td>head-referenced x,y pair</td></tr>
	<tr><td>\c SAMPLE_GAZEXY</td><td>\c 0x0400</td><td>gaze x,y pair</td></tr>
	<tr><td>\c SAMPLE_GAZERES</td><td>\c 0x0200</td><td>gaze res (x,y pixels per degree) pair</td></tr>
	<tr><td>\c SAMPLE_PUPILSIZE</td><td>\c 0x0100</td><td>pupil size</td></tr>
	<tr><td>\c SAMPLE_STATUS</td><td>\c 0x0080</td><td>error flags</td></tr>
	<tr><td>\c SAMPLE_INPUTS</td><td>\c 0x0040</td><td>input data port</td></tr>
	<tr><td>\c SAMPLE_BUTTONS</td><td>\c 0x0020</td><td>button state: LSBy state, MSBy changes</td></tr>
	<tr><td>\c SAMPLE_HEADPOS</td><td>\c 0x0010</td><td>head-position: byte tells # words</td></tr>
	<tr><td>\c SAMPLE_TAGGED</td><td>\c 0x0008</td><td>reserved variable-length tagged</td></tr>
	<tr><td>\c SAMPLE_UTAGGED</td><td>\c 0x0004</td><td>user-defineabe variable-length tagged</td></tr>
	</table>
 */
SHORTVOIDFCN(sample_data_flags)
/*! \fn def pylink::EyeLinkCBind::getEventDataFlags
	Returns the event data content flags.

	@remarks This is equivalent to the C API
	\code
	UINT16 eyelink_event_data_flags(void);
	\endcode
	@return Possible return values are a set of the following bit flags:
	<table>
	<tr><td class="indexkey">Constant Name</td><td class="indexkey" align="center">Value</td><td class="indexkey">Description</td></tr>
	<tr><td>\c EVENT_VELOCITY</td><td>\c 0x8000</td><td>Has velocity data</td></tr>
	<tr><td>\c EVENT_PUPILSIZE</td><td>\c 0x4000</td><td>Has pupil size data</td></tr>
	<tr><td>\c EVENT_GAZERES</td><td>\c 0x2000</td><td>Has gaze resolution</td></tr>
	<tr><td>\c EVENT_STATUS</td><td>\c 0x1000</td><td>Has status flags</td></tr>
	<tr><td>\c EVENT_GAZEXY</td><td>\c 0x0400</td><td>Has gaze x, y position</td></tr>
	<tr><td>\c EVENT_HREFXY</td><td>\c 0x0200</td><td>Has head-ref x, y position</td></tr>
	<tr><td>\c EVENT_PUPILXY</td><td>\c 0x0100</td><td>Has pupil x, y position</td></tr>
	<tr><td>\c FIX_AVG_ONLY</td><td>\c 0x0008</td><td>Only average data to fixation events</td></tr>
	<tr><td>\c START_TIME_ONLY</td><td>\c 0x0004</td><td>Only start-time in start events</td></tr>
	<tr><td>\c PARSEDBY_GAZE</td><td>\c 0x00C0</td><td>Events were generated by GAZE data</td></tr>
	<tr><td>\c PARSEDBY_HREF</td><td>\c 0x0080</td><td>Events were generated by HREF data</td></tr>
	<tr><td>\c PARSEDBY_PUPIL</td><td>\c 0x0040</td><td>Events were generated by PUPIL data</td></tr>
	</table>
 */
SHORTVOIDFCN(event_data_flags)
/*! \fn def pylink::EyeLinkCBind::getEventTypeFlags
	After at least one button or eye event has been read, can be used to check what type
	of events will be available.

	@remarks This is equivalent to the C API
	\code
	UINT16 eyelink_event_type_flags(void);
	\endcode
	@return Possible return values are a set of the following bit flags:
	<table>
	<tr><td class="indexkey">Constant Name</td><td class="indexkey" align="center">Value</td><td class="indexkey">Description</td></tr>
	<tr><td>\c LEFTEYE_EVENTS</td><td>\c 0x8000</td><td>Has left eye events</td></tr>
	<tr><td>\c RIGHTEYE_EVENTS</td><td>\c 0x4000</td><td>Has right eye events</td></tr>
	<tr><td>\c BLINK_EVENTS</td><td>\c 0x2000</td><td>Has blink events</td></tr>
	<tr><td>\c FIXATION_EVENTS</td><td>\c 0x1000</td><td>Has fixation events</td></tr>
	<tr><td>\c FIXUPDATE_EVENTS</td><td>\c 0x0800</td><td>Has fixation updates</td></tr>
	<tr><td>\c SACCADE_EVENTS</td><td>\c 0x0400</td><td>Has saccade events</td></tr>
	<tr><td>\c MESSAGE_EVENTS</td><td>\c 0x0200</td><td>Has message events</td></tr>
	<tr><td>\c BUTTON_EVENTS</td><td>\c 0x0040</td><td>Has button events</td></tr>
	<tr><td>\c INPUT_EVENTS</td><td>\c 0x0020</td><td>Has input port events</td></tr>
	</table>
 */
SHORTVOIDFCN(event_type_flags)


/*! \fn def pylink::EyeLinkCBind::getPositionScalar
	Returns the divisor used to convert integer eye data to floating point data.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_position_prescaler(void);
	\endcode
	@return Integer for the divisor (usually 10).
 */
SHORTVOIDFCN(position_prescaler)
/*! \fn def pylink::EyeLinkCBind::stopData
	Places tracker in idle (off-line) mode, does not flush data from queue.

	@remarks Should be followed by a call to \c waitForModeReady() method. This function
	is equivalent to the C API
	\code
	INT16 eyelink_data_stop(void);
	\endcode
	@return \c 0 if command sent fine, else link error.
 */
SHORTVOIDFCN(data_stop)
/*! \fn def pylink::EyeLinkCBind::startPlayBack
	Flushes data from queue and starts data playback. An EDF file must be open and have
	at least one recorded trial. Use \c waitForData() method to wait for data: this will
	time out if the playback failed. Playback begins from start of file or from just
	after the end of the next-but-last recording block. Link data is determined by file
	contents, not by link sample and event settings.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_playback_start(void);
	\endcode
	@return \c 0 if command sent fine, else link error.
 */
SHORTVOIDFCN(playback_start)
/*! \fn def pylink::EyeLinkCBind::stopPlayBack
	Stops playback if in progress.  Flushes any data in queue.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_playback_stop(void);
	\endcode
 */
SHORTVOIDFCN(playback_stop)
/*! \fn def pylink::EyeLinkCBind::
	\todo Need description.
 */
SHORTVOIDFCN(image_status)
/*! \fn def pylink::EyeLinkCBind::getButtonStates
	Returns a flag word with bits set to indicate which tracker buttons are currently
	pressed.  This is button \c 1 for the LSB, up to button \c 16 for the MSB.  Buttons
	above \c 8 are not realized on the EyeLink tracker.

	@remarks This function is equivalent to the C API
	\code
	UINT16 eyelink_button_states(void);
	\endcode
	@return Flag bits for buttons currently pressed.
 */
SHORTVOIDFCN(button_states)
/*! \fn def pylink::EyeLinkCBind::getCurrentMode
	This function tests the current tracker mode, and returns a set of flags based of
	what the mode is doing.
	The most useful flag using the EyeLink experiment toolkit is \c IN_USER_MENU to test
	if the EyeLink Abort menu has been activated.

	@remarks This is equivalent to the C API
	\code
	INT16 eyelink_current_mode(void);
	\endcode
	@return Set of bit flags that mark mode function:
			<table>
			<tr><td>\c IN_DISCONNECT_MODE</td><td>if  disconnected</td></tr>
			<tr><td>\c IN_IDLE_MODE</td><td>if off-line (Idle mode)</td></tr>
			<tr><td>\c IN_SETUP_MODE</td><td>if in Setup-menu related mode</td></tr>
			<tr><td>\c IN_RECORD_MODE</td><td>if tracking is in progress</td></tr>
			<tr><td>\c IN_PLAYBACK_MODE</td><td>if currently playing back data</td></tr>
			<tr><td>\c IN_TARGET_MODE</td><td>if in mode that requires a fixation target</td></tr>
			<tr><td>\c IN_DRIFTCORR_MODE</td><td>if in drift-correction</td></tr>
			<tr><td>\c IN_IMAGE_MODE</td><td>if displaying grayscale camera image</td></tr>
			<tr><td>\c IN_USER_MENU</td><td>if displaying Abort or user-defined menu</td></tr>
			</table>
 */
SHORTVOIDFCN(current_mode)
/*! \fn def pylink::EyeLinkCBind::
	\todo Need description.
 */
SHORTVOIDFCN(bitmap_ack_count)
/*! \fn def pylink::EyeLinkCBind::endFileTransfer
	Aborts send of file.
 */
SHORTVOIDFCN(end_file_transfer)
/*! \fn def pylink::EyeLinkCBind::
	\todo Need description.
 */
SHORTSTRFCN(send_command)
/*! \fn def pylink::EyeLinkCBind::
	\todo Need description.
 */
SHORTSTRFCN(send_message)
/*! \fn def pylink::EyeLinkCBind::getTrackerVersion
	After connection, determines if the connected tracker is an EyeLink I or II.  Use
	\c getTrackerVersionString() to get the string value.

	@remarks This is equivalent to the C API
	\code
	INT16 eyelink_get_tracker_version(char *c);
	\endcode
	@return The returned value is a number (\c 0 if not connected, \c 1 for EyeLink I, \c 2 for EyeLink II).
 */
SHORTSTRFCN(get_tracker_version)
SHORTSTRFCN(request_file_read)


/*! \fn def pylink::EyeLinkCBind::getLastMessage
	Returns text associated with last command response: may have error message.

	@remarks This is equivalent to the C API
	\code
	INT16 eyelink_last_message(char *buf);
	\endcode
	@return Text associated with last command response or None.
 */
RETURNSTRFCN(last_message)
/*! \fn def pylink::EyeLinkCBind::readReply
	Returns text with reply to last read request.

	@remarks This is equivalent to the C API
	\code
	INT16 eyelink_read_reply(char *buf);
	\endcode
	@return String to contain text or None.
 */
RETURNSTRFCN(read_reply)
/*! \fn def pylink::EyeLinkCBind::getCalibrationMessage
	Returns text associated with result of last calibration, validation, or drift
	correction. This usually specifies errors or other statistics.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_cal_message(char *msg);
	\endcode
	@return Message string associated with result of last calibration, validation, or drift correction.
 */
RETURNSTRFCN(cal_message)
/*! \fn def pylink::EyeLinkCBind::getTrackerVersionString
	After connection, determines if the connected tracker is an EyeLink I or II
	(use getTrackerVersion) to get number value.

	@remarks This is equivalent to the C API
	\code
	INT16 eyelink_get_tracker_version(char *c);
	\endcode
	@return A string indicating EyeLink tracker version.
 */
RETURNSTRFCN(get_tracker_version)



/*! \fn def pylink::inRealTimeMode
	returns whether the current mode is real-time.

	@return \c 1 if in realtime mode, else \c 0.
 */
INTVOIDFCN(in_realtime_mode)
/*! \fn def pylink::EyeLinkCBind::requestTime
	Sends a request the connected eye tracker to return its current time.

	@remarks The time reply can be read with \c readTime().
	@return \c 0 if no error, else link error code.
	@sa rackerTime()
 */
INTVOIDFCN(request_time)
/*! \fn def pylink::EyeLinkCBind::readTime
	Returns the tracker time requested by \c eyelink_request_time() or \c eyelink_node_request_time().

	@return \c 0 if no response yet,  else timestamp in millisecond.
	\sa \c trackerTime()
 */
INTVOIDFCN(read_time)


/*! \fn def pylink::EyeLinkCBind::resetData
	Prepares link buffers to receive new data and removes old data from buffer.
 */
static PyObject* eyelink_eyelink_reset_data(PyObject* self, PyObject* args)
{
	if (PyArg_ParseTuple(args,":"))
	{
		eyelink_reset_data(1);
		return Py_BuildValue("");
	}
	return NULL;
}

/*! \fn def pylink::EyeLinkCBind::quietMode
	Controls the level of control an application has over the tracker.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_quiet_mode(INT16 mode);
	\endcode
	@param mode \c 0 to allow all communication;\n
				\c 1 to block commands (allows only key presses, messages, and time or variable read requests);\n
				\c 2 to disable all commands, requests and messages;\n
				\c -1 to just return current setting.
	@return Returns the previous mode settings.
 */
INT16INTFCN(quiet_mode)
/*! \fn def pylink::EyeLinkCBind::flushKeybuttons
	Causes the EyeLink tracker and the EyeLink library to flush any stored button or key
	events. This should be used before a trial to get rid of old button responses.  The
	\c &lt;enable_buttons&gt; argument controls whether the EyeLink library will store button
	press and release events.  It always stores tracker key events.  Even if disabled,
	the last button pressed and button flag bits are updated.

	@remarks This is equivalent to the C API
	\code
	INT16 eyelink_flush_keybuttons(INT16 enable_buttons);
	\endcode
	@param enable_buttons Sets to \c 0 to monitor last button press only, \c 1 to queue button events.
	@return Always \c 0.
 */
INT16INTFCN(flush_keybuttons)
/*! \fn def pylink::EyeLinkCBind::dataSwitch
	Sets what data from tracker will be accepted and placed in queue.
	@remarks This does not start the tracker recording, and so can be used with \c broadcastOpen().
	It also does not clear old data from the queue.  This function is equivalent to the C API
	\code
	INT16 eyelink_data_switch(UINT16 flags);
	\endcode
	@param flags Bitwise OR of the following flags:\n
					\arg \c RECORD_LINK_SAMPLES - send samples on link.\n
					\arg \c RECORD_LINK_EVENTS - send events on link.
	@return \c 0 if no error, else link error code.
 */
INT16INTFCN(data_switch)
/*! \fn def pylink::EyeLinkCBind::waitForModeReady
	After a mode-change command is given to the EyeLink tracker, an additional 5 to 30 milliseconds
	may be needed to complete mode setup. Call this function after mode change functions.

	@remarks If it does not return \c 0, assume a tracker error has occurred.  This function is
	equivalent to the C API
	\code
	INT16 eyelink_wait_for_mode_ready(UINT32 maxwait);
	\endcode
	@param maxwait Maximum milliseconds to wait for the mode to change.
	@return \c 0 if mode switching is done, else still waiting.
 */
INTINTFCN(wait_for_mode_ready)
/*! \fn def pylink::EyeLinkCBind::requestFileBlock
	\todo Need description.
 */
INTINTFCN(request_file_block)


/*! \fn def pylink::EyeLinkCBind::startData
	Switches tracker to Record mode, enables data types for recording to EDF file or sending to
	link.  These types are set with a bit wise OR of these flags:

	<table>
	<tr><td class="indexkey">Constant Name</td><td class="indexkey" align="center">Value</td><td class="indexkey">Description</td></tr>
  	<tr><td>\c RECORD_FILE_SAMPLES</td><td>\c 1</td><td>Enables sample recording to EDF file</td></tr>
	<tr><td>\c RECORD_FILE_EVENTS</td><td>\c 2</td><td>Enables event recording to EDF file</td></tr>
	<tr><td>\c RECORD_LINK_SAMPLES</td><td>\c 4</td><td>Enables sending samples to the link</td></tr>
	<tr><td>\c RECORD_LINK_EVENTS</td><td>\c 8</td><td>Enables sending events to the link</td></tr>
	</table>
	@remarks If \c &lt;lock&gt; is nonzero, the recording may only be terminated through \c stopRecording()
	or \c stopData() method of the EyeLinkListener class, or by the Abort menu ('Ctrl' 'Alt' 'A' keys on
	the eye tracker).  If zero, the tracker 'ESC' key may be used to halt recording.  This function
	is equivalent to the C API
	\code
	INT16 eyelink_data_start(UINT16 flags, INT16 lock);
	\endcode
	@param flags Bitwise OR of flags to control what data is recorded.  If \c 0, recording will be stopped.
	@param lock If nonzero, prevents 'ESC' key from ending recording.
	@return \c 0 if command sent fine, else link error.
 */
INTINTINTFCN(data_start)
/*! \fn def pylink::EyeLinkCBind::startDriftCorrect
	Sets the position of the drift correction target, and switches the tracker to drift-correction
	mode. Should be followed by a call to \c waitForModeReady() method.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_driftcorr_start(INT16 x, INT16 y);
	\endcode
	@param x x position of the target.
	@param y y position of the target.
	@return \c 0 if command sent fine, else link error.
 */
INTINTINTFCN(driftcorr_start)
/*! \fn def pylink::EyeLinkCBind::getDataCount
	Counts total items in queue: samples, events, or both.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_data_count(INT16 samples, INT16 events);
	\endcode
	@param samples if non-zero count the samples.
	@param events if non-zero count the events.
	@return Total number of samples and events is in the queue.
 */
INTINTINTFCN(data_count)
/*! \fn def pylink::EyeLinkCBind::isInDataBlock
	Checks to see if framing events read from queue indicate that the data is in a block containing
	samples, events, or both.

	@remarks The first item in queue may not be a block start even, so this should be used in a loop
	while discarding items using \c eyelink_get_next_data(NULL).  NOTE: this function did not work
	reliably in versions of the DLL before v2.0 (did not detect end of blocks).  This function is
	equivalent to the C API
	\code
	INT16 eyelink_in_data_block(INT16 samples, INT16 events);
	\endcode
	@param samples if non-zero, check if in a block with samples.
	@param events if non-zero, check if in a block with events.
	@return \c 0 if no data of either masked type is being sent.
 */
INTINTINTFCN(in_data_block)

/*! \fn def pylink::EyeLinkCBind::waitForBlockStart
	Reads and discards events in data queue until in a recording block.  Waits for up to \c &lt;timeout&gt;
	milliseconds for a block containing samples, events, or both to be opened.  Items in the queue are
	discarded until the block start events are found and processed.  This function will fail if both
	samples and events are selected but only one of link samples and events were enabled by \c startRecording().

	@remarks This function did not work in versions previous to 2.0.  This function is equivalent to the C API
	\code
	INT16 eyelink_wait_for_block_start(UINT32 maxwait,INT16 samples, INT16 events);
	\endcode
	@param timeout Time in milliseconds to wait.
	@param samples If non-zero, check if in a block with samples.
	@param events If non-zero, check if in a block with events.
	@return \c 0 if time expired without any data of masked types available.
 */
INT_INTINTINTFCN(wait_for_block_start)
/*! \fn def pylink::EyeLinkCBind::waitForData
	Waits for data to be received from the eye tracker.  Can wait for an event, a sample, or either.
	Typically used after record start to check if data is being sent.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_wait_for_data (UINT32 maxwait, INT16 samples, INT16 events);
	\endcode
	@param maxwait Time in milliseconds to wait for data.
	@param samples If \c 1, return when first sample available.
	@param events If \c 1,  return when first event available.
	@return \c 1 if data is available; 0 if timed out.
 */
INT_INTINTINTFCN(wait_for_data)
/*! \fn def pylink::EyeLinkCBind::sendKeybutton
	Sends a key or button event to tracker.  Only key events are handled for remote control.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_send_keybutton(UINT16 code, UINT16 mods, INT16 state);
	\endcode
	@param code Key character, or <tt>KB_BUTTON (0xFF00)</tt> if sending button event.
	@param mods Button number, or key modifier (Shift, Alt and Ctrl key states).
	@param state Key or button change (\c KB_PRESS or \c KB_RELEASE).
	@return \c 0 if OK, else send link error.
 */
INT_INTINTINTFCN(send_keybutton)

#ifndef NO_SDL

/*! \fn def pylink::EyeLinkCBind::disableCustomBackgroundOnImageMode
	Internal function. This should not be used externally.
 */
VOIDVOIDFCN(disable_custombackground_on_imagemode)
/*! \fn def pylink::EyeLinkCBind::resetBackground
	Internal function. This should not be used externally.
 */
VOIDVOIDFCN(reset_background_surface)

/*! \fn def pylink::EyeLinkCBind::stopRecording
	Stops recording, resets EyeLink data mode.  Call 50 to 100 msec after an event occurs
	that ends the trial. This function waits for mode switch before returning.

	@remarks This is equivalent to the C API
	\code
	void stop_recording(void);
	\endcode
 */
#endif
VOIDVOIDFCN(stop_recording)
/*! \fn def pylink::EyeLinkCBind::setOfflineMode
	Places EyeLink tracker in off-line (idle) mode. Wait till the tracker has finished
	the mode transition.

	@remarks This is equivalent to the C API
	\code
	INT16 set_offline_mode(void);
	\endcode
 */
VOIDVOIDFCN(set_offline_mode)
/*VOIDVOIDFCN(close_eyelink_connection) */
/*! \fn def pylink::EyeLinkCBind::flushGetkeyQueue
	Initializes the key queue used by \c getkey().  It may be called at any time to get rid
	any of old keys from the queue.

	@remarks This is equivalent to the C API
	\code
	void flush_getkey_queue(void);
	\endcode
 */
VOIDVOIDFCN(flush_getkey_queue)
/*! \fn def pylink::EyeLinkCBind::exitCalibration
	This function should be called from a message or event handler if an ongoing call to
	\c doDriftCorrect() or \c doTrackerSetup() should return immediately.

	@remarks This function is equivalent to the C API
	\code
	void exit_calibration(void);
	\endcode
 */
VOIDVOIDFCN(exit_calibration)
/*! \fn def pylink::EyeLinkCBind::endRealTimeMode
	Returns the application to a priority slightly above normal, to end realtime mode. This
	function should execute rapidly, but there is the possibility that Windows will allow
	other tasks to run after this call, causing delays of 1-20 milliseconds.

	@remarks This function is equivalent to the C API
	\code
	void end_realtime_mode(void);
	\endcode
 */
VOIDVOIDFCN(end_realtime_mode)
/*VOIDVOIDFCN(set_high_priority)
VOIDVOIDFCN(set_normal_priority) */




/*! \fn def pylink::EyeLinkCBind::pumpMessages
	Forces the graphical environment to process any pending key or mouse events.

	@remarks This function is equivalent to the C API
	\code
	INT16 message_pump(HWND dialog_hook);
	\endcode
 */
VOIDINT2FCN(message_pump)
/*! \fn def pylink::EyeLinkCBind::isRecording
	Check if we are recording: if not, report an error. Call this function while recording. It
	will return true if recording is still in progress, otherwise it will throw an exception.
	It will also handle the EyeLink Abort menu.  Any errors returned by this function should
	be returned by the trial function.  On error, this will disable realtime mode and restore
	the heuristic.

	@remarks This function is equivalent to the C API
	\code
	int check_recording(void);
	\endcode
	@return <tt>TRIAL_OK (0)</tt> if no error.\n
			<tt>REPEAT_TRIAL, SKIP_TRIAL, ABORT_EXPT, TRIAL_ERROR</tt> if recording aborted.
 */
VOIDINT2FCN(check_recording)
/*! \fn def pylink::EyeLinkCBind::getRecordingStatus
	Checks if we are in Abort menu after recording stopped and returns trial exit code. Call this
	function on leaving a trial. It checks if the EyeLink tracker is displaying the Abort menu,
	and handles it if required. The return value from this function should be returned as the
	trial result code.

	@remarks This function is equivalent to the C API
	\code
	INT16 check_record_exit(void);
	\endcode
	@return \c TRIAL_OK if no error.\n
			<tt>REPEAT_TRIAL, SKIP_TRIAL, ABORT_EXPT</tt> if Abort menu activated.
 */
VOIDINT2FCN(check_record_exit)
/*! \fn def pylink::EyeLinkCBind::doTrackerSetup
	Switches the EyeLink tracker to the Setup menu, from which camera setup, calibration, validation,
	drift correction, and configuration may be performed.  Pressing the 'ESC' key on the tracker
	keyboard will exit the Setup menu and return from this function. Calling \c exitCalibration() from
	an event handler will cause any call to \c do_tracker_setup() in progress to return immediately.

	@param width Width of the screen.
	@param height Height of he screen.
 */
VOIDINT2FCN(do_tracker_setup)
/*! \fn def pylink::EyeLinkCBind::imageModeDisplay
	This handles display of the EyeLink camera images.  While in imaging mode,
 	it contiuously requests and displays the current camera image.  It also
 	displays the camera name and threshold setting.  Keys on the subject PC
 	keyboard are sent to the tracker, so the experimenter can use it during
 	setup.  It will exit when the tracker leaves imaging mode or disconnects.

 	@return \c 0 if OK, \c TERMINATE_KEY if pressed, \c -1 if disconnect.

 	@remark This function not normally used externally.  If you need camera setup
 	use doTrackerSetup() or if you need drift correction use doDriftCorrect()
 */
VOIDINT2FCN(image_mode_display)
/*! \fn def pylink::EyeLinkCBind::targetModeDisplay
	This function needs some "helper" graphics to clear the scren and draw the
	fixation targets.  Since C graphics are compiler-dependent, these are found
	in other C source files.

	While tracker is in any mode with fixation targets...
	Reproduce targets tracker needs.
	(if local_trigger) Local Spacebar acts as trigger.
	(if local_control)  Local keys echoes to tracker.
	@return \c 0 if OK, \c 27 if aborted, \c TERMINATE_KEY if pressed..
 */
VOIDINT2FCN(target_mode_display)
/*! \fn def pylink::EyeLinkCBind::closeDataFile
	Closes any currently opened EDF file on the EyeLink tracker computer's hard disk. This may take
	several seconds to complete.

	@remarks This function is equivalent to the C API
	\code
	int close_data_file(void);
	\endcode
	@return \c 0 if command executed successfully else error code.
 */
VOIDINT2FCN(close_data_file)
/*! \fn def pylink::EyeLinkCBind::key_message_pump
	Similar to \c pumpMessages(), but only processes keypresses.
 	This may help reduce latency.
 */
VOIDINT2FCN(key_message_pump)
/*! \fn def pylink::EyeLinkCBind::escapePressed
	This function tests if the 'ESC' key is held down, and is usually used to break out of nested loops.

	@remarks Under Windows XP, this call will not work in realtime mode at all, and will take several
	seconds to respond if graphics are being drawn continuously.  This function works well in realtime
	mode under Windows 2000.  This function is equivalent to the C API
	\code
	INT16 escape_pressed(void);
	\endcode
	@return \c 1 if 'ESC' key held down \c 0 if not.
 */
VOIDINT2FCN(escape_pressed)
/*! \fn def pylink::EyeLinkCBind::breakPressed
	Tests if the program is being interrupted. You should break out of loops immediately if this function
	does not return \c 0, if \c getkey() return \c TERMINATE_KEY, or if \c isConnected() method of the
	class returns \c 0.

	@remarks Under Windows XP, this call will not work in realtime mode at all, and will take several
	seconds to respond if graphics are being drawn continuously.  This function works well in realtime
	mode under Windows 2000.  This function is equivalent to the C API
	\code
	INT16 break_pressed(void);
	\endcode
	@return \c 1 if CTRL-C is pressed, \c terminalBreak() was called,  or the program has been terminated with
			ALT-F4; \c 0 otherwise
 */
VOIDINT2FCN(break_pressed)
/*! \fn def pylink::enableExtendedRealtime
	Internal function. This should not be used externally.
 */
VOIDVOIDFCN(eyelink_enable_extended_realtime)
/*! \fn def pylink::closeMessageFile
	Flush and close message file, opened by openMessageFile().
 */
VOIDVOIDFCN(close_message_file)

/*! \fn def pylink::EyeLinkCBind::getkey
	Returns the key pressed.

	@remarks Warning: This function processes and dispatches any waiting messages.  This will allow Windows
	to perform disk access and negates the purpose of realtime mode.  Usually these delays will be only a
	few milliseconds, but delays over 20 milliseconds have been observed.  You may wish to call
	\c escapePressed() or \c breakPressed() in recording loops instead of \c getkey() if timing is critical,
	for example in a gaze-contingent display.  Under Windows XP, these calls will not work in realtime mode
	at all (although these do work under Windows 2000).  Under Windows 95/98/Me, realtime performance is
	impossible even with this strategy.
	Some useful keys are:
	\arg \c CURS_UP
	\arg \c CURS_DOWN
	\arg \c CURS_LEFT
	\arg \c CURS_RIGHT
	\arg \c ESC_KEY
	\arg \c ENTER_KEY
	\arg \c TERMINATE_KEY
	\arg \c JUNK_KEY

	This function is equivalent to the C API
	\code
	unsigned getkey(void);
	\endcode
	@return \c 0 if no key pressed, else key code. \c TERMINATE_KEY if CTRL-C held down or program has been terminated.
 */
VOIDUINT2FCN(getkey)
/*! \fn def pylink::EyeLinkCBind::getkeyEx
	Returns the key pressed. Same as getkey() except, this returns a tuple with the first value contanins the key and
	the second contains value contains the modifier.

	@remarks Warning: This function processes and dispatches any waiting messages.  This will allow Windows
	to perform disk access and negates the purpose of realtime mode.  Usually these delays will be only a
	few milliseconds, but delays over 20 milliseconds have been observed.  You may wish to call
	\c escapePressed() or \c breakPressed() in recording loops instead of \c getkey() if timing is critical,
	for example in a gaze-contingent display.  Under Windows XP, these calls will not work in realtime mode
	at all (although these do work under Windows 2000).  Under Windows 95/98/Me, realtime performance is
	impossible even with this strategy.
	Some useful keys are:
	\arg \c CURS_UP
	\arg \c CURS_DOWN
	\arg \c CURS_LEFT
	\arg \c CURS_RIGHT
	\arg \c ESC_KEY
	\arg \c ENTER_KEY
	\arg \c TERMINATE_KEY
	\arg \c JUNK_KEY

	This function is equivalent to the C API
	\code
	unsigned getkey(void);
	\endcode
	@return \c 0 if no key pressed, else key code. \c TERMINATE_KEY if CTRL-C held down or program has been terminated.
 */
VOIDUINT4FCN(getkey_with_mod)
/*! \fn def pylink::EyeLinkCBind::echo_key
	Checks for Windows keystroke events and dispatches messages; similar to \c getkey(), but also sends
	keystroke to tracker.

	@remarks Warning:  Under Windows XP, this call will not work in realtime mode at all, and will take
	several seconds to respond if graphics are being drawn continuously.  This function works well in
	realtime mode under Windows 2000.  This function is equivalent to the C API
	\code
	unsigned echo_key(void);
	\endcode
	@return \c 0 if no key pressed, else key code \c TERMINATE_KEY if CTRL-C held down or program has been terminated.
 */
VOIDUINT2FCN(echo_key)
/*! \fn def pylink::EyeLinkCBind::readKeyQueue
	Read keys from the key queue. It is similar to \c getkey(), but does not process Windows messages.
	This can be used to build key-message handlers in languages other than C.

	@remarks This function is equivalent to the C API
	\code
	UINT16 read_getkey_queue(void);
	\endcode
	@return \c 0 if no key pressed.\n
			<tt>JUNK_KEY (1)</tt> if untranslatable key.\n
			<tt>TERMINATE_KEY (0x7FFF)</tt> if CTRL-C is pressed, \c terminal_break() was called,  or the program has
			been terminated with ALT-F4.\n
			or code of key if any key pressed.
 */
VOIDUINT2FCN(read_getkey_queue)

/*! \fn def pylink::EyeLinkCBind::currentTime
	Returns the current millisecond time since the initialization of the EyeLink library.

	@remarks This function is equivalent to the C API
	\code
	UINT32 current_time(void);
	\endcode
	@return Long integer for the current millisecond time since the initialization of the EyeLink library.
 */
DOUBLEVOID2FCN(current_time)
/*! \fn def pylink::EyeLinkCBind::currentUsec
	Returns the current microsecond time since the initialization of the EyeLink library.

	@remarks This is equivalent to the C API
	\code
	UINT32 current_usec(void);
	\endcode
	@return Long integer for the current microsecond time since the initialization of the EyeLink library.
 */
DOUBLEVOID2FCN(current_usec)
/*! \fn def pylink::EyeLinkCBind::currentDoubleUsec
	Returns the current microsecond time (as a double type) since the initialization of the EyeLink library.

	@remarks Same as \c currentUsec() except, this function can return large microseconds.  That is the
	\c currentUsec() can return up to \c 2147483648 microseconds starting from initialization. The
	\c currentDoubleUsec() can return up to \c 36028797018963968 microseconds.
	This is equivalent to the C API
	\code
	double current_double_usec(void);
	\endcode
	@return A float data for the current microsecond time since the initialization of the EyeLink library.
 */
DOUBLEVOID2FCN(current_double_usec)

/*! \fn def pylink::EyeLinkCBind::sendCommand
	Sends the given command to connected eyelink tracker and returns the command result.

	@remarks This is equivalent to the C API
	\code
	int eyecmd_printf(char *fmt, ...); // without any formatting.
	\endcode
	@param command_text Text command to be sent.  It does not support \c printf() kind of formatting.
	@return Command result.  If there is any problem sending the command, a runtime exception is raised.
 */
INTSTRFCN_MAX_CHECK(eyecmd_printf)
/*! \fn def pylink::EyeLinkCBind::readRequest
	Sends a text variable name whose value is read and returned by the tracker as a text string.

	@remarks If the link is initialized but not connected to a tracker, the message will be sent
	to the tracker set by \c setAddress(). However, these requests will be ignored by tracker
	versions older than EyeLink I v2.1 and EyeLink II v1.1.
	This is equivalent to the C API
	\code
	INT16 eyelink_read_request(char *text);
	\endcode
	@param text String with message to send.
	@return \c 0 if success, otherwise link error code.
 */
INTSTRFCN(eyelink_read_request)
/*! \fn def pylink::EyeLinkCBind::sendMessage
	Sends the given message to the connected eyelink tracker. The message will be written to the eyelink tracker.

	@remarks This is equivalent to the C API
	\code
	int eyemsg_printf(char *fmt, ...);
	\endcode
	@param message_text Text message to be sent.  It does not support \c printf() kind of formatting.
	@return If there is any problem sending the message, a runtime exception is raised.
 */
#ifdef WIN32
INTSTRFCN_international(eyemsg_printf)
INTSTRFCN_international(timemsg_printf)
#else
static PyObject* eyelink_eyelink_eyemsg_printf(PyObject* self, PyObject* args);
static PyObject* eyelink_eyelink_timemsg_printf(PyObject* self, PyObject* args);
#endif
/*! \fn def pylink::EyeLinkCBind::openDataFile
	Opens a new EDF file on the EyeLink tracker computer's hard disk. By calling this function will close
	any currently opened file. This may take several seconds to complete. The file name should be formatted for
	MS-DOS, usually 8 or less characters with only 0-9, A-Z, and '_' allowed.

	@remarks This function is equivalent to the C API
	\code
	int open_data_file(char *name);
	\endcode
	@param name Name of eye tracker file, 8 characters or less.
	@return \c 0 if file was opened successfully else error code.
 */

INTSTRFCN(open_data_file)
/*! \fn def pylink::openMessageFile
	Creates message file, once open call to sendMessageToFile(), will not send messages to tracker.
	Messages are kept in a queue if the application is in realtime mode, and written to disk on
	non real-time mode except when closeMessageFile() is called while in real-time mode.
	@param[in] fname Message file name
 */
INTSTRFCN(open_message_file)
/*! \fn def pylink::EyeLinkCBind::setAddress
	Sets the IP address used for connection to the EyeLink tracker. This is set to "100.1.1.1" in the
	DLL, but may need to be changed for some network configurations. This must be set before attempting
	to open a connection to the tracker.

	A "broadcast" address ("255.255.255.255") may be used if the tracker address is not known - this will
	work only if a single Ethernet card is installed, or if DLL version 2.1 or higher, and the latest
	tracker software versions (EyeLink I v2.1 or higher, and EyeLink II v1.1 or higher) are installed.

	@remarks This is equivalent to the C API
	\code
	INT16 set_eyelink_address(char *addr);
	\endcode
	@param text_IP_address Pointer to a string containing a "dotted" 4-digit IP address;
	@return \c 0 if success, \c -1 if could not parse address string.
 */
INTSTRFCN(set_eyelink_address)

/*! \fn def pylink::EyeLinkCBind::startRecording
	Starts the EyeLink tracker recording, sets up link for data reception if enabled.

	@remarks Recording may take 10 to 30 milliseconds to begin from this command. The function also waits
	until at least one of all requested link data types have been received. If the return value is not zero,
	return the result as the trial result code.
	This is equivalent to the C API
	\code
	INT16 start_recording(INT16 file_samples, INT16 file_events, INT16 link_samples, INT16 link_events);
	\endcode
	@param file_samples If \c 1, writes samples to EDF file. If \c 0, disables sample recording.
	@param file_events If \c 1, writes events to EDF file. If \c 0, disables event recording.
	@param link_samples If \c 1, sends samples through link. If \c 0, disables link sample access.
	@param link_events If \c 1, sends events through link. If \c 0, disables link event access.
	@return \c 0 if successful, else trial return code.
 */
INT_4INTFCN_NOERROR(start_recording)
/*! \fn def pylink::EyeLinkCBind::doDriftCorrect
	Performs a drift correction before a trial.

	@remarks This is equivalent to the C API
	\code
	int do_drift_correct(int x, int y, int draw, int allow_setup);
	\endcode
	@param x X Position (in pixels) of drift correction target.
	@param y Y Position (in pixels) of drift correction target.
	@param draw If \c 1, the drift correction will clear the screen to the target background color,
				draw the target, and clear the screen again when the drift correction is done.
				If \c 0, the fixation target must be drawn by the user.
	@param allow_setup If \c 1, accesses Setup menu before returning, else aborts drift correction.
	@return \c 0 if successful, \c 27 if 'ESC' key was pressed to enter Setup menu or abort.
 */
INT_4INTFCN(do_drift_correct)

/*! \fn def pylink::EyeLinkCBind::trackerTimeUsecOffset
	Returns the time difference between the tracker time and display pc time.

	@remarks This is equivalent to the C API
	\code
	double eyelink_time_usec_offset();
	\endcode
	@return A double precision data for the time difference (in microseconds) between the tracker
			time and display pc time.
 */
DOUBLEVOIDFCN(double_usec_offset);
/*! \fn def pylink::EyeLinkCBind::trackerTimeUsec
	Returns the current tracker time (in microseconds) since the tracker application started.

	@remarks This is equivalent to the C API
	\code
	UINT32 eyelink_tracker_time();
	\endcode
	@return A double precision data for the current tracker time (in microseconds) since tracker
			initialization.
 */
DOUBLEVOIDFCN(tracker_double_usec);
/*! \fn def pylink::EyeLinkCBind::trackerTimeOffset
	Returns the time difference between the tracker time and display pc time.

	@remarks This is equivalent to the C API
	\code
	UINT32 eyelink_time_offset();
	\endcode
	@return An integer data for the time difference (in milliseconds) between the tracker time
			and display pc time.
 */
DOUBLEVOIDFCN(msec_offset);
/*! \fn def pylink::EyeLinkCBind::trackerTime
	Returns the current tracker time (in milliseconds) since the tracker application started.

	@remarks This is equivalent to the C API
	\code
	UINT32 eyelink_tracker_time();
	\endcode
	@return An integer data for the current tracker time (in milliseconds) since tracker
			initialization.
 */
DOUBLEVOIDFCN(tracker_msec);
GENERICFCNPROTO(set_name);
GENERICFCNPROTO(node_send);
GENERICFCNPROTO(timed_command);
GENERICFCNPROTO(timed_commandEx);
GENERICFCNPROTO(node_send_message);
GENERICFCNPROTO(node_request_time);
GENERICFCNPROTO(open);
GENERICFCNPROTO(get_node);
GENERICFCNPROTO(node_receive);
GENERICFCNPROTO(open_node);
GENERICFCNPROTO(last_button_press);
GENERICFCNPROTO(target_check);
GENERICFCNPROTO(mode_data);
GENERICFCNPROTO(read_keybutton);
GENERICFCNPROTO(receiveDataFile);
/*! \fn def pylink::EyeLinkCBind::_drawCalTarget
	\todo Need description.
 */
GENERICFCNPROTO(map_eyelink_py_draw_cal_target_hook);
GENERICFCNPROTO( eyelink_last_button_states);


/*******************************************************************************
display
********************************************************************************/

#ifndef NO_SDL
GENERICFCNPROTO(set_calibration_colors);
GENERICFCNPROTO(set_target_size);
GENERICFCNPROTO(set_cal_sounds);
GENERICFCNPROTO(set_dcorr_sounds);
GENERICFCNPROTO(set_camera_image_position);
GENERICFCNPROTO(get_display_information);
GENERICFCNPROTO(init_expt_graphics);
GENERICFCNPROTO(close_expt_graphics);
GENERICFCNPROTO(get_image_xhair_data);


#endif




/*GENERICFCNPROTO(open_eyelink_system); removed */
/*! \fn def pylink::EyeLinkCBind::_getDataStatus
	\todo Need description.
 */
GENERICFCNPROTO(data_status);


/*! \fn def pylink::EyeLinkCBind::pumpDelay
	During calls to \c msecDelay(), Windows is not able to handle messages. One result of this is
	that windows may not appear. This is the preferred delay function when accurate timing is not
	needed. It calls \c pumpMessages() until the last 20 milliseconds of the delay, allowing
	Windows to function properly. In rare cases, the delay may be longer than expected. It does
	not process modeless dialog box messages.

	@remarks This is equivalent to the C API
	\code
	void pump_delay(UINT32 delay);
	\endcode
	@param delay An integer, which sets number of milliseconds to delay.
 */
VOIDUINT32FCN(pump_delay);
/*! \fn def pylink::EyeLinkCBind::beginRealTimeMode
	Sets the application priority and cleans up pending Windows activity to place the application in
	realtime mode. This could take up to 100 milliseconds, depending on the operation system, to set the
	application priority.

	@remarks This function is equivalent to the C API
	\code
	void begin_realtime_mode(UINT32 delay);
	\endcode
	@param delay An integer, used to set the minimum time this function takes, so that this function
				can act as a useful delay.
 */
VOIDUINT32FCN(begin_realtime_mode);
/*! \fn def pylink::EyeLinkCBind::msecDelay
	Does a unblocked delay using \c currentTime().

	@remarks This is equivalent to the C API
	\code
	void msec_delay(UINT32 n);
	\endcode
	@param delay An integer for number of milliseconds to delay.
 */
VOIDUINT32FCN(msec_delay);
/*! \fn def pylink::EyeLinkCBind::terminalBreak
	This function can be called in an event handler to signal that the program is terminating. Calling
	this function with an argument of \c 1 will cause \c breakPressed() to return \c 1, and \c getkey()
	to return \c TERMINATE_KEY. These functions can be re-enabled by calling \c terminalBreak() with
	an argument of \c 0.

	@remarks This function is equivalent to the C API
	\code
	void terminal_break(INT16 assert);
	\endcode
	@param assert \c 1 to signal a program break, \c 0 to reset break.
 */
VOIDUINT16FCN(terminal_break);
/*! \fn def pylink::EyeLinkCBind::alert
	This method is used to give a notification to the user when an error occurs.

	@remarks This function does not allow printf formatting as in c.  However you can do a formatted
	string argument in python.  This is equivalent to the C API
	\code
	void alert_printf(char *fmt, ...);
	\endcode
	@param message Text message to be displayed.
 */
VOIDSTR2FCN(alert_printf);

GENERICFCNPROTO(bitmapSaveAndBackdrop);
GENERICFCNPROTO(bitmapSave);

#ifndef NO_SDL
GENERICFCNPROTO(setCalibrationAnimationTarget);
#endif

GENERICFCNPROTO(bitmapBackdrop);

#ifdef WIN32
GENERICFCNPROTO(eyelink_calculate_velocity);
GENERICFCNPROTO(eyelink_calculate_velocity_x_y);
GENERICFCNPROTO(eyelink_calculate_overallvelocity_and_acceleration);
#endif


#define bitmapSaveAndBackdrop_doc "bitmapSaveAndBackdrop(iwidth,iheight,pixels,xs, ys, width,  height,fname,path, sv_options,xd,yd,  xferoptions)"\
"iwidth  - original image width"\
"iheight - original image height"\
"pixels  - pixels of the image in the following format.pixel=[line1, line2, ... linen] line=[pix1,pix2,...,pixn],pix=(r,g,b) "\
"xs - crop x position"\
"ys - crop y position"\
"width - crop width"\
"height - crop height"\
"fname - file name to save"\
"path -  path to save"\
"svoptions - save options(SV_NOREPLACE,SV_MAKEPATH)"\
"xd - xposition - transfer"\
"yd - yposition - transfer"\
"xferoptions - transfer options(BX_AVERAGE,BX_DARKEN,BX_LIGHTEN,BX_MAXCONTRAST,BX_NODITHER,BX_GRAYSCALE)"

#define bitmapSave_doc "bitmapSave(iwidth,iheight,pixels,xs, ys, width,  height,fname,path, sv_options)"\
"iwidth  - original image width"\
"iheight - original image height"\
"pixels  - pixels of the image in the following format.pixel=[line1, line2, ... linen] line=[pix1,pix2,...,pixn],pix=(r,g,b) "\
"xs - crop x position"\
"ys - crop y position"\
"width - crop width"\
"height - crop height"\
"fname - file name to save"\
"path -  path to save"\
"svoptions - save options(SV_NOREPLACE,SV_MAKEPATH)"

#define bitmapBackdrop_doc "bitmapBackdrop(iwidth,iheight,pixels,xs, ys, width,  height,xd,yd,  xferoptions)"\
"iwidth  - original image width"\
"iheight - original image height"\
"pixels  - pixels of the image in the following format.pixel=[line1, line2, ... linen] line=[pix1,pix2,...,pixn],pix=(r,g,b) "\
"xs - crop x position"\
"ys - crop y position"\
"width - crop width"\
"height - crop height"\
"xd - xposition - transfer"\
"yd - yposition - transfer"\
"xferoptions - transfer options(BX_AVERAGE,BX_DARKEN,BX_LIGHTEN,BX_MAXCONTRAST,BX_NODITHER,BX_GRAYSCALE)"

#define eyelink_calculate_velocity_doc "calculates velocity from queue of samples"
#define eyelink_calculate_velocity_x_y_doc "calculates velocity from queue of samples. The value is x,y are separate"
#define eyelink_calculate_overallvelocity_and_acceleration_doc "calculates velocity and acceleration from queue of samples"



/*! \fn def pylink::EyeLinkCBind::close
	Sends a disconnect message to the EyeLink tracker.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_close(int send_msg);  // with send_msg parameter 1.
	\endcode
	@return 0 if successful, otherwise link error.
 */
static PyObject* eyelink_eyelink_close(PyObject* self, PyObject* args)\
{
	if (PyArg_ParseTuple(args,":"))
	{
		int rv = eyelink_close(1);
		return Py_BuildValue("i",rv);
	}
	return NULL;
}

/*! \fn def pylink::EyeLinkCBind::reset
	Sends a reset message to the EyeLink tracker.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_close(int send_msg);  // with send_msg parameter 0.
	\endcode
	@return 0 if successful, otherwise link error.
 */
static PyObject* eyelink_eyelink_reset(PyObject* self, PyObject* args)\
{
	if (PyArg_ParseTuple(args,":"))
	{
		int rv = eyelink_close(0);
		return Py_BuildValue("i",rv);
	}
	return NULL;
}

/*! \fn def pylink::EyeLinkCBind::getNextData
	Fetches next data item from link buffer and returns the data item type.  If the item is not wanted, simply ignore it.
	Otherwise, call \c getFloatData() to read it into a buffer.
	@return \c 0 if no data, \c SAMPLE_TYPE if sample, else event type.

 */
static PyObject* eyelink_eyelink_get_next_data(PyObject* self, PyObject* args)
{
	if (PyArg_ParseTuple(args,":"))
	{
		void *data = NULL;
		UINT16 rv = eyelink_get_next_data(data);
		return Py_BuildValue("i",rv);
	}
	return NULL;
}


static PyObject* allf_data_ctopyobj(ALLF_DATA *data,  INT16 type)
{

	if(!trackerModule)
		getTrackerModule();
	if(data && type)
	{
		switch(type)
		{
		case SAMPLE_TYPE:
				return PyObject_CallFunction(newSample,"di(dd)(dd)(dd)(dd)(dd)(dd)(dd)ddiiii(iiiiiiii)",
					FLOAT_TIME(&(data->fs)),
					data->fs.flags,
					data->fs.px[0], data->fs.px[1],
					data->fs.py[0], data->fs.py[1],
					data->fs.hx[0], data->fs.hx[1],
					data->fs.hy[0], data->fs.hy[1],
					data->fs.pa[0], data->fs.pa[1],
					data->fs.gx[0], data->fs.gx[1],
					data->fs.gy[0], data->fs.gy[1],
					data->fs.rx,
					data->fs.ry,
					data->fs.status,
					data->fs.input,
					data->fs.buttons,
					data->fs.htype,
					data->fs.hdata[0],
					data->fs.hdata[1],
					data->fs.hdata[2],
					data->fs.hdata[3],
					data->fs.hdata[4],
					data->fs.hdata[5],
					data->fs.hdata[6],
					data->fs.hdata[7]
					);

		case STARTBLINK:
			return PyObject_CallFunction(newStartBlinkEvent,"diiid",
				(double)(data->fe.time),
				data->fe.type,
				data->fe.eye,
				data->fe.read,
				(double)(data->fe.sttime));

		case ENDBLINK:
			return PyObject_CallFunction(newEndBlinkEvent,"diiidd",
				(double)(data->fe.time),
				data->fe.type,
				data->fe.eye,
				data->fe.read,
				(double)(data->fe.sttime),
				(double)(data->fe.entime)
				);

		case STARTSACC:
			return PyObject_CallFunction(newStartSacadeEvent,"diiddddddddd",
				(double)(data->fe.time),
				data->fe.type,
				data->fe.eye,
				(double)(data->fe.sttime),
				data->fe.hstx,
				data->fe.hsty,
				data->fe.gstx,
				data->fe.gsty,
				data->fe.supd_x,
				data->fe.supd_y,
				data->fe.sta,
				data->fe.svel);
		case ENDSACC:
			return PyObject_CallFunction(newEndSacadeEvent,"diiidddddddddddddddddd",
				(double)(data->fe.time),
				data->fe.type,
				data->fe.eye,
				data->fe.read,
				(double)(data->fe.sttime),

				data->fe.gstx,
				data->fe.gsty,
				data->fe.hstx,
				data->fe.hsty,
				data->fe.svel,
				data->fe.supd_x,
				data->fe.supd_y,

				(double)(data->fe.entime),

				data->fe.genx,
				data->fe.geny,
				data->fe.henx,
				data->fe.heny,
				data->fe.evel,
				data->fe.avel,
				data->fe.pvel,
				data->fe.eupd_x,
				data->fe.eupd_y);



		case STARTFIX:
			return PyObject_CallFunction(newStartFixationEvent,"diiiddddddddd",
				(double)(data->fe.time),
				data->fe.type,
				data->fe.eye,
				data->fe.read,
				(double)(data->fe.sttime),
				data->fe.gstx,
				data->fe.gsty,
				data->fe.hstx,
				data->fe.hsty,
				data->fe.sta,
				data->fe.svel,
				data->fe.supd_x,
				data->fe.supd_y);

		case ENDFIX:
			return PyObject_CallFunction(newEndFixationEvent,"diiiddddddddddddddddddddddddd",
				(double)(data->fe.time),
				data->fe.type,
				data->fe.eye,
				data->fe.read,
				(double)(data->fe.sttime),
				data->fe.gstx,
				data->fe.gsty,
				data->fe.hstx,
				data->fe.hsty,
				data->fe.sta,
				data->fe.svel,
				data->fe.supd_x,
				data->fe.supd_y,
				(double)(data->fe.entime),
				data->fe.genx,
				data->fe.geny,
				data->fe.gavx,
				data->fe.gavy,
				data->fe.henx,
				data->fe.heny,
				data->fe.havx,
				data->fe.havy,
				data->fe.ena,
				data->fe.ava,
				data->fe.evel,
				data->fe.avel,
				data->fe.pvel,
				data->fe.eupd_x,
				data->fe.eupd_y);
		case FIXUPDATE:
			return PyObject_CallFunction(newFixUpdateEvent,"diiiddddddddddddddddddddddddd",
				(double)(data->fe.time),
				data->fe.type,
				data->fe.eye,
				data->fe.read,
				(double)(data->fe.sttime),
				data->fe.gstx,
				data->fe.gsty,
				data->fe.hstx,
				data->fe.hsty,
				data->fe.sta,
				data->fe.svel,
				data->fe.supd_x,
				data->fe.supd_y,
				(double)(data->fe.entime),
				data->fe.genx,
				data->fe.geny,
				data->fe.gavx,
				data->fe.gavy,
				data->fe.henx,
				data->fe.heny,
				data->fe.havx,
				data->fe.havy,
				data->fe.ena,
				data->fe.ava,
				data->fe.evel,
				data->fe.avel,
				data->fe.pvel,
				data->fe.eupd_x,
				data->fe.eupd_y);

/*			return PyObject_CallFunction(newFixUpdateEvent,"iiddddddddidddddddddddd",
				data->fe.eye,
				data->fe.sttime,
				data->fe.hstx,
				data->fe.hsty,
				data->fe.gstx,
				data->fe.gsty,
				data->fe.supd_x,
				data->fe.supd_y,
				data->fe.sta,
				data->fe.svel,
				data->fe.entime,
				data->fe.henx,
				data->fe.heny,
				data->fe.genx,
				data->fe.geny,
				data->fe.eupd_x,
				data->fe.eupd_y,
				data->fe.ena,
				data->fe.evel,
				data->fe.gavx,
				data->fe.gavy,
				data->fe.havx,
				data->fe.havy);
*/

		case BUTTONEVENT:
		case INPUTEVENT:
			if (data->io.data != 0)
			    return PyObject_CallFunction(newIOEvent,"idi",type, (double)(data->io.time), data->io.data);
			else
				return Py_BuildValue("");
		case MESSAGEEVENT:
			return PyObject_CallFunction(newMessageEvent,"dis",(double)(data->im.time), type,data->im.text);

		}
	}
	return Py_BuildValue("");
}

static PyObject* all_data_ctopyobj(ALL_DATA *data,  INT16 type)
{
	if(data && type)
	{
		switch(type)
		{
		case SAMPLE_TYPE:
				return PyObject_CallFunction(newSample,"ii(ii)(ii)(ii)(ii)(ii)(ii)(ii)iiiiii(iiiiiiii)",
					data->is.time, data->is.flags,
					data->is.px[0], data->is.px[1],
					data->is.py[0], data->is.py[1],
					data->is.hx[0], data->is.hx[1],
					data->is.hy[0], data->is.hy[1],
					data->is.pa[0], data->is.pa[1],
					data->is.gx[0], data->is.gx[1],
					data->is.gy[0], data->is.gy[1],
					data->is.rx,
					data->is.ry,
					data->is.status,
					data->is.input,
					data->is.buttons,
					data->is.htype,
					data->is.hdata[0],
					data->is.hdata[1],
					data->is.hdata[2],
					data->is.hdata[3],
					data->is.hdata[4],
					data->is.hdata[5],
					data->is.hdata[6],
					data->is.hdata[7]
					);

		case STARTBLINK:
			return PyObject_CallFunction(newStartBlinkEvent,"iiiiiiiiii",
				data->ie.eye,
				data->ie.sttime,
				data->ie.hstx,
				data->ie.hsty,
				data->ie.gstx,
				data->ie.gsty,
				data->ie.supd_x,
				data->ie.supd_y,
				data->ie.sta,
				data->ie.svel);

		case ENDBLINK:
			return PyObject_CallFunction(newEndBlinkEvent,"iiiiiiiiiiiiiiiiiiiiiii",
				data->ie.eye,
				data->ie.sttime,
				data->ie.hstx,
				data->ie.hsty,
				data->ie.gstx,
				data->ie.gsty,
				data->ie.supd_x,
				data->ie.supd_y,
				data->ie.sta,
				data->ie.svel,
				data->ie.entime,
				data->ie.henx,
				data->ie.heny,
				data->ie.genx,
				data->ie.geny,
				data->ie.eupd_x,
				data->ie.eupd_y,
				data->ie.ena,
				data->ie.evel,
				data->ie.gavx,
				data->ie.gavy,
				data->ie.havx,
				data->ie.havy
				);

		case STARTSACC:
			return PyObject_CallFunction(newStartSacadeEvent,"iiiiiiiiii",
				data->ie.eye,
				data->ie.sttime,
				data->ie.hstx,
				data->ie.hsty,
				data->ie.gstx,
				data->ie.gsty,
				data->ie.supd_x,
				data->ie.supd_y,
				data->ie.sta,
				data->ie.svel);
		case ENDSACC:
			return PyObject_CallFunction(newEndSacadeEvent,"iiiiiiiiiiiiiiiiiiiiiii",
				data->ie.eye,
				data->ie.sttime,
				data->ie.hstx,
				data->ie.hsty,
				data->ie.gstx,
				data->ie.gsty,
				data->ie.supd_x,
				data->ie.supd_y,
				data->ie.sta,
				data->ie.svel,
				data->ie.entime,
				data->ie.henx,
				data->ie.heny,
				data->ie.genx,
				data->ie.geny,
				data->ie.eupd_x,
				data->ie.eupd_y,
				data->ie.ena,
				data->ie.evel,
				data->ie.gavx,
				data->ie.gavy,
				data->ie.havx,
				data->ie.havy);

		case STARTFIX:
			return PyObject_CallFunction(newStartFixationEvent,"iiiiiiiiii",
				data->ie.eye,
				data->ie.sttime,
				data->ie.hstx,
				data->ie.hsty,
				data->ie.gstx,
				data->ie.gsty,
				data->ie.supd_x,
				data->ie.supd_y,
				data->ie.sta,
				data->ie.svel);

		case ENDFIX:
			return PyObject_CallFunction(newEndFixationEvent,"iiiiiiiiiiiiiiiiiiiiiii",
				data->ie.eye,
				data->ie.sttime,
				data->ie.hstx,
				data->ie.hsty,
				data->ie.gstx,
				data->ie.gsty,
				data->ie.supd_x,
				data->ie.supd_y,
				data->ie.sta,
				data->ie.svel,
				data->ie.entime,
				data->ie.henx,
				data->ie.heny,
				data->ie.genx,
				data->ie.geny,
				data->ie.eupd_x,
				data->ie.eupd_y,
				data->ie.ena,
				data->ie.evel,
				data->ie.gavx,
				data->ie.gavy,
				data->ie.havx,
				data->ie.havy);
		case FIXUPDATE:
			return PyObject_CallFunction(newFixUpdateEvent,"iiiiiiiiiiiiiiiiiiiiiii",
				data->ie.eye,
				data->ie.sttime,
				data->ie.hstx,
				data->ie.hsty,
				data->ie.gstx,
				data->ie.gsty,
				data->ie.supd_x,
				data->ie.supd_y,
				data->ie.sta,
				data->ie.svel,
				data->ie.entime,
				data->ie.henx,
				data->ie.heny,
				data->ie.genx,
				data->ie.geny,
				data->ie.eupd_x,
				data->ie.eupd_y,
				data->ie.ena,
				data->ie.evel,
				data->ie.gavx,
				data->ie.gavy,
				data->ie.havx,
				data->ie.havy);


		case BUTTONEVENT:
		case INPUTEVENT:
			if (data->io.data != 0)
			    return PyObject_CallFunction(newIOEvent,"iii",type, data->io.time, data->io.data);
			else
				return Py_BuildValue("");
		case MESSAGEEVENT:
			return PyObject_CallFunction(newMessageEvent,"is",data->im.time, data->im.text);

		}
	}
	return Py_BuildValue("");
}

#define GETGENERICDATAFCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	if (PyArg_ParseTuple(args,":"))\
	{\
		ALLF_DATA data; \
		UINT16 rv =0; \
		memset(&data,0,sizeof(data));\
		rv = eyelink_##x##(&data);\
		return allf_data_ctopyobj(&data, rv);\
	}\
	return NULL;\
}
#define GETSAMPLEDATAFCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	if (PyArg_ParseTuple(args,":"))\
	{\
		ALLF_DATA data; \
		INT16 rv = 0;\
		memset(&data,0,sizeof(ALLF_DATA));\
		rv = eyelink_##x##(&data);\
		return allf_data_ctopyobj(&data, (UINT16)(rv!=-1?SAMPLE_TYPE:0) );\
	}\
	return NULL;\
}
/*! \fn def pylink::EyeLinkCBind::getFloatData
	Reads data of a specific type returned by \c getNextData().  If this function called multiple times
	without calling \c getNextData(), the same data is returned.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_get_next_data(void *buf);
	\endcode
	@return None if no data available. Otherwise, a valid data is returned.  The returned data type can be:
			\arg Sample
			\arg StartBlinkEvent
			\arg EndBlinkEvent
			\arg StartSacadeEvent
			\arg EndSacadeEvent
			\arg StartFixationEvent
			\arg EndFixationEvent
			\arg FixUpdateEvent
			\arg IOEvent
			\arg MessageEvent
 */
GETGENERICDATAFCN(get_float_data)
/*! \fn def pylink::EyeLinkCBind::getLastData
	Gets an integer (unconverted) copy of the last/newest link data (sample or event) seen by
	\c getNextData().

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_get_last_data(void *buf);
	\endcode
	@return Object of type Sample or Event.
 */
GETGENERICDATAFCN(get_last_data)

GETSAMPLEDATAFCN(newest_sample)
/*! \fn def pylink::EyeLinkCBind::getNewestSample
	Check if a new sample has arrived from the link. This is the latest sample, not the oldest sample
	that is read by \c getNextData(), and is intended to drive gaze cursors and gaze-contingent displays.

	@remarks This function is equivalent to the C API
	\code
	INT16 CALLTYPE eyelink_newest_float_sample(void FARTYPE *buf);
	\endcode
	@return None if there is no sample, instance of Sample type otherwise.
 */
GETSAMPLEDATAFCN(newest_float_sample)
/*! \fn def pylink::EyeLinkCBind::getSample
	Gets an integer (unconverted) sample from end of queue, discards any events encountered.

	@remarks This is equivalent to the C API
	\code
	INT16 eyelink_get_sample(void *sample);
	\endcode
	@return Object of type Sample.
 */
GETSAMPLEDATAFCN(get_sample)
PyObject* eyelink_eyelink_init_custom_graphics(PyObject* self, PyObject* args);
PyObject* eyelink_eyelink_request_cross_hair_draw(PyObject* self, PyObject* args);


static PyMethodDef eyelink_module_methods[] =
{

	MAPFCN(inRealTimeMode,		SHORTVOIDFCNPROTO(in_realtime_mode)), // tested
	MAPFCN(flushGetkeyQueue,		SHORTVOIDFCNPROTO(flush_getkey_queue)), // tested
	MAPFCN(beginRealTimeMode,		SHORTVOIDFCNPROTO(begin_realtime_mode)), // tested
	MAPFCN(currentTime,			SHORTVOIDFCNPROTO(current_time)), // tested
	MAPFCN(currentUsec,			SHORTVOIDFCNPROTO(current_usec)), // tested
	MAPFCN(currentDoubleUsec,		SHORTVOIDFCNPROTO(current_double_usec)), // tested
	MAPFCN(endRealTimeMode,		SHORTVOIDFCNPROTO(end_realtime_mode)), // tested
	MAPFCN(pumpDelay,				SHORTVOIDFCNPROTO(pump_delay)),  // tested
	MAPFCN(msecDelay,				SHORTVOIDFCNPROTO(msec_delay)),  // tested
	MAPFCN(alert,					SHORTVOIDFCNPROTO(alert_printf)), // tested
	MAPFCN(enableExtendedRealtime, SHORTVOIDFCNPROTO(eyelink_enable_extended_realtime)), // tested

#ifndef NO_SDL
	MAPFCN(setCalibrationColors	  ,SHORTVOIDFCNPROTO(set_calibration_colors)),
	MAPFCN(setTargetSize		  ,SHORTVOIDFCNPROTO(set_target_size)),
	MAPFCN(setCalibrationSounds			  ,SHORTVOIDFCNPROTO(set_cal_sounds )),
	MAPFCN(setDriftCorrectSounds		  ,SHORTVOIDFCNPROTO(set_dcorr_sounds)),
	MAPFCN(setCameraPosition,SHORTVOIDFCNPROTO(set_camera_image_position)),
	MAPFCN(getDisplayInformation  ,SHORTVOIDFCNPROTO(get_display_information)),
	MAPFCN(openGraphics		  ,SHORTVOIDFCNPROTO(init_expt_graphics 	)),
	MAPFCN(closeGraphics	  ,SHORTVOIDFCNPROTO(close_expt_graphics)),
	

	MAPFCN(resetBackground	  ,SHORTVOIDFCNPROTO(reset_background_surface)),
	MAPFCN(disableCustomBackgroundOnImageMode	  ,SHORTVOIDFCNPROTO(disable_custombackground_on_imagemode)),
	MAPFCNEX("setCalibrationAnimationTarget"	  ,SHORTVOIDFCNPROTO(setCalibrationAnimationTarget),bitmapSave_doc),	

#endif
	MAPFCN(sendMessageToFile, SHORTVOIDFCNPROTO(timemsg_printf)), // tested
	MAPFCN(openMessageFile, SHORTVOIDFCNPROTO(open_message_file)), // tested
	MAPFCN(closeMessageFile, SHORTVOIDFCNPROTO(close_message_file)), // tested
	MAPFCN(request_cross_hair_draw, SHORTVOIDFCNPROTO(request_cross_hair_draw)), // tested
	MAPFCNEX("bitmapSave"	  ,SHORTVOIDFCNPROTO(bitmapSave),bitmapSave_doc),
	MAPFCN(openCustomGraphicsInternal		  ,SHORTVOIDFCNPROTO(init_custom_graphics)),


	


    {NULL, NULL, 0, NULL}
};


static PyMethodDef eyeTrackerType_methods[] =
{
	MAPFCN(open,					SHORTVOIDFCNPROTO(open)),//tested
	MAPFCN(dummy_open	,			SHORTVOIDFCNPROTO(dummy_open)), // tested
	MAPFCN(getNextData,			SHORTVOIDFCNPROTO(get_next_data)), // tested
	MAPFCN(getFloatData,			SHORTVOIDFCNPROTO(get_float_data)), // tested
	MAPFCN(close,					SHORTVOIDFCNPROTO(close)),     //tested
	MAPFCN(reset,					SHORTVOIDFCNPROTO(reset)),     //tested
	MAPFCN(stopRecording,			SHORTVOIDFCNPROTO(stop_recording)), // tested
	MAPFCN(doTrackerSetup,		SHORTVOIDFCNPROTO(do_tracker_setup)), //tested
	MAPFCN(setAddress,			SHORTVOIDFCNPROTO(set_eyelink_address)), // tested
	MAPFCN(startRecording,		SHORTVOIDFCNPROTO(start_recording)), // tested
	MAPFCN(doDriftCorrect,		SHORTVOIDFCNPROTO(do_drift_correct)), // tested
	MAPFCN(isConnected	,		SHORTVOIDFCNPROTO(is_connected)), // tested
	MAPFCN(getTrackerVersion,		SHORTVOIDFCNPROTO(get_tracker_version)), // tested
	MAPFCN(openNode,				SHORTVOIDFCNPROTO(open_node)), // tested
	MAPFCN(setOfflineMode,		SHORTVOIDFCNPROTO(set_offline_mode)),  //tested
	MAPFCN(sendTimedCommand,			SHORTVOIDFCNPROTO(timed_command)), // tested
	MAPFCN(sendTimedCommandEx,			SHORTVOIDFCNPROTO(timed_commandEx)), // tested
	MAPFCN(sendCommand,			SHORTVOIDFCNPROTO(eyecmd_printf)), // tested
	MAPFCN(sendMessage,			SHORTVOIDFCNPROTO(eyemsg_printf)), // tested
	MAPFCN(getkey,				SHORTVOIDFCNPROTO(getkey)), //tested
	MAPFCN(getkeyEx,				SHORTVOIDFCNPROTO(getkey_with_mod)), //tested
	MAPFCN(abort,					SHORTVOIDFCNPROTO(abort)), // tested
	MAPFCN(commandResult,			SHORTVOIDFCNPROTO(command_result)),  // tested
	MAPFCN(inSetup	,			SHORTVOIDFCNPROTO(in_setup)), // tested
	MAPFCN(getCalibrationResult,	SHORTVOIDFCNPROTO(cal_result)), // tested
	MAPFCN(applyDriftCorrect,		SHORTVOIDFCNPROTO(apply_driftcorr)), // tested
	MAPFCN(startSetup	,			SHORTVOIDFCNPROTO(start_setup)), // called. don't know the use of it.

	MAPFCN(getTrackerVersionString,SHORTVOIDFCNPROT1(get_tracker_version)), // tested
	MAPFCN(getLastMessage	,		SHORTVOIDFCNPROT1(last_message)), // tested
	MAPFCN(readRequest	,		SHORTVOIDFCNPROTO(eyelink_read_request)), // tested
	MAPFCN(readReply	,			SHORTVOIDFCNPROT1(read_reply)),  // tested
	MAPFCN(getCalibrationMessage,	SHORTVOIDFCNPROT1(cal_message)), // tested
	MAPFCN(getLastData,			SHORTVOIDFCNPROTO(get_last_data)), // tested
	//MAPFCN("getNewestSample,		SHORTVOIDFCNPROTO(newest_sample)), // tested - too confusing to have a float version and non float version
	MAPFCN(getNewestSample,		SHORTVOIDFCNPROTO(newest_float_sample)), // tested
	//MAPFCN("getNewestFloatSample,	SHORTVOIDFCNPROTO(newest_float_sample)), // tested
	MAPFCN(getSample	,			SHORTVOIDFCNPROTO(get_sample)), // tested




	MAPFCN(trackerTime,			SHORTVOIDFCNPROTO(tracker_msec)), // tested
	MAPFCN(trackerTimeOffset,		SHORTVOIDFCNPROTO(msec_offset)),// tested

	MAPFCN(trackerTimeUsec,			SHORTVOIDFCNPROTO(tracker_double_usec)), // tested
	MAPFCN(trackerTimeUsecOffset,		SHORTVOIDFCNPROTO(double_usec_offset)),// tested


	MAPFCN(eyeAvailable,			SHORTVOIDFCNPROTO(eye_available)), // tested
	MAPFCN(getCurrentMode,		SHORTVOIDFCNPROTO(current_mode)),// tested
	MAPFCN(getEventDataFlags,		SHORTVOIDFCNPROTO(event_data_flags)	),// tested
	MAPFCN(getEventTypeFlags,		SHORTVOIDFCNPROTO(event_type_flags)	),// tested
	MAPFCN(flushKeybuttons,		SHORTVOIDFCNPROTO(flush_keybuttons)), // tested
	MAPFCN(waitForModeReady,		SHORTVOIDFCNPROTO(wait_for_mode_ready)),// tested
	MAPFCN(exitCalibration,		SHORTVOIDFCNPROTO(exit_calibration)),// tested
	MAPFCN(resetData,				SHORTVOIDFCNPROTO(reset_data)),// tested
	MAPFCN(getTrackerMode	,		SHORTVOIDFCNPROTO(tracker_mode)	),// tested
	MAPFCN(readKeyQueue,			SHORTVOIDFCNPROTO(read_getkey_queue)),  // tested
	MAPFCN(acceptTrigger,			SHORTVOIDFCNPROTO(accept_trigger)),  // tested
	MAPFCN(startDriftCorrect,		SHORTVOIDFCNPROTO(driftcorr_start)),  // tested
	MAPFCN(setName,				SHORTVOIDFCNPROTO(set_name)),  // tested
	MAPFCN(stopData	,			SHORTVOIDFCNPROTO(data_stop)),  // tested
	MAPFCN(startPlayBack,			SHORTVOIDFCNPROTO(playback_start)),  // tested
	MAPFCN(stopPlayBack,			SHORTVOIDFCNPROTO(playback_stop)),  // tested
	MAPFCN(isRecording,			SHORTVOIDFCNPROTO(check_recording)),  // tested
	MAPFCN(getRecordingStatus,	SHORTVOIDFCNPROTO(check_record_exit)),  // tested
	MAPFCN(startData,				SHORTVOIDFCNPROTO(data_start)),  // tested
	MAPFCN(openDataFile,			SHORTVOIDFCNPROTO(open_data_file)), // tested
	MAPFCN(closeDataFile,			SHORTVOIDFCNPROTO(close_data_file)), // tested
	MAPFCN(escapePressed,			SHORTVOIDFCNPROTO(escape_pressed)),// tested
	MAPFCN(breakPressed,			SHORTVOIDFCNPROTO(break_pressed)),// tested
	MAPFCN(terminalBreak,			SHORTVOIDFCNPROTO(terminal_break)),// tested
	MAPFCN(getLastButtonPress,	SHORTVOIDFCNPROTO(last_button_press)),// tested
	MAPFCN(pumpMessages,			SHORTVOIDFCNPROTO(message_pump)),// tested
	MAPFCN(readKeyButton,			SHORTVOIDFCNPROTO(read_keybutton)),// tested
	MAPFCN(receiveDataFile,		SHORTVOIDFCNPROTO(receiveDataFile)),// tested


	MAPFCN(getPositionScalar,		SHORTVOIDFCNPROTO(position_prescaler)),
	MAPFCN(userMenuSelection,		SHORTVOIDFCNPROTO(user_menu_selection)),
	MAPFCN(getButtonStates,		SHORTVOIDFCNPROTO(button_states)),
	MAPFCN(getLastButtonStates,	SHORTVOIDFCNPROTO(eyelink_last_button_states)),
	MAPFCN(getDataCount,			SHORTVOIDFCNPROTO(data_count)),
	MAPFCN(isInDataBlock,			SHORTVOIDFCNPROTO(in_data_block)),
	MAPFCN(waitForBlockStart,		SHORTVOIDFCNPROTO(wait_for_block_start)),
	MAPFCN(waitForData,			SHORTVOIDFCNPROTO(wait_for_data)),
	MAPFCN(sendKeybutton,			SHORTVOIDFCNPROTO(send_keybutton)),
	MAPFCN(getTargetPositionAndState,SHORTVOIDFCNPROTO(target_check)),
	MAPFCN(getModeData,			SHORTVOIDFCNPROTO(mode_data)),



	MAPFCN(getSampleDataFlags,	SHORTVOIDFCNPROTO(sample_data_flags)	), // called not documented
	//MAPFCN(requestFileRead,		SHORTVOIDFCNPROTO(request_file_read)),  // called not documented April, 11, 2006 should not be exported
	MAPFCN(echo_key,				SHORTVOIDFCNPROTO(echo_key)), // not documented

	MAPFCN(key_message_pump,		SHORTVOIDFCNPROTO(key_message_pump)), // not documented
	//MAPFCN(endFileTransfer,		SHORTVOIDFCNPROTO(end_file_transfer)), // not documented April, 11, 2006 should not be exported
	//MAPFCN(requestFileBlock,		SHORTVOIDFCNPROTO(request_file_block)), // not documentedApril, 11, 2006 should not be exported


	/* broad cast related  - not supported at this time.*/

	MAPFCN(broadcastOpen,		SHORTVOIDFCNPROTO(broadcast_open)), // tested
	MAPFCN(nodeSend,				SHORTVOIDFCNPROTO(node_send)), // tested
	MAPFCN(nodeReceive,			SHORTVOIDFCNPROTO(node_receive)), // tested
	MAPFCN(dataSwitch,			SHORTVOIDFCNPROTO(data_switch)), // tested
	MAPFCN(nodeSendMessage,		SHORTVOIDFCNPROTO(node_send_message)),
	MAPFCN(nodeRequestTime,		SHORTVOIDFCNPROTO(node_request_time)),
	MAPFCN(getNode,				SHORTVOIDFCNPROTO(get_node)),

	MAPFCN(pollTrackers,			SHORTVOIDFCNPROTO(poll_trackers)),
	MAPFCN(pollRemotes	,		SHORTVOIDFCNPROTO(poll_remotes)),
	MAPFCN(pollResponses,			SHORTVOIDFCNPROTO(poll_responses)),
	//MAPFCN(buttonStates,			SHORTVOIDFCNPROTO(button_states)), see getButtonStates
	MAPFCN(quietMode,			    SHORTVOIDFCNPROTO(quiet_mode)),

	MAPFCN(imageModeDisplay,     SHORTVOIDFCNPROTO(image_mode_display)), // not linked to anythinng
	MAPFCN(targetModeDisplay,    SHORTVOIDFCNPROTO(target_mode_display)), // not linked to anything
	MAPFCN(_getDataStatus,		SHORTVOIDFCNPROTO(data_status)),
	MAPFCN(requestTime,			SHORTVOIDFCNPROTO(request_time)),
	MAPFCN(readTime,				SHORTVOIDFCNPROTO(read_time)),
	MAPFCN(_drawCalTarget	  ,SHORTVOIDFCNPROTO(map_eyelink_py_draw_cal_target_hook)),

#ifndef NO_SDL	
	MAPFCN(getImageCrossHairData	  ,SHORTVOIDFCNPROTO(get_image_xhair_data)),
#endif
	MAPFCNEX("bitmapSaveAndBackdrop"	  ,SHORTVOIDFCNPROTO(bitmapSaveAndBackdrop),bitmapSaveAndBackdrop_doc),
	MAPFCNEX("bitmapBackdrop"	  ,SHORTVOIDFCNPROTO(bitmapBackdrop),bitmapBackdrop_doc),
	#ifdef WIN32
        MAPFCNEX("calculateVelocity"	  ,SHORTVOIDFCNPROTO(eyelink_calculate_velocity),eyelink_calculate_velocity_doc),
	MAPFCNEX("calculateVelocityXY"	  ,SHORTVOIDFCNPROTO(eyelink_calculate_velocity_x_y),eyelink_calculate_velocity_x_y_doc),
	MAPFCNEX("calculateOverallVelocityAndAcceleration"	  ,SHORTVOIDFCNPROTO(eyelink_calculate_overallvelocity_and_acceleration),eyelink_calculate_overallvelocity_and_acceleration_doc),
        #endif

    {NULL, NULL, 0, NULL}
};


static PyTypeObject EyelinkTrackerType = {
    PyObject_HEAD_INIT(NULL)
    0,
    "EyeLinkCBind",
    sizeof(EyelinkTrackerObject),
    0,
    eyelink_dealloc, /*tp_dealloc*/
    0,          /*tp_print*/
    eyelink_getattr,          /*tp_getattr*/
    0,//eyelink_setattr,          /*tp_setattr*/
    0,          /*tp_compare*/
    0,          /*tp_repr*/
    0,          /*tp_as_number*/
    0,          /*tp_as_sequence*/
    0,          /*tp_as_mapping*/
    0,          /*tp_hash */
	0,			/* tp_call */
	0,			/* tp_str*/
	0,			/* tp_getattro*/
	0,			/* tp_setattro*/
	0,			/* tp_as_buf*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_CHECKTYPES,
	"EyeLinkCBind",
	0,			/* tp_traverse*/
	0,			/* tp_clear*/
	0,			/* tp_richcompare*/
	0,			/* tp_weaklistoffset*/
	0,			/* tp_iter*/
	0,			/* tp_iternext*/
	eyeTrackerType_methods,			/* tp_methods*/
	0,			/* tp_members*/
	0,			/* tp_getset*/
	0,			/* tp_base*/
	0,			/* tp_dict*/
	0,			/* tp_descr_get*/
	0,			/* tp_descr_set*/
	0,			/* tp_dictoffset*/
	0,//Eyelink__new__,/*NewTracker,*/	/* tp_init*/
	0,//PyType_GenericAlloc,			/* tp_alloc*/
	Eyelink__new__,//PyType_GenericNew,			/* tp_new*/
//	_PyObject_Del,			/* tp_free*/
};


#define PYGAMEAPI_LOCAL_ENTRY "_PYGAME_C_API"
void getTrackerModule()
{

		static int moduleLoad = 0;
		
		if(PY_MAJOR_VERSION >= 2 && PY_MINOR_VERSION >=6)
			trackerModule = PyImport_ImportModule("pylink.tracker");
		else
			trackerModule = PyImport_ImportModule("tracker");


		if(!trackerModule)
		{
			if(moduleLoad)
				{
					printf("PyLink: Could not load module tracker!.\n");
					exit(1);
				}
			else
			{
				moduleLoad = 1;
				printf("PyLink: Could not load module tracker! delaying module loading.\n");
				return;
			}

		}

		{
			newSample           	= PyDict_GetItemString(PyModule_GetDict(trackerModule),"newSample");
			newStartBlinkEvent  	= PyDict_GetItemString(PyModule_GetDict(trackerModule),"newStartBlinkEvent");
			newEndBlinkEvent    	= PyDict_GetItemString(PyModule_GetDict(trackerModule),"newEndBlinkEvent");
			newStartSacadeEvent 	= PyDict_GetItemString(PyModule_GetDict(trackerModule),"newStartSaccadeEvent");
			newEndSacadeEvent     	= PyDict_GetItemString(PyModule_GetDict(trackerModule),"newEndSaccadeEvent");
			newStartFixationEvent 	= PyDict_GetItemString(PyModule_GetDict(trackerModule),"newStartFixationEvent");
			newEndFixationEvent   	= PyDict_GetItemString(PyModule_GetDict(trackerModule),"newEndFixationEvent");
			newFixUpdateEvent   	= PyDict_GetItemString(PyModule_GetDict(trackerModule),"newFixUpdateEvent");
			newDisplayInfo   		= PyDict_GetItemString(PyModule_GetDict(trackerModule),"newDisplayInfo");
			updatePerTrialDynamic	= PyDict_GetItemString(PyModule_GetDict(trackerModule),"_updatePerTrialDynamic");
			updateBooleanFlags		= PyDict_GetItemString(PyModule_GetDict(trackerModule),"_updateBooleanFlags");
			updateTimeStamps		= PyDict_GetItemString(PyModule_GetDict(trackerModule),"_updateTimeStamps");
			updateStatusFlags		= PyDict_GetItemString(PyModule_GetDict(trackerModule),"_updateStatusFlags");
			updateTypeFlags			= PyDict_GetItemString(PyModule_GetDict(trackerModule),"_updateTypeFlags");
			updateSampleEventData	= PyDict_GetItemString(PyModule_GetDict(trackerModule),"_updateSampleEventData");
			updateNetworkData		= PyDict_GetItemString(PyModule_GetDict(trackerModule),"_updateNetworkData");
			updateAddresses			= PyDict_GetItemString(PyModule_GetDict(trackerModule),"_updateAddresses");
			updateReserved			= PyDict_GetItemString(PyModule_GetDict(trackerModule),"_updateReserved");
			drawCalTarget			= PyDict_GetItemString(PyModule_GetDict(trackerModule),"drawCalTarget");










			newIOEvent = PyDict_GetItemString(PyModule_GetDict(trackerModule),"newIOEvent");
			newMessageEvent   = PyDict_GetItemString(PyModule_GetDict(trackerModule),"newMessageEvent");
			EyelinkMessage = PyDict_GetItemString(PyModule_GetDict(trackerModule),"EyelinkMessage");


			if(
				!newSample			||
				!newStartBlinkEvent ||
				!newEndBlinkEvent   ||
				!newStartSacadeEvent||
				!newEndSacadeEvent  ||
				!newStartFixationEvent ||
				!newEndFixationEvent  ||
				!newIOEvent 			||
				!newMessageEvent		||
				!newFixUpdateEvent		||
				!newDisplayInfo			||
				!updatePerTrialDynamic	||
				!updateBooleanFlags		||
				!updateTimeStamps		||
				!updateStatusFlags		||
				!updateTypeFlags		||
				!updateSampleEventData	||
				!updateNetworkData		||
				!updateAddresses		||
				!updateReserved||
				!EyelinkMessage||
				!drawCalTarget)
				{
					printf("Error: one of the required attribute not found in module pylink\n");
					exit(-1);
				}


	}
}

DL_EXPORT(void) initpylink(void)
{
	PyObject *apiobj = NULL;
	static void* c_api[1];
	PyObject *dict = NULL;
	PyObject *thismodule = NULL;
    EyelinkTrackerType.ob_type = &PyType_Type;
    thismodule = Py_InitModule3("pylink", eyelink_module_methods, "Eyelink tracker python interface");
    dict = PyModule_GetDict(thismodule);


	PyDict_SetItemString(dict, "EyeLinkType", (PyObject *)&EyelinkTrackerType);
	PyDict_SetItemString(dict, "EyeLinkCBind", (PyObject *)&EyelinkTrackerType);


	c_api[0] = &EyelinkTrackerType;
	apiobj = PyCObject_FromVoidPtr(c_api, NULL);
	PyDict_SetItemString(dict, PYGAMEAPI_LOCAL_ENTRY, apiobj);
	Py_DECREF(apiobj);

	if(eyelink_reset_clock(1)!=0)
    {
      alert_printf("Cannot initialize link: Check network and TCP/IP setup");
      exit(1);
    }
	getTrackerModule();
}
static void eyelink_dealloc(PyObject* self)
{
//    PyObject_Del(self); - Having uncommented crashes on python 2.3 when no connection to tracker is made.
}
static PyObject *eyelink_getattr(PyObject *obj, char *name)
{
	return Py_FindMethod(eyeTrackerType_methods, (PyObject *)obj, name);
}
static int eyelink_setattr(PyObject *obj, char *name, PyObject *v)
{
    return -1;
}


INT16 ELCALLTYPE text_to_elinkaddr(char *addr, ELINKADDR node, int remote);
/*! \fn def pylink::EyeLinkCBind::setName
	Sets the node name of this computer (up to 35 characters).

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_set_name(char *name);
	\endcode
	@param name String to become new name.
 */
GENERICFCNPROTO(set_name)
{
	char *str = NULL;
	if (PyArg_ParseTuple(args,"s",&str))
	{
		eyelink_set_name(str);
		return Py_BuildValue("");
	}
	return NULL;
}

#define TEXTVALUE(x,y) PyString_AsString(PyObject_GetAttrString(x,y)
#define RETURNERROR(rv,x){\
	char *rvm = eyelink_get_error(rv, #x);\
		if(!rvm || !strlen(rvm))\
			return Py_BuildValue("i",rv);\
		else \
			PyErr_Format(PyExc_RuntimeError, "%s", rvm);\
		return NULL;\
	}


void printELINKADDR(ELINKADDR addr)
{
	int i =0;
	printf(" address is ");
	for(i =0; i < 16; i ++)
	{
		printf("%x ", ((char *)addr)[i] &0x000000ff);
	}
	printf("\n");
}



void * toELINKADDR(PyObject *addr, ELINKADDR node)
{
	PyObject *getIP = PyObject_GetAttrString(addr,"getIP");
	PyObject *getPort = PyObject_GetAttrString(addr,"getPort");

	if(!getIP || !getPort)
		{
			PyErr_Format(PyExc_TypeError, "Given object is not an instance of EyeLinkAddress");
			return NULL;
		}
	{
		long pt =0;
		PyObject * ip = PyObject_CallFunction(getIP,"");
		PyObject * port = PyObject_CallFunction(getPort,"");
		if(!ip || !port)
			return NULL;

		memset(node, 0,sizeof(ELINKADDR));
		((char *)node)[0] = (char)PyInt_AsLong(PyTuple_GetItem(ip,0));
		((char *)node)[1] = (char)PyInt_AsLong(PyTuple_GetItem(ip,1));
		((char *)node)[2] = (char)PyInt_AsLong(PyTuple_GetItem(ip,2));
		((char *)node)[3] = (char)PyInt_AsLong(PyTuple_GetItem(ip,3));
		pt = PyInt_AsLong(port);
		memcpy(((char *)node) + 4, &pt,2);
	}
	//printELINKADDR(node);
	return "";
}

/*! \fn def pylink::EyeLinkCBind::nodeSend
	Sends a given data to the given node.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_node_send(ELINKADDR node, void *data, UINT16 dsize);
	\endcode
	@param addr the address of the node.
	@param data Pointer to buffer containing data to send.
	@param length Number of bytes of data.
	@return \c 0 if successful, otherwise link error.
 */
GENERICFCNPROTO(node_send)
{
	PyObject *addr = NULL;
	char * data = NULL;
	unsigned short len = 0;
	if (PyArg_ParseTuple(args,"Os#", &addr, &data,&len))
	{
		ELINKADDR node;
		int rv = 0;
		if(toELINKADDR(addr,node))
		{
			rv = eyelink_node_send(node,data,len);
			RETURNERROR(rv,eyelink_node_send);
		}
	}
	return NULL;
}

/*! \fn def pylink::EyeLinkCBind::sendTimedCommandEx
	Sends a command to the connected eye tracker, wait for reply.
	@return List of 2 items. The first item contains the return value of \c eyelink_timed_command().
	The second item contains a string description of the error message.

	@remark If there is an error, no exception is raised.
	@sa \c sendTimedCommand()
 */

GENERICFCNPROTO(timed_commandEx)
{
	char * cmd = NULL;
	unsigned int dur = 0;
	if (PyArg_ParseTuple(args,"is",&dur, &cmd))
	{
		int rv = 0;
		char *rvm = NULL;
		if(strlen(cmd) > 129)
		{
			PyErr_Format(PyExc_RuntimeError, "Command too long: %s", cmd);
			return NULL;
		}

		rv =eyelink_timed_command(dur,cmd);
		rvm = eyelink_get_error(rv, "eyelink_timed_command");
		if(!rvm)
			rvm = "";
		return Py_BuildValue("[is]", rv,rvm);
	}
	return NULL;
}

/*! \fn def pylink::EyeLinkCBind::sendTimedCommand
	Sends a command to the connected eye tracker, wait for reply.


	@exception If there is an error, Runtime exception is raised.
	@sa sendTimedCommandEx()
 */
GENERICFCNPROTO(timed_command)
{
	char * cmd = NULL;
	unsigned int dur = 0;
	if (PyArg_ParseTuple(args,"is",&dur, &cmd))
	{
		int rv = 0;
		if(strlen(cmd) > 129)
		{
			PyErr_Format(PyExc_RuntimeError, "Command too long: %s", cmd);
			return NULL;
		}

		rv = eyelink_timed_command(dur,cmd);
		RETURNERROR(rv,eyelink_command_result);
	}
	return NULL;
}
/*! \fn def pylink::EyeLinkCBind::nodeSendMessage
	Sends a text message the connected eye tracker.  The text will be added to the EDF file.

	@remarks If the link is initialized but not connected to a tracker, the message will be sent
	to the tracker set by \c setAddress() of the pylink module.  This function is equivalent
	to the C API
	\code
	INT16 eyelink_node_send_message(ELINKADDR node, char *msg);
	\endcode
	@param address Address of a specific tracker.
	@param message Text to send to the tracker.
	@return \c 0 if no error, else link error code.
 */
GENERICFCNPROTO(node_send_message)
{
	PyObject *addr = NULL;
	char * msg = NULL;
	if (PyArg_ParseTuple(args,"Os",&addr, &msg))
	{
		ELINKADDR node;
		int rv;
		if(toELINKADDR(addr,node))
		{
			rv = eyelink_node_send_message(node,msg);
			RETURNERROR(rv,eyelink_node_send_message);
		}
	}
	return NULL;
}
/*! \fn def pylink::EyeLinkCBind::nodeRequestTime
	Sends a request the connected eye tracker to return its current time.

	@remarks The time reply can be read with \c getTrackerTime().  This function is equivalent
	to the C API
	\code
	UINT32 eyelink_node_request_time(ELINKADDR node);
	\endcode
	@param address Text IP address (for example, "100.1.1.1")  for a specific tracker.
	@return 0 if no error, else link error code.
 */
GENERICFCNPROTO(node_request_time)
{
	PyObject * addr= NULL;
	if (PyArg_ParseTuple(args,"O",&addr))
	{
		int rv;
		ELINKADDR node;
		if(toELINKADDR(addr,node))
		{
			rv = eyelink_node_request_time(node);
			RETURNERROR(rv,eyelink_node_request_time);
		}
	}
	return NULL;
}
/*! \fn def pylink::EyeLinkCBind::getNode
	Reads the responses returned by other trackers or remotes in response to \c pollTrackers() or
	\c pollRemotes(). It can also read the tracker broadcast address and remote broadcast addresses.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_get_node(INT16 resp, void *data);
	\endcode
	@param resp Nmber of responses to read: \c 0 gets our data, \c 1 get first response, \c 2 gets
				the second response, etc. \c -1 to read the tracker broadcast address. \c -2 to
				read remote broadcast addresses.
	@return If successful, an instance of EyeLinkMessage class returned.
 */
GENERICFCNPROTO(get_node)
{
	short resp =0;
	if (PyArg_ParseTuple(args,"i",&resp))
	{
		int rv;
		ELINKNODE node;
		rv = eyelink_get_node(resp,&node);
		if(rv ==0)
		{
			short port =0;
			port =  *(((char *)(node.addr))+5);
			port = port << 8;
			port = port| *(((char *)(node.addr))+4);
			return PyObject_CallFunction(EyelinkMessage,"((iiii),i,s)",((char *)(node.addr))[0],((char *)(node.addr))[1],((char *)(node.addr))[2],((char *)(node.addr))[3],port&0x0000ffff,node.name);
		}
		RETURNERROR(rv,eyelink_get_node);
	}
	return NULL;
}
/*! \fn def pylink::EyeLinkCBind::nodeReceive
	Checks for and gets the last packet received, stores the data and the node address sent from.

	@remarks Data can only be read once, and is overwritten if a new packet arrives before the last
	packet has been read.  This function is equivalent to the C API
	\code
	INT16 eyelink_node_receive(ELINKADDR node, void *data);
	\endcode
	@return An instance of EyeLinkMessage class is returned, if successful.
 */
GENERICFCNPROTO(node_receive)
{
	if (PyArg_ParseTuple(args,":node_receive")) // the message and the address is returned back
	{
		char data[256];
		ELINKADDR node;
		int rv = eyelink_node_receive(node,data);
		if(rv > 0)
		{
			short port =0;
			port =  *(((char *)node)+5);
			port = port << 8;
			port = port| *(((char *)node)+4);
			return PyObject_CallFunction(EyelinkMessage,"((iiii),i,s)",((char *)node)[0],((char *)node)[1],((char *)node)[2],((char *)node)[3],port&0x0000ffff,data );
		}
		else
				return Py_BuildValue("");
	}
	return NULL;
}

/*! \fn def pylink::EyeLinkCBind::openNode
	Allows the computer to connect to tracker, where the tracker is on the same network.

	@remarks This is equivalent to the C API
	\code
	INT16 eyelink_open_node(ELINKADDR node, INT16 busytest);
	\endcode
	with node parameter converted from text to ELINKADDR.
	@param eyelink_address Text IP address of the host PC (the default value is, "100.1.1.1").
	@param busytest If non-zero the call to \c openNode() will not disconnect an existing connection.
	@return Throws Runtime Exception if it connects to the remote host.
 */
GENERICFCNPROTO(open_node)
{
	short busytest=0;
	char * addr= NULL;
	if (PyArg_ParseTuple(args,"si",&addr, &busytest))
	{
		int rv = 0;
		ELINKADDR node;
		text_to_elinkaddr(addr,node,0);
		rv = eyelink_open_node(node,busytest);
		RETURNERROR(rv,eyelink_open_node);
	}
	return NULL;
}

/*! \fn def pylink::EyeLinkCBind::getLastButtonPress
	Reads the number of the last button detected by the EyeLink tracker. This is \c 0 if no buttons
	were pressed since the last call, or since the buttons were flushed. If a pointer to a variable
	is supplied the eye-tracker timestamp of the button may be read. This could be used to see if
	a new button has been pressed since the last read. If multiple buttons were pressed since
	the last call, only the last button is reported.

	@remarks This function is equivalent to the C API
	\code
	UINT16 eyelink_last_button_press(UINT32 *time);
	\endcode
	@return Two-item tuple, recording the button last pressed (\c 0 if no button pressed since last
			read) and the time of the button press.
 */
GENERICFCNPROTO(last_button_press)
{

	if (PyArg_ParseTuple(args,":"))
	{
		UINT32 time=0;
		int rv = eyelink_last_button_press(&time);
		return Py_BuildValue("ii",rv, time) ;
	}
	return NULL;
}

/*! \fn def pylink::EyeLinkCBind::getTargetPositionAndState
	Returns the current target position and state.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink_target_check(INT16 *x, INT16 *y);
	\endcode
	@return A three-item tuple holding (in the following order):
			\arg the target visibility (\c 1 if visible, 0 if not),
			\arg x position of the target,
			\arg and y position of the target.
 */
GENERICFCNPROTO(target_check)
{
	if (PyArg_ParseTuple(args,":"))
	{
		short x=0;
		short y=0;
		int rv = eyelink_target_check(&x, &y);
		return Py_BuildValue("iii",rv,x,y) ;
	}
	return NULL;
}
/*! \fn def pylink::EyeLinkCBind::getModeData
	After calling \c waitForBlockStart(), or after at least one sample or eye event has been read,
	returns EyeLink II extended mode data.

	@remarks This function is equivalent to the C API
	\code
	INT16 eyelink2_mode_data(INT16 *sample_rate, INT16 *crmode, INT16 *file_filter, INT16 *link_filter);
	\endcode
	@return A five-item tuple holding (in the following order):
			\arg success value of call to eyelink2_mode_data(). 0 if link state data available, -1 otherwise.
			\arg sampling rate (samples per second),
			\arg CR mode flag (\c 0 if pupil-only mode, else pupil-CR mode),
			\arg filter level applied to file samples (\c 0 = off, \c 1 = std, \c 2 = extra),
			\arg filter level applied to link and analog output samples (\c 0 = off, \c 1 = std, \c 2 = extra).
 */
GENERICFCNPROTO(mode_data)
{
	if (PyArg_ParseTuple(args,":"))
	{
		INT16 sample_rate = -32768;
		INT16 crmode = -32768;
		INT16 file_filter = -32768;
		INT16 link_filter = -32768;
                #ifdef WIN32
		int rv = eyelink_mode_data(&sample_rate, &crmode, &file_filter, &link_filter);
                #else
      		int rv = eyelink2_mode_data(&sample_rate, &crmode, &file_filter, &link_filter);
                #endif
		if(rv == -1)
		{
			sample_rate = -32768;
			crmode = -32768;
			file_filter = -32768;
			link_filter = -32768;
		}
		return Py_BuildValue("iiiii",rv,sample_rate, crmode, file_filter, link_filter) ;
	}
	return NULL;
}

/*! \fn def pylink::EyeLinkCBind::readKeyButton
	Reads any queued key or button events from tracker.

	@remarks This function is equivalent to the C API
	\code
	UINT16 eyelink_read_keybutton(INT16 *mods,INT16 *state, UINT16 *kcode, UINT32 *time);
	\endcode
	@return A five-item tuple, recording (in the following order):
			\arg Key character if key press/release/repeat, <tt>KB_BUTTON (0xFF00)</tt> if button press or release,
			\arg Button number or key modifier (Shift, Alt and Ctrl key states),
			\arg Key or button change (\c KB_PRESS, \c KB_RELEASE, or \c KB_REPEAT),
			\arg Key scan code,
			\arg Tracker time of the key or button change.
 */
GENERICFCNPROTO(read_keybutton)
{
	if (PyArg_ParseTuple(args,":"))
	{
		INT16 sample_rate;
		INT16 crmode;
		INT16 file_filter;
		UINT32 link_filter;
		int rv = eyelink_read_keybutton(&sample_rate, &crmode, &file_filter, &link_filter);
		return Py_BuildValue("iiiii",rv,sample_rate, crmode, file_filter, link_filter) ;
	}
	return NULL;
}

GENERICFCNPROTO(open_eyelink_system)
{
	int sz =0;
	if (PyArg_ParseTuple(args,"i",&sz))
	{
		int rv = open_eyelink_system(sz,NULL);
		RETURNERROR(rv,open_eyelink_system);
	}
	return NULL;
}
GENERICFCNPROTO(data_status)
{
	PyObject *trackerInfo = NULL;
	if (PyArg_ParseTuple(args,"O:_getDataStatus",&trackerInfo))
	{
		ILINKDATA *data  = eyelink_data_status();
		if(data)
		{
			PyObject_CallFunction(updatePerTrialDynamic,"O(iiiiiiiii)",trackerInfo,
								data->samrate,data->samdiv,
								data->prescaler,data->vprescaler,
								data->pprescaler,data->hprescaler,
								data->sample_data,data->event_data,
								data->event_types);


			PyObject_CallFunction(updateBooleanFlags,"O(iiiiiiii)",trackerInfo,
								data->in_sample_block,data->in_event_block,
								data->have_left_eye,data->have_right_eye,
								data->samples_on,data->events_on,
								data->control_read,	data->first_in_block);


			PyObject_CallFunction(updateTimeStamps,"O(iii)",trackerInfo,
								data->time,data->last_data_item_time,
								data->last_rcve_time);


			PyObject_CallFunction(updateStatusFlags,"O(iiiiii)",trackerInfo,
								data->last_status,data->packet_flags,
								data->link_flags,data->state_flags,
								data->link_dstatus,	data->link_pendcmd);


			PyObject_CallFunction(updateTypeFlags,"O(iii)",trackerInfo,
								data->last_data_gap_types,
								data->last_data_buffer_type,
								data->last_data_item_type);


			PyObject_CallFunction(updateReserved,"Oi",trackerInfo,data->reserved);


			//PyObject_CallFunction(updateSampleEventData,"O(iiiiiiiiiii)(ii)O",trackerInfo,
			//					data->last_data_buffer_size,data->last_data_item_contents,
			//					data->block_number,data->block_sample,
			//					data->block_event,data->last_resx,
			//					data->last_resy,data->queued_samples,
			//					data->queued_events,data->queue_size,
			//					data->queue_free,
			//					data->last_pupil[0],data->last_pupil[1],
			//					all_data_ctopyobj(&(data->last_data_item),data->last_data_item_type));

			//PyObject_CallFunction(updateNetworkData,"O",trackerInfo,
			//PyObject_CallFunction(updateAddresses,"O",trackerInfo,

			return Py_BuildValue("") ;


		}
		else
			PyErr_Format(PyExc_RuntimeError,"Could not collect data status.");

	}
	return NULL;
}




static void getDisplayConfiguration(int *width, int *height)
{
	PyObject *srgraphics = NULL;
	*width = 640;
	*height = 480;


	srgraphics = PyImport_ImportModule("srgraphics_c");
	if(srgraphics)
	{
		PyObject *display = NULL;
		PyObject * getDisplay   = PyDict_GetItemString(PyModule_GetDict(srgraphics),"getDisplay");
		if(getDisplay)
			display = PyObject_CallFunction(getDisplay,"");
		if(display)
		{
			PyObject *w = NULL;
			PyObject *h = NULL;
			w = PyObject_GetAttrString(display,"width");
			h = PyObject_GetAttrString(display,"height");
			if(w&&h)
			{
				*width  = PyLong_AsLong(w);
				*height =PyLong_AsLong(h);
			}
		}
	}
	PyErr_Clear();
}
static PyObject* PyEyelink_New(PyTypeObject *type)
{
	PyObject *self;
	self = type->tp_alloc(type, 0);
	return self;
}



/*! \fn def pylink::EyeLinkCBind::open
	Opens connection to single tracker.  If no parameters are given, it tries to open connection
	to the default host (100.1.1.1).

	@remarks This is equivalent to the C API
	\code
	INT16 eyelink_open_node(ELINKADDR node, INT16 busytest);
	\endcode
	@param eyelink_address Optional argument. Text IP address of the host PC (the default value is, "100.1.1.1").
	@param busytest Optional argument. If non-zero the call to \c eyelink_open_node() will not disconnect an existing connection.
	@return Throws Runtime error exception if it cannot open the connection.
 */
GENERICFCNPROTO(open)
{
	char *addr = "100.1.1.1";
	short busytest = 0;
	if (PyArg_ParseTuple(args,"|si",&addr, &busytest))
		{
			int rv = 0;
#if defined(MACOSX)  && !defined(BIGENDIAN)
			if(busytest !=0)
				printf("On intel mac's busy test !=0 is not support to overcome stability issues!(September 5, 2007)\n");
			set_eyelink_address(addr);
			rv = open_eyelink_connection(0);	
#else
			
			ELINKADDR node;
			text_to_elinkaddr(addr,node,0);
			rv = eyelink_open_node(node,busytest);
#endif
			if(rv==0)
			{
                return Py_BuildValue("") ;
			}
			else
				PyErr_Format(PyExc_RuntimeError, "Could not connect to tracker at %s", addr);
				return NULL;
		}
		return NULL;
}
static PyObject* Eyelink__new__(PyTypeObject *type, PyObject *args, PyObject *kwds)
{


	if(PyErr_Occurred())
			{
			PyErr_Print();
	}

	if(open_eyelink_connection(-1) == 0)
		return PyEyelink_New(type);
	else
		{
			PyErr_Format(PyExc_RuntimeError, "eyelink_core initialization failed ");
			return NULL;
		}
}

#ifndef NO_SDL
/*! \fn def pylink::setCalibrationColors
	Passes the colors of the display background and fixation target to the eyelink_core_graphics
	library. During calibration, camera image display, and drift correction, the display background
	should match the brightness of the experimental stimuli as closely as possible, in order to
	maximize tracking accuracy. This function passes the colors of the display background and
	fixation target to the eyelink_core_graphics library. This also prevents flickering of the
	display at the beginning and end of drift correction.

	@remarks This is equivalent to the C API
	\code
	void set_calibration_colors(SDL_Color *fg, SDL_Color *bg);
	\endcode
	@param foreground_color Color for foreground calibration target.
	@param background_color Color for foreground calibration background.

	Both colors must be a three-integer (from 0 to 255) tuple encoding the red, blue, and green
	color component.

	\b Example:
	\code
	setCalibrationColors((0, 0, 0), (255, 255, 255))

	This sets the calibration target in black and calibration background in white.
	\endcode
 */
GENERICFCNPROTO(set_calibration_colors)
{

	int fr =0,
		fg =0,
		fb =0,
		br =0,
		bg =0,
		bb =0;
	if(PyArg_ParseTuple(args,"(iii)(iii):setCalibrationColors", &fr, &fg, &fb,&br, &bg, &bb))
	{
		SDL_Color fore={0,0,0};
		SDL_Color back = {0,0,0};


		fore.r = (unsigned char)fr;
		fore.g = (unsigned char)fg;
		fore.b = (unsigned char)fb;

		back.r = (unsigned char)br;
		back.g = (unsigned char)bg;
		back.b = (unsigned char)bb;

		set_calibration_colors(&fore,&back);
		return Py_BuildValue("") ;
	}
	return NULL;
}

/*! \fn def pylink::setTargetSize
	The standard calibration and drift correction target is a disk (for peripheral delectability)
	with a central "hole" target (for accurate fixation). The sizes of these features may be set
	with this function.

	@remarks This function is equivalent to the C API
	\code
	void set_target_size(UINT16 diameter, UINT16 holesize);
	\endcode
	@param diameter Size of outer disk, in pixels.
	@param holesize Size of central feature, in pixels.  If holesize is \c 0, no central feature
					will be drawn.  The disk is drawn in the calibration foreground color, and
					the hole is drawn in the calibration background color.
 */
GENERICFCNPROTO(set_target_size)
{
	int h1 =0;
	int h2 =0;
	if(PyArg_ParseTuple(args,"ii:setTargetSize", &h1, &h2))
	{
                set_target_size(h1, h2);
		return Py_BuildValue("") ;
	}
	return NULL;
}

#ifdef ENABLE_INTERNATIONALIZATION


/*! \fn def pylink::setCalibrationSounds
	Selects the sounds to be played during \c do_tracker_setup(), including calibration, validation
	and drift correction. These events are the display or movement of the target, successful
	conclusion of calibration or good validation, and failure or interruption of calibration or validation.

	@remarks If no sound card is installed, the sounds are produced as "beeps" from the PC speaker.
	Otherwise, sounds can be selected by passing a string. If the string is "" (empty), the default
	sounds are played. If the string is "off", no sound will be played for that event. Otherwise,
	the string should be the name of a .WAV file to play.  This function is equivalent to the C API
	\code
	void set_cal_sounds(char *target, char *good, char *error);
	\endcode
	@param target Sets sound to play when target moves.
	@param good Sets sound to play on successful operation.
	@param error Sets sound to play on failure or interruption.
 */

GENERICFCNPROTO(set_cal_sounds)
{
	char* ontarget =NULL;
	char* ongood =NULL;
	char* onbad =NULL;
	int targetlen =0;
	int goodlen =0;
	int badlen =0;

	if(PyArg_ParseTuple(args,"es#es#es#:setCalibrationSounds", "utf-16",&ontarget,&targetlen,"utf-16", &ongood, &goodlen,"utf-16", &onbad,&badlen))
	{
		if(targetlen >2)
			ontarget = getMultiByte(ontarget,targetlen);
		else
			ontarget = "";
		if(goodlen >2)
			ongood = getMultiByte(ongood,goodlen);
		else
			ongood = "";
		if(badlen >2)
			onbad = getMultiByte(onbad,badlen);
		else
			onbad = "";

		set_cal_sounds(ontarget,ongood, onbad);
		if(targetlen >2)
			free(ontarget);
		if(goodlen >2)
			free(ongood);
		if(badlen >2)
			free(onbad);



		return Py_BuildValue("") ;
	}
	return NULL;
}

/*! \fn def pylink::setDriftCorrectSounds
	Selects the sounds to be played during \c doDriftCorrect(). These events are the display or movement of
	the target, successful conclusion of drift correction, and pressing the 'ESC' key to start the Setup menu.

	@remarks If no sound card is installed, the sounds are produced as "beeps" from the PC speaker.
	Otherwise, sounds can be selected by passing a string. If the string is "" (empty), the default sounds
	are played. If the string is "off", no sound will be played for that event. Otherwise, the string should
	be the name of a .WAV file to play.  This function is equivalent to the C API
	\code
	void set_dcorr_sounds(char *target, char *good, char *setup);
	\endcode
	@param target Sets sound to play when target moves.
	@param good Sets sound to play on successful operation.
	@param setup Sets sound to play on 'ESC' key pressed.
 */
GENERICFCNPROTO(set_dcorr_sounds)
{
	char* ontarget =NULL;
	char* ongood =NULL;
	char* onbad =NULL;
	int targetlen =0;
	int goodlen =0;
	int badlen =0;

	if(PyArg_ParseTuple(args,"es#es#es#:setDriftCorrectSounds",  "utf-16",&ontarget,&targetlen,"utf-16", &ongood, &goodlen,"utf-16", &onbad,&badlen))
	{
		if(targetlen >2)
			ontarget = getMultiByte(ontarget,targetlen);
		else
			ontarget = "";
		if(goodlen >2)
			ongood = getMultiByte(ongood,goodlen);
		else
			ongood = "";
		if(badlen >2)
			onbad = getMultiByte(onbad,badlen);
		else
			onbad = "";

		set_dcorr_sounds(ontarget,ongood, onbad);
		if(targetlen >2)
			free(ontarget);
		if(goodlen >2)
			free(ongood);
		if(badlen >2)
			free(onbad);
		return Py_BuildValue("") ;
	}
	return NULL;
}

#else
GENERICFCNPROTO(set_cal_sounds)
{
	char* ontarget =NULL;
	char* ongood =NULL;
	char* onbad =NULL;
	if(PyArg_ParseTuple(args,"sss:setCalibrationSounds", &ontarget,&ongood, &onbad))
	{
		set_cal_sounds(ontarget,ongood, onbad);
		return Py_BuildValue("") ;
	}
	return NULL;
}
GENERICFCNPROTO(set_dcorr_sounds)
{
	char* ontarget =NULL;
	char* ongood =NULL;
	char* onbad =NULL;
	if(PyArg_ParseTuple(args,"sss:setDriftCorrectSounds", &ontarget,&ongood, &onbad))
	{
		set_dcorr_sounds(ontarget,ongood, onbad);
		return Py_BuildValue("") ;
	}
	return NULL;
}
#endif


/*! \fn def pylink::setCameraPosition
	Sets the camera position on the display computer.  Moves the top left hand corner of the camera position
	to new location.

	@param left X coordinate of upper-left corner of the camera image window.
	@param top Y coordinate of upper-left corner of the camera image window.
	@param right X coordinate of lower-right corner of the camera image window.
	@param bottom Y coordinate of lower-right corner of the camera image window.
 */
GENERICFCNPROTO(set_camera_image_position)
{
	short h1 =0;
	short h2 =0;
	short h3 =0;
	short h4 =0;
	if(PyArg_ParseTuple(args,"iiii:setCameraImagePosition", &h1, &h2, &h3, &h4))
	{
		set_camera_image_position(h1, h2, h3, h4);
		 return Py_BuildValue("") ;
	}
	return NULL;
}

/*! \fn def pylink::getDisplayInformation
	Returns the display configuration.

	@return Instance of DisplayInfo class. The width, height, bits, and refresh rate of the display can be
			accessed from the returned value.

	\b Example:
	\code
	display = getDisplayInformation()
	print display.width, display.height, display.bits, display.refresh
	\endcode
 */
GENERICFCNPROTO(get_display_information )
{
	if(PyArg_ParseTuple(args,":getDisplayInformation"))
	{
		DISPLAYINFO dispinfo;
		get_display_information(&dispinfo);
		return PyObject_CallFunction(newDisplayInfo,"iiif",dispinfo.width, dispinfo.height, dispinfo.bits, dispinfo.refresh);
	}
	return NULL;
}

/*! \fn def pylink::EyeLinkCBind::getImageCrossHairData
	\deprecated This function is deprecated. Use lib::tracker::EyeLinkCustomDisplay::draw_cross_hair()
 */
GENERICFCNPROTO(get_image_xhair_data)
{
	if(PyArg_ParseTuple(args,":getImageCrossHairData"))
	{
		INT16 x[4];
		INT16 y[4];
		INT16 enabled;
		int channel;
		channel = get_image_xhair_data(x,y,&enabled);
		if(enabled)
		{
			int i =0;
			PyObject * xv = PyList_New(4);
			PyObject * yv = PyList_New(4);

			for(i =0; i < 4; i++)
			{
				PyList_SetItem(xv,i, PyInt_FromLong(x[i]));
				PyList_SetItem(yv,i, PyInt_FromLong(y[i]));
			}
			return Py_BuildValue("(iOO)",channel,xv,yv);
		}
		else
			return Py_None;// not available
	}
	return NULL;
}
/*! \fn def pylink::EyeLinkCBind::closeGraphics
	Notifies the eyelink_core_graphics to close or release the graphics.

	@remarks This is equivalent to the C API
	\code
	void close_expt_graphics(void);
	\endcode
 */

GENERICFCNPROTO(close_expt_graphics)
{
	if(PyArg_ParseTuple(args,":closeGraphics"))
	{
		close_expt_graphics();
		 return Py_BuildValue("") ;
	}
	return NULL;
}
#endif

static struct _DISPFCNS
{
	HOOKFCNS hfcns;
	PyObject *self; //  pointer to module
}dispfcns;

GENERICFCNPROTO(map_eyelink_py_draw_cal_target_hook)
{
	INT16 x =0;
	INT16 y =0;
	if(PyArg_ParseTuple(args,"(hh):drawCalTarget", &x, &y))
		{
                         if(dispfcns.hfcns.draw_cal_target_hook)
				dispfcns.hfcns.draw_cal_target_hook(x, y);
			 return Py_BuildValue("") ;
		}
	return NULL;
}


void   ELCALLBACK  eyelink_py_draw_cal_target_hook(INT16 x, INT16 y)
{
	if(PyErr_Occurred())
	{
		PyErr_Print();
	}

	PyObject_CallFunction(drawCalTarget,"ii",x, y);

        if(PyErr_Occurred())
	{
                printf("except found \n");
		PyErr_Print();
		exit(1);
	}

}


void setup_remap_hooks(PyObject *self)
{
	HOOKFCNS *hooks = get_all_hook_functions();
	memset(&(dispfcns.hfcns),0,sizeof(HOOKFCNS));
	memcpy(&(dispfcns.hfcns),hooks,sizeof(HOOKFCNS));
	dispfcns.self = self;
	hooks->draw_cal_target_hook		=		eyelink_py_draw_cal_target_hook;
    setup_graphic_hook_functions(hooks);
}

#ifndef NO_SDL
/*! \fn def pylink::EyeLinkCBind::openGraphics
	Opens the graphics if the display mode is not set. If the display mode is already set, uses the existing
	display mode.

	@remarks This is equivalent to the SDL version C API
	\code
	INT16 init_expt_graphics(SDL_Surface * s, DISPLAYINFO *info);
	\endcode
	@param dimension Two-item tuple of display containing width and height information.
	@param bits Color bits.
	@return None or run-time error.
 */
GENERICFCNPROTO(init_expt_graphics)
{
	int w = 0;
	int h =0;
	int depth =0;
	if(PyArg_ParseTuple(args,"|(ii)i:openGraphics", &w, &h, &depth))
	{
		DISPLAYINFO dispinfo;
		DISPLAYINFO *dsp = NULL;
		memset(&dispinfo,0,sizeof(dispinfo));
		if(w &&h)
		{
			dispinfo.width =w;
			dispinfo.height = h;
			dispinfo.bits = depth;
			dsp = &dispinfo;
		}		if(init_expt_graphics(NULL,dsp)==0)
		{
			setup_remap_hooks(self);
			 return Py_BuildValue("") ;
		}
		else
			PyErr_Format(PyExc_RuntimeError, "Could not initalize  graphics");
	}
	return NULL;
}
#endif

#ifdef ENABLE_INTERNATIONALIZATION
char * getMultiByte(char *text,int len)
{
	int size = 0;
	char* rtn = NULL;

	rtn = malloc(sizeof(char)*len+2);

	if(!rtn)
		{
			PyErr_Format(PyExc_RuntimeError, "Insufficient Memory");
			return NULL;
		}
	else if(len==2)
	{
		PyErr_Format(PyExc_RuntimeError, "Null string passed in");
		return NULL;
	}


	size = WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)text+1, (len/2)-1, rtn,
					   (len), NULL, NULL );

	if(size >0)
	{
		rtn[size] = 0;
		return rtn;
	}
	else
	{
		DWORD lasterror = GetLastError();
		switch(lasterror)
		{
			case ERROR_INSUFFICIENT_BUFFER:
				PyErr_Format(PyExc_RuntimeError, "Could not create hash value for string! Insufficient buffer");
				break;
			case ERROR_INVALID_FLAGS:
				PyErr_Format(PyExc_RuntimeError, "Could not create hash value for string! Invalid flags");
				break;
			case ERROR_INVALID_PARAMETER:
				PyErr_Format(PyExc_RuntimeError, "Could not create hash value for string! Invalid parameter");
				break;
			default:
				PyErr_Format(PyExc_RuntimeError, "Could not create hash value for string! Unknown reason");
				break;

		}
	}

	free(rtn);
	return NULL;
}
#endif
#ifdef ENABLE_INTERNATIONALIZATION
static PyObject* eyelink_eyelink_eyemsg_printf(PyObject* self, PyObject* args)
{
	char *text=NULL;
	int len = 0;
	int offset =0;
	if (PyArg_ParseTuple(args,"es#|i","utf-16",&text,&len, &offset))
	{
		char *rvm = NULL;
		int rv =0;
		char *rtn = getMultiByte(text,len);
		if(rtn)
		{
			if(strlen(rtn)>130)
				rtn[129]=0;
			rv = eyelink_send_message_ex(offset, rtn);
			free(rtn);
			rvm = eyelink_get_error(rv, "eyelink_send_message_ex");
			if(!rvm || !strlen(rvm) || rv == NO_REPLY)
				return Py_BuildValue("i",rv);
			else
				PyErr_Format(PyExc_RuntimeError, "%s", rvm);
		}

	}
        return NULL;
}

/*! \fn def pylink::sendMessageToFile
	Very similar to sendMessage(), with the following features:
	\arg Optionally write the timestamped message to file instead of sending over the link to tracker.
	If message file is not opened using openMessageFile() and a tracker connection is available, this function sends the message to tracker,
	otherwise, the message is written to the message file with a time stamp.
	\arg Optional time of event.
	@param msg message text
	@param t optional time of event (0 = NOW)
 */
static PyObject* eyelink_eyelink_timemsg_printf(PyObject* self, PyObject* args)
{
		char *text=NULL;
		int len = 0;
		int tme = 0;
		if (PyArg_ParseTuple(args,"es#|i","utf-16",&text,&len,&tme))
		{
			char *rvm = NULL;
			int rv =0;
			char *rtn = getMultiByte(text,len);
			if(rtn)
			{
				if(strlen(rtn)>130)
					rtn[129]=0;
				rv = timemsg_printf(tme, rtn);
				free(rtn);
				rvm = eyelink_get_error(rv, "timemsg_printf");
				if(!rvm || !strlen(rvm) || rv == NO_REPLY)
					return Py_BuildValue("i",rv);
				else
					PyErr_Format(PyExc_RuntimeError, "%s", rvm);
			}

		}
	return NULL;
}

static PyObject *eyelink_self=NULL;
void ELCALLTYPE receiveDataFile_progress(unsigned int size,unsigned int received)
{
	if(PyErr_Occurred())PyErr_Print();
	if(eyelink_self)
		PyObject_CallMethod(eyelink_self,"progressUpdate","ii",size,received);
	if(PyErr_Occurred())PyErr_Print();
}
/*! \fn def pylink::EyeLinkCBind::receiveDataFile
	This receives a data file from the EyeLink tracker PC. Source file name and destination file
	name should be given.

	@remarks This function is equivalent to the C API
	\code
	int receive_data_file(char *src, char *dest, int is_path);
	\endcode
	@param src Name of eye tracker file (including extension).
	@param dest Name of local file to write to (including extension).
	@return Size of file if successful, Otherwise Runtime Exception is raised.
 */
GENERICFCNPROTO(receiveDataFile)
{
	char *src = NULL;
	char *dest = NULL;
	int srclen =0;
	int destlen =0;
	if (PyArg_ParseTuple(args,"es#es#","utf-16", &src,&srclen, "utf-16",&dest,&destlen))
	{
		int rv =0;
		dest = getMultiByte(dest,destlen);
		if(dest)
		{
			if(srclen >2)
			{
				src = getMultiByte(src,srclen);
				eyelink_self = self;
				receive_data_file_feedback(src, dest, 0,receiveDataFile_progress);
				eyelink_self = NULL;
				free(src);
				free(dest);
			}
			else
			{
				src = "";
				eyelink_self = self;
				receive_data_file_feedback(src, dest, 0,receiveDataFile_progress);
				eyelink_self = NULL;
				free(dest);
			}
			RETURNERROR(rv,receive_data_file);
		}
	}
	return NULL;

}
#else
static PyObject* eyelink_eyelink_eyemsg_printf(PyObject* self, PyObject* args)
{
	char *text=NULL;
	int offset =0;
	if (PyArg_ParseTuple(args,"s|i",&text, &offset))
	{
		char *rvm = NULL;
		int rv =0;
		char *rtn = malloc(strlen(text)+1);
		if(rtn)
		{
                        strcpy(rtn,text);
			if(strlen(rtn)>130)
				rtn[129]=0;
			rv = eyelink_send_message_ex(offset, rtn);
			free(rtn);
			rvm = eyelink_get_error(rv, "eyelink_send_message_ex");
			if(!rvm || !strlen(rvm) || rv == NO_REPLY)
				return Py_BuildValue("i",rv);
			else
				PyErr_Format(PyExc_RuntimeError, "%s", rvm);
		}

	}
        return NULL;
}
static PyObject* eyelink_eyelink_timemsg_printf(PyObject* self, PyObject* args)
{
	char *text=NULL;
	int offset =0;
	if (PyArg_ParseTuple(args,"s|i",&text, &offset))
	{
		char *rvm = NULL;
		int rv =0;
		char *rtn = malloc(strlen(text)+1);
		if(rtn)
		{
                        strcpy(rtn,text);
			if(strlen(rtn)>130)
				rtn[129]=0;
			rv = timemsg_printf(offset, rtn);
			free(rtn);
			rvm = eyelink_get_error(rv, "timemsg_printf");
			if(!rvm || !strlen(rvm) || rv == NO_REPLY)
				return Py_BuildValue("i",rv);
			else
				PyErr_Format(PyExc_RuntimeError, "%s", rvm);
		}

	}
        return NULL;
}


GENERICFCNPROTO(receiveDataFile)
{
	char *src = NULL;
	char *dest = NULL;
	if (PyArg_ParseTuple(args,"ss",&src,&dest))
	{
                int rv =0;
		receive_data_file(src, dest, 0);
		RETURNERROR(rv,receive_data_file);
	}
	return NULL;

}
#endif




/*! \fn def pylink::EyeLinkCBind::bitmapSaveAndBackdrop
	This function saves the entire bitmap as a .BMP, .JPG, .PNG, or .TIF file, and
	transfers the image to tracker as backdrop for gaze cursors.
	@param iwidth Original image width.
	@param iheight Original image height.
	@param pixels Pixels of the image in the following format.pixel=[line1, line2, ... linen] line=[pix1,pix2,...,pixn],pix=(r,g,b).
	@param xs Crop x position.
	@param ys Crop y position.
	@param width Crop width.
	@param height Crop height.
	@param fname File name to save.
	@param path Path to save.
	@param svoptions Save options(\c SV_NOREPLACE, \c SV_MAKEPATH). If the file exists, it replaces the file unless SV_NOREPLACE is specified.
	@param xd X position - transfer.
	@param yd Y position - transfer.
	@param xferoptions Transfer options(<tt>BX_AVERAGE,BX_DARKEN,BX_LIGHTEN,BX_MAXCONTRAST,BX_NODITHER,BX_GRAYSCALE</tt>).
	Transfer options set with bitwise OR of the following
	constants, determines how bitmap is processed:
	\arg \c BX_AVERAGE Averaging combined pixels
	\arg \c BX_DARKEN  Choosing darkest and keep thin dark lines.
	\arg \c BX_LIGHTEN Choosing darkest and keep thin white lines and control how
	bitmap size is reduced to fit tracker display.
	\arg \c BX_MAXCONTRAST Maximizes contrast for clearest image.
	\arg \c BX_NODITHER Disables the dithering of the image.
	\arg \c BX_GREYSCALE Converts the image to grayscale (grayscale works
	best for EyeLink I, text, etc.).

	\sa bitmapBackdrop(),bitmapSave()
 */
GENERICFCNPROTO(bitmapSaveAndBackdrop)
{
	PyObject *pixels = NULL;
	int xs;
	int ys;
	int width;
	int height;
	int sv_options;
	int xd;
	int yd;
	int xferoptions;
        int imgwidth;
	int imgheight;
	char *fname=NULL;
	char *path = NULL;
#ifdef ENABLE_INTERNATIONALIZATION
	int fnamelen =0;
	int pathlen =0;
	if (PyArg_ParseTuple(args,"iiOiiiies#es#iiii",&imgwidth, &imgheight, &pixels, &xs, &ys,&width, &height,"utf-16",&fname,&fnamelen,"utf-16", &path,&pathlen, &sv_options,&xd,&yd,&xferoptions))
	{
		fname = getMultiByte(fname,fnamelen);
		path = getMultiByte(path,pathlen);
#else
	if (PyArg_ParseTuple(args,"iiOiiiissiiii",&imgwidth, &imgheight, &pixels, &xs, &ys,&width, &height,&fname, &path, &sv_options,&xd,&yd,&xferoptions))
        {
#endif

		{
			char *pix;
			int x,y;
			EYEBITMAP *dst = malloc(sizeof(EYEBITMAP));
			memset(dst,0,sizeof(EYEBITMAP));
			dst->w= imgwidth;
			dst->h = imgheight;
			dst->pixels = malloc(dst->w*4*dst->h);
			pix = dst->pixels;
			for(y =0; y <dst->h; y++)
			{
				PyObject *line = PyList_GetItem(pixels,y);
				for(x =0; x <dst->w; x++)
				{
					PyObject *item = PyList_GetItem(line,x);
					int r = PyInt_AsLong(PyTuple_GetItem(item,0));
					int g = PyInt_AsLong(PyTuple_GetItem(item,1));
					int b = PyInt_AsLong(PyTuple_GetItem(item,2));
                                        #ifdef BIGENDIAN
					*(pix++)=b;
					*(pix++)=g;
					*(pix++)=r;
					*(pix++)=0;
                                        #else
                                        *(pix++)=0; // alpha
					*(pix++)=r;
					*(pix++)=g;
					*(pix++)=b;
                                        #endif
				}
			}

			dst->depth = 32;
			dst->pitch = dst->w*4;

			dst->format = malloc(sizeof(EYEPIXELFORMAT));
			memset(dst->format,0,sizeof(EYEPIXELFORMAT));
			el_bitmap_save_and_backdrop(dst,(INT16)xs,(INT16)ys,(INT16)width,(INT16)height,fname,path,(UINT16)sv_options, (INT16)xd,(INT16)yd,(UINT16)xferoptions);
		}

		return Py_BuildValue("");
	}
	return NULL;
}

/*! \fn def pylink::bitmapSave
	This function saves the entire bitmap or selected part of a bitmap in an
	image file (with an extension of .png, .bmp, .jpg, or .tif). It creates
	the specified file if this file does not exist.

	@param iwidth Original image width.
	@param iheight Original image height.
	@param pixels Pixels of the image in the following format.pixel=[line1, line2, ... linen] line=[pix1,pix2,...,pixn],pix=(r,g,b).
	@param xs Crop x position.
	@param ys Crop y position.
	@param width Crop width.
	@param height Crop height.
	@param fname File name to save.
	@param path Path to save.
	@param svoptions Save options(\c SV_NOREPLACE, \c SV_MAKEPATH). If the file exists, it replaces the file unless SV_NOREPLACE is specified.

	This function is equivalent to the C API el_bitmap_save()
 */
GENERICFCNPROTO(bitmapSave)
{
	PyObject *pixels = NULL;
	int xs;
	int ys;
	int width;
	int height;
	int sv_options;
        int imgwidth;
	int imgheight;

	char *fname=NULL;
	char *path = NULL;
#ifdef ENABLE_INTERNATIONALIZATION
	int fnamelen =0;
	int pathlen =0;

	if (PyArg_ParseTuple(args,"iiOiiiies#es#i",&imgwidth, &imgheight, &pixels, &xs, &ys,&width, &height,"utf-16",&fname,&fnamelen,"utf-16", &path,&pathlen, &sv_options))
	{
		fname = getMultiByte(fname,fnamelen);
		path = getMultiByte(path,pathlen);
#else
	if (PyArg_ParseTuple(args,"iiOiiiissi",&imgwidth, &imgheight, &pixels, &xs, &ys,&width, &height,&fname,&path, &sv_options))
	{

#endif
		{
			char *pix;
			int x,y;
			EYEBITMAP *dst = malloc(sizeof(EYEBITMAP));
			memset(dst,0,sizeof(EYEBITMAP));
			dst->w= imgwidth;
			dst->h = imgheight;
			dst->pixels = malloc(dst->w*4*dst->h);
			pix = dst->pixels;
			for(y =0; y <dst->h; y++)
			{
				PyObject *line = PyList_GetItem(pixels,y);
				for(x =0; x <dst->w; x++)
				{
					PyObject *item = PyList_GetItem(line,x);
					int r = PyInt_AsLong(PyTuple_GetItem(item,0));
					int g = PyInt_AsLong(PyTuple_GetItem(item,1));
					int b = PyInt_AsLong(PyTuple_GetItem(item,2));
					*(pix++)=b; // blue
					*(pix++)=g; //green
					*(pix++)=r;
					*(pix++)=0; // alpha
				}
			}

			dst->depth = 32;
			dst->pitch = dst->w*4;

			dst->format = malloc(sizeof(EYEPIXELFORMAT));
			memset(dst->format,0,sizeof(EYEPIXELFORMAT));
			dst->format->Rmask = 0x0000ff00;
			dst->format->Gmask = 0x00ff0000;
			dst->format->Bmask = 0xff000000;
			dst->format->Amask = 0x000000ff;
			el_bitmap_save(dst,(INT16)xs,(INT16)ys,(INT16)width,(INT16)height,fname,path,(UINT16)sv_options);
		}

		return Py_BuildValue("");
	}
	return NULL;
}

#ifndef NO_SDL
#ifdef WIN32
GENERICFCNPROTO(setCalibrationAnimationTarget)
{
	char *fname = NULL;
	int fnamelen = -1;
	int playcount = -1;
	if (PyArg_ParseTuple(args,"es#i","utf-16", &fname,&fnamelen,&playcount))
	{
		fname = getMultiByte(fname,fnamelen);
		set_cal_animation_target(fname,playcount,0);
		free(fname);
		return Py_BuildValue("");
	}
	return NULL;
}
#else
GENERICFCNPROTO(setCalibrationAnimationTarget)
{
	PyErr_Format(PyExc_RuntimeError, "%s", "setCalibraionAnimationTarget not avilable in this platform ");
	return NULL;
}
#endif
#endif

/*! \fn def pylink::EyeLinkCBind::bitmapBackdrop
	This function transfers the bitmap to the tracker PC as backdrop for gaze
	cursors.
	@param iwidth Original image width.
	@param iheight Original image height.
	@param pixels Pixels of the image in the following format.pixel=[line1, line2, ... linen] line=[pix1,pix2,...,pixn],pix=(r,g,b).
	@param xs Crop x position.
	@param ys Crop y position.
	@param width Crop width.
	@param height Crop height.
	@param xd X position - transfer.
	@param yd Y position - transfer.
	@param xferoptions Transfer options(<tt>BX_AVERAGE,BX_DARKEN,BX_LIGHTEN,BX_MAXCONTRAST,BX_NODITHER,BX_GRAYSCALE</tt>).
	Transfer options set with bitwise OR of the following
	constants, determines how bitmap is processed:
	\arg \c BX_AVERAGE Averaging combined pixels
	\arg \c BX_DARKEN  Choosing darkest and keep thin dark lines.
	\arg \c BX_LIGHTEN Choosing darkest and keep thin white lines and control how
	bitmap size is reduced to fit tracker display.
	\arg \c BX_MAXCONTRAST Maximizes contrast for clearest image.
	\arg \c BX_NODITHER Disables the dithering of the image.
	\arg \c BX_GREYSCALE Converts the image to grayscale (grayscale works
	best for EyeLink I, text, etc.).

	@remark This function should not be called when timing is critical, as this might take very long to return.
 */
GENERICFCNPROTO(bitmapBackdrop)
{
	PyObject *pixels = NULL;
	int xs;
	int ys;
	int width;
	int height;
	int xd;
	int yd;
	int xferoptions;

	int imgwidth;
	int imgheight;
	if (PyArg_ParseTuple(args,"iiOiiiiiii",&imgwidth, &imgheight, &pixels, &xs, &ys,&width, &height,&xd,&yd,&xferoptions))
	{
		{
			char *pix;
			int x,y;
			EYEBITMAP *dst = malloc(sizeof(EYEBITMAP));
			memset(dst,0,sizeof(EYEBITMAP));
			dst->w= imgwidth;
			dst->h = imgheight;
			dst->pixels = malloc(dst->w*4*dst->h);
			pix = dst->pixels;
			for(y =0; y <dst->h; y++)
			{
				PyObject *line = PyList_GetItem(pixels,y);
				for(x =0; x <dst->w; x++)
				{
					PyObject *item = PyList_GetItem(line,x);
					int r = PyInt_AsLong(PyTuple_GetItem(item,0));
					int g = PyInt_AsLong(PyTuple_GetItem(item,1));
					int b = PyInt_AsLong(PyTuple_GetItem(item,2));
					*(pix++)=b;
					*(pix++)=g;
					*(pix++)=r;
					*(pix++)=0;
				}
			}

			dst->depth = 32;
			dst->pitch = dst->w*4;

			dst->format = malloc(sizeof(EYEPIXELFORMAT));
			memset(dst->format,0,sizeof(EYEPIXELFORMAT));
			el_bitmap_to_backdrop(dst,(INT16)xs,(INT16)ys,(INT16)width,(INT16)height, (INT16)xd,(INT16)yd,(UINT16)xferoptions);
		}

		return Py_BuildValue("");
	}
	return NULL;
}
#ifdef WIN32
/*! \fn def pylink::EyeLinkCBind::calculateVelocity
	Calculates overall velocity for left and right eyes separately.
	@param[in] slen Sample model to use for velocity calculation. Acceptable models
				are \c FIVE_SAMPLE_MODEL, \c NINE_SAMPLE_MODEL, \c SEVENTEEN_SAMPLE_MODEL and
				\c EL1000_TRACKER_MODEL.
	@return A list with 3 elements:
	- First two elements of the list:
				- Upon return of this function, vel[0] will contain overall velocity for left eye
				and vel[1] will contain overall velocity for right eye.  If velocity cannot be
				calculated for any reason(eg. insufficient samples, no data) MISSING_DATA is
				filled for the given velocity.
	- Third element of the list:
				- vel_sample Velocity for sample.
 */
GENERICFCNPROTO(eyelink_calculate_velocity)
{
	int slen =0;
	if (PyArg_ParseTuple(args,"i:calculateVelocity",&slen))
	{
		PyObject *sam=NULL;
		float vel[2] = {0,0};
		ALLF_DATA vsam;
		memset(&vsam,0,sizeof(vsam));
		eyelink_calculate_velocity(slen,vel,&(vsam.fs));
		sam = allf_data_ctopyobj(&vsam,SAMPLE_TYPE);
		{
			PyObject *rv = Py_BuildValue("(ddO)",vel[0],vel[1],sam);
			Py_DECREF(sam);
			return rv;
		}

	}
	return NULL;
}

/*! \fn def pylink::EyeLinkCBind::calculateVelocityXY
	Calculates left x velocity, left y velocity, right x velocity and right y velocity from queue of samples.
	@param[in] slen Sample model to use for velocity calculation. Acceptable models
				are \c FIVE_SAMPLE_MODEL, \c NINE_SAMPLE_MODEL, \c SEVENTEEN_SAMPLE_MODEL and
				\c EL1000_TRACKER_MODEL.

	@return A list with 3 elements
	- The first element of the list:
				- leftvel x and y velocity for left eye.
				The float tuple of 2 filled with x and y velocity values.  Upon return
				of this function leftvel[0] contains the left x velocity data and leftvel[1] contains
				left y velocity data.  If velocity cannot be calculated for any reason
				(eg. insufficient samples, no data) MISSING_DATA is filled for the given
				velocity.
	- The second element of the list:
			    - rightvel x and y velocity for right eye.
				The float tuple of 2 filled with x and y velocity values.  Upon return
				of this function rightvel[0] contains the right x velocity data and rightvel[1] contains
				right y velocity data.  If velocity cannot be calculated for any reason
				(eg. insufficient samples, no data) MISSING_DATA is filled for the given
				velocity.
	- The third element of the list:
				- vel_sample Velocity for sample.

*/
GENERICFCNPROTO( eyelink_calculate_velocity_x_y)
{
	int slen =0;
	if (PyArg_ParseTuple(args,"i:calculateVelocityXY",&slen))
	{
		PyObject *sam=NULL;
		float xvel[2] = {0,0};
		float yvel[2] = {0,0};
		ALLF_DATA vsam;
		memset(&vsam,0,sizeof(vsam));
		eyelink_calculate_velocity_x_y(slen,xvel,yvel,&(vsam.fs));
		sam = allf_data_ctopyobj(&vsam,SAMPLE_TYPE);
		{
			PyObject *rv = Py_BuildValue("((dd)(dd)O)",xvel[0],yvel[0],xvel[1],yvel[1],sam);
			Py_DECREF(sam);
			return rv;
		}

	}
	return NULL;
}

/*! \fn def pylink::EyeLinkCBind::calculateOverallVelocityAndAcceleration
	Calculates overall velocity and acceleration for left and right eyes separately.
	@param[in] slen Sample model to use for velocity calculation. Acceptable models
				are \c FIVE_SAMPLE_MODEL, \c NINE_SAMPLE_MODEL, \c SEVENTEEN_SAMPLE_MODEL and
				\c EL1000_TRACKER_MODEL.
	@return A list with 3 elements
	- The first element of the list:
			- overall velocity for left and right eye.
			Upon return of this function,vel[0] will contain overall velocity for
			left eye and vel[1] will contain overall velocity for right eye.  If
			velocity cannot be calculated for any reason(eg. insufficient
			samples, no data) MISSING_DATA is filled for the given velocity.
	- The second element of the list:
			- overall acceleration for left and right eye. Upon return of this function,
			acc[0] will contain overall acceleration for left eye and acc[1] will contain overall acceleration
			for right eye.  If acceleration cannot be calculated for any reason(eg. insufficient
			samples, no data) MISSING_DATA is filled for the given acceleration.
	- The third element of the list:
			- vel_sample Velocity for sample.

 */
GENERICFCNPROTO( eyelink_calculate_overallvelocity_and_acceleration)
{
	int slen =0;
	if (PyArg_ParseTuple(args,"i:calculateOverallVelocityAndAcceleration",&slen))
	{

		PyObject *sam=NULL;
		float vel[2] = {0,0};
		float acc[2] = {0,0};
		ALLF_DATA vsam;
		memset(&vsam,0,sizeof(vsam));
		eyelink_calculate_overallvelocity_and_acceleration(slen,vel,acc,&(vsam.fs));
		sam = allf_data_ctopyobj(&vsam,SAMPLE_TYPE);
		{
			PyObject *rv = NULL;
			rv = Py_BuildValue("((dd)(dd)O)",vel[0],vel[1],acc[0],acc[1],sam);
			Py_DECREF(sam);
			return rv;
	    }

	}
	return NULL;
}
/*! \fn def pylink::EyeLinkCBind::getLastButtonStates
	Returns a flag word with bits set to indicate which tracker buttons are currently pressed.
	This is button 1 for the LSB, up to button 16 for the MSB.  Same as getButtonStates()
	except, the time is also returned.
	@return a list of with values time and states of 7 buttons.
	Example:
	@code
	v = eyelink.getLastButtonStates()
	print "time of last button states ", v[0]
	print "Button states"
	button_states = v[1:]
	button =0
	for x in button_states:
		button = button +1
		if x!=0:
			print "Button ",button," Pressed "

	@endcode
	\sa \c eyelink_send_keybutton()
 */
#endif
GENERICFCNPROTO( eyelink_last_button_states)
{
	if (PyArg_ParseTuple(args,":"))
	{
		UINT32 time =0;
		UINT16 state = eyelink_last_button_states(&time);
		if(state)
			return Py_BuildValue("(iiiiiiiii)",time,
				state & 0x01,
				(state & 0x02) >> 1,
				(state & 0x04) >> 2,
				(state & 0x08) >> 3,
				(state & 0x10) >> 4,
				(state & 0x20) >> 5,
				(state & 0x40) >> 6,
				(state & 0x80) >> 7
		);
		else
			return Py_BuildValue("O",Py_None);

	}
	return NULL;
}


