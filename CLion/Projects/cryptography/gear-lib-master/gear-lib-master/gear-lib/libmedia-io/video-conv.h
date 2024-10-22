/******************************************************************************
 * Copyright (C) 2014-2020 Zhifeng Gong <gozfree@163.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/
#ifndef VIDEO_CONV_H
#define VIDEO_CONV_H

#include <stdint.h>
#include <video-def.h>

int video_yuyv2rgb(struct video_frame *in, struct video_frame *out);
#if 0
int uvc_uyvy2rgb(uvc_frame_t *in, uvc_frame_t *out);
int uvc_any2rgb(uvc_frame_t *in, uvc_frame_t *out);

int uvc_yuyv2bgr(uvc_frame_t *in, uvc_frame_t *out);
int uvc_uyvy2bgr(uvc_frame_t *in, uvc_frame_t *out);
int uvc_any2bgr(uvc_frame_t *in, uvc_frame_t *out);

int uvc_yuyv2y(uvc_frame_t *in, uvc_frame_t *out);
int uvc_yuyv2uv(uvc_frame_t *in, uvc_frame_t *out);

#ifdef LIBUVC_HAS_JPEG
int uvc_mjpeg2rgb(uvc_frame_t *in, uvc_frame_t *out);
#endif
#endif

#endif
