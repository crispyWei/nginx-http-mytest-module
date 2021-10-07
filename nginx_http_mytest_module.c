/*
nginx_http_mytest是ngx_command_t結構中的成員
*/
static ngx_command_t nginx_http_mytest_commands[] = {
    {
        ngx_string = ("mytest"), /*命令名稱*/
        NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_NOARGS,
        nginx_http_mytest,
        NGX_HTTP_LOC_CONF_OFFSET,
        0,
        NULL
    },
    ngx_null_command /*執行結束*/
}

static char *

nginx_http_mytes(ngx_conf_t*cf, ngx_command_t*cmd, void*conf){

}