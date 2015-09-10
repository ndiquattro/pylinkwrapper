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
 * $Date: 2007/08/29 18:54:53 $
 */


#include <Python.h>
#include "core_expt.h"


static PyObject* customDisplay = NULL;

INT16  ELCALLBACK pylink_setup_cal_display     (void);
void   ELCALLBACK pylink_exit_cal_display      (void) ;
void   ELCALLBACK pylink_record_abort_hide     (void) ;
INT16  ELCALLBACK pylink_setup_image_display   (INT16 width, INT16 height) ;
void   ELCALLBACK pylink_image_title           (INT16 threshold, char *cam_name) ;
void   ELCALLBACK pylink_draw_image_line       (INT16 width, INT16 line, INT16 totlines, byte *pixels) ;
void   ELCALLBACK pylink_set_image_palette     (INT16 ncolors, byte r[], byte g[], byte b[]) ;
void   ELCALLBACK pylink_exit_image_display    (void) ;
void   ELCALLBACK pylink_clear_cal_display     () ;
void   ELCALLBACK pylink_erase_cal_target      ();
void   ELCALLBACK pylink_draw_cal_target       (INT16 x, INT16 y);
void   ELCALLBACK pylink_cal_target_beep       (void) ;
void   ELCALLBACK pylink_cal_done_beep         (INT16 error) ;
void   ELCALLBACK pylink_dc_done_beep          (INT16 error) ;
void   ELCALLBACK pylink_dc_target_beep        (void) ;
short  ELCALLBACK pylink_get_input_key         (InputEvent * event);
void   ELCALLBACK pylink_alert_printf          (const char *);


PyObject* eyelink_eyelink_init_custom_graphics(PyObject* self, PyObject* args)
{
	if (PyArg_ParseTuple(args,"O:", &customDisplay))
	{
	  HOOKFCNS fcns;
	  Py_INCREF(customDisplay);
	  memset(&fcns,0,sizeof(fcns));


	  fcns.setup_cal_display_hook = pylink_setup_cal_display;
	  fcns.exit_cal_display_hook  = pylink_exit_cal_display;
	  fcns.record_abort_hide_hook = pylink_record_abort_hide;
	  fcns.setup_image_display_hook =pylink_setup_image_display;
	  fcns.image_title_hook       = pylink_image_title;
	  fcns.draw_image_line_hook   = pylink_draw_image_line;
	  fcns.set_image_palette_hook = pylink_set_image_palette;
	  fcns.exit_image_display_hook= pylink_exit_image_display;
	  fcns.clear_cal_display_hook = pylink_clear_cal_display;
	  fcns.erase_cal_target_hook  = pylink_erase_cal_target;
	  fcns.draw_cal_target_hook   = pylink_draw_cal_target;
	  fcns.cal_target_beep_hook   = pylink_cal_target_beep;
	  fcns.cal_done_beep_hook     = pylink_cal_done_beep;
	  fcns.dc_done_beep_hook      = pylink_dc_done_beep;
	  fcns.dc_target_beep_hook    = pylink_dc_target_beep;
	  fcns.get_input_key_hook     = pylink_get_input_key;
	  fcns.alert_printf_hook      = pylink_alert_printf;
	  setup_graphic_hook_functions(&fcns);
	  return Py_BuildValue("");
	}
	return NULL;

}


INT16  ELCALLBACK pylink_setup_cal_display (void)
{
	if(PyErr_Occurred())PyErr_Print();
	PyObject_CallMethod(customDisplay,"setup_cal_display","");
	if(PyErr_Occurred())PyErr_Print();
	return 0;
}

void   ELCALLBACK pylink_exit_cal_display(void)
{
	if(PyErr_Occurred())PyErr_Print();
	PyObject_CallMethod(customDisplay,"exit_cal_display","");
	if(PyErr_Occurred())PyErr_Print();
}

void   ELCALLBACK pylink_record_abort_hide(void)
{
	if(PyErr_Occurred())PyErr_Print();
	PyObject_CallMethod(customDisplay,"record_abort_hide","");
	if(PyErr_Occurred())PyErr_Print();
}

INT16  ELCALLBACK pylink_setup_image_display(INT16 width, INT16 height)
{
	if(PyErr_Occurred())PyErr_Print();
	PyObject_CallMethod(customDisplay,"__updateimgsize__","ii", width,height);
	if(PyErr_Occurred())PyErr_Print();
	PyObject_CallMethod(customDisplay,"setup_image_display","ii", width,height);
	if(PyErr_Occurred())PyErr_Print();
	return 0;
}


void   ELCALLBACK pylink_image_title(INT16 threshold, char *title)
{
	if(PyErr_Occurred())PyErr_Print();
	PyObject_CallMethod(customDisplay,"image_title","s",title);
	if(PyErr_Occurred())PyErr_Print();
}

void   ELCALLBACK pylink_draw_image_line(INT16 width, INT16 line, INT16 totlines, byte *pixels)
{
	int i =0;
	PyObject * buffer =PyList_New(width);

	//printf("pylink_draw_image_line %d %x ", width, pixels);
	//fflush(NULL);
	for( i =0; i <width; i++)
	{
		PyObject *item = PyInt_FromLong((unsigned int)pixels[i]);
		if(PyErr_Occurred())PyErr_Print();
		if(!item)
		{
			printf("item is null \n");
			fflush(NULL);
			break;
		}
		PyList_SetItem(buffer,i, item);
		if(PyErr_Occurred())PyErr_Print();
		//Py_DECREF(item);
		if(PyErr_Occurred())PyErr_Print();
	}

	//printf("done %d \n", i);
	fflush(NULL);
	PyObject_CallMethod(customDisplay,"draw_image_line","iiiO",width,line,totlines,buffer);
	Py_DECREF(buffer);

	if(PyErr_Occurred())
		PyErr_Print();


}


void   ELCALLBACK pylink_set_image_palette(INT16 ncolors, byte r[], byte g[], byte b[])
{
	int i =0;
	PyObject * red   = PyList_New(ncolors);
	PyObject * green = PyList_New(ncolors);
	PyObject * blue  = PyList_New(ncolors);
	for(i =0; i < ncolors; i++)
	{
		PyObject *item =NULL;
		item = PyInt_FromLong(r[i]); PyList_SetItem(red,i,   item);// Py_DECREF(item);
		item = PyInt_FromLong(g[i]); PyList_SetItem(green,i, item);// Py_DECREF(item);
		item = PyInt_FromLong(b[i]); PyList_SetItem(blue,i,  item);// Py_DECREF(item);
	}
	if(PyErr_Occurred())PyErr_Print();
	PyObject_CallMethod(customDisplay,"set_image_palette","OOO",red,green,blue);
	if(PyErr_Occurred())PyErr_Print();
	Py_DECREF(red);
	Py_DECREF(green);
	Py_DECREF(blue);
	if(PyErr_Occurred())
		PyErr_Print();



}

void   ELCALLBACK pylink_exit_image_display(void)
{
	if(PyErr_Occurred())PyErr_Print();
	PyObject_CallMethod(customDisplay,"exit_image_display","");
	if(PyErr_Occurred())PyErr_Print();
}


void   ELCALLBACK pylink_clear_cal_display()
{
	if(PyErr_Occurred())PyErr_Print();
	PyObject_CallMethod(customDisplay,"clear_cal_display","");
	if(PyErr_Occurred())PyErr_Print();
}

void   ELCALLBACK pylink_erase_cal_target()
{
	if(PyErr_Occurred())PyErr_Print();
	PyObject_CallMethod(customDisplay,"erase_cal_target","");
	if(PyErr_Occurred())PyErr_Print();
}

void   ELCALLBACK pylink_draw_cal_target(INT16 x, INT16 y)
{
	if(PyErr_Occurred())PyErr_Print();
	PyObject_CallMethod(customDisplay,"draw_cal_target","ii",x,y);
	if(PyErr_Occurred())
			PyErr_Print();

}

#define CAL_ERR_BEEP   -1
#define DC_ERR_BEEP    -2
#define CAL_GOOD_BEEP   0
#define CAL_TARG_BEEP   1
#define DC_GOOD_BEEP    2
#define DC_TARG_BEEP    3


void   ELCALLBACK pylink_cal_target_beep(void)
{
	if(PyErr_Occurred())PyErr_Print();
	PyObject_CallMethod(customDisplay,"play_beep","i",DC_TARG_BEEP);
	if(PyErr_Occurred())PyErr_Print();
}

void   ELCALLBACK pylink_cal_done_beep(INT16 error)
{
	if(PyErr_Occurred())PyErr_Print();
	PyObject_CallMethod(customDisplay,"play_beep","i",error?CAL_ERR_BEEP:CAL_GOOD_BEEP);
	if(PyErr_Occurred())PyErr_Print();
}

void   ELCALLBACK pylink_dc_done_beep(INT16 error)
{
	if(PyErr_Occurred())PyErr_Print();
	PyObject_CallMethod(customDisplay,"play_beep","i",error?DC_ERR_BEEP:DC_GOOD_BEEP);
	if(PyErr_Occurred())PyErr_Print();
}


void   ELCALLBACK pylink_dc_target_beep(void)
{
	if(PyErr_Occurred())PyErr_Print();
	PyObject_CallMethod(customDisplay,"play_beep","i",DC_TARG_BEEP);
	if(PyErr_Occurred())PyErr_Print();
}

short  ELCALLBACK pylink_get_input_key(InputEvent * event)
{
	PyObject *rv = NULL;
	if(PyErr_Occurred())PyErr_Print();
	rv = PyObject_CallMethod(customDisplay,"get_input_key","");
	if(PyErr_Occurred())PyErr_Print();
	if(rv != Py_None && PyList_Check(rv) && PyList_Size(rv) >0)
	{
		int len = PyList_Size(rv);


		event->key.type = KEYINPUT_EVENT;
		event->key.state = 1; // for now
		event->key.key = PyInt_AsLong(PyObject_GetAttrString(PyList_GetItem(rv,len-1),"__key__"));
		event->key.modifier = PyInt_AsLong(PyObject_GetAttrString(PyList_GetItem(rv,len-1),"__mod__"));

		//printf("keys found %d  %d \n", len, event->key.modifier);
		//fflush(NULL);
		Py_DECREF(rv);
		return 1;
	}
	Py_DECREF(rv);
	return 0;
}


void   ELCALLBACK pylink_alert_printf(const char *msg)
{
	PyObject_CallMethod(customDisplay,"alert_printf","s",msg);
}





void drawLine(CrossHairInfo *chi, int x1, int y1, int x2, int y2, int colorindex)
{
	//printf("drawLine call %d %d %d %d \n", x1, y1, x2,y2);
	if(PyErr_Occurred())PyErr_Print();
	PyObject_CallMethod(chi->userdata,"draw_line","iiiii",x1,y1,x2,y2,colorindex);
	if(PyErr_Occurred())PyErr_Print();
}
void drawLozenge(CrossHairInfo *chi, int x1, int y1, int x2, int y2, int colorindex)
{
	if(PyErr_Occurred())PyErr_Print();
	PyObject_CallMethod(chi->userdata,"draw_lozenge","iiiii",x1,y1,x2,y2,colorindex);
	if(PyErr_Occurred())PyErr_Print();
}

void getMouseState(CrossHairInfo *chi, int *x, int *y, int *state)
{
	PyObject *rv = NULL;
	if(PyErr_Occurred())PyErr_Print();
	rv = PyObject_CallMethod(chi->userdata,"get_mouse_state","");
	if(rv)
	{
		int len = PyTuple_Size(rv);
		if(len ==2)
		{
			PyObject *pos =	PyTuple_GetItem(rv,0);
			PyObject *pstate =	PyTuple_GetItem(rv,1);
			*state = PyInt_AsLong(pstate);

			*x =	PyInt_AsLong(PyTuple_GetItem(pos,0));
			*y =	PyInt_AsLong(PyTuple_GetItem(pos,1));
		}
		Py_DECREF(rv);
	}
	if(PyErr_Occurred())PyErr_Print();
}



PyObject* eyelink_eyelink_request_cross_hair_draw(PyObject* self, PyObject* args)
{
	PyObject *eyeLinkCustomDisplay = NULL;
	if (PyArg_ParseTuple(args,"O:request_cross_hair_draw", &eyeLinkCustomDisplay))
	{
		CrossHairInfo crossHairInfo;
		PyObject *width = PyObject_GetAttrString(eyeLinkCustomDisplay,"__imgwidth__");
		PyObject *height = PyObject_GetAttrString(eyeLinkCustomDisplay,"__imgheight__");

		memset(&crossHairInfo,0,sizeof(crossHairInfo));

		if(PyErr_Occurred())PyErr_Print();
		crossHairInfo.w =PyInt_AsLong(width);
		if(PyErr_Occurred())PyErr_Print();
		crossHairInfo.h = PyInt_AsLong(height);
		if(PyErr_Occurred())PyErr_Print();
		crossHairInfo.drawLozenge = drawLozenge;
		crossHairInfo.drawLine = drawLine;
		crossHairInfo.getMouseState = getMouseState;
		crossHairInfo.userdata = eyeLinkCustomDisplay;
		eyelink_draw_cross_hair(&crossHairInfo);
		return Py_BuildValue("");
	}
	return NULL;
}