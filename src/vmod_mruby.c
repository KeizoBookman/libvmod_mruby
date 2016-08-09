#include "vmod_mruby.h"
#include "vmod_class.h"

#include "pthread.h"

static void make_key();

typedef struct mrb_vcl_ctx_t {
    mrb_state *mrb;
}mrb_vcl_ctx_t;

static mrb_vcl_ctx_t* mrb_vcl_ctx_new()
{
    mrb_vcl_ctx_t *ctx = malloc( sizeof(mrb_vcl_ctx_t) );
    ctx->mrb = mrb_open();
    return ctx;
}

static void mrb_vcl_ctx_close(void *p)
{
    mrb_vcl_ctx_t *ctx = (mrb_vcl_ctx_t*)p;
    mrb_close(ctx->mrb);
    free(ctx);
}



typedef void (*thread_desruct_t)(void*);

static pthread_once_t thread_once = PTHREAD_ONCE_INIT;
pthread_key_t thread_vm_key ;
pthread_key_t thread_req_key;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


mrb_vcl_ctx_t *get_vcl_ctx()
{
    return (mrb_vcl_ctx_t*)pthread_getspecific(thread_vm_key);
}

int
init_function(struct vmod_priv *priv, const struct VCL_conf *conf)
{

    pthread_once(&thread_once, make_key);

	return 0;
}

static void make_key()
{
    pthread_key_create(&thread_vm_key, (thread_desruct_t)mrb_vcl_ctx_close);
    pthread_key_create(&thread_req_key, (thread_desruct_t)mrb_vcl_ctx_close);
}


VCL_STRING vmod_exec_str(VRT_CTX, struct vmod_priv *priv, VCL_STRING code)
{
    CHECK_OBJ_NOTNULL(ctx, VRT_CTX_MAGIC);

    mrb_state *mrb = mrb_open();
    mrb_value v = mrb_load_string(mrb,code); 
    if(!mrb_string_p(v))
    {
        v = mrb_obj_as_string(mrb,v);
    }

    mrb_close(mrb);
    return RSTRING_PTR(v);
}

VCL_INT vmod_exec_int(VRT_CTX, struct vmod_priv *priv, VCL_STRING code)
{
    CHECK_OBJ_NOTNULL(ctx, VRT_CTX_MAGIC);

    mrb_state *mrb = mrb_open();
    mrb_value v = mrb_load_string(mrb, code);
    if(!mrb_fixnum_p(v))
    {
        return 0;
    }
    mrb_close(mrb);
    return mrb_fixnum(v);
}

VCL_VOID vmod_exec(VRT_CTX, struct vmod_priv *priv, VCL_STRING code)
{
    CHECK_OBJ_NOTNULL(ctx, VRT_CTX_MAGIC);

    mrb_state *mrb = mrb_open();
    mrb_load_string(mrb,code);
    mrb_close(mrb);
    return ;

}


VCL_INT vmod_init(VRT_CTX, struct vmod_priv *priv)
{
    CHECK_OBJ_NOTNULL(ctx, VRT_CTX_MAGIC);
    mrb_vcl_ctx_t  *mrb = mrb_vcl_ctx_new();
    if(!mrb)
    {
        pthread_mutex_lock(&mutex);
        mrb = mrb_vcl_ctx_new();
        mrb_define_vcl_class(mrb->mrb);
        pthread_mutex_unlock(&mutex);
        pthread_setspecific(thread_vm_key, mrb);
    }

    return 0;
}


VCL_INT vmod_handler(VRT_CTX, struct vmod_priv *priv, VCL_STRING path)
{
    CHECK_OBJ_NOTNULL(ctx, VRT_CTX_MAGIC);
    FILE *fp;
    mrb_vcl_ctx_t *mrb = get_vcl_ctx();
    struct mrb_parser_state *parser;
    struct RProc *proc;
    if(!mrb)
    {
        pthread_mutex_lock(&mutex);
        mrb = mrb_vcl_ctx_new();
        pthread_mutex_unlock(&mutex);
        pthread_setspecific(thread_vm_key, mrb);
    }
    mrb->mrb->ud = (void*)ctx;
    fp = fopen(path, "r");
    if(NULL == fp )
    {
        return -1;
    }

    pthread_mutex_lock(&mutex);
    parser = mrb_parse_file(mrb->mrb, fp, NULL);
    pthread_mutex_unlock(&mutex);

    fclose(fp);
    if(NULL == parser)
    {
        return -1;
    }

    proc = mrb_generate_code(mrb->mrb, parser);
    mrb_pool_close(parser->pool);
    if( NULL == proc)
    {
        return -1;
    }
    mrb_run(mrb->mrb, proc, mrb_top_self(mrb->mrb));
    if(mrb->mrb->exc)
    {
        return -1;
    }
    return 0;
}

