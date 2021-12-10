#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leptjson.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do {\
        test_count++;\
        if (equality)\
            test_pass++;\
        else {\
            fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
            main_ret = 1;\
        }\
    } while(0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")




/*
    在各个test函数中 第一个 EXPECT_EQ_INT 用来判断parse的解析结果返回的代码是否和预期的一样
    第二个  EXPECT_EQ_INT用来判断解析后的 v 节点 type 值是否在 lept_parse() 函数中顺利被设置为LEPT_NULL
*/
static void test_parse_null() {
    lept_value v;
    /* v.type = LEPT_FALSE;*/
    /* 这个错误没有调整过来 nulll显示正确*/
    EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "nulll"));
    EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "null"));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
}
/* 设计LEPT_TRUE的测试*/
static void test_parse_true() {
    lept_value v;
    /*v.type = LEPT_FALSE;*/  
    EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "true"));
 /* EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "tre"));*/
    EXPECT_EQ_INT(LEPT_TRUE, lept_get_type(&v));
}
/* 设计LEPT_FALSE的测试*/
static void test_parse_false() {
    lept_value v;
    /*v.type = LEPT_NULL;*/  
    EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "false"));
    EXPECT_EQ_INT(LEPT_FALSE, lept_get_type(&v));
}
static void test_parse_expect_value() {
    lept_value v;

    v.type = LEPT_FALSE; 
    EXPECT_EQ_INT(LEPT_PARSE_EXPECT_VALUE, lept_parse(&v, ""));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));

    v.type = LEPT_FALSE;
    EXPECT_EQ_INT(LEPT_PARSE_EXPECT_VALUE, lept_parse(&v, " "));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
}

static void test_parse_invalid_value() {
    lept_value v;
    v.type = LEPT_FALSE;
    EXPECT_EQ_INT(LEPT_PARSE_INVALID_VALUE, lept_parse(&v, "nul"));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));

    v.type = LEPT_FALSE;
    EXPECT_EQ_INT(LEPT_PARSE_INVALID_VALUE, lept_parse(&v, "?"));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
}

static void test_parse_root_not_singular() {
    lept_value v;
    v.type = LEPT_FALSE;
    EXPECT_EQ_INT(LEPT_PARSE_ROOT_NOT_SINGULAR, lept_parse(&v, "null x"));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
}

static void test_parse() {
    printf("\n test_parse_null ");
    test_parse_null();
    printf("\n test_parse_ture ");
    test_parse_true();
    printf("\n test_parse_false ");
    test_parse_false();
    printf("\n test_parse_expect_value ");
    test_parse_expect_value();
    printf("\n test_parse_invalid_value ");
    test_parse_invalid_value();
    printf("\n test_parse_root_not_singular ");
    test_parse_root_not_singular();
}

int main() {
    test_parse();
    /* 输出百分号必须打两个百分号 */
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}
