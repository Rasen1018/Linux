#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	Mat m1(3, 5, CV_8UC1);
	Mat m2(m1.size(), CV_32FC1);

	for (int i = 0, k = 0; i < m1.rows; i++)
	{
		uchar* ptr_m1 = m1.ptr(i);
		float* ptr_m2 = m2.ptr<float>(i);
		for (int j=0; j < m1.cols; j++)
		{
			ptr_m1[j] = j;
			*(ptr_m2 + j) = (float)j;
		}
	}

	cout << "m1 = " << endl << m1 << endl << endl;
	cout << "m2 = " << endl << m2 << endl;

	return 0;
}
