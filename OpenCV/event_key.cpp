#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	Mat image(200, 300, CV_8U, Scalar(255));
	namedWindow("keyboard event", WINDOW_AUTOSIZE);
	imshow("keyboard event", image);

	while (1)
	{
		int key = waitKeyEx(3000);
		if (key == 27) break;

		switch (key)
		{
			case 'a': cout << "press a key" << endl; break;
			case 'b': cout << "press b key" << endl; break;
			case 0x41: cout << "press A key" << endl; break;
			case 66: cout << "press B key" << endl; break;

			case 0xff51: cout << "press left key" << endl; break;
			case 0xff52: cout << "press up key" << endl; break;
			case 0xff53: cout << "press right key" << endl; break;
			case 0xff54: cout << "press down key" << endl; break;
		}
	}

	return 0;
}
