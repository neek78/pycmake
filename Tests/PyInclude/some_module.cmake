
function(some_function INPARAM1 RETPARAM1)
    message(STATUS "IN INPARAM1 ${INPARAM1}") 
    set("${RETPARAM1}" "snake" PARENT_SCOPE)
endfunction()

set("YEP_INCLUDED" "FROGS")

