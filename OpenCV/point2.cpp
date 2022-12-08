#include <opencv2/opencv.hpp>

using namespace std;

int main()
{
	cv::Point_<int> pt1(100, 200);
	cv::Point_<float> pt2(92.3f, 125.23f);
	cv::Point_<double> pt3(100.2, 300.9);

	cv::Point2i pt4(120, 69);
	cv::Point2f pt5(0.3f, 0.f), pt6(0.f, 0.4f);
	cv::Point2d pt7(0.25, 0.6);

	cv::Point pt8 = pt1 + (cv::Point) pt2;
	cv::Point2f pt9 = pt6 * 3.14f;
	cv::Point2d pt10 = (pt3 + (cv::Point2d) pt6) * 10;
	
	cout << "pt8 =" << pt8.x << ", " << pt8.y << endl;
	cout << "[pt9] =" << pt9 << endl;
	cout << (pt2 == pt6) << endl;
	cout << "pt7 and pt8 product : " << pt7.dot(pt8) << endl;
	
	return 0;
}
