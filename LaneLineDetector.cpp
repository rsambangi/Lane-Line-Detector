#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <iomanip>

using namespace cv;
using namespace std;

//Method used to average the lines from the Hough Line Transform
vector<Vec4i> averageLines(vector<Vec4i> linesP, int marginPos, int marginNeg) {
	// Average the lines
	vector<Vec4i> linesPFiltered;

	if (linesP.size() > 0) {
		while (linesP.size() != 0) {
			//cout << linesP.size() << endl;
			Vec4i compare = linesP[0];
			int x1Ave = compare[0];// Get the start and end points of each of the Hough Line Transform lines
			int y1Ave = compare[1];
			int x2Ave = compare[2];
			int y2Ave = compare[3];
			int count = 1;
			for (int i = 1; i < linesP.size(); i++) {// Find the next line and compare its start/end points with the first line
				Vec4i compare2 = linesP[i];
				int x1 = compare[0];
				int x2 = compare2[0];
				int xCompare = x1 - x2;
				int y1 = compare[3];
				int y2 = compare2[3];
				int yBottomCompare = y1 - y2;
				int y3 = compare[1];
				int y4 = compare2[1];
				int yTopCompare = y3 - y4;
				int y5 = compare[1];
				int y6 = compare2[3];
				int yInverseCompare = y5 - y6;
				int y7 = compare[3];
				int y8 = compare2[1];
				int yInverseCompare2 = y7 - y8;

				//If the lines are close enough (within the margin) then add up all their start and end points and increment count
				if ((xCompare >= 0 && xCompare < marginPos) || (xCompare <= 0 && xCompare > marginNeg)) {
					if (((yBottomCompare >= 0 && yBottomCompare < marginPos) ||
						(yBottomCompare <= 0 && yBottomCompare > marginNeg))
						|| ((yTopCompare >= 0 && yTopCompare < marginPos) ||
							(yTopCompare <= 0 && yTopCompare > marginNeg)) ||
						((yInverseCompare >= 0 && yInverseCompare < marginPos) ||
							(yInverseCompare <= 0 && yInverseCompare > marginNeg)) ||
						((yInverseCompare2 >= 0 && yInverseCompare2 < marginPos) ||
							(yInverseCompare2 <= 0 && yInverseCompare2 > marginNeg))) {
						x1Ave += compare2[0];
						y1Ave += compare2[1];
						x2Ave += compare2[2];
						y2Ave += compare2[3];
						count++;
						linesP.erase(linesP.begin() + i);// Remove the similar line from the vector
						i--;
						continue;
					}
					else {
						continue;
					}
				}
				else {
					continue;
				}
			}
			float x1NAve = x1Ave / count;// Take the average of all similar lines and append them to the vector
			float y1NAve = y1Ave / count;
			float x2NAve = x2Ave / count;
			float y2NAve = y2Ave / count;
			linesPFiltered.push_back(Vec4i(x1NAve, y1NAve, x2NAve, y2NAve));
			linesP.erase(linesP.begin());
		}
		return linesPFiltered;
	}
}

//Method draws the corresponding lines based on dashed ans solid properties
void drawLaneLines(Mat output, Mat img) {
	// Probabilistic Line Transform
	vector<Vec4i> linesP; // will hold the results of the detection
	HoughLinesP(output, linesP, 1, CV_PI / 180, 50, 50, 10); // runs the actual detection

	// Average the lines
	vector<Vec4i> linesPFiltered = averageLines(linesP, 60, -60);

	////Draw the averaged lines (used in testing)
	//for (int i = 0; i < linesPFiltered.size(); i++)
	//{
	//	Vec4i l = linesPFiltered[i];
	//	line(img, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 0, 255), 3, LINE_AA);
	//	float slope = 0;
	//	if (l[1] > l[3]) {
	//		slope = (float) (l[1] - l[3]) / (l[2] - l[0]);
	//	}
	//	else if (l[0] == l[2]) {
	//		slope = (float) l[3] - l[1];
	//	}
	//	else {
	//		slope = (float) (l[1] - l[3]) / (l[2] - l[0]);
	//	}
	//	cout << "Line: " << i + 1 << endl;
	//	cout << l[0] << " , " << l[1] << endl;// output used for testing
	//	cout << l[2] << " , " << l[3] << endl;
	//	cout << setprecision(4) << slope << endl;
	//}

	float marginSlopePos = 0.3;
	float marginSlopeNeg = -0.3;
	vector<Vec4i> dashedLines;
	vector<Vec4i> solidLines;

	//Isolate solid and dashed lines
	//Add comparison that if any lines are touching they are also part of a solid line (to avoid multiple detected lines in 
	// a solid line to be shown as a dashed line)
	while (linesPFiltered.size() != 0)
	{
		Vec4i test = linesPFiltered[0];
		float slope1 = 0;
		if (test[1] > test[3]) {
			slope1 = (float)(test[1] - test[3]) / (test[2] - test[0]);// Get the slopes of the line
		}
		else if (test[0] == test[2]) {
			slope1 = (float)test[3] - test[1];
		}
		else {
			slope1 = (float)(test[1] - test[3]) / (test[2] - test[0]);
		}
		int count = 0;

		for (int j = 1; j < linesPFiltered.size(); j++) {
			Vec4i test2 = linesPFiltered[j];
			float slope2 = 0;
			if (test2[1] > test2[3]) {
				slope2 = (float)(test2[1] - test2[3]) / (test2[2] - test2[0]);// compare the slope with other lines' slopes
			}
			else if (test2[0] == test2[2]) {
				slope2 = (float)test2[3] - test2[1];
			}
			else {
				slope2 = (float)(test2[1] - test2[3]) / (test2[2] - test2[0]);
			}

			float compare = slope1 - slope2;
			float compareStraightLine = abs(test[0] - test2[0]);

			if (compare <= marginSlopePos && compare >= marginSlopeNeg) {// If the slopes are similar, remove the similar line
				linesPFiltered.erase(linesPFiltered.begin() + j);
				j--;
				count++;
				continue;
			}
			else if (compareStraightLine <= 5) {// Compare the limit lines
				linesPFiltered.erase(linesPFiltered.begin() + j);
				j--;
				count++;
				continue;
			}
			else {
				continue;
			}
		}
		if (count > 0) {
			dashedLines.push_back(test);
		}
		else {
			solidLines.push_back(test);
		}
		linesPFiltered.erase(linesPFiltered.begin());

	}

	//vector<Vec4i> solidLines2 = averageLines(solidLines, 250, -250);
	//vector<Vec4i> dashedLines2 = averageLines(dashedLines, 250, -250);

	//Scale up and draw the lines
	for (int i = 0; i < solidLines.size(); i++)
	{
		Vec4i l = solidLines[i];
		float slope1 = 0;
		if (l[1] > l[3]) {
			slope1 = (float)(l[1] - l[3]) / (l[2] - l[0]);
		}
		else if (l[0] == l[2]) {
			slope1 = (float)l[3] - l[1];
		}
		else {
			slope1 = (float)(l[1] - l[3]) / (l[2] - l[0]);
		}

		//Use image width to determine bounds of lines
		float zero = l[0];
		float one = l[1];
		float two = l[2];
		float three = l[3];
		while (one <= 340 && zero >= 0) {
			one = (float)one + slope1;
			zero = (float)zero - 1;
		}

		while (three >= 0 && two <= 800) {
			three = (float)three - slope1;
			two = (float)two + 1;
		}

		if (slope1 <= 0.25 && slope1 >= -0.25) {
			line(img, Point(zero, one), Point(two, three), Scalar(255, 0, 0), 3, LINE_AA);
		}
		else {
			line(img, Point(zero, one), Point(two, three), Scalar(0, 0, 255), 3, LINE_AA);
		}
		cout << "Solid Line: " << i + 1 << endl;
		cout << zero << " , " << one << endl;// output used for testing
		cout << two << " , " << three << endl;
		cout << slope1 << endl;
	}

	for (int i = 0; i < dashedLines.size(); i++)
	{
		Vec4i l = dashedLines[i];
		float slope1 = 0;
		if (l[1] > l[3]) {
			slope1 = (float)(l[1] - l[3]) / (l[2] - l[0]);
		}
		else if (l[0] == l[2]) {
			slope1 = (float)l[3] - l[1];
		}
		else {
			slope1 = (float)(l[1] - l[3]) / (l[2] - l[0]);
		}

		//Use image width
		float zero = l[0];
		float one = l[1];
		float two = l[2];
		float three = l[3];
		while (one <= 340 && zero >= 0) {
			one = (float)one + slope1;
			zero = (float)zero - 1;
		}

		while (three >= 0 && two <= 800) {
			three = (float)three - slope1;
			two = (float)two + 1;
		}
		if (slope1 <= 0.25 && slope1 >= -0.25) {
			line(img, Point(zero, one), Point(two, three), Scalar(255, 0, 0), 3, LINE_AA);
		}
		else {
			line(img, Point(zero, one), Point(two, three), Scalar(0, 255, 0), 3, LINE_AA);
		}

		cout << "Dashed Line: " << i + 1 << endl;
		cout << zero << " , " << one << endl;// output used for testing
		cout << two << " , " << three << endl;
	}
}

void findLaneLines(String imgName, int hmin, int hmax, int smin, int smax, int vmin, int vmax, int hmin2, int hmax2, int smin2, int smax2, int vmin2, int vmax2) {
	// define the necessary matrices for the program
	Mat imgOriginal, img, imgCrop, imgHSV, imgBlur, white_mask, yellow_mask, canny_output, imgBlur2, canny_output2;

	// saves originial image into matrix
	imgOriginal = imread(imgName);
	resize(imgOriginal, img, Size(800, 450), INTER_LINEAR);
	cv::Rect crop_region(0, 110, 800, 340);

	imgCrop = img(crop_region);

	// Converting image into hsv color space for better detection
	cvtColor(imgCrop, imgHSV, COLOR_BGR2HSV);

	Scalar lower(hmin, smin, vmin);
	Scalar upper(hmax, smax, vmax);
	inRange(imgHSV, lower, upper, white_mask);

	GaussianBlur(white_mask, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, canny_output, 45, 90);

	drawLaneLines(canny_output, imgCrop);

	// Doing the same with the yellow lines
	Scalar lower2(hmin2, smin2, vmin2);
	Scalar upper2(hmax2, smax2, vmax2);
	inRange(imgHSV, lower2, upper2, yellow_mask);

	GaussianBlur(yellow_mask, imgBlur2, Size(3, 3), 3, 0);
	Canny(imgBlur2, canny_output2, 45, 90);

	drawLaneLines(canny_output2, imgCrop);

	//imshow("White Mask", white_mask);
	imshow("LaneLineDetection", imgCrop);
	waitKey(0);
}

void main() {
	// User inputs image name and the hsv value range for white and yellow
	findLaneLines("left0000.jpg", 0, 179, 0, 255, 255, 255, 0, 179, 120, 255, 139, 255);
}