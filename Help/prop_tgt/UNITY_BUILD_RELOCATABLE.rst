UNITY_BUILD_RELOCATABLE
-----------------------

.. versionadded:: 3.32

By default, the unity file generated when :prop_tgt:`UNITY_BUILD` is enabled
uses absolute paths to reference the original source files. This causes the
unity file to result in a different output depending on the location of the
source files.

When this property is set to true, the ``#include`` lines inside the generated
unity source files will attempt to use relative paths to the original source
files if possible in order to standardize the output of the unity file.

The unity file's path to the original source file will use the following
priority:

* relative path to the generated unity file if the source file exists
  directly or in subfolder under the :variable:`CMAKE_BINARY_DIR`

* relative path to :variable:`CMAKE_SOURCE_DIR` if the source file exists
  directly or in subfolder under the :variable:`CMAKE_SOURCE_DIR`

* absolute path to the source file.

This target property *does not* guarantee a consistent unity file across
different environments as the final priority is an absolute path.

Example usage:

.. code-block:: cmake

  add_library(example_library
              source1.cxx
              source2.cxx
              source3.cxx)

  set_target_properties(example_library PROPERTIES
                        UNITY_BUILD True
                        UNITY_BUILD_RELOCATABLE TRUE)
