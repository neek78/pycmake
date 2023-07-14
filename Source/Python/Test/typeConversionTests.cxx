
#include "Python/cmPythonTypeConversion.h"

#include <gtest/gtest.h>

#include <pybind11/embed.h>
#include <pybind11/stl.h>

#include <string>

namespace py = pybind11;

TEST(typeConversionPyToCMakeTest, PythonTests)
{
    EXPECT_EQ(ConvertArgPythonToCMake(py::str("some_string")), "some_string");
    EXPECT_EQ(ConvertArgPythonToCMake(py::int_(1)), "1");
    EXPECT_EQ(ConvertArgPythonToCMake(py::float_(3.141592654)), "3.141592654");
    EXPECT_EQ(ConvertArgPythonToCMake(py::bool_(true)), "TRUE");
    EXPECT_EQ(ConvertArgPythonToCMake(py::bool_(false)), "FALSE");

    // none should fail (for now)
    EXPECT_THROW(ConvertArgPythonToCMake(py::none()), py::type_error);

    // dicts should fail to convert
    EXPECT_THROW(ConvertArgPythonToCMake(py::dict()), py::type_error);

    py::dict dict;
    dict["a"] = "b";
    dict["c"] = "d";
    EXPECT_THROW(ConvertArgPythonToCMake(dict), py::type_error);
    
    // dict.keys() and .values() are ok
    EXPECT_EQ(ConvertArgPythonToCMake(dict.attr("keys")()), "a;c");
    EXPECT_EQ(ConvertArgPythonToCMake(dict.attr("values")()), "b;d");

    // dict_items are not..
    EXPECT_THROW(ConvertArgPythonToCMake(dict.attr("items")()), py::type_error);
    
    const auto tuple = py::make_tuple("cat", "dog", "giraffe");
    const std::string result = "cat;dog;giraffe";

    EXPECT_EQ(ConvertArgPythonToCMake(tuple), result);
    EXPECT_EQ(ConvertArgPythonToCMake(py::list(tuple)), result);

    const auto tuple2 = py::make_tuple("cat", true, 3);
    const std::string result2 = "cat;TRUE;3";
    EXPECT_EQ(ConvertArgPythonToCMake(tuple2), result2);

    // FIXME: need a test for sets - the order is undefined, so simple str comparisons don't work
    //EXPECT_EQ(ConvertArgPythonToCMake(py::set(tuple)), "giraffe;cat;dog");
    
    //FIXME: test lists in lists etc - what's the correct behviour
}

TEST(typeConversionCMakeToPyTest_SimpleTypes, PythonTests)
{  
    EXPECT_TRUE(ConvertArgCMakeToPython("").is_none());

    EXPECT_TRUE(ConvertArgCMakeToPython("some_string").equal(py::str("some_string")));
      
    // need to check these cases to make sure they don't implicitly get turned into bools
    EXPECT_EQ(ConvertArgCMakeToPython("0"), py::int_(0));
    EXPECT_EQ(ConvertArgCMakeToPython("1"), py::int_(1));
    EXPECT_EQ(ConvertArgCMakeToPython("2"), py::int_(2));

    EXPECT_TRUE(ConvertArgCMakeToPython("3.141592654").equal(py::float_(3.141592654)));

    // this effectively tests cmValue.IsOn() / cmValue.IsOff()
    EXPECT_EQ(ConvertArgCMakeToPython("TRUE"), py::bool_(true));
    EXPECT_EQ(ConvertArgCMakeToPython("ON"), py::bool_(true));
    EXPECT_EQ(ConvertArgCMakeToPython("YES"), py::bool_(true));

    EXPECT_EQ(ConvertArgCMakeToPython("FALSE"), py::bool_(false));
    EXPECT_EQ(ConvertArgCMakeToPython("OFF"), py::bool_(false));
    EXPECT_EQ(ConvertArgCMakeToPython("NO"), py::bool_(false));
    EXPECT_EQ(ConvertArgCMakeToPython("IGNORE"), py::bool_(false));
}

TEST(typeConversionCMakeToPyTest_ListTypes_SameType, PythonTests)
{
    const auto list = py::list(py::make_tuple("cat", "dog", "giraffe"));
    const std::string strList = "cat;dog;giraffe";

    EXPECT_TRUE(ConvertArgCMakeToPython(strList).equal(list));

}
TEST(typeConversionCMakeToPyTest_ListTypes_MultiType, PythonTests)
{
    const auto list = py::list(py::make_tuple("cat", true, 57, 4.8876)); 
    const std::string strList = "cat;TRUE;57;4.8876";

    EXPECT_TRUE(ConvertArgCMakeToPython(strList).equal(list));

    // test all the cases of cmValue
}

