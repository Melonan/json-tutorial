#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL */
#include <stdio.h>

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

static int lept_parse_null(lept_context* c, lept_value* v) {
	EXPECT(c, 'n');
	if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
		return LEPT_PARSE_INVALID_VALUE;
	c->json += 3;
	v->type = LEPT_NULL;
	return LEPT_PARSE_OK;
}

static int lept_parse_false(lept_context* c , lept_value* v)
{
		const char* alse = "alse";
		EXPECT(c,'f');
		
		while (*c->json == *alse && *alse!='\0'){
			c->json++;
			alse++;
		}
		if(*c->json!='\0' || *alse != '\0')
		return LEPT_PARSE_INVALID_VALUE;
		
		v->type = LEPT_FALSE;
		return LEPT_PARSE_OK;
}



static int lept_parse_true(lept_context* c , lept_value* v)
{
		const char* ure = "rue";
		EXPECT(c,'t');
		/*
		如果没有*ure !+='\0'这个选项,
		那么*ure 和 *c->json在结尾\0仍判定相等
		他俩就会都移到下一格,即指向\0字符后的地址, 那里什么都没有*/
		while (*c->json == *ure && *ure!='\0')
		{
		/*printf(" *c->json: %c , *ure: %c \n", *c->json, *ure);*/
			c->json++;
			ure++;
		}
		if(*c->json!='\0' || *ure != '\0')
		return LEPT_PARSE_INVALID_VALUE;
		
		v->type = LEPT_TRUE;
		return LEPT_PARSE_OK;
}



static int lept_parse_value(lept_context* c, lept_value* v) {
	switch (*c->json) {
		case 'n':  return lept_parse_null(c, v);
		case 'f':  return lept_parse_false(c,v);
		case 't':	 return lept_parse_true(c,v);
		case '\0': return LEPT_PARSE_EXPECT_VALUE;
		default:   return LEPT_PARSE_INVALID_VALUE;
	}      
}

int lept_parse(lept_value* v, const char* json) {
	lept_context c;
	const char* tmp_json = json; 
	assert(v != NULL);
	c.json = json;
	v->type = LEPT_NULL;
	lept_parse_whitespace(&c);

	/*检查是出现 在空白符之后仍有字符 的情况*/
	/*我写的*/
	for(;*tmp_json!='\0';tmp_json++) 
	if(*tmp_json == ' ' || *tmp_json == '\t' || *tmp_json == '\n' || *tmp_json == '\r')
	break;

	while (*tmp_json == ' ' || *tmp_json == '\t' || *tmp_json == '\n' || *tmp_json == '\r')
	{
		tmp_json++;
	}

	if(*tmp_json != '\0')
	return LEPT_PARSE_ROOT_NOT_SINGULAR;

	return lept_parse_value(&c, v);
	

	/*
	int  ret;
	先在lept_parse_value函数中执行parse， 然后发现parse成功， 但不确定后面还有没有东西
	此时ret已经等于LEPT_PARSE_OK
	if((ret = lept_parse_value(&c,v)) == LEPT_PARSE_OK)
	{
		lept_parse_whitespace(&c);
		如果后面还有东西， 证明有问题 
		if(*c.json != '\0')
			ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
	}
	
	return ret;
	*/
}

lept_type lept_get_type(const lept_value* v) {
	assert(v != NULL);
	return v->type;
}
