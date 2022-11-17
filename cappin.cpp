#include <opencv4/opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//function to make a mask of the frame given
Mat genMask(Mat frame){

	//declare variables needed
	Mat hsv, lower, upper, red, masked;

	//convert bgr frame to hsv frame
	cvtColor( frame, hsv, COLOR_BGR2HSV) ;

	//the following 2 are python code. the two lines after should work.
    //lower_mask = cv.inRange(hsv, lower1, upper1)
    //upper_mask = cv.inRange(hsv, lower2, upper2)

    inRange(hsv, Scalar(0, 100, 20), Scalar(10, 255, 255), lower);
    inRange(hsv, Scalar(160, 50, 50), Scalar(179, 255, 255), upper);

    //combine masks into one mask, then perform a bitwise and on the frame (change to hsv?? PLEASE CHECK THIS)
    //ERROR - redMask not working, bitwise_and also not working
    redMask = lower & upper;
    bitwise_and(hsv,hsv, masked, mask=redMask)

    //ERROR - no return statement in function returning non void
    return masked

}

//structure to store results of getPos
//ERROR - new types may not be defined in a return type
//ERROR - two or more data types in declaration of getPos
struct result {
	double xAvg;
	double yAvg;
	bool calculate;
}

//wtf is an auto again??
auto getPos(Mat frame) {

	//declare the result structure as calcs
    result calcs;

    //set values
	bool calculate = false;
	int height = frame.shape[0]; // height
	int width = frame.shape[1]; // width
	double xAvg = 0;
	double yAvg = 0;
	int count = 0;

	//go through all pixels, if pixel is not black then add it to the average
	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){
			if (frame[y,x] != 0){
				xAvg += x
				yAvg += y
				count = count+1
			}
		}
	}

	if (count > 350){ //If red pixel count is above x amount
		xAvg = (xAvg/count) * 2;
		yAvg = (yAvg/count) * 2;
		calculate = true;
	}

	//store the values gotten into calcs and return it
	calcs.xAvg = xAvg;
	calcs.yAvg = yAvg;
	calcs.calculate = calculate;
    return calcs;

    //in main, get the struct by declaring result calcs, then calcs = getPos(Mat frame) without Mat
    //then calcs.xAvg, calcs.yAvg, calcs.calculate
}


//here we go
int main(int argc, char* argv[])
{
	//open up the video capture, if it can't then just end the function
	VideoCapture cap(0);

	if (cap.isOpened() == false){
		cout << "Cannot open camera." << endl;
		cin.get();
		return -1;
	}

	//I don't really need this since I'll be resizing the frame anyways, but leave in for now
	double dWidth = cap.get(CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(CAP_PROP_FRAME_WIDTH);
	cout << "Res of vid : " << dWidth << "x" << dHeight << endl;

	//we use 640x480 because we gangsta
	double width = 640;
	double height = 480;

	//no clue why I need this but leave in just in case something breaks
	string windowName = "cam";
	namedWindow(windowName);


	//here we go part 2 (while loop for getting frames and 
	while (true){

		//declare my variables and structures
		Mat frame, mask, rsMask;
		result calcs;

		//can the capture read the frame? if so, great! if not, break!
		bool bSuccess = cap.read(frame);
		if (bSuccess == false){
			cout << "Can't read frame, disconnecting." << endl;
			cin.get();
			break;
		}

		//first resize to fit the 640x480 I've been working with this whole time
		resize(frame, frame, Size(width, height));

		//get le mask
		mask = genMask(frame);

		//python variant, NEEDS REWRITE (done?)
		//split the mask into hue, saturation, value. really don't need the h and s, can I toss those out a window?
		Mat channels[3];
		split(mask, channels);
		Mat v = channels[2];

		//rescale to be smoller, maybe use resize? would be resize(v, rsMask, Size(width/2, height/2));
		//edit: put it in already because why write some function when I don't need to
		//rsMask = rescaleFrame(v, 50);
		resize(v, rsMask, Size(width/2, height/2));

		//should put the struct into calcs
		//ERROR - expected primary expression before 'frame'
		//ERROR - getPos was not declared in this scop, did you mean fgetpos?
		calcs = getPos(Mat frame);

		//if it calculated enough reds, run this
		if (calcs.calculate = true){

			//python variant, NEEDS REWRITE (done?)
			//basically draws a smol circle at the center for visualization purposes. mostly unnecessary for the final.
			//cv.circle(frame, (int(xAvg), int(yAvg)), 10, (255,0,0), 5);
			circle(frame, Point(calcs.xAvg, calcs.yAvg), 10, (255,0,0), 5);

			//declare these strings now
			string textx, texty;

			//the following is my thoughts on how to do this from python. it's probably important
			/*
			Center of a screen with 640 and 480 is roughly 320,240
			So let's make a "rectangle of center" that counts as the center.
			As long as the average center is in that rectangle, it's for all intents and purposes in the center

			Let's then define a larger rectangle that is also "in" the center.
			If color pixels start popping outside of this bigger rectangle, the robot is close enough
			That should account for safety

			Let's say small square is 40x40 and big square is 200x200 to start
			*/

			//idk what this is for but I have a feeling if I delete it something will break
			string text = "";

			//Center camera/head on object
			//Gonna need to change this up so that it outputs the right strings to the console and doesn't print to frame
			//The main idea is to get the robot to read these strings and turn, rn it's just for visualization
			if (calcs.xAvg > 340){
				//look right until it's in center rect
				textx = "Object is to right";
			}
			else if (calcs.xAvg < 300){
				//look left until it's in center rect
				textx = "Object is to left";
			}
			else{
				textx = "Object is in center on x axis";
			}

			if (calcs.yAvg > 260){
				//look down until it's in center rect
				texty = "Object is down";
			}
			else if (calcs.yAvg < 220){
				//look up until it's in center rect
				texty = "Object is up";
			}
			else{
				texty = "Object is in center on y axis";
			}

			//turn body so that it and the head are facing object, then take a step forward

			//python variant, NEEDS REWRITE (I think I did this right)
			//more unnecessary code other than for debugging and such. idk. keep it for now.
			putText(frame, textx, Point(0, 430), FONT_HERSHEY_SIMPLEX, 1, (0,255,0), 2);
			putText(frame, texty, Point(0, 460), FONT_HERSHEY_SIMPLEX, 1, (0,255,0), 2);

		}

		//this should work, and should output the circle as well as the text showing if it's to the left or right
		imshow(windowName, frame);

		//ekoroshia bitch (hit esc for kill command)
		if (waitKey(10) == 27)
		{
			cout << "Esc key was pressed, vid stopping..." << endl;
			break;
		}
	}
	return 0;
}
