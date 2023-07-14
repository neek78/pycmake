
macro(macro2x2 INPARAM1 INPARAM2 RETPARAM1 RETPARAM2)
    message(STATUS "IN Function2")
    message(STATUS "IN INPARAM1 ${INPARAM1} INPARAM2 ${INPARAM2}")
    message(STATUS "OUT RETPARAM1 ${RETPARAM1} RETPARAM2 ${RETPARAM2}")
    set("${RETPARAM1}" "foo")
    set("${RETPARAM2}" "bar")
    #set("${RETPARAM2}" "TRue;foo;bar;baz" PARENT_SCOPE)
endmacro()

message(STATUS "SUBdir1 finsihed.................")
