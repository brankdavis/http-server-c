#include <stdio.h>
#include "../src/utils/utils.h"
#include "../src/server/helpers/helpers.h"
#include "assert.h"

#define RUN_TESTS


void test_strlen_noSpaces() {
    char * req1 =
        " Chrome/100.0.0.0\r\n\r\n";
    ASSERT_EQ(strlen_noSpaces(req1), 16);

    char * req2 =
        "    Firefox/92.0   ";
    ASSERT_EQ(strlen_noSpaces(req2), 12);

    char * req3 = 
        " raspberry/orange-mango";
    ASSERT_EQ(strlen_noSpaces(req3), 22);

    char * req4 = 
        " echo/blueberry";
    ASSERT_EQ(strlen_noSpaces(req4), 14);

    char * req5 = 
        " strawberry/grape";
    ASSERT_EQ(strlen_noSpaces(req5), 16);

}

void test_strlen_noEcho() {
    char * req1 =
        " echo/pineapple";
    ASSERT_EQ(strlen_noEcho(req1), 9);

     char * req2 = 
        " echo/blueberry";
    ASSERT_EQ(strlen_noEcho(req2), 9);
}

void test_extract_req_path() {
     char * req1 =
        " echo/pineapple";
    ASSERT_EQ_STR(extract_req_path(req1), "pineapple");

     char * req2 = 
        " echo/blueberry";
    ASSERT_EQ_STR(extract_req_path(req2), "blueberry");

}

void test_extract_request_method() {
    char * req1 =
        "GET";
    ASSERT_EQ(extract_request_method(req1), GET);

    char * req2 =
        "POST";
    ASSERT_EQ(extract_request_method(req2), POST);

}

#ifdef RUN_TESTS
    //#define NO_SPACES_TEST
    #define EXTRACT_METHOD_TEST
    // #define NO_ECHO_TEST
    // #define EXTRACT_REQ_TEST
int main() {

    #ifdef NO_SPACES_TEST
        printf("running tests for strlen_noSpaces()...\n");
        test_strlen_noSpaces();
        printf("\n");
    #endif

    #ifdef NO_ECHO_TEST
        printf("running tests for strlen_noEcho()...\n");
        test_strlen_noEcho();
        printf("\n");
    #endif

    #ifdef EXTRACT_REQ_TEST
        printf("running tests for extract_req_path()...\n");
        test_extract_req_path();
        printf("\n");
    #endif

    #ifdef EXTRACT_METHOD_TEST
        printf("running tests for test_extract_request_method()...\n");
        test_extract_request_method();
        printf("\n");
    #endif


    return 0;
}
#endif
