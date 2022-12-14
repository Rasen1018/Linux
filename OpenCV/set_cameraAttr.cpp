#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void put_string(Mat &frame, string text, Point pt, int value) {}

VideoCapture capture;

void zoom_bar(int value, void*) {
	capture.set(CAP_PROP_ZOOM, value);
}

void focus_bar(int value, void*) {
	capture.set(CAP_PROP_FOCUS, value);
}

int main()
{
	capture.open(0);
	CV_Assert(capture.isOpened());
	capture.set(CAP_PROP_FRAME_WIDTH, 400);
	capture.set(CAP_PROP_FRAME_HEIGHT, 300);
	capture.set(CAP_PROP_AUTOFOCUS, 0);
	capture.set(CAP_PROP_BRIGHTNESS, 150);

	int zoom = capture.get(CAP_PROP_ZOOM);
	int focus = capture.get(CAP_PROP_FOCUS);

	string title = "change camera component";
	namedWindow(title);
	createTrackbar("zoom", title, &zoom, 10, zoom_bar);
	createTrackbar("focus", title, &focus, 40, focus_bar);

	for(;;) {
		Mat frame;
		capture >> frame;

		put_string(frame, "zoom: ", Point(10, 240), zoom);
		put_string(frame, "focus: ", Point(10, 270), focus);

		imshow(title, frame);
		if (waitKey(30) >= 0) break;
	}
	
	return 0;
}
