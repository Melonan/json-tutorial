#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL */



int lept_parse(lept_value* v, const char* json)
{
    return LEPT_PARSE_OK;
}

lept_type lept_get_type(const lept_value* v)
{
    assert(v != NULL);
    return v->type;
}