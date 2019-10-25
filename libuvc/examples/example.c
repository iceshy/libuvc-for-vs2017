#include <windows.h>

//#include <unistd.h>

#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/core/mat.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

#include "libuvc/libuvc.h"

/* This callback function runs once per frame. Use it to perform any
 * quick processing you need, or have it put the frame into your application's
 * input queue. If this function takes too long, you'll start losing frames. */
void cb(uvc_frame_t *frame, void *ptr) {
	unsigned char *raw, *bgr;

	raw = (unsigned char *)malloc(frame->width * frame->height * 3);
	bgr = (unsigned char *)malloc(frame->width * frame->height * 3);

	if (raw == nullptr || bgr == nullptr)
		return;

	memcpy(raw,frame->data, frame->width * frame->height * 2);

	Mat yuv(frame->height, frame->width, CV_8UC2, raw);
	Mat img(frame->height, frame->width, CV_8UC3, bgr);

	cvtColor(yuv, img, COLOR_YUV2BGR_YUYV);
	namedWindow("UVCViewer", CV_WINDOW_NORMAL);
	imshow("UVCViewer", img);
	waitKey(10);

	free(raw);
	free(bgr);

}


int main(int argc, char **argv) {
  uvc_context_t *ctx;
  uvc_device_t *dev;
  uvc_device_handle_t *devh;
  uvc_stream_ctrl_t ctrl;
  uvc_error_t res;

  /* Initialize a UVC service context. Libuvc will set up its own libusb
   * context. Replace NULL with a libusb_context pointer to run libuvc
   * from an existing libusb context. */
  res = uvc_init(&ctx, NULL);

  if (res < 0) {
    uvc_perror(res, "uvc_init");
    return res;
  }

  puts("UVC initialized");
  /* Locates the first attached UVC device, stores in dev */
  res = uvc_find_device(
      ctx, &dev,
      0, 0, NULL); /* filter devices: vendor_id, product_id, "serial_num" */

  if (res < 0) {
    uvc_perror(res, "uvc_find_device"); /* no devices found */
  } else {
    puts("Device found");

    /* Try to open the device: requires exclusive access */
    res = uvc_open(dev, &devh);

    if (res < 0) {
      uvc_perror(res, "uvc_open"); /* unable to open device */
    } else {
      puts("Device opened");

      /* Print out a message containing all the information that libuvc
       * knows about the device */
      //uvc_print_diag(devh, stderr); 

      /* Try to negotiate a 640x480 30 fps YUYV stream profile */
	  //ctrl.bInterfaceNumber = 0;
      res = uvc_get_stream_ctrl_format_size(
          devh, &ctrl, /* result stored in ctrl */
		  UVC_FRAME_FORMAT_YUYV, /* YUV 422, aka YUV 4:2:2. try _COMPRESSED */
          640, 480, 30 /* width, height, fps */
      );

	  ctrl.bInterfaceNumber = 1;

      /* Print out the result */
      uvc_print_stream_ctrl(&ctrl, stderr);

      if (res < 0) {
        uvc_perror(res, "get_mode"); /* device doesn't provide a matching stream */
      } else {
        /* Start the video stream. The library will call user function cb:
         *   cb(frame, (void*) 12345)
         */
		res = uvc_start_streaming(devh, &ctrl, cb, (void *)12345, 0);

        if (res < 0) {
          uvc_perror(res, "start_streaming"); /* unable to start stream */
        } else {
          puts("Streaming...");

          //uvc_set_ae_mode(devh, 1); /* e.g., turn on auto exposure */

          Sleep(10*1000); /* stream for 10 seconds */

          /* End the stream. Blocks until last callback is serviced */
          uvc_stop_streaming(devh);
          puts("Done streaming.");
        }
      }

      /* Release our handle on the device */
	  uvc_close(devh);
      puts("Device closed");
    }

    /* Release the device descriptor */
    uvc_unref_device(dev);
  }

  /* Close the UVC context. This closes and cleans up any existing device handles,
   * and it closes the libusb context if one was not provided. */
  uvc_exit(ctx);
  puts("UVC exited");

  return 0;
}

