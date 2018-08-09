//
//  ncbi-json-ut.cpp
//  ncbi-oauth-test
//
//  Created by User on 7/5/18.
//  Copyright © 2018 NCBI. All rights reserved.
//

#include "gtest/gtest.h"
#include <ncbi/json.hpp>

#include <iostream>

namespace ncbi
{
    /* JSON Construction
     *
     **********************************************************************************/
    class JSONFixture_WhiteBox : public :: testing :: Test
    {
    public:
        enum JSONType { Object, Array, Value };
        
        void SetUp ()
        {
            pos = 0;
            jObj = nullptr;
        }
        
        void TearDown ()
        {
            delete jObj;
        }
        
        void make_throw ( JSONType type, const std :: string &json, bool consume_all = true )
        {
            pos = 0;
            
            switch ( type )
            {
                case Object:
                    EXPECT_ANY_THROW ( JSONObject :: make ( json ) );
                    break;
                case Array:
                    EXPECT_ANY_THROW ( JSONArray :: test_parse ( json ) );
                    break;
                case Value:
                    EXPECT_ANY_THROW ( JSONValue :: test_parse ( json, consume_all ) );
                    break;
            }
        }
        
        void make ( JSONType type, const std :: string &json, bool consume_all = true )
        {
            pos = 0;
            
            switch ( type )
            {
                case Object:
                {
                    jObj = JSONObject :: make ( json );
                    break;
                }
                case Array:
                {
                    jObj = JSONArray :: parse ( json, pos );
                    break;
                }
                case Value:
                {
                    JSONValue *val = JSONValue :: test_parse ( json, consume_all );
                    ASSERT_TRUE ( val != nullptr );
                    
                    jObj = val;
                    break;
                }
            }
        }
        
        void make_and_verify_eq ( JSONType type, const std :: string &json, const std :: string &expected,
                                 bool consume_all = true )
        {
            pos = 0;
            
            switch ( type )
            {
                case Object:
                {
                    jObj = JSONObject :: make ( json );
                    break;
                }
                case Array:
                {
                    jObj = JSONArray :: parse ( json, pos );
                    break;
                }
                case Value:
                {
                    JSONValue *val = JSONValue :: test_parse ( json, consume_all );
                    ASSERT_TRUE ( val != nullptr );
                    
                    jObj = val;
                    break;
                }
            }
            
            EXPECT_STREQ ( jObj -> toJSON() . c_str(), expected . c_str () );
        }
    
    protected:
        size_t pos;
        JSONValue *jObj;
    };

    /* Object
     * {}
     * { members }
     */
    TEST_F ( JSONFixture_WhiteBox, JSONObject_Empty )
    {
        make_and_verify_eq ( Object , "{}", "{}" );
    }
    TEST_F ( JSONFixture_WhiteBox, JSONObject_EmptyArray )
    {
        make_and_verify_eq ( Object , "{\"\":[]}", "{\"\":[]}" );
    }
    TEST_F ( JSONFixture_WhiteBox, JSONObject_String_Member )
    {
        make_and_verify_eq ( Object , "{\"name\":\"value\"}", "{\"name\":\"value\"}" );
    }
    
    // JSONObject Exceptions
    TEST_F ( JSONFixture_WhiteBox, JSONObject_Throw_Empty )
    {
        make_throw ( Object, "" );  // Empty JSON object
    }
    TEST_F ( JSONFixture_WhiteBox, JSONObject_Throw_ExpecttRightBrace )
    {
        make_throw ( Object, "{" ); // Expected '}'
    }
    TEST_F ( JSONFixture_WhiteBox, JSONObject_Throw_ExpectLeftBrace )
    {
        make_throw ( Object, "}" ); // Expected '{'
    }
    TEST_F ( JSONFixture_WhiteBox, JSONObject_Throw_ExpectColon )
    {
        make_throw ( Object, "{\"name\"\"value\"" ); // Expected ':'
    }
    TEST_F ( JSONFixture_WhiteBox, JSONObject_Throw_ExpectRightBrace2 )
    {
        make_throw ( Object, "{\"name\":\"value\"" ); // Expected '}'
    }
    TEST_F ( JSONFixture_WhiteBox, JSONObject_Throw_TrailingBytes )
    {
        make_throw ( Object, "{\"name\":\"value\"}trailing" ); // Expected '}'
    }
    
    /* Array
     * []
     * [ elements ]
     */
    TEST_F ( JSONFixture_WhiteBox, JSONArray_Empty )
    {
        make_and_verify_eq ( Array , "[]", "[]" );
    }
    TEST_F ( JSONFixture_WhiteBox, JSONArray_String_Elems )
    {
        make_and_verify_eq( Array , "[\"name\",\"value\"]", "[\"name\",\"value\"]" );
    }
    
    // JSONArray Exceptions
    TEST_F ( JSONFixture_WhiteBox, JSONArray_Throw_Empty )
    {
        make_throw ( Array, "" );  // Empty JSON array
    }
    TEST_F ( JSONFixture_WhiteBox, JSONArray_Throw_ExpectLeftBracket )
    {
        make_throw ( Array, "]" ); // Expected '['
    }
    TEST_F ( JSONFixture_WhiteBox, JSONArray_Throw_ExpectRightBracket )
    {
        make_throw ( Array, "[" ); // Expected ']'
    }
    TEST_F ( JSONFixture_WhiteBox, JSONArray_Throw_ExpectRightBracket2 )
    {
        make_throw ( Array, "[\"name\",\"name\"" ); // Expected ']'
    }
    TEST_F ( JSONFixture_WhiteBox, JSONArray_Throw_TrailingBytes )
    {
        make_throw ( Array, "[\"name\",\"name\"]trailing" ); // Expected ']'
    }
  
    /* String
     * ""
     * " chars "
     */
    TEST_F ( JSONFixture_WhiteBox, String_Empty )
    {
        make_and_verify_eq( Value , "\"\"", "\"\"" );
    }
    TEST_F ( JSONFixture_WhiteBox, String_Char )
    {
        make_and_verify_eq( Value , "\"a\"", "\"a\"" );
    }
    TEST_F ( JSONFixture_WhiteBox, String_Chars )
    {
        make_and_verify_eq( Value , "\"abc\"", "\"abc\"" );
    }
    
    /* Bool
     * true
     * false
     */
    TEST_F ( JSONFixture_WhiteBox, Bool_True )
    {
        make_and_verify_eq( Value , "true", "true" );
    }
    TEST_F ( JSONFixture_WhiteBox, Bool_False )
    {
        make_and_verify_eq( Value , "false", "false" );
    }
    
    /* Integer
     * digit
     * digit1-9 digits
     * -digit
     * -digit1-9 digits
     */
    TEST_F ( JSONFixture_WhiteBox, Integer_Single )
    {
        make_and_verify_eq ( Value , "0", "0" );
    }
    TEST_F ( JSONFixture_WhiteBox, Integer_Multiple )
    {
        make_and_verify_eq( Value , "12345", "12345" );
    }
    TEST_F ( JSONFixture_WhiteBox, Integer_Single_Negative )
    {
        make_and_verify_eq( Value , "-0", "0" );
        make_and_verify_eq( Value , "-1", "-1" );
    }
    TEST_F ( JSONFixture_WhiteBox, Integer_Multiple_Negative )
    {
        make_and_verify_eq( Value , "-12345", "-12345" );
    }
    
    /* Floating point
     * int frac
     * int exp
     * int frac exp
     */
    TEST_F ( JSONFixture_WhiteBox, Float_Frac )
    {
        make_and_verify_eq( Value , "0.0", "0.0" );
        make_and_verify_eq( Value , "1.2", "1.2" );
    }
    TEST_F ( JSONFixture_WhiteBox, Float_Frac_Precision )
    {
        make_and_verify_eq( Value , "1234.56789", "1234.56789" );
    }
    
    TEST_F ( JSONFixture_WhiteBox, Float_eE_nodigit )
    {
        // invalid exp format, but construction should not fail
        // as it is the nature of parsers to consume tokens, not
        // entire strings - should return "0" and consumed
        // only one digit
        make_and_verify_eq ( Value , "0E", "0", false );
    }
    TEST_F ( JSONFixture_WhiteBox, Float_eE_digit )
    {
        make ( Value , "0e0" );
        make ( Value , "0E0" );
        
    }
    TEST_F ( JSONFixture_WhiteBox, Float_eE_plus_digits )
    {
        make ( Value , "0e+0" );
        make ( Value , "0E+0" );
    }
    TEST_F ( JSONFixture_WhiteBox, Float_eE_minus_digits )
    {
        make ( Value , "0e-0" );
        make ( Value , "0E-0" );
    }

    TEST_F ( JSONFixture_WhiteBox, Float_Frac_Exp )
    {
        make ( Value, "0.0e0" );
    }
    
    /* Exceptions
     */
    
    TEST_F ( JSONFixture_WhiteBox, JSONValue_Null_Throw_InvJSONFmt )
    {
        make_throw ( Value, "a" );
    }
    TEST_F ( JSONFixture_WhiteBox, JSONValue_Null_Throw_InvNullFmt_Missing )
    {
        make_throw ( Value, "n" );
    }
    TEST_F ( JSONFixture_WhiteBox, JSONValue_Null_Throw_InvNullFmt_Bad )
    {
        make_throw ( Value, "nulll" );
    }
    TEST_F ( JSONFixture_WhiteBox, JSONValue_String_Throw_InvBeginFormat )
    {
        make_throw ( Value, "\"" ); // Invalid begin of string format
    }
    TEST_F ( JSONFixture_WhiteBox, JSONValue_String_Throw_InvEscChar_Missing )
    {
        make_throw ( Value, "\"\\" ); // Invalid escape character
    }
    TEST_F ( JSONFixture_WhiteBox, JSONValue_String_Throw_InvEscChar_Bad )
    {
        make_throw ( Value, "\"\\y" ); // Invalid escape character
    }
    TEST_F ( JSONFixture_WhiteBox, JSONValue_String_Throw_InvUEscSeq_Missing )
    {
        make_throw ( Value, "\"\\u" ); // Invalid \u escape sequence
    }
    TEST_F ( JSONFixture_WhiteBox, JSONValue_String_Throw_InvUEscSeq_Short )
    {
        make_throw ( Value, "\"\\uabc" ); // Invalid \u escape sequence
    }
    TEST_F ( JSONFixture_WhiteBox, JSONValue_String_Throw_InvUEscSeq_Bad )
    {
        make_throw ( Value, "\"\\uabcz" ); // Invalid \u escape sequence
    }
    TEST_F ( JSONFixture_WhiteBox, JSONValue_String_Throw_InvEndFormat )
    {
        make_throw ( Value, "\"\\u0061" ); // Invalid end of string format
    }
    TEST_F ( JSONFixture_WhiteBox, JSONValue_String_Throw_Trailing )
    {
        make_throw ( Value, "\"validtext\"trailing" );
    }
    TEST_F ( JSONFixture_WhiteBox, JSONValue_Bool_Throw_True_Missing )
    {
        make_throw ( Value, "t" );
    }
    TEST_F ( JSONFixture_WhiteBox, JSONValue_Bool_Throw_True_Bad )
    {
        make_throw ( Value, "truee" );
    }
    TEST_F ( JSONFixture_WhiteBox, JSONValue_Bool_Throw_False_Missing )
    {
        make_throw ( Value, "f" );
    }
    TEST_F ( JSONFixture_WhiteBox, JSONValue_Bool_Throw_False_Bad )
    {
        make_throw ( Value, "falsee" );
    }
    TEST_F ( JSONFixture_WhiteBox, JSONValue_Integer_Throw_Negative_Missing )
    {
        make_throw ( Value, "-" ); // Expected digit
    }
    TEST_F ( JSONFixture_WhiteBox, JSONValue_Integer_Throw_Negative_Bad )
    {
        make_throw ( Value, "-a" ); // Expected digit
    }
} // ncbi
