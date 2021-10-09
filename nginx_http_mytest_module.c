#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static ngx_int_t nginx_http_mytest_handler(ngx_http_request_t *r);
static char *nginx_http_mytest(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

/*
nginx_http_mytest是ngx_command_t結構中的成員
*/
static ngx_command_t nginx_http_mytest_commands[] = 
{
    {
        ngx_string("mytest"), /*命令名稱*/
        NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_NOARGS,
        nginx_http_mytest,
        0,
        0,
        NULL
    },
    ngx_null_command /*執行結束*/
};
static char *
nginx_http_mytest(ngx_conf_t*cf, ngx_command_t*cmd, void*conf){
    ngx_http_core_loc_conf_t *clcf; /*main、srv、loc配置項目*/
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    /*http客戶端請求到NGX_HTTP_CONTENT_PHASE階段時，如果請求URI與mytest配置相同時，就會去調用nginx_http_mytest_handler*/
    clcf->handler = nginx_http_mytest_handler;
    return NGX_CONF_OK;
}
/*
若無相關工作需在HTTP框架初始化完成時就不必設置相關callback
*/
static ngx_http_module_t nginx_http_mytest_module_ctx = {
    NULL, /* preconfiguration */
    NULL, /* postconfiguration */
    NULL, /* create main configuration */
    NULL, /* init main configuration */
    NULL, /* create server configuration */
    NULL, /* merge server configuration */
    NULL, /* create location configuration */
    NULL /* merge location configuration */
};
/*
定義模組
*/
ngx_module_t nginx_http_mytest_module = {
    NGX_MODULE_V1,
    &nginx_http_mytest_module_ctx, /* module context */
    nginx_http_mytest_commands, /* module directives */
    NGX_HTTP_MODULE, /* module type */
    NULL, /* init master */
    NULL, /* init module */
    NULL, /* init process */
    NULL, /* init thread */
    NULL, /* exit thread */
    NULL, /* exit process */
    NULL, /* exit master */
    NGX_MODULE_V1_PADDING
};
static ngx_int_t nginx_http_mytest_handler(ngx_http_request_t *r)
{   
    if (!(r->method & (NGX_HTTP_GET | NGX_HTTP_HEAD)))
    {
        return NGX_HTTP_NOT_ALLOWED;
    }

    //獲取到請求，並轉換成資源檔案
    ngx_int_t rc = ngx_http_discard_request_body(r);
    if(rc != NGX_OK){
        return rc;
    }

    ngx_str_t response = ngx_string("Hello My Test");
    ngx_str_t type = ngx_string("text/plain");
    /*  設置 Content-Type 標頭 */
    r->headers_out.content_length_n = response.len;
    r->headers_out.content_type = type;
    r->headers_out.status = NGX_HTTP_OK; //http 狀態碼為 200
    rc = ngx_http_send_header(r);
    if(rc == NGX_ERROR || rc > NGX_OK || r->header_only){
        return rc;
    }

    ngx_buf_t *b;
    b = ngx_create_temp_buf(r->pool, response.len);
    if (b == NULL)
    {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    ngx_memcpy(b->pos, response.data, response.len);
    b->last = b->pos + response.len;
    b->last_buf = 1;

    ngx_chain_t out;
    out.buf = b;
    out.next = NULL;

    return ngx_http_output_filter(r, &out);
};