// THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
// BY HAND!!
//
// Generated by lcm-gen

#include <string.h>
#include "mbot_lcm_msgs_mbot_system_reset_t.h"

static int __mbot_lcm_msgs_mbot_system_reset_t_hash_computed;
static uint64_t __mbot_lcm_msgs_mbot_system_reset_t_hash;

uint64_t __mbot_lcm_msgs_mbot_system_reset_t_hash_recursive(const __lcm_hash_ptr *p)
{
    const __lcm_hash_ptr *fp;
    for (fp = p; fp != NULL; fp = fp->parent)
        if (fp->v == __mbot_lcm_msgs_mbot_system_reset_t_get_hash)
            return 0;

    __lcm_hash_ptr cp;
    cp.parent =  p;
    cp.v = __mbot_lcm_msgs_mbot_system_reset_t_get_hash;
    (void) cp;

    uint64_t hash = (uint64_t)0xa0daccf9c9736ae0LL
         + __int64_t_hash_recursive(&cp)
         + __int32_t_hash_recursive(&cp)
         + __string_hash_recursive(&cp)
         + __boolean_hash_recursive(&cp)
        ;

    return (hash<<1) + ((hash>>63)&1);
}

int64_t __mbot_lcm_msgs_mbot_system_reset_t_get_hash(void)
{
    if (!__mbot_lcm_msgs_mbot_system_reset_t_hash_computed) {
        __mbot_lcm_msgs_mbot_system_reset_t_hash = (int64_t)__mbot_lcm_msgs_mbot_system_reset_t_hash_recursive(NULL);
        __mbot_lcm_msgs_mbot_system_reset_t_hash_computed = 1;
    }

    return __mbot_lcm_msgs_mbot_system_reset_t_hash;
}

int __mbot_lcm_msgs_mbot_system_reset_t_encode_array(void *buf, int offset, int maxlen, const mbot_lcm_msgs_mbot_system_reset_t *p, int elements)
{
    int pos = 0, element;
    int thislen;

    for (element = 0; element < elements; element++) {

        thislen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &(p[element].utime), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __int32_t_encode_array(buf, offset + pos, maxlen - pos, &(p[element].slam_mode), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __string_encode_array(buf, offset + pos, maxlen - pos, &(p[element].slam_map_location), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __boolean_encode_array(buf, offset + pos, maxlen - pos, &(p[element].retain_pose), 1);
        if (thislen < 0) return thislen; else pos += thislen;

    }
    return pos;
}

int mbot_lcm_msgs_mbot_system_reset_t_encode(void *buf, int offset, int maxlen, const mbot_lcm_msgs_mbot_system_reset_t *p)
{
    int pos = 0, thislen;
    int64_t hash = __mbot_lcm_msgs_mbot_system_reset_t_get_hash();

    thislen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &hash, 1);
    if (thislen < 0) return thislen; else pos += thislen;

    thislen = __mbot_lcm_msgs_mbot_system_reset_t_encode_array(buf, offset + pos, maxlen - pos, p, 1);
    if (thislen < 0) return thislen; else pos += thislen;

    return pos;
}

int __mbot_lcm_msgs_mbot_system_reset_t_encoded_array_size(const mbot_lcm_msgs_mbot_system_reset_t *p, int elements)
{
    int size = 0, element;
    for (element = 0; element < elements; element++) {

        size += __int64_t_encoded_array_size(&(p[element].utime), 1);

        size += __int32_t_encoded_array_size(&(p[element].slam_mode), 1);

        size += __string_encoded_array_size(&(p[element].slam_map_location), 1);

        size += __boolean_encoded_array_size(&(p[element].retain_pose), 1);

    }
    return size;
}

int mbot_lcm_msgs_mbot_system_reset_t_encoded_size(const mbot_lcm_msgs_mbot_system_reset_t *p)
{
    return 8 + __mbot_lcm_msgs_mbot_system_reset_t_encoded_array_size(p, 1);
}

int __mbot_lcm_msgs_mbot_system_reset_t_decode_array(const void *buf, int offset, int maxlen, mbot_lcm_msgs_mbot_system_reset_t *p, int elements)
{
    int pos = 0, thislen, element;

    for (element = 0; element < elements; element++) {

        thislen = __int64_t_decode_array(buf, offset + pos, maxlen - pos, &(p[element].utime), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &(p[element].slam_mode), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __string_decode_array(buf, offset + pos, maxlen - pos, &(p[element].slam_map_location), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __boolean_decode_array(buf, offset + pos, maxlen - pos, &(p[element].retain_pose), 1);
        if (thislen < 0) return thislen; else pos += thislen;

    }
    return pos;
}

int __mbot_lcm_msgs_mbot_system_reset_t_decode_array_cleanup(mbot_lcm_msgs_mbot_system_reset_t *p, int elements)
{
    int element;
    for (element = 0; element < elements; element++) {

        __int64_t_decode_array_cleanup(&(p[element].utime), 1);

        __int32_t_decode_array_cleanup(&(p[element].slam_mode), 1);

        __string_decode_array_cleanup(&(p[element].slam_map_location), 1);

        __boolean_decode_array_cleanup(&(p[element].retain_pose), 1);

    }
    return 0;
}

int mbot_lcm_msgs_mbot_system_reset_t_decode(const void *buf, int offset, int maxlen, mbot_lcm_msgs_mbot_system_reset_t *p)
{
    int pos = 0, thislen;
    int64_t hash = __mbot_lcm_msgs_mbot_system_reset_t_get_hash();

    int64_t this_hash;
    thislen = __int64_t_decode_array(buf, offset + pos, maxlen - pos, &this_hash, 1);
    if (thislen < 0) return thislen; else pos += thislen;
    if (this_hash != hash) return -1;

    thislen = __mbot_lcm_msgs_mbot_system_reset_t_decode_array(buf, offset + pos, maxlen - pos, p, 1);
    if (thislen < 0) return thislen; else pos += thislen;

    return pos;
}

int mbot_lcm_msgs_mbot_system_reset_t_decode_cleanup(mbot_lcm_msgs_mbot_system_reset_t *p)
{
    return __mbot_lcm_msgs_mbot_system_reset_t_decode_array_cleanup(p, 1);
}

int __mbot_lcm_msgs_mbot_system_reset_t_clone_array(const mbot_lcm_msgs_mbot_system_reset_t *p, mbot_lcm_msgs_mbot_system_reset_t *q, int elements)
{
    int element;
    for (element = 0; element < elements; element++) {

        __int64_t_clone_array(&(p[element].utime), &(q[element].utime), 1);

        __int32_t_clone_array(&(p[element].slam_mode), &(q[element].slam_mode), 1);

        __string_clone_array(&(p[element].slam_map_location), &(q[element].slam_map_location), 1);

        __boolean_clone_array(&(p[element].retain_pose), &(q[element].retain_pose), 1);

    }
    return 0;
}

mbot_lcm_msgs_mbot_system_reset_t *mbot_lcm_msgs_mbot_system_reset_t_copy(const mbot_lcm_msgs_mbot_system_reset_t *p)
{
    mbot_lcm_msgs_mbot_system_reset_t *q = (mbot_lcm_msgs_mbot_system_reset_t*) malloc(sizeof(mbot_lcm_msgs_mbot_system_reset_t));
    __mbot_lcm_msgs_mbot_system_reset_t_clone_array(p, q, 1);
    return q;
}

void mbot_lcm_msgs_mbot_system_reset_t_destroy(mbot_lcm_msgs_mbot_system_reset_t *p)
{
    __mbot_lcm_msgs_mbot_system_reset_t_decode_array_cleanup(p, 1);
    free(p);
}

int mbot_lcm_msgs_mbot_system_reset_t_publish(lcm_t *lc, const char *channel, const mbot_lcm_msgs_mbot_system_reset_t *p)
{
      int max_data_size = mbot_lcm_msgs_mbot_system_reset_t_encoded_size (p);
      uint8_t *buf = (uint8_t*) malloc (max_data_size);
      if (!buf) return -1;
      int data_size = mbot_lcm_msgs_mbot_system_reset_t_encode (buf, 0, max_data_size, p);
      if (data_size < 0) {
          free (buf);
          return data_size;
      }
      int status = lcm_publish (lc, channel, buf, data_size);
      free (buf);
      return status;
}

struct _mbot_lcm_msgs_mbot_system_reset_t_subscription_t {
    mbot_lcm_msgs_mbot_system_reset_t_handler_t user_handler;
    void *userdata;
    lcm_subscription_t *lc_h;
};
static
void mbot_lcm_msgs_mbot_system_reset_t_handler_stub (const lcm_recv_buf_t *rbuf,
                            const char *channel, void *userdata)
{
    int status;
    mbot_lcm_msgs_mbot_system_reset_t p;
    memset(&p, 0, sizeof(mbot_lcm_msgs_mbot_system_reset_t));
    status = mbot_lcm_msgs_mbot_system_reset_t_decode (rbuf->data, 0, rbuf->data_size, &p);
    if (status < 0) {
        fprintf (stderr, "error %d decoding mbot_lcm_msgs_mbot_system_reset_t!!!\n", status);
        return;
    }

    mbot_lcm_msgs_mbot_system_reset_t_subscription_t *h = (mbot_lcm_msgs_mbot_system_reset_t_subscription_t*) userdata;
    h->user_handler (rbuf, channel, &p, h->userdata);

    mbot_lcm_msgs_mbot_system_reset_t_decode_cleanup (&p);
}

mbot_lcm_msgs_mbot_system_reset_t_subscription_t* mbot_lcm_msgs_mbot_system_reset_t_subscribe (lcm_t *lcm,
                    const char *channel,
                    mbot_lcm_msgs_mbot_system_reset_t_handler_t f, void *userdata)
{
    mbot_lcm_msgs_mbot_system_reset_t_subscription_t *n = (mbot_lcm_msgs_mbot_system_reset_t_subscription_t*)
                       malloc(sizeof(mbot_lcm_msgs_mbot_system_reset_t_subscription_t));
    n->user_handler = f;
    n->userdata = userdata;
    n->lc_h = lcm_subscribe (lcm, channel,
                                 mbot_lcm_msgs_mbot_system_reset_t_handler_stub, n);
    if (n->lc_h == NULL) {
        fprintf (stderr,"couldn't reg mbot_lcm_msgs_mbot_system_reset_t LCM handler!\n");
        free (n);
        return NULL;
    }
    return n;
}

int mbot_lcm_msgs_mbot_system_reset_t_subscription_set_queue_capacity (mbot_lcm_msgs_mbot_system_reset_t_subscription_t* subs,
                              int num_messages)
{
    return lcm_subscription_set_queue_capacity (subs->lc_h, num_messages);
}

int mbot_lcm_msgs_mbot_system_reset_t_unsubscribe(lcm_t *lcm, mbot_lcm_msgs_mbot_system_reset_t_subscription_t* hid)
{
    int status = lcm_unsubscribe (lcm, hid->lc_h);
    if (0 != status) {
        fprintf(stderr,
           "couldn't unsubscribe mbot_lcm_msgs_mbot_system_reset_t_handler %p!\n", hid);
        return -1;
    }
    free (hid);
    return 0;
}

