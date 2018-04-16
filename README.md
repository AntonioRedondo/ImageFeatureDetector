# Image Feature Detector

Image Feature Detector (IFD) is a desktop computer program with an intuitive graphic user interface aimed to calculate and show image features and descriptors obtained with some of the most usual [image feature detectors](https://en.wikipedia.org/wiki/Feature_detection_%28computer_vision%29) in computer vision: [Harris](https://en.wikipedia.org/wiki/Harris_affine_region_detector), [FAST](https://en.wikipedia.org/wiki/Features_from_accelerated_segment_test), [SIFT](https://en.wikipedia.org/wiki/Scale-invariant_feature_transform) and [SURF](https://en.wikipedia.org/wiki/Speeded_up_robust_features). It is programmed in C++ with the [Qt framework](http://qt.io). To calculate features IFD makes use of the [OpenCV libraries](http://opencv.org).

<img align="center" src="https://github.com/AntonioRedondo/ImageFeatureDetector/raw/master/readmeImages/mainWindow.png" />

This program was originally part of my end of degree project _Image Features Extraction for Mobile Robots Navigation_ of my Telecommunications Degree, presented at the [Cáceres Polytechnic School](https://www.unex.es/conoce-la-uex/centros/epcc) of the [University of Extremadura](http://www.unex.es), Spain, on February 2011.

The project memory and presentation are available for download on PDF from the [doc folder](http://github.com/AntonioRedondo/ImageFeatureDetector/tree/master/doc). They are published under Creative Commons Attribution-Share Alike license. If you want to have a laugh and guess how terrible and nonsensical was my English I was using just a few years ago these PDFs are the right reading material to find out.

In the beginning the project was hosted from 2011 to 2015 on Google Code. In 2015 I moved it to GitHub.

The image detectors used in IFD have been the result of complex research projects. The papers which presented them to the scientific community are:

-  **Harris**: [A Combined Corner And Edge Detector](http://www.bmva.org/bmvc/1988/avc-88-023.pdf), by Chris Harris and Mike Stephens, [Roke Manor Research](http://roke.co.uk).
-  **FAST**: [Machine learning for high-speed corner detection](http://www.edwardrosten.com/work/fast.html), by Edward Rosten and Tom Drummond, [University of Cambridge](http://cam.ac.uk).
-  **SIFT**: [Distinctive Image Features from Scale-Invariant Keypoints](http://www.cs.ubc.ca/~lowe/papers/ijcv04.pdf), by David G. Lowe, [University of British Columbia](http://ubc.ca).
-  **SURF**: [SURF: Speeded Up Robust Features](http://www.vision.ee.ethz.ch/~surf/), by Herbert Bay, Andreas Ess, Tinne Tuytelaars and Luc Van Gool, [ETH Zurich](http://ethz.ch) and [K. U. Leuven](http://kuleuven.be).



## Details

The program let you choose an image stored on your computer and apply one of the four feature detectors. Then the obtained descriptors are overimposed in red on the image, and the number of them and the calculation time is shown on the status bar.

<img align="center" src="https://github.com/AntonioRedondo/ImageFeatureDetector/raw/master/readmeImages/startup.png" />

Pictures can be taken directly from a webcam within the program.

<img align="center" src="https://github.com/AntonioRedondo/ImageFeatureDetector/raw/master/readmeImages/captureWebcam.png" />

FAST features can be calculated in real time from the video stream of a webcam.

<img align="center" src="https://github.com/AntonioRedondo/ImageFeatureDetector/raw/master/readmeImages/fastRT.png" />

The input parameters for every detector can be tweaked from the _Detector parameters_ bar.

**Harris**

<img align="center" src="https://github.com/AntonioRedondo/ImageFeatureDetector/raw/master/readmeImages/barHarris.png" />

**FAST**

<img align="center" src="https://github.com/AntonioRedondo/ImageFeatureDetector/raw/master/readmeImages/barFast.png" />

**SIFT**

<img align="center" src="https://github.com/AntonioRedondo/ImageFeatureDetector/raw/master/readmeImages/barSift.png" />

**SURF**

<img align="center" src="https://github.com/AntonioRedondo/ImageFeatureDetector/raw/master/readmeImages/barSurf.png" />

All four detectors can be compared at the same time with the _Do4!_ option. A new window will pop up showing four images with a different detector applied to each picture. The parameter values of every detector are those ones set on each detector parameters bar.

<img align="center" src="https://github.com/AntonioRedondo/ImageFeatureDetector/raw/master/readmeImages/do4.png" />

The time shown only represents the time taken to purely calculate the features on an image which has already been prepared to be analised. That is it, it does not take into account the extra time needed to transform the images from 8U3C/8U4C (colour) to 8U1C (black and white, the detector's input requires an image on this format) and the time taken to paint the red circles showing the detected features. My guess™ is that it is a neglectable amount of time.



## Compiling the project

IFD is a simple C++ project. It only has 9 `cpp` classes, 8 `h` headers and 12 `ui` Qt GUI files. Once you have the dependencies solved compiling the code is a matter of seconds.

To configure the project building and binary linking IFD makes use of CMake. Thanks to this IFD can be built on Linux, OS X and Windows. The screenshots shown here have been taken from a computer running Linux with KDE 5.

#### Resolving dependencies

IFD only makes use of two external libraries: Qt framework (v5+) and OpenCV (v3+). There are other direct but minor dependencies, like the compiler and the C++ Standard Library, but they are solved as long as you solve Qt and OpenCV dependencies because they (and every single C++ project) also depend on them. Once the first two libraries are fulfiled you do not have to worry about extra dependencies.

- [Installing Qt](http://doc.qt.io/qt-5/linux.html): on Linux and Debian-based distributions you don't need to compile Qt. The easiest and fastest way to install Qt is with `apt-get` (details on the link).
- [Installing OpenCV](http://opencv.org/quickstart.html): due to some OpenCV copyrighted code, the modules containing SIFT and SURF detectors aren't available on Debian repositories like the rest of [OpenCV modules](https://packages.debian.org/search?keywords=opencv). This means you will have to compile OpenCV libraries by yourself. By and large you will need to download the main [OpenCV modules](https://github.com/Itseez/opencv) plus the [OpenCV contrib](https://github.com/Itseez/opencv_contrib) ones (the code containing SIFT and SURF detectors), and when you are about to compile the main OpenCV modules set the `OPENCV_EXTRA_MODULES_PATH` CMake variable to `<opencv_contrib>/modules`. Better explained [here](https://github.com/Itseez/opencv_contrib).

On Unix machines, once you have these two dependencies solved, from the command shell just run:

```
$ cd <imagefeaturedetector_root_folder>
$ mkdir CMakeFiles
$ cd CMakeFiles
$ cmake ..
$ make
```

and in a minute IFD should be compiled and ready to be executed on the `<imagefeaturedetector_root_folder>/bin` folder. `CMakeFiles` is an empty folder created to store the temporary files created by CMake so that IFD root folder remains clean.



## Do you provide some ready-to-use file like a `deb` package or a Windows `exe`?

I have tried to build a Linux Debian `deb` package for x86-64 machines. But after compiling statically OpenCV libraries to include them together with the IFD binary, when building IFD and at linking time `make` complains time after time about endless number of missing libraries. Once all missing libraries have been added to the `make -lanotherlibrary` command, again a new dependency misses, thus meaning I'm probably in the middle of a deep [dependency hell](https://en.wikipedia.org/wiki/Dependency_hell). I guess eventually I will find out the right order of library inclusion and will be able to generate a `deb` package.



## Contribute!

If you have an awesome pull request no matter whether it's a new feature or a bug, send it over! Your contribution to the computer vision community is priceless.



## License

Image Feature Detector makes use of the [GNU GPL v3.0](http://choosealicense.com/licenses/gpl-3.0/) license. Remember to make public your project source code when reusing IFD.

Icons come from the KDE Plasma 5 Breeze theme: https://github.com/NitruxSA/plasma-next-icons (repository has been removed).
