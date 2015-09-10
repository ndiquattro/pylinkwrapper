#ifndef _EYELINK_MODULE_H__
#define _EYELINK_MODULE_H__
typedef struct
{
    PyObject_HEAD
}
EyelinkTrackerObject;
#define GENERICFCNPROTO(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)
#define SHORTVOIDFCNPROTO(x) eyelink_eyelink_##x
#define SHORTVOIDFCNPROT1(x) eyelink_eyelinks_##x
char * getMultiByte(char *text,int len);
static PyObject * Eyelink__new__(PyTypeObject *type, PyObject *args, PyObject *kwds);
static void getTrackerModule();
static void			eyelink_dealloc(PyObject* self);
static PyObject *	eyelink_getattr(PyObject *obj, char *name);
static int			eyelink_setattr(PyObject *obj, char *name, PyObject *v);
#define MAPFCN(x,y)  { #x,##y##,METH_VARARGS,##x##_doc}
#define MAPFCNEX(x,y,z)  { ##x##,##y##,METH_VARARGS,z}

#define SHORTVOIDFCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	if (PyArg_ParseTuple(args,":"))\
	{\
		UINT16 rv = eyelink_##x##();\
		char *rvm = eyelink_get_error(rv, "eyelink_"#x);\
		if(!rvm || !strlen(rvm))\
			return Py_BuildValue("i",rv);\
		else \
			PyErr_Format(PyExc_RuntimeError, "%s", rvm);\
	}\
	return NULL;\
}

#define VOIDVOIDFCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	if (PyArg_ParseTuple(args,":"))\
	{\
		##x##();\
		return Py_BuildValue("");\
	}\
	return NULL;\
}

#define VOIDVOIDFCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	if (PyArg_ParseTuple(args,":"))\
	{\
		##x##();\
		return Py_BuildValue("");\
	}\
	return NULL;\
}


#define VOIDINT2FCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	if (PyArg_ParseTuple(args,":"))\
	{\
		INT16 rv = ##x##();\
		char *rvm = eyelink_get_error(rv, #x);\
		if(!rvm || !strlen(rvm))\
			return Py_BuildValue("i",rv);\
		else \
			PyErr_Format(PyExc_RuntimeError, "%s", rvm);\
	}\
	return NULL;\
}

#define VOIDUINT2FCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	if (PyArg_ParseTuple(args,":"))\
	{\
		UINT16 rv = ##x##();\
		return Py_BuildValue("i",rv);\
	}\
	return NULL;\
}
#define VOIDUINT4FCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	if (PyArg_ParseTuple(args,":"))\
	{\
		UINT16 unicode[2] ={0,0}; \
		UINT32 rv = ##x##(unicode);\
		unicode[1] =0;\
		if(rv)	{ return Py_BuildValue("(iis)",rv&0x0000ffff,rv>>16, unicode);}\
		return Py_BuildValue("O",Py_None);\
	}\
	return NULL;\
}
#define VOIDUINT32FCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	UINT32 arg = 0;\
	if (PyArg_ParseTuple(args,"i",&arg))\
	{\
		##x##(arg);\
		return Py_BuildValue("");\
	}\
	return NULL;\
}

#define VOIDUINT16FCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	UINT16 arg = 0;\
	if (PyArg_ParseTuple(args,"h",&arg))\
	{\
		##x##(arg);\
		return Py_BuildValue("");\
	}\
	return NULL;\
}


#define DOUBLEVOIDFCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	if (PyArg_ParseTuple(args,":"))\
	{\
		double rv = eyelink_##x##();\
		return Py_BuildValue("d",rv);\
	}\
	return NULL;\
}


#define DOUBLEVOID2FCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	if (PyArg_ParseTuple(args,":"))\
	{\
		double rv = ##x##();\
		return Py_BuildValue("d",rv);\
	}\
	return NULL;\
}

#define INTVOIDFCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	if (PyArg_ParseTuple(args,":"))\
	{\
		UINT32 rv = eyelink_##x##();\
		char *rvm = eyelink_get_error(rv, "eyelink_"#x);\
		if(!rvm || !strlen(rvm))\
			return Py_BuildValue("i",rv);\
		else \
			PyErr_Format(PyExc_RuntimeError, "%s", rvm);\
	}\
	return NULL;\
}

#define INTINTFCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	int arg =0;\
	if (PyArg_ParseTuple(args,"i", &arg))\
	{\
		int rv = eyelink_##x##(arg);\
		char *rvm = eyelink_get_error(rv, "eyelink_"#x);\
		if(!rvm || !strlen(rvm))\
			return Py_BuildValue("i",rv);\
		else \
			PyErr_Format(PyExc_RuntimeError, "%s", rvm);\
	}\
	return NULL;\
}

#define INTSTRFCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	char *arg =NULL;\
	if (PyArg_ParseTuple(args,"s", &arg))\
	{\
		int rv = ##x##(arg);\
		char *rvm = eyelink_get_error(rv, #x);\
		if(!rvm || !strlen(rvm) || rv == NO_REPLY)\
			return Py_BuildValue("i",rv);\
		else \
			PyErr_Format(PyExc_RuntimeError, "%s", rvm);\
	}\
	return NULL;\
}

#define INTSTRFCN_international(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args);
#define VOIDSTR2FCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	char *arg =NULL;\
	if (PyArg_ParseTuple(args,"s", &arg))\
	{\
		##x##(arg);\
		return Py_BuildValue("");\
	}\
	return NULL;\
}

#define VOID3STRFCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	char *arg1 =NULL;\
	char *arg2 =NULL;\
	char *arg3 =NULL;\
	if (PyArg_ParseTuple(args,"sss", &arg1,&arg2,&arg3))\
	{\
		##x##(arg1, arg2, arg3);\
		return Py_BuildValue("");\
	}\
	return NULL;\
}


#define INTINTINTFCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	INT16 arg1 =0;\
	INT16 arg2 =0; \
	if (PyArg_ParseTuple(args,"hh", &arg1, &arg2))\
	{\
		int rv = eyelink_##x##(arg1, arg2);\
		char *rvm = eyelink_get_error(rv, "eyelink_"#x);\
		if(!rvm || !strlen(rvm))\
			return Py_BuildValue("i",rv);\
		else \
			PyErr_Format(PyExc_RuntimeError, "%s", rvm);\
	}\
	return NULL;\
}

#define INT_INTINTINTFCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	INT16 arg1 =0;\
	INT16 arg2 =0; \
	INT16 arg3 =0;\
	if (PyArg_ParseTuple(args,"hhh", &arg1, &arg2, &arg3))\
	{\
		int rv = eyelink_##x##(arg1, arg2, arg3);\
		char *rvm = eyelink_get_error(rv, "eyelink_"#x);\
		if(!rvm || !strlen(rvm))\
			return Py_BuildValue("i",rv);\
		else \
			PyErr_Format(PyExc_RuntimeError, "%s", rvm);\
	}\
	return NULL;\
}

#define INT_4INTFCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	INT16 arg1 =0;\
	INT16 arg2 =0;\
	INT16 arg3 =0;\
	INT16 arg4 =0;\
	if (PyArg_ParseTuple(args,"hhhh", &arg1, &arg2, &arg3, &arg4))\
	{\
		int rv = ##x##(arg1, arg2, arg3, arg4);\
		char *rvm = eyelink_get_error(rv, #x);\
		if(!rvm || !strlen(rvm))\
			return Py_BuildValue("i",rv);\
		else \
			PyErr_Format(PyExc_RuntimeError, "%s", rvm);\
	}\
	return NULL;\
}

#define INT_4INTFCN_NOERROR(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	INT16 arg1 =0;\
	INT16 arg2 =0;\
	INT16 arg3 =0;\
	INT16 arg4 =0;\
	if (PyArg_ParseTuple(args,"hhhh", &arg1, &arg2, &arg3, &arg4))\
	{\
		int rv = ##x##(arg1, arg2, arg3, arg4);\
		return Py_BuildValue("i",rv);\
	}\
	return NULL;\
}



#define INT16INTFCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	UINT16 arg =0;\
	if (PyArg_ParseTuple(args,"h", &arg))\
	{\
		int rv = eyelink_##x##(arg);\
		char *rvm = eyelink_get_error(rv, "eyelink_"#x);\
		if(!rvm || !strlen(rvm))\
			return Py_BuildValue("i",rv);\
		else \
			PyErr_Format(PyExc_RuntimeError, "%s", rvm);\
	}\
	return NULL;\
}

#define SHORTSTRFCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	if (PyArg_ParseTuple(args,":"))\
	{\
		char * str = NULL;\
		UINT16 rv = eyelink_##x##(str);\
		char *rvm = eyelink_get_error(rv, "eyelink_"#x);\
		if(!rvm || !strlen(rvm))\
			return Py_BuildValue("i",rv);\
		else \
			PyErr_Format(PyExc_RuntimeError, "%s", rvm);\
	}\
	return NULL;\
}

#define RETURNSTRFCN(x) static PyObject* eyelink_eyelinks_##x##(PyObject* self, PyObject* args)\
{\
	if (PyArg_ParseTuple(args,":"))\
	{\
		char str[1024];\
		memset(str,0,sizeof(str));\
		eyelink_##x##(str);\
		return Py_BuildValue("s", str);\
	}\
	return NULL;\
}


#define SHORTVOIDPTRFCN(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	if (PyArg_ParseTuple(args,":"))\
	{\
		void *data = NULL;\
		UINT16 rv = eyelink_##x##(data);\
		char *rvm = eyelink_get_error(rv, "eyelink_"#x);\
		if(!rvm || !strlen(rvm))\
			return Py_BuildValue("i",rv);\
		else \
			PyErr_Format(PyExc_RuntimeError, "%s", rvm);\
	}\
	return NULL;\
}

#define INTSTRFCN_MAX_CHECK(x) static PyObject* eyelink_eyelink_##x##(PyObject* self, PyObject* args)\
{\
	char *arg =NULL;\
	if (PyArg_ParseTuple(args,"s", &arg))\
	{\
		char *rvm=NULL;\
		int rv = 0;\
		if(strlen(arg) > 129)\
		{\
			PyErr_Format(PyExc_RuntimeError, "Command too long: %s", arg);\
			return NULL;\
		}\
		rv = ##x##(arg);\
		rvm = eyelink_get_error(rv, #x);\
		if(!rvm || !strlen(rvm) || rv == NO_REPLY)\
			return Py_BuildValue("i",rv);\
		else \
			PyErr_Format(PyExc_RuntimeError, "%s", rvm);\
	}\
	return NULL;\
}
#endif
