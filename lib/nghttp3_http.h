/*
 * nghttp3
 *
 * Copyright (c) 2019 nghttp3 contributors
 * Copyright (c) 2015 nghttp2 contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef NGHTTP3_HTTP_H
#define NGHTTP3_HTTP_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <nghttp3/nghttp3.h>

struct nghttp3_stream;
typedef struct nghttp3_stream nghttp3_stream;

/* HTTP related flags to enforce HTTP semantics */
typedef enum {
  NGHTTP3_HTTP_FLAG_NONE = 0,
  /* header field seen so far */
  NGHTTP3_HTTP_FLAG__AUTHORITY = 1,
  NGHTTP3_HTTP_FLAG__PATH = 1 << 1,
  NGHTTP3_HTTP_FLAG__METHOD = 1 << 2,
  NGHTTP3_HTTP_FLAG__SCHEME = 1 << 3,
  /* host is not pseudo header, but we require either host or
     :authority */
  NGHTTP3_HTTP_FLAG_HOST = 1 << 4,
  NGHTTP3_HTTP_FLAG__STATUS = 1 << 5,
  /* required header fields for HTTP request except for CONNECT
     method. */
  NGHTTP3_HTTP_FLAG_REQ_HEADERS = NGHTTP3_HTTP_FLAG__METHOD |
                                  NGHTTP3_HTTP_FLAG__PATH |
                                  NGHTTP3_HTTP_FLAG__SCHEME,
  NGHTTP3_HTTP_FLAG_PSEUDO_HEADER_DISALLOWED = 1 << 6,
  /* HTTP method flags */
  NGHTTP3_HTTP_FLAG_METH_CONNECT = 1 << 7,
  NGHTTP3_HTTP_FLAG_METH_HEAD = 1 << 8,
  NGHTTP3_HTTP_FLAG_METH_OPTIONS = 1 << 9,
  NGHTTP3_HTTP_FLAG_METH_ALL = NGHTTP3_HTTP_FLAG_METH_CONNECT |
                               NGHTTP3_HTTP_FLAG_METH_HEAD |
                               NGHTTP3_HTTP_FLAG_METH_OPTIONS,
  /* :path category */
  /* path starts with "/" */
  NGHTTP3_HTTP_FLAG_PATH_REGULAR = 1 << 11,
  /* path "*" */
  NGHTTP3_HTTP_FLAG_PATH_ASTERISK = 1 << 12,
  /* scheme */
  /* "http" or "https" scheme */
  NGHTTP3_HTTP_FLAG_SCHEME_HTTP = 1 << 13,
  /* set if final response is expected */
  NGHTTP3_HTTP_FLAG_EXPECT_FINAL_RESPONSE = 1 << 14,
  NGHTTP3_HTTP_FLAG__PROTOCOL = 1 << 15,
} nghttp3_http_flag;

/*
 * This function is called when HTTP header field |nv| in |frame| is
 * received for |stream|.  This function will validate |nv| against
 * the current state of stream.  Pass nonzero if this is request
 * headers.  Pass nonzero to |trailers| if |nv| is included in
 * trailers.
 *
 * This function returns 0 if it succeeds, or one of the following
 * negative error codes:
 *
 * NGHTTP3_ERR_MALFORMED_HTTP_HEADER
 *     Invalid HTTP header field was received.
 * NGHTTP3_ERR_IGN_HTTP_HEADER
 *     Invalid HTTP header field was received but it can be treated as
 *     if it was not received because of compatibility reasons.
 */
int nghttp3_http_on_header(nghttp3_stream *stream, nghttp3_qpack_nv *nv,
                           int request, int trailers);

/*
 * This function is called when request header is received.  This
 * function performs validation and returns 0 if it succeeds, or one
 * of the following negative error codes:
 *
 * NGHTTP3_ERR_MALFORMED_HTTP_HEADER
 *     Required HTTP header field was not received; or an invalid
 *     header field was received.
 */
int nghttp3_http_on_request_headers(nghttp3_stream *stream, int64_t frame_type);

/*
 * This function is called when response header is received.  This
 * function performs validation and returns 0 if it succeeds, or one
 * of the following negative error codes:
 *
 * NGHTTP3_ERR_MALFORMED_HTTP_HEADER
 *     Required HTTP header field was not received; or an invalid
 *     header field was received.
 */
int nghttp3_http_on_response_headers(nghttp3_stream *stream);

/*
 * This function is called when END_STREAM flag is seen in incoming
 * frame.  This function performs validation and returns 0 if it
 * succeeds, or one of the following negative error codes:
 *
 * NGHTTP3_ERR_MALFORMED_HTTP_MESSAGING
 *     HTTP messaging is violated.
 */
int nghttp3_http_on_remote_end_stream(nghttp3_stream *stream);

/*
 * This function is called when chunk of data is received.  This
 * function performs validation and returns 0 if it succeeds, or one
 * of the following negative error codes:
 *
 * NGHTTP3_ERR_MALFORMED_HTTP_MESSAGING
 *     HTTP messaging is violated.
 */
int nghttp3_http_on_data_chunk(nghttp3_stream *stream, size_t n);

/*
 * This function inspects header fields in |nva| of length |nvlen| and
 * records its method in stream->http_flags.
 */
void nghttp3_http_record_request_method(nghttp3_stream *stream,
                                        const nghttp3_nv *nva, size_t nvlen);

#endif /* NGHTTP3_HTTP_H */
