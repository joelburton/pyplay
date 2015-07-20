/** @file
 *
 * @brief Spam python module.
 *
 * This is both an executable C program and a 'spam' module for importing
 * into Python. Whee!
 *
 */

#include <Python.h>


/** SpamError exception class. */

static PyObject *SpamError;


/** system() function for Python.
 *
 * @param self: ignored
 * @param args: arguments (takes a single string of command to run)
 *
 * @return (long) result code
 */

static PyObject *spam_system(PyObject __unused *self, PyObject *args)
{
    const char *command;
    int sts;

    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;

    sts = system(command);
    if (sts != 0) {
        PyErr_SetString(SpamError, "Gah");
        return NULL;
    }
    return PyLong_FromLong(sts);
}


/** Python function times(): perform callback multiple times
 *
 * @param self: ignored
 * @param args: arguments (integer number of times, callback callable)
 *
 * Calls the callback, providing the iteration variable as both as
 * the first argument to the callback and also as a keyword argument 'count'.
 *
 * @return None
 */

static PyObject *times(PyObject __unused *self, PyObject *args)
{
    unsigned int ntimes = 0;
    PyObject *callback;
    PyObject *result;

    if (!PyArg_ParseTuple(args, "IO:times", &ntimes, &callback))
        return NULL;

    if (!PyCallable_Check(callback)) {
        PyErr_SetString(PyExc_TypeError, "parameter must be callable");
        Py_DECREF(callback);
        return NULL;
    }

    Py_INCREF(callback);
    for (int i = 0; i < ntimes; i++) {
        PyObject *arglist = Py_BuildValue("(i)", i);
        PyObject *kwarglist = Py_BuildValue("{s:i}", "count", i);
        result = PyObject_Call(callback, arglist, kwarglist);
        Py_DECREF(arglist);
        Py_DECREF(kwarglist);
        if (result == NULL)
            return NULL;
        Py_DECREF(result);
    }
    Py_DECREF(callback);

    Py_INCREF(Py_None);
    return Py_None;
}


/** List of methods for the 'spam' module */

static PyMethodDef SpamMethods[] = {
        {"system", spam_system, METH_VARARGS, "Execute a shell command"},
        {"times",  times,       METH_VARARGS, "Do something a certain number of times"},
        {NULL, NULL, 0, NULL}   // sentinel that we're done
};


/** Definition for the spam module */

static struct PyModuleDef spammodule = {
        PyModuleDef_HEAD_INIT,
        "spam",    // name of module
        "Hello?",  // module documentation or NULL
        -1,        // size of per-interpreter state of module or -1 for state in global
        SpamMethods
};


/** Initializer for the spam module.
 *
 * Adds an exception type for SpamError.
 */

PyMODINIT_FUNC PyInit_spam(void)
{
    PyObject *m = PyModule_Create(&spammodule);
    if (m == NULL)
        return NULL;
    SpamError = PyErr_NewExceptionWithDoc("spam.SpamError", "Doc", NULL, NULL);
    Py_INCREF(SpamError);
    PyModule_AddObject(m, "SpamError", SpamError);
    return m;
}



/** Main function: embeds a Python interpreter and runs test module. */

#ifdef DEBUG

int main(int argc, char *argv[])
{
    FILE *f = fopen("/Users/joel/ClionProjects/pyplay/test.py", "r");
    if (!f) {
        printf("ACK: %s", strerror(errno));
        exit(1);
    }

    // Add a built-in module, before PyInitialize
    if (PyImport_AppendInittab("spam", PyInit_spam) != 0) {
        printf("Can't add spam\n");
        return 1;
    }

    // Pass argv[0] to Python interpreter
    Py_SetProgramName((wchar_t *) argv[0]);

    // Initialize Py interpreter
    Py_Initialize();

    // Optionally, import module, if we wanted to use it ourselves
    // PyObject *spammodule = PyImport_ImportModule("spam");
    // ...
    // Py_DECREF(spammodule);

    int result = PyRun_SimpleFile(f, "test");
    fclose(f);
    Py_Finalize();

    return result;
}

#endif