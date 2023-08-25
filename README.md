# Lane-Line-Detector

A lane line detection algorithm made with C++ and OpenCV with 94% accuracy on a 1000-image dataset and implemented in the SAE AutoDrive Challenge II for autonomous lane changes. The algorithm is able to differentiate between dashed, solid, white, and yellow lane lines along with detecting limit lines for intersections.

# How It Works

* Image Preprocessing
  * Frame is converted to HSV for accurate color masking
  * Color mask is applied to isolate white and yellow colored lane lines
  * Gaussian blur and canny edge detection are applied to the image mask to soften the edges and isolate lane line outlines
* Lane Line Detection
  * Hough Line Transform is applied to the image mask to isolate all lines in the image
  * To isolate dashed lines, any lines that have duplicates with the same slope or are within marginal x-bounds are clustered together as one dashed line
  * All duplicate lines are then removed
* Final Scaling
  * All lines are then scaled to the full length of the frame and assigned to their respective vector category (dashed and solid, yellow and white)

# What I Learned

* Hough Line Transform application and region of interest (ROI) masking to reduce the computational load
* OpenCV image processing techniques (Color Masks, Gaussian Blur, Canny Edge Detection, etc.)
* ROS Node Programming

# Test Images
