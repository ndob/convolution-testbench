# convolution-testbench
Utility to test different [image convolution kernels](https://en.wikipedia.org/wiki/Kernel_%28image_processing%29). 

Tested only on x64-linux (Ubuntu). Uses QT so theoretically should run also on other operating systems. Prebuilt binaries are available for x64-linux in /bin. Make sure you have QT-shared libraries installed.

# Usage
![Main screen](https://github.com/ndob/convolution-testbench/blob/master/img/screenshot.png)

1. Open input image (File -> Open Image)
2. Specify an image convolution kernel as a 3x3 matrix in the middle of the screen. There are a couple of predefined kernels available too.
3. Press "Filter".
4. Optionally you can save the output image (File -> Save Result as)

# Building and running
```shell
git clone https://github.com/ndob/convolution-testbench.git
cd convolution-testbench
qmake
make
./convolution-testbench
```
