#include "vmod_mruby.h"
#include "vmod_class.h"
#include "vrt_obj.h"
#include "vsa.h"

#include <pthread.h>


static inline struct RClass* mrb_define_class_under_varnish(mrb_state *mrb, char *name)
{
    struct RClass *varnish, *child;
    varnish = mrb_class_get(mrb, "Varnish"); 
    child =  mrb_define_class_under(mrb, varnish, name, mrb->object_class);
    MRB_SET_INSTANCE_TT(child, MRB_TT_DATA);
    return child;
}




static mrb_value mrb_vcl_beresp_init(mrb_state *mrb, mrb_value self)
{
    return self;
}

static mrb_value mrb_vcl_beresp_backend_ip(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_str_new_cstr(mrb, VRT_IP_string(ctx, VRT_r_beresp_backend_ip(ctx)));
}

static mrb_value mrb_vcl_beresp_backend_name(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;

    return mrb_str_new_cstr(mrb, VRT_r_beresp_backend_name(ctx));
}

static mrb_value mrb_vcl_beresp_backend_port(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_fixnum_value( VSA_Port(VRT_r_beresp_backend_ip(ctx)));
}

static mrb_value mrb_vcl_beresp_do_esi(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_fixnum_value( VRT_r_beresp_do_esi(ctx));
}

static mrb_value mrb_vcl_beresp_do_gunzip(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_fixnum_value( VRT_r_beresp_do_gunzip(ctx));
}

static mrb_value mrb_vcl_beresp_do_gzip(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_fixnum_value( VRT_r_beresp_do_gzip(ctx));
}

static mrb_value mrb_vcl_beresp_do_stream(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_fixnum_value( VRT_r_beresp_do_stream(ctx) );
}

static mrb_value mrb_vcl_beresp_grace(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_float_value(mrb, VRT_r_beresp_grace(ctx));
}

static mrb_value mrb_vcl_beresp_keep(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_float_value(mrb, VRT_r_beresp_keep(ctx));
}

static mrb_value mrb_vcl_beresp_proto(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_str_new_cstr(mrb, VRT_r_beresp_proto(ctx));
}

static mrb_value mrb_vcl_beresp_reason(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_str_new_cstr(mrb, VRT_r_beresp_reason(ctx));
}

static mrb_value mrb_vcl_beresp_status(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_fixnum_value( VRT_r_beresp_status(ctx));
}

static mrb_value mrb_vcl_beresp_storage_hint(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_str_new_cstr(mrb, VRT_r_beresp_storage_hint(ctx));
}

static mrb_value mrb_vcl_beresp_ttl(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_fixnum_value(VRT_r_beresp_ttl(ctx));
}

static mrb_value mrb_vcl_beresp_was_304(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_fixnum_value( VRT_r_beresp_was_304(ctx));

}

void mrb_define_vcl_beresp_class(mrb_state *mrb)
{
    struct RClass *beresp;
    beresp = mrb_define_class_under_varnish(mrb, "Beresp");

    mrb_define_method(mrb, beresp,  "initialize", mrb_vcl_beresp_init,   MRB_ARGS_NONE());
    mrb_define_method(mrb, beresp,  "backend_ip", mrb_vcl_beresp_backend_ip, MRB_ARGS_NONE());
    mrb_define_method(mrb, beresp,  "backend_port", mrb_vcl_beresp_backend_port, MRB_ARGS_NONE());
    mrb_define_method(mrb, beresp,  "backend_name", mrb_vcl_beresp_backend_name, MRB_ARGS_NONE());
    mrb_define_method(mrb, beresp,  "do_esi", mrb_vcl_beresp_do_esi, MRB_ARGS_NONE());
    mrb_define_method(mrb, beresp,  "do_gunzip", mrb_vcl_beresp_do_gunzip, MRB_ARGS_NONE());
    mrb_define_method(mrb, beresp,  "do_gzip", mrb_vcl_beresp_do_gzip, MRB_ARGS_NONE());
    mrb_define_method(mrb, beresp,  "do_stresam", mrb_vcl_beresp_do_stream, MRB_ARGS_NONE());
    mrb_define_method(mrb, beresp,  "grace", mrb_vcl_beresp_grace, MRB_ARGS_NONE());
    mrb_define_method(mrb, beresp,  "keep", mrb_vcl_beresp_keep, MRB_ARGS_NONE());
    mrb_define_method(mrb, beresp,  "proto", mrb_vcl_beresp_proto, MRB_ARGS_NONE());
    mrb_define_method(mrb, beresp,  "reason",mrb_vcl_beresp_reason, MRB_ARGS_NONE());
    mrb_define_method(mrb, beresp,  "status", mrb_vcl_beresp_status, MRB_ARGS_NONE());
    mrb_define_method(mrb, beresp,  "storage_hint", mrb_vcl_beresp_storage_hint, MRB_ARGS_NONE());
    mrb_define_method(mrb, beresp,  "ttl", mrb_vcl_beresp_ttl, MRB_ARGS_NONE());
    mrb_define_method(mrb, beresp,  "was_304", mrb_vcl_beresp_was_304, MRB_ARGS_NONE());

    return ;
}


static mrb_value mrb_vcl_bereq_between_bytes_timeout(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    double timeout = VRT_r_bereq_between_bytes_timeout(ctx);
    return mrb_float_value(mrb, timeout);
}

static mrb_value mrb_vcl_bereq_connect_timeout(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    double timeout = VRT_r_bereq_connect_timeout(ctx);
    return mrb_float_value(mrb, timeout);
}

static mrb_value mrb_vcl_bereq_first_byte_timeout(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    double timeout = VRT_r_bereq_first_byte_timeout(ctx);
    return mrb_float_value(mrb, timeout);
}

static mrb_value mrb_vcl_bereq_method(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    const char *name = VRT_r_bereq_method(ctx);
    return mrb_str_new_cstr(mrb,name);
}

static mrb_value mrb_vcl_bereq_proto(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    const char *name = VRT_r_bereq_proto(ctx);
    return mrb_str_new_cstr(mrb, name);
}

static mrb_value mrb_vcl_bereq_retries(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    const int count = VRT_r_bereq_retries(ctx);
    return mrb_fixnum_value(count);
}

static mrb_value mrb_vcl_bereq_uncacheable(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    int val;
    val = VRT_r_bereq_uncacheable(ctx);
    if(0 == val)
    {
        return mrb_true_value();
    }else{
        return mrb_false_value();

    }
}

static mrb_value mrb_vcl_bereq_url(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    const char *url = VRT_r_bereq_url(ctx);
    return mrb_str_new_cstr(mrb, url);
}

static mrb_value mrb_vcl_bereq_xid(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    const char *xid = VRT_r_bereq_xid(ctx);
    return mrb_str_new_cstr(mrb, xid);

}


void mrb_define_vcl_bereq_class(mrb_state *mrb)
{
    struct RClass *bereq;
    bereq = mrb_define_class_under_varnish(mrb, "Bereq");

    mrb_define_method(mrb, bereq, "between_bytes_timeout",  mrb_vcl_bereq_between_bytes_timeout,    MRB_ARGS_NONE());
    mrb_define_method(mrb, bereq, "connect_timeout",        mrb_vcl_bereq_connect_timeout,          MRB_ARGS_NONE());
    mrb_define_method(mrb, bereq, "first_byte_timeout",     mrb_vcl_bereq_first_byte_timeout,       MRB_ARGS_NONE());
    mrb_define_method(mrb, bereq, "proto",                  mrb_vcl_bereq_proto,                    MRB_ARGS_NONE());
    mrb_define_method(mrb, bereq, "retries",                mrb_vcl_bereq_retries,                  MRB_ARGS_NONE());
    mrb_define_method(mrb, bereq, "uncacheable",            mrb_vcl_bereq_uncacheable,              MRB_ARGS_NONE());
    mrb_define_method(mrb, bereq, "url",                    mrb_vcl_bereq_url,                      MRB_ARGS_NONE());
    mrb_define_method(mrb, bereq, "xid",                    mrb_vcl_bereq_xid,                      MRB_ARGS_NONE());
    mrb_define_method(mrb, bereq, "method",                 mrb_vcl_bereq_method,                   MRB_ARGS_NONE());
    // maybe, http_get is hard to impl.
    // mrb_define_method(mrb, bereq, "http_get",               mrb_vcl_bereq_http_get,                 MRB_ARGS_NONE());


    return ;

}

static mrb_value mrb_vcl_resp_is_streaming(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    VCL_BOOL v = VRT_r_resp_is_streaming(ctx);
    if(0 == v)
    {
        return mrb_false_value();
    }else{
        return mrb_true_value();
    }



}

static mrb_value  mrb_vcl_resp_proto(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_str_new_cstr(mrb, VRT_r_resp_proto(ctx));
}

static mrb_value mrb_vcl_resp_reason(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_str_new_cstr(mrb, VRT_r_resp_reason(ctx));
}

static mrb_value mrb_vcl_resp_status(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_float_value(mrb, VRT_r_resp_status(ctx) );
}

void mrb_define_vcl_resp_class(mrb_state *mrb)
{
    struct RClass *resp;
    resp = mrb_define_class_under_varnish(mrb, "Resp");

    mrb_define_method(mrb, resp, "is_streaming", mrb_vcl_resp_is_streaming, MRB_ARGS_NONE());
    mrb_define_method(mrb, resp, "reason", mrb_vcl_resp_reason, MRB_ARGS_NONE());
    mrb_define_method(mrb, resp, "proto", mrb_vcl_resp_proto, MRB_ARGS_NONE());
    mrb_define_method(mrb, resp, "status", mrb_vcl_resp_status, MRB_ARGS_NONE());
    return ;
}



static mrb_value mrb_vcl_req_can_gzip(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    VCL_BOOL v = VRT_r_req_can_gzip(ctx);
    if(0 == v)
    {
    return mrb_true_value();
    }else
    {
        return mrb_false_value();
    }
}

static mrb_value mrb_vcl_req_esi(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_float_value(mrb, VRT_r_req_esi(ctx));
}

static mrb_value mrb_vcl_req_esi_level(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_float_value(mrb, VRT_r_req_esi_level(ctx));
}


static mrb_value mrb_vcl_req_hash_always_miss(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_float_value(mrb, VRT_r_req_hash_always_miss(ctx));
}

static mrb_value mrb_vcl_req_hash_ignore_busy(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_float_value(mrb, VRT_r_req_hash_ignore_busy(ctx));
}


static mrb_value mrb_vcl_req_proto(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_str_new_cstr(mrb, VRT_r_req_proto(ctx));
}


void mrb_define_vcl_req_class(mrb_state *mrb)
{
    struct RClass *req;
    req = mrb_define_class_under_varnish(mrb, "Req");

    mrb_define_method(mrb, req, "can_gzip", mrb_vcl_req_can_gzip, MRB_ARGS_NONE());
    mrb_define_method(mrb, req, "esi", mrb_vcl_req_esi, MRB_ARGS_NONE());
    mrb_define_method(mrb, req, "esi_level", mrb_vcl_req_esi_level, MRB_ARGS_NONE());
    mrb_define_method(mrb, req, "hash_always_miss", mrb_vcl_req_hash_always_miss, MRB_ARGS_NONE());
    mrb_define_method(mrb, req, "hash_ignore_busy", mrb_vcl_req_hash_ignore_busy, MRB_ARGS_NONE());
    mrb_define_method(mrb, req, "proto", mrb_vcl_req_proto, MRB_ARGS_NONE());
    return ;
}

mrb_value mrb_vcl_varnish_server_hostname(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_str_new_cstr(mrb, VRT_r_server_hostname(ctx));
}

mrb_value mrb_vcl_varnish_server_identity(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    return mrb_str_new_cstr(mrb, VRT_r_server_identity(ctx));
}

mrb_value mrb_vcl_varnish_server_ip(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    VCL_IP ip = VRT_r_server_ip(ctx);
    return mrb_str_new_cstr(mrb, VRT_IP_string(ctx,ip));
}

void mrb_define_vcl_server_class(mrb_state *mrb)
{
    struct RClass *server;
    server = mrb_define_class_under_varnish(mrb, "Server");
    mrb_define_method(mrb, server, "hostname", mrb_vcl_varnish_server_hostname, MRB_ARGS_NONE());
    mrb_define_method(mrb, server, "identity", mrb_vcl_varnish_server_identity, MRB_ARGS_NONE());
    mrb_define_method(mrb, server, "ip", mrb_vcl_varnish_server_ip, MRB_ARGS_NONE());

}

static mrb_value mrb_vcl_client_identity(mrb_state *mrb, mrb_value self)
{
    return self;
}

static mrb_value mrb_vcl_client_ip(mrb_state *mrb, mrb_value self)
{
    return self;
}

static mrb_value mrb_vcl_client_port(mrb_state *mrb, mrb_value self)
{
    return self;
}

void mrb_define_vcl_client_class(mrb_state *mrb)
{
    struct RClass *client;
    client = mrb_define_class_under_varnish(mrb, "Client");

    mrb_define_method(mrb, client, "identity", mrb_vcl_client_identity, MRB_ARGS_NONE());
    mrb_define_method(mrb, client, "ip", mrb_vcl_client_ip, MRB_ARGS_NONE());
    mrb_define_method(mrb, client, "port", mrb_vcl_client_port, MRB_ARGS_NONE());
    return ;
}

static mrb_value mrb_vcl_obj_grace(mrb_state *mrb, mrb_value self)
{
    return self;
}

static mrb_value mrb_vcl_obj_hits(mrb_state *mrb, mrb_value self)
{
    return self;
}

static mrb_value mrb_vcl_obj_keep(mrb_state *mrb, mrb_value self)
{
    return self;
}

static mrb_value mrb_vcl_obj_lastuse(mrb_state *mrb, mrb_value self)
{
    return self;
}

static mrb_value mrb_vcl_obj_proto(mrb_state *mrb, mrb_value self)
{
    return self;
}

static mrb_value mrb_vcl_obj_response(mrb_state *mrb, mrb_value self)
{
    return self;
}

static mrb_value mrb_vcl_obj_ttl(mrb_state *mrb, mrb_value self)
{
    return self;
}


void mrb_define_vcl_obj_class(mrb_state *mrb)
{
    struct RClass *obj;
    obj = mrb_define_class_under_varnish(mrb, "Obj");

    mrb_define_method(mrb, obj, "grace", mrb_vcl_obj_grace, MRB_ARGS_NONE());
    mrb_define_method(mrb, obj, "hits", mrb_vcl_obj_hits, MRB_ARGS_NONE());
    mrb_define_method(mrb, obj, "keep", mrb_vcl_obj_keep, MRB_ARGS_NONE());
    mrb_define_method(mrb, obj, "lastuse", mrb_vcl_obj_lastuse, MRB_ARGS_NONE());
    mrb_define_method(mrb, obj, "proto", mrb_vcl_obj_proto, MRB_ARGS_NONE());
    mrb_define_method(mrb, obj, "ttl", mrb_vcl_obj_ttl, MRB_ARGS_NONE());
    mrb_define_method(mrb, obj, "response", mrb_vcl_obj_response, MRB_ARGS_NONE());
}



static mrb_vcl_internal_state *mrb_vcl_internal_state_init(mrb_state *mrb)
{
    return (mrb_vcl_internal_state*)mrb_malloc(mrb, sizeof(mrb_vcl_internal_state) );
}

static void mrb_vcl_varnish_state_free(mrb_state *mrb, void *p)
{
    mrb_free(mrb, p);

}


static const mrb_data_type mrb_vcl_varnish_data_type = {
    "mrb_vcl_internal_state", mrb_vcl_varnish_state_free
};

mrb_value mrb_vcl_varnish_init(mrb_state *mrb, mrb_value self)
{
    mrb_vcl_internal_state *state = DATA_PTR(self);
    if(state)
    {
        mrb_free(mrb, state);
    }
    state = mrb_vcl_internal_state_init(mrb);
    mrb_data_init(self, state, &mrb_vcl_varnish_data_type);

        

    return self;
}

mrb_value mrb_vcl_return(mrb_state *mrb, mrb_value self)
{
    mrb_value klass;
    TMP_VRT_CTX;
    unsigned hand  = 0;
    mrb_get_args(mrb,"o",&klass);
    VRT_handling(ctx, hand);
    return mrb_nil_value();
}

mrb_value mrb_vcl_synth(mrb_state *mrb, mrb_value self)
{
    TMP_VRT_CTX;
    mrb_value word;
    mrb_int code;
    mrb_get_args(mrb, "io", &code, &word);
    VRT_synth(ctx,(unsigned)code, RSTRING_PTR(word));
    return mrb_nil_value();
}

void mrb_define_vcl_methods(mrb_state *mrb)
{
    struct RClass *varnish = mrb_class_get(mrb, "Varnish");
    mrb_define_class_method(mrb, varnish, "return", mrb_vcl_return,  MRB_ARGS_REQ(1));
    mrb_define_method(mrb, varnish, "initialize", mrb_vcl_varnish_init, MRB_ARGS_NONE());
    mrb_define_method(mrb, varnish, "return", mrb_vcl_return, MRB_ARGS_NONE());
    mrb_define_method(mrb, varnish, "synth", mrb_vcl_synth, MRB_ARGS_REQ(1));
    return;
}

void mrb_define_vcl_class(mrb_state *mrb)
{
    struct RClass *varnish = mrb_define_class(mrb, "Varnish", mrb->object_class);
    MRB_SET_INSTANCE_TT(varnish, MRB_TT_DATA);




    mrb_define_vcl_methods(mrb);
    mrb_define_vcl_beresp_class(mrb);
    mrb_define_vcl_bereq_class(mrb);
    mrb_define_vcl_resp_class(mrb);
    mrb_define_vcl_req_class(mrb);
    mrb_define_vcl_server_class(mrb);
    mrb_define_vcl_client_class(mrb);
    mrb_define_vcl_obj_class(mrb);

}



