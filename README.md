# Lane-Line-Detector

A lane line detection algorithm made with C++ and OpenCV with 94% accuracy on a 1000-image dataset and implemented in the SAE AutoDrive Challenge II for autonomous lane changes. The algorithm is able to differentiate between dashed, solid, white, and yellow lane lines along with detecting limit lines for intersections.

# How It Works

* Image Preprocessing
  * Frame is converted to HSV for accurate color masking
  * Color mask is applied to isolate white and yellow colored lane lines
  * Gaussian blur and canny edge detection are applied to the image mask to soften the edges and isolate lane line outlines
![ImageMask](https://github.com/rsambangi/Lane-Line-Detector/assets/143136941/b3e7da86-3262-4090-b2a2-4154056ea1b3)

* Line Detection
  * Hough Line Transform is applied to the image mask to isolate all lines in the image
  * Overlapping lines are all averaged so that each dash and solid line only has one occurrence
 ![AveragedLines](https://github.com/rsambangi/Lane-Line-Detector/assets/143136941/af44a2c6-8e45-4005-aa7c-eb2b2c672fa2)

* Final Lane Line Detection and Scaling
  * To isolate dashed lines, any lines that have duplicates with the same slope or are within marginal x-bounds are clustered together as one dashed line
  * All duplicate lines are then removed
  * All lines are then scaled to the full length of the frame and assigned to their respective vector category (dashed and solid, yellow and white)
Key: Red is solid, and Green is dashed
![OverheadTest](https://github.com/rsambangi/Lane-Line-Detector/assets/143136941/3c09c2b2-3c85-4582-b328-cd6d1b831d5b)

# What I Learned

* Hough Line Transform application and region of interest (ROI) masking to reduce the computational load
* OpenCV image processing techniques (Color Masks, Gaussian Blur, Canny Edge Detection, etc.)
* ROS Node Programming
