
function(function2x2 INPARAM1 INPARAM2 RETPARAM1 RETPARAM2)
    # message(STATUS "IN Function2")
    # message(STATUS "IN INPARAM1 ${INPARAM1} INPARAM2 ${INPARAM2}")
    # message(STATUS "OUT RETPARAM1 ${RETPARAM1} RETPARAM2 ${RETPARAM2}")
    set("${RETPARAM1}" "foo" PARENT_SCOPE)
    set("${RETPARAM2}" "bar" PARENT_SCOPE)
    #set("${RETPARAM2}" "TRue;foo;bar;baz" PARENT_SCOPE)
endfunction()

function(function2x1opt INPARAM1 INPARAM2 RETPARAM1 RETPARAM2)
    # message(STATUS "IN Function2x1opt")
    # message(STATUS "IN INPARAM1 ${INPARAM1} INPARAM2 ${INPARAM2}")
    # message(STATUS "OUT RETPARAM1 ${RETPARAM1} RETPARAM2 ${RETPARAM2}")
    set("${RETPARAM1}" "foo" PARENT_SCOPE)
    # set nothing for second return var
endfunction()
# message(STATUS "SUBdir1 finsihed.................")

function(function_return_input INPARAM RETPARAM)
    set("${RETPARAM}" "${INPARAM}" PARENT_SCOPE)
endfunction()
