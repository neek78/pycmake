pycmake
*******
.. contents:: Table of Contents
.. section-numbering::

Disclaimer
==========

This is an experimental project to bind the Python interpreter into the CMake environment. This is in no way an "official" CMake project, anything like that. It's offered here in the hope that it is useful, etc.

You should most certainly NOT use with any production code, or really any code that you need to compile reliably. The system is far from thoroughly tested. 

There is no interface stability at the moment; the entire thing might be redesigned. There will almost certainly be breaking changes in the near future. 

This is very much my first project integrating C++ and Python (and thus also using pybind11) so things certainly may not be implemented the ideal way. Feedback is more than welcome.

Overview
========

pycmake is built on top of the current CMake master branch, with as few changes to the existing code as possible. It adds a Python interpreter as an alternative (not a replacement) to the built-in language, via the extreme niftiness of pybind11. 

  * It offers a relatively straightforward transition from CMake script to Python, with roughly the same interface available. Python and CMake script can mostly co-exist. Python functions can be presented to- and called by CMake script, and vice-versa. 
  * Some degree of automatic type conversion between Python types and CMake's "type system".
  * It uses the standard CPython from the system (via pybind11) and doesn't place many restrictions on what you can do. 
  * Some internal CMake state is presented as more python-friendly structures (eg the cache, variables, directories and targets). Note that these interfaces in particular are highly experimental at this stage.
  * Should generally be usable with standard Python development and debugging tools.   

Rationale
=========
CMake has become the MS-DOS of our time. It provides a useful utility, but the reason for its ubiquity mostly seems to be its ubiquity. Also lots of it is in CAPITAL LETTERS for some reason.

Despite its faults, as a common interface between libraries it is really valuable. Additionally, it provides an incredible amount of functionality out of the box, which is a massive timesaver, especially in cross-platform projects. A large number of people understand it. Using CMake as your (meta) build system is the safe option. 

It's becoming commonplace to expect a c/c++ library to come with at least a CMake build script. In fact, certain guides advise you to evangelically raise a bug with project maintainers if their project does not come with a CMake script. Make of that what you will.

Despite the value in the system, CMake's language has always frustrated me; I don't think I am alone in this. Politely, the language and the internal data model could be described as "opaque"; once you get beyond a very simple setup, it quickly becomes very cumbersome. It seems to go out of its way to avoid having a type system or avoid telling you that you've made an error. This flies in the face of working with a C++ codebase where the environment is tuned towards picking up errors at compile time. The language lacks many of the features that languages, such as Python, had in the year 2000 when the first CMake version was released.

Python was chosen for the following reasons:
    * It exists.
    * It is widely adopted and works on a wide variety of systems. 
    * It has useful builtin types, a solid and comprehensive standard library and modern features like return values.
    * It has over 30 years of development history and real-world use and problem solving.
    * There is stable process for changing / deprecating language and library features.
    * There is a large, existing set of people who already know the language, and tools to develop/debug with it. 
    * It has strong "reflection" features, allowing for easier exploration of the data model.
    * I didn't want to learn just enough of yet another special-purpose language to get by. 

Building pycmake
================

pycmake is built in much the same way as regular cmake. It adds pybind11/Python integration in the standard recommended way - that is, pybind11 is included as a git submodule, and pybind11's build logic is used for locating Python.

See the original README <README.original.rst> for more details on this. 

If you didn't clone the repo with --recursive, in your source dir, run -

``git submodule update --init``

Then run cmake and build as normal.

It is possible to build pycmake without Python, thus making it pretty-much standard cmake. Keep an eye on this in case pybind11 is not located correctly. In any case, the resulting binary is still named "cmake".

If Python is not being found, make sure that python/python.exe is in your path. If the scripts can find this, they can usually work out where everything else is. Also beware of stale data; When in doubt, start with a clean build dir / CMakeCache.txt. I had the ghost of a stale Python install location haunting me for a while. 

``cmake --python-information`` will show if Python support is compiled into a given binary.

Googletest is used for c++ unit testing; These tests will be enabled if gtest is discovered during configuration.

Getting Started
===============

CMakeLists.txt and cmake_build.py
---------------------------------

In the world of CMake, the directory is king. When a new directory is entered by pycmake (``add_subdirectory()``), it will look for the traditional CMakeLists.txt, or the new cmake_build.py (the former is taken by preference at the moment). This same logic is applied at the root of the source tree. 

In general, each directory's CMake scripting should be implemented in only one of the two languages; It's best to avoid mixing them in the same directory. That being said, there are various mechanisms to allow interoperability. 

Across different directories, you can use whichever language you prefer. Most entities in CMake should be visible to both languages fairly consistently. 

Entrypoints
-----------
pycmake will load your python script from cmake_build.py in a given directory, parse it, and then call a supplied function named ``cmake_main()``. 

An example -
::
  import cmake
  
  def cmake_main(*, cm_dir, cm_global):
    cm_global.message("CMAKE VERSION ", str(cm_global.cmake_version))
    cm_global.message("CMAKE_ROOT is ", cm_dir.combined_var['CMAKE_ROOT'])

    cm_global.cmake_minimum_required(cmake.CMakeVersion(3, 26))

    cm_dir.project("simpleCpp")

    cm_dir.add_executable("simpleCpp", "simpleCpp.cpp")

cm_dir and cm_global must be typed as named parameters. In general they are optional, but you wont get very far without them.

The two provided objects give access into CMake's interfaces. The reason for the separation is that many CMake commands and entities have an implicit directory parameter (somewhat like a ``this``/``self`` parameter). Anything that doesn't have this implicit parameter should be under cm_global. 

The code executes similarly to regular CMake code; As commands are executed, internal state is updated. When code execution is completed, the regular generation process executes, and build-system files are outputted.

Variable expansion is performed in parameters in the normal CMake way - that is, the following works from Python as you might expect -  ``cm_global.message("c++ compiler ${CMAKE_CXX_COMPILER}")``.

Raw Interface
-------------

The Raw interface is where CMake's C++ code is bound for access by Python scripts. Almost all CMake commands are available via the raw interface. Everything except commands that really don't make sense under Python should be there. Commands excluded include flow control commands (like ``if()``, ``while()``, etc) and long-deprecated commands (like ``add_command()``).

The raw interface object is bound into both ``cm_dir.raw`` and ``cm_global.raw``, and contains simple bindings to every command. The Python interface knows nothing about the nature of each command. Every command simply takes an arbitrary number of positional parameters, and will pass these back to the CMake implementation of each command (incidentally, this is also how CMake script works). 

Calls to raw functions won't generate return values, but you should get an exception if CMake declares an error. 

A simple example of calling into the raw interface - 
:: 
  cm_dir.raw.project("simpleCpp")
  cm_dir.raw.add_executable("simpleCpp", "simpleCpp.cpp")

In many cases, a nicer interface is provided in the Native Command interface (see below), but it is not yet complete.

Native Command Interface
------------------------
Some commands have a more Python-friendly interface, implemented directly in cm_dir and cm_global. In most cases, the command names match the native cmake. However, sometimes it made more sense to break one interface up into multiple functions (for example ``add_library``, ``add_library_imported``, etc). These are implemented as Python wrappers around the raw function interface.

Commands whose use is discouraged these days will show up in cm_dir.legacy and cm_global.legacy - for example ``cm_dir.legacy.include_dirs()``

At present, a number of commands are implemented natively - See cm_dir.py and cm_global.py. There is no real documentation outside of this file, but all commands are simple wrappers around their CMake script counterparts. 

The best documentation currently are the tests in Tests/Py* and PyModules/test/*

Please note also that the integration tests are far from complete, so it's possible there are numerous bugs in the python wrappers at this stage.

Some notes: 

* ``find_package()`` - The find_package wrapper will attempt to identify vars and targets created when calling find_package scripts, and return these in a dictionary. This is a bit of a hack to determine what a script is doing rather than a solid interface. In particular, it wont detect cache values touched, as there is no clear way from Python currently to see which cache values have been touched vs which were already set in the cache from a previous run. This is obvious in scripts like FindBoost.cmake which set cache values rather than creating targets. 

* ``include()`` - counterintuitively, include() works, and is available from Python. See the interoperability section for more details. The RESULT_VARIABLE interface instead works as a return value.

In some cases, a more natural replacement command exists in other places. For example, the set()/unset() commands exist in ``cm_dir.var`` and ``cm_global.cache``.

CMake Python Module
-------------------
The outer-layer interface is implemented as a Python module. This should be located at ``${CMAKE_ROOT}/PyModules/cmake``. This should be in parallel to wherever the normal CMake Modules directory is installed. Much of the interface code is found in ``cm_dir.py`` and ``cm_global.py``.

At the moment this is the quickest way to understand the interface as the documentation is somewhat lacking. 

When run under CMake, the Python/C++ interface will be injected into cmake._builtin, and then populated throughout the module. 

Accessing CMake Variables
-------------------------

There are three views into CMake's variable system - ``cm_dir.combined_var``, ``cm_dir.vars``, and ``cm_global.cache``. All three implement a Python dictionary interface, so the familiar keys(), values() and items() functions are there, along with subscript access ([]). 

Please note that you don't have to use CMake's variables outside of interacting with CMake itself or CMake script - That is, use native Python variables and function calls for your own purposes. 

As it is very common to use ``if(FOO)`` in CMake, a shorthand exists as ``cm_dir.is_set("FOO")`` -  this is equivalent to ``"FOO" in cm_dir.combined_var``.

combined_var
------------

``cm_dir.combined_var`` provides a read-only view identical to that used when applying the default CMake variable expansion algorithm. That is, ``cm_dir.combined_var["FOO"]`` should give the same result as ``"${FOO}"`` in CMake script. This means a combined view of both variables and cache values. 

``combined_var.expand()`` will run CMake's normal variable expansion on an expression - for example ``combined_var.expand("${FOO}/${BAR}/${BAZ}")``. This version will raise an exception when variables can't be expanded (etc). 

``combined_var.expand_relaxed()`` will do the same, but quietly allow expansion failures - matching the default cmake behaviour.

var
---
``cm_dir.var`` provides access to CMake variables only (that is, not cache values). This is a read/write interface and exists from the context of the current variable scope (typically this means the current directory). 

``cm_dir.set()`` and ``.unset()`` work similarly to the regular set() and unset() commands, though options relating to the cache are not available, and access to environment variables is not available. Read/write dictionary-style access is also possible. 

``cm_dir.push_to_parent_scope()`` promotes an existing variable to the parent scope.

cache
-----

``cm_global.cache`` provides access to CMake's cache. Unlike variables, the cache exists globally - not in the current scope. Similarly to var, this is a read/write interface. Both ``set()`` and ``unset()`` functions are available, along with dictionary-style access. 

Unlike ``var`` and ``combined_var``, the cache returns cache value objects. There are a number of properties on a per-entry basis available from this object. The actual stored value is available from ``entity.value``.

dir_attr
--------

``cm_dir.dir_attr`` provides visibility into the current dir's CMake entities in a Python friendly way, such as targets, source and build dirs, etc. Currently this is read only, but replaces having to call many of the get_ functions. This interface in particular is subject to a lot of potential change, as it's pretty experimental right now.  

Type Conversion
---------------

pycmake will attempt to automatically convert types between CMake and Python. This is hindered by CMake's very basic and context-dependant type system. This is subject to change as it might be more trouble than it's worth. 

Generally, strings, paths, bools, ints, floats and sequences (lists/sets/dict_keys/dict_values) should automatically convert between environments where possible. Cached values have a little more type info, so can convert slightly better. More complex behaviours are not clearly defined right now (eg passing None or nested lists or dictionaries). Also things like CMake's "magic" (really, afterthought implementation) strings such as "NOTFOUND" and its "internally on" state are not properly handled at present.

Interoperability between Python and CMake script
================================================

You can have a mix of Python and CMake script in your project, and interoperability is supported to some degree. 

Adding subdirectories
---------------------
The add_subdirectory() command works as normal from both CMake script and Python - the subdirectory can have code in either CMake script or Python regardless of the current directory's language choice.

Calling Python functions from CMake script
------------------------------------------

Python functions can be registered to be called from CMake script using the ``cm_dir.py_functions`` interface.
::
  def some_function(msg):
    print ("some_function ", msg)
        
  def cmake_main(*, cm_dir, cm_global):
    cm_dir.py_functions.add(some_function)


In the above example, cmake_main() executes during the initial run of the cmake_build.py script, registering some_function() to be called from CMake script (or if you're feeling masochistic, from other Python code via CMake). It can be called from CMake script just any other user function -
::
  some_function("hello python")


For the further examples, we'll omit the registration section; But this is always required - Python functions are not automatically visible from CMake script without registration.

The above example can be used to (say) write a file or process and return some values. But you need to declare the standard two objects to call back into CMake.

Listing Functions
^^^^^^^^^^^^^^^^^

cm_dir.py_functions implements a dictionary interface, so you can use this to enumerate already registered functions.

Accessing CMake 
^^^^^^^^^^^^^^^

Getting the standard interface objects is done the same way as in cmake_main() -
::
  def another_function(msg, *, cm_dir, cm_global):
    cm_global.message("another_function ", msg)

Adding the cm_dir and cm_global arguments - they must be keyword arguments (not positional) - will give you access back to CMake in the normal way. They're both optional, so you can declare only one if that's all you need. 

At present, there's no way to create other named arguments when calling from CMake. 

Positional Arguments
^^^^^^^^^^^^^^^^^^^^

You can create formal positional arguments in the normal way in the Python function, and this interface will be enforced at runtime. 
::
  def three_spring_rolls(p1, p2, p3, *, cm_dir, cm_global):
    cm_global.message("msg'd ", p1, p2, p3)


This function can then just be called from CMake script as normal -
::
  three_spring_rolls("cheerleader", "so and "so", "what's her face")
  
  
You can also create informal positional arguments in the normal way -
::
  def yet_another_function(p1, *args, cm_dir, cm_global):
    cm_global.message("yep ", p1, args)

Exceptions
^^^^^^^^^^

If you want to stop execution, you can raise an exception from Python code; There's presently no way to catch this in CMake script, so it'll be regarded as a fatal error and stop execution.

You can also use ``cm_global.message('...', level=MsgLevel.FATAL_ERROR)`` in the normal CMake way.

Return Values
^^^^^^^^^^^^^

CMake scripts don't have any (real) notion of a return value from a function. If you want to return something, you need to write it into the variable scope of the caller. Typically, this is done by passing in the name of the variable to write the result(s) into. Yes, it really is 2023.

Python functions operate similarly to CMake functions (rather than macros), in that they have their own variable scope. If you want to return something to the caller, you have to push it to the parent scope. 
::
  def complex_mathematics(p1, ret1, *, cm_dir, cm_global):
    v = p1 * 7
    cm_dir.var.set_parent_scope(ret1, v)

pycmake doesn't change this functionality, however it automates this process somewhat. 

You can use Python's annotation feature to mark arguments as containing the name of a variable into which a return value should be written, and pycmake will automatically handle processing return values into these vars. 
::
  def two_return_values(p1, ret1 : cmake.ReturnType, ret2 : cmake.ReturnType, *, cm_dir):
    return "cats", "dogs"
    	
If now called from cmake ...
:: 
  two_return_values("foo", A1, A2)
	
... A1 and A2 will now contain "cats" and "dogs" respectively.

See Tests/PyFunctionFromCMakeTest for more examples.

Calling CMake functions from Python
-----------------------------------

Functions created with CMake script's ``function()`` are visible to Python code via ``cm_dir.functions``. This object will contain a method for each registered function. 
::
  function(return_input INPARAM RETPARAM)
    set("${RETPARAM}" "${INPARAM}" PARENT_SCOPE)
  endfunction()

This can now be called from Python using -
::
  cm_dir.functions.return_input("foo", "bar")
  b = cm_dir.var["bar"]
  
You can call both CMake user functions and macros from Python via this interface (in fact, also Python functions registered for CMake scripts too) - they exist in the same namespace. There's a bit of a misconception that CMake macros work like c/c++'s #define, and logically shouldn't work by calling them from Python; This is not really true. Macros and functions are very similar in CMake, the former just don't have their own variable scope, and thus any variables they set are left in the current scope without any cleanup. The 'return value' mechanism works the same way, just without needing the caller to write into the parent namespace. It's wild needing to understand the mechanics of multiple calling conventions in an interpreted script language. 

Listing Functions
^^^^^^^^^^^^^^^^^

cm_dir.functions implements a dictionary interface, so you can use this to enumerate available functions.

Return Values
^^^^^^^^^^^^^

Similarly to calling Python from CMake, some automation is provided to handle return values automatically without changing the underlying mechanism. 

The previous example's Python code can be modified to - 
::
  b = cm_dir.functions.return_input("foo", cmake.ReturnParam())

In this case, a unique variable name is devised, the return value extracted from the appropriate scope, cleaned up and returned. An exception is thrown if the CMake code does not set a value. 
  
All of these options are controllable - See Tests/PyCMakeFunctionFromPythonTest for more examples.

Calling CMake kwargs functions
------------------------------

CMake script provides a rudimentary keyword args mechanism, using `cmake_parse_arguments() <https://cmake.org/cmake/help/latest/command/cmake_parse_arguments.html>`_ . This is an ancillary function that parses the existing formal and/or informal arguments; it doesn't change the actual calling mechanism. It is a built-in function now (ie implemented in CMake's C++ code), though it was previously implemented as a CMake script function.

It takes three kinds of named arguments - Keyword, One Value and Multi Value arguments. These are automatically rendered by calling using Python named arguments. 

One-value and multi-value arguments can be generated from python using normal name="value", and name=['value1', 'value2'] arguments.

Plain keyword arguments are specified in CMake by including the keyword to represent true or on, and omitting the keyword to represent false. This is handled by passing in a ``cmake.KwArg`` object. 

This example shows all three parameter types, as well as regular positional arguments -
::
  self.cm_dir.functions.kw_func1(
    "positional1", "positional2", 7, True, 
    B1=cmake.KwArg(),                   # KW arg - B1 is true
    B2=cmake.KwArg(True),               # B2 also true
    B3=cmake.KwArg(False),              # B3 is false, as is B4
    OV1="Cats",                         # One-value arg
    MV1=[1,2,3], MV2=["Dogs", True, 7]) # multi-value args

See Tests/PyKwArgsTest for a working example. 

The include() command
---------------------

Perversely, the include() command works from Python and lets you "include" CMake script into Python code. I'd discourage its use as it's a bit odd to wrap your head around, but it does work. 

Firstly a little background. You can skip this section of course, but it is useful to understand what's going on internally. 

There's a bit of a misconception that "include" in CMake is like c/c++'s #include, including in the CMake source code itself which includes the comment "In almost every sense, this is identical to a C/C++ #include command". This statement is wrong in almost every sense. 

CMake's include command is much closer to eval() or exec() functions in other scripting languages. CMake doesn't really build an abstract syntax tree or have any notion of a preprocessor. When a CMake script is loaded, it's parsed down into a set of commands - including things like flow control statements like if(), while() etc. At runtime, each command is looked up, the parameters are expanded (ie variable references replaced with their current value), and the command is executed with these expanded string parameters.  For a normal command, when execution is done, the next command in the list is executed. For a flow-control command, the "program counter" might be modified to control which is the next command executed. When the body of a function is encountered, the commands are recorded, but not executed until the function is called. That's it really.

When you call ``add_subdirectory()``, a bunch of new state and a scope is created, and the process repeats recursively. Entities like functions are registered globally, so the are visible everywhere once they're interpreted from their source script. The ``include`` function works the same way, except without creating a new dir's state or scope. So the side effects of executing the included script are reflected in the current dir scope - functionally, like it was included in the original text, hence the comparison with #include.

It's worth noting that this is largely how Python scripts executed in CMake work. From the CMake core's point of view, it's just seeing a bunch of command invocations coming from the Python interpreter. It doesn't know the difference between that and CMake script doing the calls. SO when you call include() from Python, it invokes the built-in interpreter, and it runs the CMake script in the context and scope of the Python script's directory. Whatever side effects are caused by the CMake script, like creating targets, functions or other entities will be visible to Python via the CMake interface after execution is completed.

Other interoperability notes
----------------------------

You don't need to use the built in FILE or MATH commands (for example) - Python's standard library should always be available to access instead. Nor do you have to store your data in CMake vars if it's not going to be visible to CMake. Normal Python types, variables and functions work as expected. 

Having said that, it's best not to try to communicate directly between code in different directory contexts (that is, added using ``add_subdirectory()``. These are somewhat separate interpreter invocations, and if you manage to pass objects between these directly, the exact behaviour is not defined (yet). Entities such as functions, targets etc will remain visible across directories in the standard way, and can be accessed via the CMake interfaces.

Tricks
======

Checking if Python scripting is available from CMake script
-----------------------------------------------------------

The CMAKE_PYTHON_AVAILABLE var will be set when the interpreter is compiled in and has initialised successfully.

Using IPython as a debugger
--------------------------

You can use an embedded interactive Python interface - such as IPython - to interrogate the state of CMake at any point during script execution, even if the script is not written in Python. Pdb also works. 

The easiest way to do this is -

1. Create a new subdirectory somewhere in your project, and then add this to the project with add_subdirectory().
2. Drop in cmake_build.py to the new directory which registers a function which will launch IPython
::
  import cmake
  import IPython 

  def launch_ipython(*, cm_dir, cm_global):
    IPython.embed(colors="neutral")

  def cmake_main(*, cm_dir, cm_global):
    cm_dir.py_functions.add(launch_ipython)
    
3. Call ``launch_ipython()`` from wherever you want to drop into an interactive prompt.

Gotchas
=======

pathlib.Path and sys.modules.append()
-------------------------------------

pycmake will try to generate pathlib.Path objects instead of plain strings where appropriate - for example when a cache entry has type FILEPATH. If you're using these to feed into sys.modules.append() and its ilk, be aware of https://github.com/python/cpython/issues/76823 (that is - make sure they're just plain strings, not Path objects).

Other Python modules named cmake
--------------------------------

pycmake will try to load its internal python module - which is named "cmake" - upon startup. If there are other modules of the same name in the PYTHONPATH, they may get loaded instead, and results will be undefined. 

Known Issues
============

* It's really early days - there are many rough edges.
* Automated testing is dramatically lacking.
* Python function wrappers in cm_dir and cm_global, whilst mostly unit tested, are mostly not integration tested, and may be incorrect in many cases. CMake's command interface is syntactically inconsistent, and really finicky to get right.
* Function objects returned from cm_dir.functions.values() (amongst others) are actually methods, so can't be called without a cm_dir.functions object; It's a bit misleading right now
* Python scripting is not available for implementing find scripts (eg called from find_package) yet. 
* Python scripting is not available when cmake is called-back from the build system (ie at build-time).

Implementation Notes
====================

This is very much a prototype, and far from final. There are many issues, including that there is far too much copying of data going on. The intent was to modify the existing cmake code as little as possible; The downside of this involved often sub-optimal implementations. 

Right now, the interfaces are somewhat inconsistent - especially with regards as to what methods are implemented. Sometimes types act like dictionaries, other times they just return dictionaries. 

Iterator behaviour when underlying structures are modified 
----------------------------------------------------------
There does not seem to be a rule for how exactly existing Python iterators should behave when their underlying structure is modified. The current pycmake iterator implementation queries the base data structure (rather than copying it on creation), but its behaviour when elements are added or deleted is not defined (though it won't segfault). Basically, just don't add/remove elements from structures whilst you're iterating over them.  

Internal Python module
----------------------

Much of the interface exposed to client scripts is implemented in Python, which is in a module named cmake, that lives under ${CMAKE_ROOT}/PyModules (parallel to the existing Modules dir, wherever that is installed on your system). 

CMake commands always take an array of strings
----------------------------------------------

The raw interface wraps the CMake command implementation. I'd hoped to directly expose a nicer, more type-friendly interface from C++; however, the existing CMake implementation currently passes a vector of strings, and each command manually breaks this vector down. The 'parsing' code is completely intermingled with the command logic so exposing a nicer interface would involve a major refactoring job. This would also be somewhat risky given the apparent lack of unit testing in CMake.

Automated Testing
-----------------

There are three main places that automated tests are implemented -

* Integrated with the existing CMake (integration) test suite - these are in Tests/Py*. This tests integration between Python and C++ code.
* C++ unit tests - There isn't really much unit testing in CMake; there is a new, minimal GoogleTest based C++ unit test in Sources/Python/Test
* Python unit tests - There's a Python unittest-based test suite in PyModules/test which tests Python code in isolation.

Style
-----

The C++ style is a bit inconsistent at this point, it needs to be cleaned up.

Other Python Implementations
----------------------------

pycmake has only been tested with CPython, but it should work with other Python implementations if pybind11 supports them; I think this just means PyPy. At the moment, I don't think any other implementations qualify, as we use pybind11's embedded mode - which doesn't support PyPy as far as I know. 

Other
-----
- There are nowhere near enough docstrings and similar 
- Too much copying
- Is automated type conversion a good thing? Maybe being explicit with types is better. 
