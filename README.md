# 3D-virtual-trackball

This program helped me to learn basic 3D model manipulation and basic user interface.

First, I set up OpenGL and GLUT. In MacOS, I used XCode and created a command line tool in C++. Next, I added the OpenGL and GLUT frameworks to the project's frameworks and libraries.

I utilized skeleton code  so that a 3D teapot would be drawn in runtime.

I implemented the virtual trackball rotation, which is toggled by using mouse motion. I used the mouse position as the projection of a point onto a hemi-sphere. Then I tracked the mouse movement, which tracks the difference between the old mouse hemi-sphere point and the new mouse hemi-sphere point. The normal to this plane is calculate and aids in rotation by multiplying velocity by the roation scale. Next, I reset the model-view matrix and roate about the axis. We multiply the new roatation matrix by the saved 3D viewing matrix.

Next, I implemented translation along the x, y, and z directions in the eye space by using
key pressing. This was relativley simple and can be done by calling an onKeyPress where the key press is inputed. We use a simple if/else if statement to identify which movement is used. 
If the a key is pressed, we translate the image by -1.0 along the x axis (left move), and if the d key is pressed, we translate the viewing space by 1.0 along the x axis (right move). If the w key is pressed, we translate the image by 1.0 along the y axis (up move), and if the s key is pressed, we translate the viewing space by -1.0 along the y axis (down move). If the x key is pressed, we translate the image by 1.0 along the z axis (in move), and if the z key is pressed, we translate the viewing space by -1.0 along the z axis (out move); this is essentially the same as zooming.
The amount translated is determined by how many times the keys are pressed.

Lastly, I implemented zoom in and zoom out scaling using up and down key pressing. I had to refrence the Specialkey callback function, and then I implemented two simple if statements identifying if up or down is pressed. If the up key is pressed, the view scales up by 1.10, and if the down key is pressed, the view scales out by .90. The amount zoomed in and out is determined by how many times the keys are pressed.

This program resulted in a 3D teapot that can be rotatated using virtual
trackball, translated in the x, y, and z directions using the keys (a, w, s, d, z, x), and zoomed in and zoomed out using scaling (up and down keys). This basic user interface for 3D model manipulation gave me a better understanding of 3D transformations in OpenGL.
