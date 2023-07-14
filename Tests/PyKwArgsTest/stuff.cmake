
SET(STUFF_INCLUDED TRUE)

function (expect V VAL)
    if("${${V}}" STREQUAL "${VAL}")
        message(STATUS "${V} is ${VAL} (correct)")
    else()
        message(SEND_ERROR "${V} - ${${V}} != ${VAL}")
    endif()
endfunction()

function (expect_undefined V)
    if(DEFINED ${V})
        message(SEND_ERROR "${V} is defined when it should not be")
    else()
        message(STATUS "${V} is undefined (correct)")
    endif()
endfunction()

function(kw_func1)
    #message("ARGV ${ARGV}")

    cmake_parse_arguments(
        PARSE_ARGV
        0
        PARSED_ARGS 
        "B1;B2;B3;B4"
        "OV1;OV2"
        "MV1;MV2"
    )

    expect(PARSED_ARGS_B1 TRUE)
    expect(PARSED_ARGS_B2 TRUE)
    expect(PARSED_ARGS_B3 FALSE)
    expect(PARSED_ARGS_B4 FALSE)

    expect(PARSED_ARGS_OV1 "Cats")
    expect_undefined(PARSED_ARGS_OV2)

    expect(PARSED_ARGS_MV1 "1;2;3")
    expect(PARSED_ARGS_MV2 "Dogs;TRUE;7")
   
    expect(PARSED_ARGS_UNPARSED_ARGUMENTS "")
    expect(PARSED_ARGS_KEYWORDS_MISSING_VALUES "")
endfunction()

function(kw_and_pos_func1 POS1 POS2)
    message("ARGV ${ARGV}")

    cmake_parse_arguments(
        PARSE_ARGV
        2
        PARSED_ARGS 
        "B1;B2"
        ""
        ""
    )

    expect(POS1 lions)
    expect(POS2 tigers)
    expect(PARSED_ARGS_B1 TRUE)
    expect(PARSED_ARGS_B2 TRUE)

    expect(PARSED_ARGS_UNPARSED_ARGUMENTS "")
    expect(PARSED_ARGS_KEYWORDS_MISSING_VALUES "")
   
endfunction()
