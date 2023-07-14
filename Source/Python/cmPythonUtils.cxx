
#include "cmPythonUtils.h"
#include "cmMessenger.h"

#include <pybind11/stl.h>

#include <sstream>

void OutputExceptionAndSetFatal(cmMessenger& messenger, const std::string_view& extra, 
        const pybind11::error_already_set& ex)
{
    std::stringstream ss;
    ss << extra << ex.what() 
    //    << "\ntype " << ex.type() 
    //    << "\nvalue " << ex.value() 
    //    << "\ntrace " << ex.trace() 
    //    << "\n"
        ;

    messenger.IssueMessage(MessageType::FATAL_ERROR, ss.str());
    cmSystemTools::SetFatalErrorOccurred();
}

void OutputExceptionAndSetFatal(cmMessenger& messenger, const std::string_view& extra, 
        const std::exception& ex)
{
    std::string msg = std::string(extra) + ex.what();
    messenger.IssueMessage(MessageType::FATAL_ERROR, msg);
    cmSystemTools::SetFatalErrorOccurred();
}

