#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL, strtod() */
#include <errno.h>  /*errno c库宏*/
#include <math.h>   /*HUGE_VAL 宏*/
#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)

typedef struct {
    const char* json;
}lept_context;

static void lept_parse_whitespace(lept_context* c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

#if 0
static int lept_parse_true(lept_context* c, lept_value* v) {
    EXPECT(c, 't');
    if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = LEPT_TRUE;
    return LEPT_PARSE_OK;
}

static int lept_parse_false(lept_context* c, lept_value* v) {
    EXPECT(c, 'f');
    if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 4;
    v->type = LEPT_FALSE;
    return LEPT_PARSE_OK;
}

static int lept_parse_null(lept_context* c, lept_value* v) {
    EXPECT(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = LEPT_NULL;
    return LEPT_PARSE_OK;
}
#endif

static int lept_parse_literal(lept_context *c,  lept_value* v, const char* literal, lept_type type){
    
    EXPECT(c,literal[0]);
    size_t  bias = 1;
    while(*c->json == literal[bias] && literal[bias]!='\0')
    {
        bias++;
        c->json++;
    }
    if(literal[bias] =='\0')
    {
        v->type = type;
        return LEPT_PARSE_OK;
    }
    return LEPT_PARSE_INVALID_VALUE;
}
# define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
# define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')
static int lept_parse_number(lept_context* c, lept_value* v) {

#if 0
    if(!ISDIGIT(*c->json))
    {
        char ch = *c->json;
        if(ch == '-' && ISDIGIT(*(c->json+1)))
        ;
        else
        return LEPT_PARSE_INVALID_VALUE;
    }
    char* tmp = c->json;
    while(*(tmp+1) != '\0') tmp++; 
    if(!ISDIGIT(*tmp)) 
    return LEPT_PARSE_INVALID_VALUE;
#endif

    /*这段代码明显对着自动机写的， 很牛皮*/
    /*
    const char* p = c->json;
    if (*p == '-') p++;
    if (*p == '0') p++;
    else {
        if (!ISDIGIT1TO9(*p)) return LEPT_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }
    if (*p == '.') {
        p++;
        if (!ISDIGIT(*p)) return LEPT_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }
    if (*p == 'e' || *p == 'E') {
        p++;
        if (*p == '+' || *p == '-') p++;
        if (!ISDIGIT(*p)) return LEPT_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }
    */
    const char* p = c->json;
    if(*p == '-') p++;
    if(*p == '0') p++;
    else
    {
        if(!ISDIGIT1TO9(*p)) return LEPT_PARSE_INVALID_VALUE;
        while(ISDIGIT(*p)) p++;
    }

    if(*p == '.')
    {
        p++;
        if(!ISDIGIT(*p))return LEPT_PARSE_INVALID_VALUE;
        while(ISDIGIT(*p)) p++;    
    }
    else 
    ;


    if (*p == 'e' || *p == 'E') {
        p++;
        if (*p == '+' || *p == '-') p++;
        if (!ISDIGIT(*p)) return LEPT_PARSE_INVALID_VALUE;
        while(ISDIGIT(*p)) p++;
    }


    errno = 0;
    /* \TODO validate number */

#if 0
strtod ：将c风格字符串转换为浮点数
函数：
double strtod (const char* str, char** endptr);
参数
str
    C-string beginning with the representation of a floating-point number.
endptr
    Reference to an already allocated object of type char*, whose value is set by the function 
    to the next character in str after the numerical value.
    This parameter can also be a null pointer, in which case it is not used.

#endif


    v->n = strtod(c->json, NULL);
    /*如果是空的*/
/*
    errno - number of last error

    C 库宏 ERANGE 
        表示一个范围错误，它在输入参数超出数学函数定义的范围时发生，errno 被设置为 ERANGE。
    HUGE_VAL
        当函数的结果不可以表示为浮点数时。如果是因为结果的幅度太大以致于无法表示，
        则函数会设置 errno 为 ERANGE 来表示范围错误，并返回一个由宏 HUGE_VAL 或者它的否定（- HUGE_VAL）命名的
        一个特定的很大的值。
        如果结果的幅度太小，则会返回零值。
        在这种情况下，error 可能会被设置为 ERANGE，也有可能不会被设置为 ERANGE。
*/
    if(errno == ERANGE  && (v->n == HUGE_VAL || v->n == -HUGE_VAL))
        return LEPT_PARSE_NUMBER_TOO_BIG;

    v->type = LEPT_NUMBER;

    /*这一句很关键， 如果去掉的话就无法检测 test_parse_root_not_singular 函数中invalid number部分了*/
    c->json = p;
    return LEPT_PARSE_OK;
}

static int lept_parse_value(lept_context* c, lept_value* v) {
    switch (*c->json) {
        case 't':  return lept_parse_literal(c, v, "true", LEPT_TRUE);
        case 'f':  return lept_parse_literal(c, v,"false" ,LEPT_FALSE);
        case 'n':  return lept_parse_literal(c, v,"null", LEPT_NULL);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;
        default:   return lept_parse_number(c, v);
    }
}

int lept_parse(lept_value* v, const char* json) {
    lept_context c;
    int ret;
    assert(v != NULL);
    c.json = json;
    v->type = LEPT_NULL;
    lept_parse_whitespace(&c);
    if ((ret = lept_parse_value(&c, v)) == LEPT_PARSE_OK) {
        lept_parse_whitespace(&c);
        if (*c.json != '\0') {
            v->type = LEPT_NULL;
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    return ret;
}

lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}

double lept_get_number(const lept_value* v) {
    assert(v != NULL && v->type == LEPT_NUMBER);
    return v->n;
}
