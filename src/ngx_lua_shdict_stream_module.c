
/*
 * Copyright (C) Yichun Zhang (agentzh)
 */


#include "ngx_lua_shdict_common.h"


static ngx_int_t ngx_lua_shdict_stream_pre_config(ngx_conf_t *cf);


static ngx_command_t ngx_lua_shdict_stream_cmds[] = {

    { ngx_string("lua_shared_mem"),
      NGX_STREAM_MAIN_CONF|NGX_CONF_TAKE2|NGX_MAIN_CONF,
      ngx_lua_shdict,
      0,
      0,
      NULL },

    ngx_null_command
};


ngx_stream_module_t ngx_lua_shdict_stream_module_ctx = {
    ngx_lua_shdict_stream_pre_config,   /*  preconfiguration */
    NULL,                               /*  postconfiguration */

    NULL,                               /*  create main configuration */
    NULL,                               /*  init main configuration */

    NULL,                               /*  create server configuration */
    NULL                                /*  merge server configuration */
};


ngx_module_t ngx_lua_shdict_stream_module = {
    NGX_MODULE_V1,
    &ngx_lua_shdict_stream_module_ctx, /* module context */
    ngx_lua_shdict_stream_cmds,        /* module directives */
    NGX_STREAM_MODULE,                 /* module type */
    NULL,                              /* init master */
    NULL,                              /* init module */
    NULL,                              /* init process */
    NULL,                              /* init thread */
    NULL,                              /* exit thread */
    NULL,                              /* exit process */
    NULL,                              /* exit master */
    NGX_MODULE_V1_PADDING
};


static ngx_int_t
ngx_lua_shdict_stream_pre_config(ngx_conf_t *cf)
{
    ngx_str_t                     name = ngx_string("~stream_pre");
    ssize_t                       size = 8192;
    ngx_shm_zone_t               *zone;

    /* ensure ngx_stream_module run init_by_lua in ngx_stream_lua_shared_memory_init */
    zone = ngx_stream_lua_shared_memory_add(cf, &name, (size_t) size,
                                   &ngx_lua_shdict_module);
    zone->init = ngx_lua_shdict_fake_init;

    return NGX_OK;
}
