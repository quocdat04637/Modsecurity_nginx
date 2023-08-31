/*
 * ModSecurity connector for nginx, http://www.modsecurity.org/
 * Copyright (c) 2015 Trustwave Holdings, Inc. (http://www.trustwave.com/)
 *
 * You may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact Trustwave Holdings, Inc.
 * directly using the email address security@modsecurity.org.
 *
 */



#ifndef MODSECURITY_DDEBUG
#define MODSECURITY_DDEBUG 0
#endif
#include "ddebug.h"
#include <string.h>
#include "tokenizer.c"
#include <tensorflow/c/c_api.h>
#include "ngx_http_modsecurity_common.h"
#include <time.h>
#include <stdio.h>



TF_Session* session;
TF_SessionOptions* session_opts;
TF_Buffer* run_opts;
TF_Graph* graph;
TF_Status* status; // Luu tru trang thai khi load session
const char* saved_model_dir = "/home/modsecurity/model1"; // Duong dan den model

int loadSessionFromSavedModel(const char* saved_model_dir) {
    // Tạo các tùy chọn phiên (session options)
    session_opts = TF_NewSessionOptions();
    
    // Tạo các tùy chọn chạy (run options)
    run_opts = NULL;  // Để mặc định
    
    // Tạo đối tượng đồ thị (graph)
    graph = TF_NewGraph();
    
    // Tạo danh sách các nhãn (tags)
    const char* tags[] = {"serve"};
    int ntags = 1;
    
    // Tạo đối tượng trạng thái (status) để lưu thông báo lỗi
    status = TF_NewStatus();
    
    // Tải phiên (session) từ mô hình đã lưu
    session = TF_LoadSessionFromSavedModel(session_opts, run_opts, saved_model_dir, tags, ntags, graph, NULL, status);
    if (TF_GetCode(status) != TF_OK) {
        printf("Lỗi khi tải phiên (session) từ mô hình đã lưu: %s\n", TF_Message(status));
        TF_DeleteSessionOptions(session_opts);
        TF_DeleteGraph(graph);
        TF_DeleteStatus(status);
        return -1;  // Trả về -1 nếu tải không thành công
    }
    
    return 0;  // Trả về 0 nếu tải thành công
}

    



static ngx_http_output_header_filter_pt ngx_http_next_header_filter;

static ngx_int_t ngx_http_modsecurity_resolv_header_server(ngx_http_request_t *r, ngx_str_t name, off_t offset);
static ngx_int_t ngx_http_modsecurity_resolv_header_date(ngx_http_request_t *r, ngx_str_t name, off_t offset);
static ngx_int_t ngx_http_modsecurity_resolv_header_content_length(ngx_http_request_t *r, ngx_str_t name, off_t offset);
static ngx_int_t ngx_http_modsecurity_resolv_header_content_type(ngx_http_request_t *r, ngx_str_t name, off_t offset);
static ngx_int_t ngx_http_modsecurity_resolv_header_last_modified(ngx_http_request_t *r, ngx_str_t name, off_t offset);
static ngx_int_t ngx_http_modsecurity_resolv_header_connection(ngx_http_request_t *r, ngx_str_t name, off_t offset);
static ngx_int_t ngx_http_modsecurity_resolv_header_transfer_encoding(ngx_http_request_t *r, ngx_str_t name, off_t offset);
static ngx_int_t ngx_http_modsecurity_resolv_header_vary(ngx_http_request_t *r, ngx_str_t name, off_t offset);

ngx_http_modsecurity_header_out_t ngx_http_modsecurity_headers_out[] = {

    { ngx_string("Server"),
            offsetof(ngx_http_headers_out_t, server),
            ngx_http_modsecurity_resolv_header_server },

    { ngx_string("Date"),
            offsetof(ngx_http_headers_out_t, date),
            ngx_http_modsecurity_resolv_header_date },

    { ngx_string("Content-Length"),
            offsetof(ngx_http_headers_out_t, content_length_n),
            ngx_http_modsecurity_resolv_header_content_length },

    { ngx_string("Content-Type"),
            offsetof(ngx_http_headers_out_t, content_type),
            ngx_http_modsecurity_resolv_header_content_type },

    { ngx_string("Last-Modified"),
            offsetof(ngx_http_headers_out_t, last_modified),
            ngx_http_modsecurity_resolv_header_last_modified },

    { ngx_string("Connection"),
            0,
            ngx_http_modsecurity_resolv_header_connection },

    { ngx_string("Transfer-Encoding"),
            0,
            ngx_http_modsecurity_resolv_header_transfer_encoding },

    { ngx_string("Vary"),
            0,
            ngx_http_modsecurity_resolv_header_vary },

#if 0
    { ngx_string("Content-Encoding"),
            offsetof(ngx_http_headers_out_t, content_encoding),
            NGX_TABLE },

    { ngx_string("Cache-Control"),
            offsetof(ngx_http_headers_out_t, cache_control),
            NGX_ARRAY },

    { ngx_string("Location"),
            offsetof(ngx_http_headers_out_t, location),
            NGX_TABLE },

    { ngx_string("Content-Range"),
            offsetof(ngx_http_headers_out_t, content_range),
            NGX_TABLE },

    { ngx_string("Accept-Ranges"),
            offsetof(ngx_http_headers_out_t, accept_ranges),
            NGX_TABLE },

    returiders_out[i].name 1;
    { ngx_string("WWW-Authenticate"),
            offsetof(ngx_http_headers_out_t, www_authenticate),
            NGX_TABLE },

    { ngx_string("Expires"),
            offsetof(ngx_http_headers_out_t, expires),
            NGX_TABLE },
#endif
    { ngx_null_string, 0, 0 }
};


#if defined(MODSECURITY_SANITY_CHECKS) && (MODSECURITY_SANITY_CHECKS)
int
ngx_http_modsecurity_store_ctx_header(ngx_http_request_t *r, ngx_str_t *name, ngx_str_t *value)
{
    ngx_http_modsecurity_ctx_t     *ctx;
    ngx_http_modsecurity_conf_t    *mcf;
    ngx_http_modsecurity_header_t  *hdr;

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity_module);
    if (ctx == NULL || ctx->sanity_headers_out == NULL) {
        return NGX_ERROR;
    }

    mcf = ngx_http_get_module_loc_conf(r, ngx_http_modsecurity_module);
    if (mcf == NULL || mcf->sanity_checks_enabled == NGX_CONF_UNSET)
    {
        return NGX_OK;
    }

    hdr = ngx_array_push(ctx->sanity_headers_out);
    if (hdr == NULL) {
        return NGX_ERROR;
    }

    hdr->name.data = ngx_pnalloc(r->pool, name->len);
    hdr->value.data = ngx_pnalloc(r->pool, value->len);
    if (hdr->name.data == NULL || hdr->value.data == NULL) {
        return NGX_ERROR;
    }

    ngx_memcpy(hdr->name.data, name->data, name->len);
    hdr->name.len = name->len;
    ngx_memcpy(hdr->value.data, value->data, value->len);
    hdr->value.len = value->len;

    return NGX_OK;
}
#endif


static ngx_int_t
ngx_http_modsecurity_resolv_header_server(ngx_http_request_t *r, ngx_str_t name, off_t offset)
{
    static char ngx_http_server_full_string[] = NGINX_VER;
    static char ngx_http_server_string[] = "nginx";

    ngx_http_core_loc_conf_t *clcf = NULL;
    ngx_http_modsecurity_ctx_t *ctx = NULL;
    ngx_str_t value;

    clcf = ngx_http_get_module_loc_conf(r, ngx_http_core_module);
    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity_module);

    if (r->headers_out.server == NULL) {
        if (clcf->server_tokens) {
            value.data = (u_char *)ngx_http_server_full_string;
            value.len = sizeof(ngx_http_server_full_string);
        } else {
            value.data = (u_char *)ngx_http_server_string;
            value.len = sizeof(ngx_http_server_string);
        }
    } else {
        ngx_table_elt_t *h = r->headers_out.server;
        value.data = h->value.data;
        value.len =  h->value.len;
    }

#if defined(MODSECURITY_SANITY_CHECKS) && (MODSECURITY_SANITY_CHECKS)
    ngx_http_modsecurity_store_ctx_header(r, &name, &value);
#endif

    return msc_add_n_response_header(ctx->modsec_transaction,
        (const unsigned char *) name.data,
        name.len,
        (const unsigned char *) value.data,
        value.len);
}


static ngx_int_t
ngx_http_modsecurity_resolv_header_date(ngx_http_request_t *r, ngx_str_t name, off_t offset)
{
    ngx_http_modsecurity_ctx_t *ctx = NULL;
    ngx_str_t date;

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity_module);

    if (r->headers_out.date == NULL) {
        date.data = ngx_cached_http_time.data;
        date.len = ngx_cached_http_time.len;
    } else {
        ngx_table_elt_t *h = r->headers_out.date;
        date.data = h->value.data;
        date.len = h->value.len;
    }

#if defined(MODSECURITY_SANITY_CHECKS) && (MODSECURITY_SANITY_CHECKS)
    ngx_http_modsecurity_store_ctx_header(r, &name, &date);
#endif

    return msc_add_n_response_header(ctx->modsec_transaction,
        (const unsigned char *) name.data,
        name.len,
        (const unsigned char *) date.data,
        date.len);
}


static ngx_int_t
ngx_http_modsecurity_resolv_header_content_length(ngx_http_request_t *r, ngx_str_t name, off_t offset)
{
    ngx_http_modsecurity_ctx_t *ctx = NULL;
    ngx_str_t value;
    char buf[NGX_INT64_LEN+2];

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity_module);

    if (r->headers_out.content_length_n > 0)
    {
        ngx_sprintf((u_char *)buf, "%O%Z", r->headers_out.content_length_n);
        value.data = (unsigned char *)buf;
        value.len = strlen(buf);

#if defined(MODSECURITY_SANITY_CHECKS) && (MODSECURITY_SANITY_CHECKS)
        ngx_http_modsecurity_store_ctx_header(r, &name, &value);
#endif
        return msc_add_n_response_header(ctx->modsec_transaction,
            (const unsigned char *) name.data,
            name.len,
            (const unsigned char *) value.data,
            value.len);
    }

    return 1;
}


static ngx_int_t
ngx_http_modsecurity_resolv_header_content_type(ngx_http_request_t *r, ngx_str_t name, off_t offset)
{
    ngx_http_modsecurity_ctx_t *ctx = NULL;

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity_module);

    if (r->headers_out.content_type.len > 0)
    {

#if defined(MODSECURITY_SANITY_CHECKS) && (MODSECURITY_SANITY_CHECKS)
        ngx_http_modsecurity_store_ctx_header(r, &name, &r->headers_out.content_type);
#endif

        return msc_add_n_response_header(ctx->modsec_transaction,
            (const unsigned char *) name.data,
            name.len,
            (const unsigned char *) r->headers_out.content_type.data,
            r->headers_out.content_type.len);
    }

    return 1;
}


static ngx_int_t
ngx_http_modsecurity_resolv_header_last_modified(ngx_http_request_t *r, ngx_str_t name, off_t offset)
{
    ngx_http_modsecurity_ctx_t *ctx = NULL;
    u_char buf[1024], *p;
    ngx_str_t value;

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity_module);

    if (r->headers_out.last_modified_time == -1) {
        return 1;
    }

    p = ngx_http_time(buf, r->headers_out.last_modified_time);

    value.data = buf;
    value.len = (int)(p-buf);

#if defined(MODSECURITY_SANITY_CHECKS) && (MODSECURITY_SANITY_CHECKS)
    ngx_http_modsecurity_store_ctx_header(r, &name, &value);
#endif

    return msc_add_n_response_header(ctx->modsec_transaction,
        (const unsigned char *) name.data,
        name.len,
        (const unsigned char *) value.data,
        value.len);
}


static ngx_int_t
ngx_http_modsecurity_resolv_header_connection(ngx_http_request_t *r, ngx_str_t name, off_t offset)
{
    ngx_http_modsecurity_ctx_t *ctx = NULL;
    ngx_http_core_loc_conf_t *clcf = NULL;
    char *connection = NULL;
    ngx_str_t value;

    clcf = ngx_http_get_module_loc_conf(r, ngx_http_core_module);
    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity_module);

    if (r->headers_out.status == NGX_HTTP_SWITCHING_PROTOCOLS) {
        connection = "upgrade";
    } else if (r->keepalive) {
        connection = "keep-alive";
        if (clcf->keepalive_header)
        {
            u_char buf[1024];
            ngx_sprintf(buf, "timeout=%T%Z", clcf->keepalive_header);
            ngx_str_t name2 = ngx_string("Keep-Alive");

            value.data = buf;
            value.len = strlen((char *)buf);

#if defined(MODSECURITY_SANITY_CHECKS) && (MODSECURITY_SANITY_CHECKS)
            ngx_http_modsecurity_store_ctx_header(r, &name2, &value);
#endif

            msc_add_n_response_header(ctx->modsec_transaction,
                (const unsigned char *) name2.data,
                name2.len,
                (const unsigned char *) value.data,
                value.len);
        }
    } else {
        connection = "close";
    }

    value.data = (u_char *) connection;
    value.len = strlen(connection);

#if defined(MODSECURITY_SANITY_CHECKS) && (MODSECURITY_SANITY_CHECKS)
    ngx_http_modsecurity_store_ctx_header(r, &name, &value);
#endif

    return msc_add_n_response_header(ctx->modsec_transaction,
        (const unsigned char *) name.data,
        name.len,
        (const unsigned char *) value.data,
        value.len);
}

static ngx_int_t
ngx_http_modsecurity_resolv_header_transfer_encoding(ngx_http_request_t *r, ngx_str_t name, off_t offset)
{
    ngx_http_modsecurity_ctx_t *ctx = NULL;

    if (r->chunked) {
        ngx_str_t value = ngx_string("chunked");

        ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity_module);

#if defined(MODSECURITY_SANITY_CHECKS) && (MODSECURITY_SANITY_CHECKS)
        ngx_http_modsecurity_store_ctx_header(r, &name, &value);
#endif

        return msc_add_n_response_header(ctx->modsec_transaction,
            (const unsigned char *) name.data,
            name.len,
            (const unsigned char *) value.data,
            value.len);
    }

    return 1;
}

static ngx_int_t
ngx_http_modsecurity_resolv_header_vary(ngx_http_request_t *r, ngx_str_t name, off_t offset)
{
#if (NGX_HTTP_GZIP)
    ngx_http_modsecurity_ctx_t *ctx = NULL;
    ngx_http_core_loc_conf_t *clcf = NULL;

    clcf = ngx_http_get_module_loc_conf(r, ngx_http_core_module);
    if (r->gzip_vary && clcf->gzip_vary) {
        ngx_str_t value = ngx_string("Accept-Encoding");

        ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity_module);

#if defined(MODSECURITY_SANITY_CHECKS) && (MODSECURITY_SANITY_CHECKS)
        ngx_http_modsecurity_store_ctx_header(r, &name, &value);
#endif

        return msc_add_n_response_header(ctx->modsec_transaction,
            (const unsigned char *) name.data,
            name.len,
            (const unsigned char *) value.data,
            value.len);
    }
#endif

    return 1;
}

ngx_int_t
ngx_http_modsecurity_header_filter_init(void)
{
    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_modsecurity_header_filter;

    return NGX_OK;
}

void NoOpDeallocator(void* data, size_t a, void* b) {};

ngx_int_t
ngx_http_modsecurity_header_filter(ngx_http_request_t *r)
{
    

    ngx_http_modsecurity_ctx_t *ctx;
    ngx_list_part_t *part = &r->headers_out.headers.part;
    ngx_table_elt_t *data = part->elts;
    ngx_uint_t i = 0;
    int ret = 0;
    ngx_uint_t status;
    char *http_response_ver;
    ngx_pool_t *old_pool;


/* XXX: if NOT_MODIFIED, do we need to process it at all?  see xslt_header_filter() */

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity_module);

    dd("header filter, recovering ctx: %p", ctx);

    if (ctx == NULL)
    {
        dd("something really bad happened or ModSecurity is disabled. going to the next filter.");
        return ngx_http_next_header_filter(r);
    }
    // fprintf(stderr," First return2");

    if (ctx->intervention_triggered) {
        return ngx_http_next_header_filter(r);
    }

/* XXX: can it happen ?  already processed i mean */
/* XXX: check behaviour on 'ModSecurity off' */

    if (ctx && ctx->processed)
    {
        /*
         * FIXME: verify if this request is already processed.
         */
        dd("Already processed... going to the next header...");
        return ngx_http_next_header_filter(r);
    }

    /*
     * Lets ask nginx to keep the response body in memory
     *
     * FIXME: I don't see a reason to keep it `1' when SecResponseBody is disabled.
     */
    r->filter_need_in_memory = 1;

    ctx->processed = 1;
    /*
     *
     * Assuming ModSecurity module is running immediately before the
     * ngx_http_header_filter, we will be able to populate ModSecurity with
     * headers from the headers_out structure.
     *
     * As ngx_http_header_filter place a direct call to the
     * ngx_http_write_filter_module, we cannot hook between those two. In order
     * to enumerate all headers, we first look at the headers_out structure,
     * and later we look into the ngx_list_part_t. The ngx_list_part_t must be
     * checked. Other module(s) in the chain may added some content to it.
     *
     */
    for (i = 0; ngx_http_modsecurity_headers_out[i].name.len; i++)
    {
        dd(" Sending header to ModSecurity - header: `%.*s'.",
            (int) ngx_http_modsecurity_headers_out[i].name.len,
            ngx_http_modsecurity_headers_out[i].name.data);

                ngx_http_modsecurity_headers_out[i].resolver(r,
                    ngx_http_modsecurity_headers_out[i].name,
                    ngx_http_modsecurity_headers_out[i].offset);
    }

    for (i = 0 ;; i++)
    {
        if (i >= part->nelts)
        {
            if (part->next == NULL) {
                break;
            }

            part = part->next;
            data = part->elts;
            i = 0;
        }

#if defined(MODSECURITY_SANITY_CHECKS) && (MODSECURITY_SANITY_CHECKS)
        ngx_http_modsecurity_store_ctx_header(r, &data[i].key, &data[i].value);
#endif

        /*
         * Doing this ugly cast here, explanation on the request_header
         */
        msc_add_n_response_header(ctx->modsec_transaction,
            (const unsigned char *) data[i].key.data,
            data[i].key.len,
            (const unsigned char *) data[i].value.data,
            data[i].value.len);
    }

    /* prepare extra paramters for msc_process_response_headers() */
    if (r->err_status) {
        status = r->err_status;
    } else {
        status = r->headers_out.status;
    }

    /*
     * NGINX always sends HTTP response with HTTP/1.1, except cases when
     * HTTP V2 module is enabled, and request has been posted with HTTP/2.0.
     */
    http_response_ver = "HTTP 1.1";
#if (NGX_HTTP_V2)
    if (r->stream) {
        http_response_ver = "HTTP 2.0";
    }
#endif

    old_pool = ngx_http_modsecurity_pcre_malloc_init(r->pool);
    msc_process_response_headers(ctx->modsec_transaction, status, http_response_ver);
    ngx_http_modsecurity_pcre_malloc_done(old_pool);
    
    ret = ngx_http_modsecurity_process_intervention(ctx->modsec_transaction, r, 0);
    //fprintf(stderr,"\nResult of ngx_http_modsecurity_process_intervention: %i", ret );
    if (r->error_page) {
        // fprintf(stderr,"\nIn if 1");
        return ngx_http_next_header_filter(r);
    }
    if (ret > 0) {
        // fprintf(stderr,"\nIn if 2");
        return ngx_http_filter_finalize_request(r, &ngx_http_modsecurity_module, ret);
    }
    if(r->headers_out.status != NGX_HTTP_FORBIDDEN){


    
    clock_t start_time1, end_time1;
    double total_time1;
    start_time1 = clock();

    ///// Tokenizer ////
    /// Tu khoa trainning ///
    const int MAX_NUMBER_OF_KEYWORDS = 90;
    char keyname[100] = "/home/modsecurity/tokenizer/keywords.txt";
    FILE *kptr =  fopen(keyname, "r");
    if(kptr==NULL)
    {
        //fprintf(stderr,"File keywords does not exist");
        exit(1);
        
    }
    char **KEYWORDS_IN_TRAINING;
    KEYWORDS_IN_TRAINING = (char**) malloc(sizeof(char*) * MAX_NUMBER_OF_KEYWORDS); // Allocate memory for KEYWORDS_IN_TRAINING
    if(KEYWORDS_IN_TRAINING == NULL)
    {
        //fprintf(stderr,"Memory allocation failed");
        fclose(kptr); // Close the file before returning
        exit(1);
    } 
    
    for (int i = 0; i < MAX_NUMBER_OF_KEYWORDS; i++) {
        KEYWORDS_IN_TRAINING[i] = (char*) malloc(sizeof(char) * 100); // Allocate memory for each line in KEYWORDS_IN_TRAINING
        if(KEYWORDS_IN_TRAINING[i] == NULL)
        {
            //fprintf(stderr,"Memory allocation failed");
            fclose(kptr); // Close the file before returning
            // Free the memory allocated so far
            for (int j = 0; j < i; j++) {
                free(KEYWORDS_IN_TRAINING[j]);
            }
            free(KEYWORDS_IN_TRAINING);
        }
    }
    int number_line_keyword = 0;
    while(fgets(KEYWORDS_IN_TRAINING[number_line_keyword], 100, kptr))
    {
        KEYWORDS_IN_TRAINING[number_line_keyword][strlen(KEYWORDS_IN_TRAINING[number_line_keyword]) - 1] = '\0';
        number_line_keyword++;
    }
    fclose(kptr); // Close the file after reading
    // KEYWORDS_IN_TRAINING now contains the lines read from the file
    fprintf(stderr,"\nnumber_line_keyword: %d", number_line_keyword);
    //Print out the keywords
    fprintf(stderr, "\nKEYWORDS_IN_TRAINING read from file: \n");
    for (int i = 0; i < number_line_keyword; i++) {
        fprintf(stderr, "%s\n", KEYWORDS_IN_TRAINING[i]);
    }



    /// Ky tu dac biet ///
    char *punctuations = "/+?&;=,()<>*!$#|^{}\\~@.`[]:'\"";



    /// Read pattern from file ///
    const int MAX_NUMBER_OF_PATTERNS = 350;
    char fname[100] = "/home/modsecurity/tokenizer/token97.txt";
    FILE *fptr =  fopen(fname, "r");
    if(fptr==NULL)
    {
        //fprintf(stderr,"File token does not exist");
        exit(1);
        
    }
    char **pattern;
    pattern = (char**) malloc(sizeof(char*) * MAX_NUMBER_OF_PATTERNS); // Allocate memory for pattern
    if(pattern == NULL)
    {
        //fprintf(stderr,"Memory allocation failed");
        fclose(fptr); // Close the file before returning
        exit(1);
        
    } 
    int number_line_pattern = 0;
    for (int i = 0; i < MAX_NUMBER_OF_PATTERNS; i++) {
        pattern[i] = (char*) malloc(sizeof(char) * 100); // Allocate memory for each line in pattern
        if(pattern[i] == NULL)
        {
            //fprintf(stderr,"Memory allocation failed");
            fclose(fptr); // Close the file before returning
            // Free the memory allocated so far
            for (int j = 0; j < i; j++) {
                free(pattern[j]);
            }
            free(pattern);
        }
    }
    while (fgets(pattern[number_line_pattern], 100, fptr)) {
        pattern[number_line_pattern][strlen(pattern[number_line_pattern]) - 1] = '\0';
        //printf("Pattern %d: %s\n", number_line_pattern, pattern[number_line_pattern]);
        number_line_pattern++;
    }
    fclose(fptr); // Close the file after reading
    //pattern now contains the lines read from the file
    fprintf(stderr,"\nNumber_line_pattern: %d", number_line_pattern);
    //Print out the patterns
    fprintf(stderr, "\nPatterns read from file: \n");
    for (int i = 0; i < number_line_pattern; i++) {
        fprintf(stderr, "%s\n", pattern[i]);
    }




    // Read extension from file //
    int MAX_NUMBER_OF_EXTENSIONS = 2500; 
    char extname[100] = "/home/modsecurity/tokenizer/extension.txt";
    FILE *ext_ptr = fopen(extname,"r");
    if(ext_ptr==NULL)
    {
        //printf("File extension does not exist");
        exit(1);
    }
    char **ext;
    ext = (char**) malloc(sizeof(char*) * MAX_NUMBER_OF_EXTENSIONS); // Allocate memory for ext
    if(ext == NULL)
    {
        //fprintf(stderr,"Memory allocation failed");
        fclose(ext_ptr); // Close the file before returning
        exit(1);
        
    } 
    for (int i = 0; i < MAX_NUMBER_OF_EXTENSIONS; i++) {
        ext[i] = (char*) malloc(sizeof(char) * 100); // Allocate memory for each line in ext
        if(ext[i] == NULL)
        {
            //fprintf(stderr,"Memory allocation failed");
            fclose(ext_ptr); // Close the file before returning
            // Free the memory allocated so far
            for (int j = 0; j < i; j++) {
                free(ext[j]);
            }
            free(ext);
            
        }
    }
    int number_line_ext = 0;
    while(fgets(ext[number_line_ext], 100, ext_ptr))
	{
        ext[number_line_ext][strlen(ext[number_line_ext]) - 1] = '\0';
        number_line_ext++;
    }
    fclose(ext_ptr); // Close the file after reading
    // ext now contains the lines read from the file 
    fprintf(stderr,"\nNumber_line_ext: %d", number_line_ext);
    //Print out the patterns
    fprintf(stderr, "\nExtensions read from file: \n");
    for (int i = 0; i < number_line_ext; i++) {
        fprintf(stderr, "%s\n", ext[i]);
    }


    /// Trich xuat thong tin url(noi dung, do dai) ///
    int len_path = r->uri.len; // do dai path
    u_char *raw_data_uri = ngx_pcalloc(r->pool,len_path);
    ngx_memcpy(raw_data_uri,r->uri.data,len_path);
    u_char *data_uri = malloc(len_path); // noi dung path
    strncpy((char*)data_uri,(char*)raw_data_uri,len_path);

    /// Xu ly duong dan ///
    char* parse_path;
    parse_path = (char*) malloc(sizeof(char) * 1000);
    handle_path(parse_path, (char*)data_uri,len_path,punctuations);


    /// Xu ly mang path ///
    int len_path_arr = 0;
    int* token_path;
    token_path = (int*) malloc(sizeof(int) * 100);
    tokenizer_path(token_path, parse_path, pattern, number_line_pattern, punctuations, ext, number_line_ext, &len_path_arr);
    // Print out token path array after tokenization
    fprintf(stderr,"\nToken_path: ");
    for(int i=0; i<len_path_arr; i++){
        fprintf(stderr," %i ", token_path[i]);
    }

    /// Xu ly body trong duong dan ///
    int len_param = r->args.len;
    fprintf(stderr,"\nLen param: %li", r->args.len);
    u_char *raw_data_param = ngx_pcalloc(r->pool,len_param);
    ngx_memcpy(raw_data_param,r->args.data,len_param);
    u_char *data_param = malloc(len_param+1);
    strncpy((char*)data_param,(char*)raw_data_param,len_param);
    data_param[len_param] = '\0';
    fprintf(stderr,"\nData param: %s", data_param);


    int len_param_arr = 0;
    int* token_param;
    token_param = (int*) malloc(sizeof(int) * 200);
    char* parse_param;
    parse_param = (char*) malloc(sizeof(char) * 10000);
    if(len_param > 0)
    {
        u_char *decoded_param = urlDecode(data_param);
        while (true)
        {
            u_char *tmp_decoded_param = urlDecode(decoded_param);
            if (strcmp((char*) tmp_decoded_param, (char*) decoded_param) == 0) 
                break; 
            else 
                decoded_param = tmp_decoded_param;
        }
        
        fprintf(stderr,"\nDecoded_param: %s", decoded_param);
        handle_data(parse_param, (char*)decoded_param,strlen((const char*)decoded_param),punctuations);
        //fprintf(stderr,"\nparse_param: %s", parse_param);
        //parse_param = (char*)decoded_param;
        fprintf(stderr,"\nParse_param: %s", parse_param);
        tokenizer_data(token_param, toLower(parse_param), pattern, number_line_pattern, &len_param_arr, punctuations, KEYWORDS_IN_TRAINING, number_line_keyword); 
        fprintf(stderr,"\nToken_param: ");
        for(int i=0; i<len_param_arr; i++){
            fprintf(stderr," %i ", token_param[i]);
        }
    }


    int len_tokenizer = len_path_arr + len_param_arr;
    float final_tokenizer[556]; // kich thuoc toi da la 556 === kich thuoc toi da cua model
    fprintf(stderr,"\nFinal_tokennizer: ");
    for(int i =0; i<556 ; i++)
    {
        if(i < len_path_arr){
            final_tokenizer[i] = (float)token_path[i];
            fprintf(stderr,"%f ",final_tokenizer[i]);
           }
        else if(i>=len_path_arr && i < len_tokenizer){
            final_tokenizer[i] = (float)token_param[i-len_path_arr];
            fprintf(stderr,"%f ",final_tokenizer[i]);
            }
        else 
            final_tokenizer[i] = 0.0; // cac phan tu con lai gan bang 0
    }


    end_time1 = clock();
    total_time1 = (double)(end_time1 - start_time1) / CLOCKS_PER_SEC;
    fprintf(stderr, "\nThoi gian chay sau khi tien xu ly du lieu: %f giay\n", total_time1);
   
    //fprintf(stderr,"Len of tokenizer: %f %f %f %f %f abctest", final_tokenizer[0], final_tokenizer[1], final_tokenizer[2], final_tokenizer[3], final_tokenizer[4]);

   
    /////////

    /*
     * Proxies will not like this... but it is necessary to unset
     * the content length in order to manipulate the content of
     * response body in ModSecurity.
     *
     * This header may arrive at the client before ModSecurity had
     * a change to make any modification. That is why it is necessary
     * to set this to -1 here.
     *
     * We need to have some kind of flag the decide if ModSecurity
     * will make a modification or not. If not, keep the content and
     * make the proxy servers happy.
     *
     */

    /*
     * The line below is commented to make the spdy test to work
     */
     //r->headers_out.content_length_n = -1;

    clock_t start_time2, end_time2;
    double total_time2;
    start_time2 = clock();


    // Graph = TF_NewGraph();
    // Status = TF_NewStatus();

    // //Create session options and buffer for run options
    // SessionOpts = TF_NewSessionOptions();
    // RunOpts = NULL;


 
    
    // Neu phien session chua co thi moi goi ham loadSessionFromSavedModel de load session va cac thong tin khac
    if (session == NULL) 
    {
        int loadStatus = loadSessionFromSavedModel(saved_model_dir);
        if (loadStatus != 0) {
            fprintf(stderr, "Lỗi khi tải phiên (session) từ mô hình đã lưu.\n");
            return -1;
        }
    }
    
    end_time2 = clock();
    total_time2 = (double)(end_time2 - start_time2) / CLOCKS_PER_SEC;
    fprintf(stderr, "\nThoi gian chay khi load session tu model va cac thong tin khac: %f giay\n", total_time2);


/*     const char* saved_model_dir_session = "/home/modsecurity/"
    // Lưu phiên (session) vào SavedModel
    int saveStatus = saveSessionToSavedModel(session, saved_model_dir_session);
    if (saveStatus != 0) {
        fprintf(stderr, "Lỗi khi lưu phiên (session) vào SavedModel.\n");
        // Xử lý lỗi
        return -1;
    } */

    // Khoi tao runStatus luu tru trang thai cho cac hanh dong phia sau
    TF_Status* runStatus = TF_NewStatus();
    //fprintf(stderr, "run status: %ld\n", runStatus);
    

    // Sử dụng phiên đã tải để thực hiện tính toán
    // ...
    // Sử dụng biến "session" để thực hiện các phương thức TensorFlow khác
    


    //Session = TF_LoadSessionFromSavedModel(SessionOpts, RunOpts, saved_model_dir, &tags, ntags, Graph, NULL, Status);
    /* fprintf(stderr, "SessionOpts: %p\n", SessionOpts);
    fprintf(stderr, "RunOpts: %p\n", RunOpts);
    fprintf(stderr, "saved_model_dir: %s\n", saved_model_dir);
    fprintf(stderr, "tags: ");
    for (int i = 0; i < ntags; i++) {
        fprintf(stderr, "%d ", tags[i]);
    }
    fprintf(stderr, "\n");
    fprintf(stderr, "ntags: %d\n", ntags);
    fprintf(stderr, "Graph: %p\n", Graph);
    fprintf(stderr, "NULL: %p\n", NULL);
    fprintf(stderr, "Status: %p\n", Status); */
    
    //TF_Status* reset_status = TF_NewStatus();
    //TF_Reset(Session, NULL, reset_status);



    // // Save session
    // const char* saved_model_file = "/home/modsecurity/save_session";
    // TF_Status* status = TF_NewStatus();
    // TF_SaveSession(Session, SessionOpts, saved_model_file, &tags, ntags, status);
    // if (TF_GetCode(status) != TF_OK) {
    //     fprintf(stderr, "Error saving session: %s", TF_Message(status));
    //     // Handle error
    // }
    // TF_DeleteStatus(status);


    // //To load the saved session, you can use the TF_LoadSessionFromSavedModel() function
    // Session = TF_LoadSessionFromSavedModel(SessionOpts, RunOpts, saved_model_file, &tags, ntags, Graph, NULL, Status);
    // if (TF_GetCode(Status) != TF_OK) {
    //     fprintf(stderr, "Error loading session: %s", TF_Message(Status));
    //     // Handle error
    // }




   
/*     // Load the model into the graph and session objects if session is not already created
    if (Session == NULL) {
        //Create session options and buffer for run options
        TF_SessionOptions* SessionOpts = TF_NewSessionOptions();
        TF_Buffer* RunOpts = NULL;

        const char* saved_model_dir = "/home/modsecurity/model1/";
        const char* tags = "serve";
        int ntags = 1;

        // Load the model into the graph and session objects
        Session = TF_LoadSessionFromSavedModel(SessionOpts, RunOpts, saved_model_dir, &tags, ntags, Graph, NULL, Status);

        // Free session options and buffer for run options
        TF_DeleteSessionOptions(SessionOpts);
        TF_DeleteBuffer(RunOpts);
    } */

    // ... Run inference using the Session object within a loop ...

    clock_t start_time3, end_time3;
    double total_time3;
    start_time3 = clock();
    // Find input and output tensors
    TF_Output input_tensor = {TF_GraphOperationByName(graph, "serving_default_input_21"), 0};
    TF_Output output_tensor = {TF_GraphOperationByName(graph, "StatefulPartitionedCall"), 0};
    if (input_tensor.oper == NULL) {
        fprintf(stderr, "Failed to find input tensor '%s'\n", "serving_default_input_21");
        
    }
    if (output_tensor.oper == NULL) {
        fprintf(stderr, "Failed to find output tensor '%s'\n", "StatefulPartitionedCall");
        
    }
    end_time3 = clock();
    total_time3 = (double)(end_time3 - start_time3) / CLOCKS_PER_SEC;
    fprintf(stderr, "\nThoi gian chay khi khoi tao input va output tensor: %f giay\n", total_time3);




    clock_t start_time4, end_time4;
    double total_time4;
    start_time4 = clock();
    // Allocate data for inputs & outputs
    //len_final_tokenizer = 556 - max 
    const int64_t input_dims[] = {1, 100}; // Co the xuong thap hon 556 de toi uu thoi gian chay
    const int64_t output_dims[] = {1, 2};
    const size_t input_size_bytes = 100 * sizeof(float);  // Co the xuong thap hon 556 de toi uu thoi gian chay
    const size_t output_size_bytes = 2 * sizeof(float);

    TF_Tensor* input_tensor_value = TF_NewTensor(TF_FLOAT, input_dims, 2, final_tokenizer, input_size_bytes, &NoOpDeallocator, 0);
    if (input_tensor_value == NULL) {
        fprintf(stderr, "Failed to allocate input tensor\n");
        //return false;
    }
    TF_Tensor* output_tensor_value = TF_AllocateTensor(TF_FLOAT, output_dims, 2, output_size_bytes);
    if (output_tensor_value == NULL) {
        fprintf(stderr, "Failed to allocate output tensor\n");
        TF_DeleteTensor(input_tensor_value);
        //return false;
    }

    end_time4 = clock();
    total_time4 = (double)(end_time4 - start_time4) / CLOCKS_PER_SEC;
    fprintf(stderr, "\nThoi gian chay khi cung cap du lieu (bytes) cho input va output tensor: %f giay\n", total_time4);

    /* int NumInputs = 1;
    TF_Output* Input = (TF_Output*)malloc(sizeof(TF_Output) * NumInputs);
    TF_Output t0 = {TF_GraphOperationByName(Graph, "serving_default_input_21"), 0};
    if(t0.oper == NULL)
        fprintf(stderr,"\nERROR: Failed TF_GraphOperationByName serving_default_input_21\n");
    else
        fprintf(stderr,"\nTF_GraphOperationByName serving_default_input_21 is OK\n");
    Input[0] = t0;



    int NumOutputs = 1;
    TF_Output* Output = (TF_Output*)malloc(sizeof(TF_Output) * NumOutputs);

    TF_Output t2 = {TF_GraphOperationByName(Graph, "StatefulPartitionedCall"), 0};
    if(t2.oper == NULL)
        fprintf(stderr,"\nERROR: Failed TF_GraphOperationByName StatefulPartitionedCall\n");
    else	
        fprintf(stderr,"TF_GraphOperationByName StatefulPartitionedCall is OK\n");
    Output[0] = t2; */




   /*  // Allocate data for inputs & outputs
    TF_Tensor** InputValues = (TF_Tensor**)malloc(sizeof(TF_Tensor*)*NumInputs);
    TF_Tensor** OutputValues = (TF_Tensor**)malloc(sizeof(TF_Tensor*)*NumOutputs);

    int ndims = 2;
    int64_t dims[] = {1,556};
    int ndata = sizeof(float)*1*556 ;// This is tricky, it number of bytes not number of element

    TF_Tensor* int_tensor = TF_NewTensor(TF_FLOAT, dims, ndims, final_tokenizer, ndata, &NoOpDeallocator, 0);
    if (int_tensor != NULL)
    {
        fprintf(stderr,"TF_NewTensor is OK\n");
    }
    else
        fprintf(stderr,"ERROR: Failed TF_NewTensor\n");
    
    InputValues[0] = int_tensor; */

    clock_t start_time5, end_time5;
    double total_time5;
    start_time5 = clock();


    // Run the session
    TF_SessionRun(session, NULL, &input_tensor, &input_tensor_value, 1, &output_tensor, &output_tensor_value, 1, NULL, 0, NULL, runStatus);


    end_time5 = clock();
    total_time5 = (double)(end_time5 - start_time5) / CLOCKS_PER_SEC;
    fprintf(stderr, "\nThoi gian chay khi run session: %f giay\n", total_time5);
    
    //Run the Session
    //TF_SessionRun(Session, NULL, input_tensors, input_values, num_inputs, output_tensors, output_values, num_outputs, target_op_names, num_targets, NULL, Status)
/*     TF_SessionRun(Session, NULL, Input, InputValues, NumInputs, Output, OutputValues, NumOutputs, NULL, 0,NULL , Status);
    if(TF_GetCode(Status) == TF_OK)
    {
        //fprintf(stderr,"Session is OK\n");
    }
    else
    {
        fprintf(stderr,"%s",TF_Message(Status));
    } */

    clock_t start_time6, end_time6;
    double total_time6;
    start_time6 = clock();


    // // Giải phóng phiên (session) khi không cần thiết
    // if (session != NULL) {
    //     TF_DeleteSession(session, NULL);
    //     session = NULL;
    // }

    //Free memory (Khong giai phong)
    //TF_DeleteGraph(graph);
    //TF_CloseSession(session, runStatus);
    //TF_DeleteSession(session, runStatus);
    //TF_DeleteSessionOptions(session_opts);
    //TF_DeleteStatus(runStatus);

    void* output_data = TF_TensorData(output_tensor_value);
    float* prediction_scores = (float*)output_data;
    fprintf(stderr, "Prediction scores: %f, %f\n", prediction_scores[0], prediction_scores[1]);
    end_time6 = clock();
    total_time6 = (double)(end_time6 - start_time6) / CLOCKS_PER_SEC;
    fprintf(stderr, "\nThoi gian chay khi dua ra du doan: %f giay\n", total_time6);

   /*  void* buff = TF_TensorData(OutputValues[0]);
    float* offsets = (float*)buff;
    fprintf(stderr,"Percent of 0: %f and percent of 1: %f",offsets[0], offsets[1]); */

    double total = (double)total_time1 + (double)total_time2 + (double)total_time3 + (double)total_time4 + (double)total_time5 + (double)total_time6;
    fprintf(stderr, "\nTONG THOI GIAN CHAY CUA TOAN BO MODULE MACHINE LEARNING MODEL: %f giay", total);
   
    if (prediction_scores[0] > prediction_scores[1])
    {
        return NGX_HTTP_FORBIDDEN;
    }

    /* if(offsets[0] > offsets[1])
    {
        //ngx_log_error(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,"block by module A");
        return NGX_HTTP_FORBIDDEN;
    }
 */
}
    

    return ngx_http_next_header_filter(r);
}
