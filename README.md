# Image Feature Detector

Image Feature Detector (IFD) is a desktop computer program with an intuitive graphic user interface aimed to calculate and show image features calculated with some of the most usual [image feature detectors](https://en.wikipedia.org/wiki/Feature_detection_%28computer_vision%29) in computer vision: [Harris](https://en.wikipedia.org/wiki/Harris_affine_region_detector), [FAST](https://en.wikipedia.org/wiki/Features_from_accelerated_segment_test), [SIFT](https://en.wikipedia.org/wiki/Scale-invariant_feature_transform) and [SURF](https://en.wikipedia.org/wiki/Speeded_up_robust_features). It is programmed in C++ with the [Qt framework](http://qt.io). To calculate features IFD makes use of the [OpenCV libraries](http://opencv.org).

<img align="center" src="http://i.imgur.com/KH48YeK.png" />

This program was originally part of the end of degree project _Image Features Extraction for Mobile Robots Navigation_ of my Telecommunications Degree, exposed at the Cáceres Polytechnic School of the [University of Extremadura](http://unex.es), Spain, on February 2011.

The project memory and presentation are available to download on PDF from the [doc folder](http://github.com/AntonioRedondo/ImageFeatureDetector/tree/master/doc). If you want to have a laugh and guess how terrible and nonsensical were the phrases and expressions I tried to use in English just a few years ago these PDFs are the right reading material.

In the beginning the project was hosted on Google Code for 4 years. Afterwards I moved the project to GitHub and I have been carrying on maintaining it.

Below are the papers which presented to the scientific community these image detectors:

-  **Harris**: [A Combined Corner And Edge Detector](http://www.bmva.org/bmvc/1988/avc-88-023.pdf), by Chris Harris and Mike Stephens, [Roke Manor Research](http://roke.co.uk).
-  **FAST**: [Machine learning for high-speed corner detection](http://www.edwardrosten.com/work/fast.html), by Edward Rosten and Tom Drummond, [Cambridge University](http://cam.ac.uk).
-  **SIFT**: [Distinctive Image Features from Scale-Invariant Keypoints](http://www.cs.ubc.ca/~lowe/papers/ijcv04.pdf), by David G. Lowe, [University of British Columbia](http://ubc.ca).
-  **SURF**: [SURF: Speeded Up Robust Features](http://www.vision.ee.ethz.ch/~surf/), by Herbert Bay, Andreas Ess, Tinne Tuytelaars and Luc Van Gool, [ETH Zurich](http://ethz.ch) and [K. U. Leuven](http://kuleuven.be).



## Details

The program let you choose an image stored on your computer and apply one of the four feature detectors. Then the detectors are overimposed in red on the image, and the number of them and the time it took to calculated them is shown on the status bar.

<img align="center" src="http://i.imgur.com/8tgjA4y.png" />

Pictures can be taken directly from a webcam.

<img align="center" src="http://i.imgur.com/TstkzkV.png" />

FAST features can be calculated in real time from the image of a webcam.

<img align="center" src="http://i.imgur.com/eZme6eH.png" />

The parameters for every detector can be tweaked from the _Detector parameters_ top bar.

**Harris**

<img align="center" src="http://i.imgur.com/yGZJoFX.png" />

**FAST**

<img align="center" src="http://i.imgur.com/h9RJ5P9.png" />

**SIFT**

<img align="center" src="http://i.imgur.com/iGbdBiL.png" />

**SURF**

<img align="center" src="http://i.imgur.com/OQldIfg.png" />

Finally, all four detectors can be compared at the same time with the _Do4!_ option. A new window will appear showing four images with a diferent detector applied to each picture.

<img align="center" src="http://i.imgur.com/KcqSRY0.png" />



## Compiling the project

IFD is a simple C++ project. It only has 9 `cpp` classes, 8 `h` headers and 12 `ui` Qt GUI files. Once you have the dependencies solved to compile the code is a matter of seconds.

To configure the project building, linking and installation IFD makes uses of CMake. Thanks to this IFD can be built on Linux, Mac or Windows.

#### Resolving dependencies

IFD only makes use of two external libraries: Qt framework and OpenCV. There are other minor dependencies, like the compiler used and the C++ standard library but they are automatically handled by CMake.

- [Install Qt](http://doc.qt.io/qt-5/linux.html): On Linux and Debian-like distributions you don't need to compile Qt. The easiest and fastest way to install Qt is with `apt-get` (details on the above link).
- [Install OpenCV](http://opencv.org/quickstart.html): due to some OpenCV copyrighted code, the modules containing SIFT and SURF detectors aren't available on Debian repositories like the rest of [OpenCV modules](https://packages.debian.org/search?keywords=opencv). This means you will have to compile by yourself the OpenCV libraries. By and large you will need to download the main [OpenCV modules](https://github.com/Itseez/opencv) plus the [OpenCV contrib](https://github.com/Itseez/opencv_contrib) ones (the code containing SIFT and SURF detectors), and when you are about to compile the main OpenCV modules set the `OPENCV_EXTRA_MODULES_PATH` CMake variable to `<opencv_contrib>/modules`. Better explained [here](https://github.com/Itseez/opencv_contrib).

Once you have these two dependencies solved, from the command shell just run:

```
$ cd <imagefeaturedetector_root_folder>
$ mkdir CMakeFiles
$ cd CMakeFiles
$ cmake ..
$ make
```

and in a minute IFD should be compiled and ready to be executed on the `bin` folder. `CMakeFiles` is an empty folder created to stored the temporary files created by CMake so that IFD root folder remains clean.



## Some ready-to-use file like a `deb` package or a Win `exe`?

I have tried to build a Linux `deb` package for amd64 machines, but after compiling statically OpenCV libraries to include them with the IFD binary, when building IFD and at linking time `make` complains time after time about endless number of libraries. Once adding a missing library to the `make -lanotherlibrary` command a new dependency misses, thus meaning I'm probably in the middle of a deep [dependency hell](https://en.wikipedia.org/wiki/Dependency_hell). I guess eventually I will find out the right order of library inclusion and will be able to generate a `deb` package.



## License

Image Feature Detector makes use of the [GNU GPL v3.0](http://choosealicense.com/licenses/gpl-3.0/) license. Remember to make public your project source code when reusing IFD.

Icons come from the KDE Plasma 5 Breeze theme: https://github.com/NitruxSA/plasma-next-icons.
