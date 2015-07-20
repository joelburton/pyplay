/** @file
 *
 * @brief Bigram Python type..
 * *
 * This is both a test case and a Python library for bigram data structures.
 */


#include <Python.h>
#include <structmember.h>

/** Bigram is a structure of two words. */

typedef struct {
    PyObject_HEAD
    PyObject *word1;  ///< first word
    PyObject *word2;  ///< second word
} Bigram;



/** Boilerplate method to provide access to subobjects for GC. */

static int Bigram_traverse(Bigram *self, visitproc visit, void *arg)
{
    Py_VISIT(self->word1);
    Py_VISIT(self->word2);
    return 0;
}


/** Boilerplate method to free contents of Bigram. */

static int Bigram_clear(Bigram *self)
{
    PyObject *tmp;

    tmp = self->word1;
    self->word1 = NULL;
    Py_XDECREF(tmp);

    tmp = self->word2;
    self->word2 = NULL;
    Py_XDECREF(tmp);

    return 0;
}


/** Free Bigram (and it's members) */

static void Bigram_dealloc(Bigram *self)
{
    Bigram_clear(self);
    Py_TYPE(self)->tp_free((PyObject *) self);
}


/** Create a Bigram (used by Python's "new" method) */

static PyObject *Bigram_new(PyTypeObject *type,
                            PyObject  __unused *args,
                            PyObject  __unused *kwds)
{
    Bigram *self;

    self = (Bigram *) type->tp_alloc(type, 0);
    if (self != NULL) {

        self->word1 = PyUnicode_FromString("");
        if (self->word1 == NULL) {
            Py_DECREF(self);
            return NULL;
        }
        self->word2 = PyUnicode_FromString("");
        if (self->word2 == NULL) {
            Py_DECREF(self);
            return NULL;
        }
    }
    return (PyObject *) self;
}


/** Initialize a Bigram.
 *
 * The Bigram() class takes two strings, for word1 and word2.
 * Both are required.
 */

static int Bigram_init(Bigram *self,
                       PyObject *args,
                       PyObject __unused *kwds)
{
    PyObject *word1 = NULL;
    PyObject *word2 = NULL;
    PyObject *tmp;

    if (!PyArg_ParseTuple(args, "OO", &word1, &word2))
        return -1;

    if (!PyUnicode_Check(word1)) {
        PyErr_SetString(PyExc_TypeError, "Attribute 'word1' must be a string");
        return -1;
    }

    if (!PyUnicode_Check(word2)) {
        PyErr_SetString(PyExc_TypeError, "Attribute 'word2' must be a string");
        return -1;
    }

    tmp = self->word1;
    Py_INCREF(word1);
    self->word1 = word1;
    Py_DECREF(tmp);

    tmp = self->word2;
    Py_INCREF(word2);
    self->word2 = word2;
    Py_DECREF(tmp);

    return 0;
}


/** Hash a bigram.
 *
 * In order to store bigrams in dictionaries, we need a hash that
 * finds that bigrams with the same words hash the same.
 *
 * We do this by doing a bitwise XOR of the two individual hashes.
 */

static Py_hash_t Bigram_hash(Bigram *self)
{
    return Py_TYPE(self->word1)->tp_hash(self->word1) ^
           Py_TYPE(self->word2)->tp_hash(self->word2);
}


/** Compare two bigrams.
 *
 * In order to store bigrams in dictionaries, we need to make sure
 * that two individual bigrams with the same words are compared
 * as being equal.
 *
 * We don't implemented <, <= etc.
 */

static PyObject *Bigram_richcompare(Bigram *self, Bigram *other, int op)
{
    if (op == Py_EQ) {
        if ((!PyUnicode_Compare(self->word1, other->word1)) &&
            (!PyUnicode_Compare(self->word2, other->word2))) {
            Py_INCREF(Py_True);
            return Py_True;
        } else {
            Py_INCREF(Py_False);
            return Py_False;
        }
    }

    if (op == Py_NE) {
        if ((PyUnicode_Compare(self->word1, other->word1)) ||
            (PyUnicode_Compare(self->word2, other->word2))) {
            Py_INCREF(Py_True);
            return Py_True;
        } else {
            Py_INCREF(Py_False);
            return Py_False;
        }
    }

    // All other comparisons aren't implemented
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
}


/** Return bigram phrase as two space-separated words. */

static PyObject *Bigram_phrase(Bigram *self)
{
    return PyUnicode_FromFormat("%S %S", self->word1, self->word2);
}


/** List of bigram methods */

static PyMethodDef Bigram_methods[] = {
        {"phrase", (PyCFunction) Bigram_phrase, METH_NOARGS, "Phrase"},
        {NULL}
};


/** Getter for bigram word1 */

static PyObject *Bigram_getword1(Bigram *self, void __unused *closure)
{
    Py_INCREF(self->word1);
    return self->word1;

}


/** Getter for bigram word2. */

static PyObject *Bigram_getword2(Bigram *self, void __unused *closure)
{
    Py_INCREF(self->word2);
    return self->word2;

}


/** List of bigram getters 8 setters */

static PyGetSetDef Bigram_getseters[] = {
        {"word1", (getter) Bigram_getword1, NULL, "word 1", NULL},
        {"word2", (getter) Bigram_getword2, NULL, "word 2", NULL},
};


/** Definition of Bigram type. */

static PyTypeObject BigramType = {
        PyVarObject_HEAD_INIT(NULL, 0)
        "bigram.Bigram",                                // tp_name
        sizeof(Bigram),                                 // tp_basicsize 
        0,                                              // tp_itemsize 
        (destructor) Bigram_dealloc,                    // tp_dealloc 
        0,                                              // tp_print 
        0,                                              // tp_getattr 
        0,                                              // tp_setattr 
        0,                                              // tp_reserved 
        (reprfunc) Bigram_phrase,                       // tp_repr 
        0,                                              // tp_as_number 
        0,                                              // tp_as_sequence 
        0,                                              // tp_as_mapping 
        (hashfunc) Bigram_hash,                         // tp_hash  
        0,                                              // tp_call 
        (reprfunc) Bigram_phrase,                       // tp_str 
        0,                                              // tp_getattro 
        0,                                              // tp_setattro 
        0,                                              // tp_as_buffer 
        Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE |      // tp_flags
            Py_TPFLAGS_HAVE_GC,
        "Two-word structure",                           // tp_doc 
        (traverseproc) Bigram_traverse,                 // tp_traverse 
        (inquiry) Bigram_clear,                         // tp_clear
        (richcmpfunc) Bigram_richcompare,               // tp_richcompare 
        0,                                              // tp_weaklistoffset 
        0,                                              // tp_iter 
        0,                                              // tp_iternext 
        Bigram_methods,                                 // tp_methods 
        0,                                              // tp_members 
        Bigram_getseters,                               // tp_getset 
        0,                                              // tp_base 
        0,                                              // tp_dict 
        0,                                              // tp_descr_get 
        0,                                              // tp_descr_set 
        0,                                              // tp_dictoffset 
        (initproc) Bigram_init,                         // tp_init 
        0,                                              // tp_alloc 
        Bigram_new,                                     // tp_new 
};


/** bigram module structure. */

static PyModuleDef bigrammodule = {
        PyModuleDef_HEAD_INIT,
        "bigram",
        "Bigrams",
        -1,
        NULL, NULL, NULL, NULL, NULL
};


/** Initializer for bigram module. */

PyMODINIT_FUNC __unused  PyInit_bigram(void)
{
    PyObject *m;

    if (PyType_Ready(&BigramType) < 0)
        return NULL;

    m = PyModule_Create(&bigrammodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&BigramType);
    PyModule_AddObject(m, "Bigram", (PyObject *) &BigramType);
    return m;
}


/** Main function: embeds a Python interpreter and runs test module. */

#ifdef DEBUG

int main(int argc, char *argv[])
{
    // Add a built-in module, before PyInitialize
    if (PyImport_AppendInittab("bigram", PyInit_bigram) != 0) {
        printf("Can't add bigram\n");
        return 1;
    }

    // Pass argv[0] to Python interpreter
    Py_SetProgramName((wchar_t *) argv[0]);

    // Initialize Py interpreter
    Py_Initialize();

    int result = PyRun_SimpleString(
            "from bigram import Bigram;"
                    "b = Bigram('a', 'b');"
                    "print(b)");
    Py_Finalize();

    return result;
}

#endif