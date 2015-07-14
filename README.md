# Image Feature Detector

Image Feature Detector (IFD) is a desktop computer program with an intuitive graphic user interface aimed to calculate and show image features and descriptors obtained with some of the most usual [image feature detectors](https://en.wikipedia.org/wiki/Feature_detection_%28computer_vision%29) in computer vision: [Harris](https://en.wikipedia.org/wiki/Harris_affine_region_detector), [FAST](https://en.wikipedia.org/wiki/Features_from_accelerated_segment_test), [SIFT](https://en.wikipedia.org/wiki/Scale-invariant_feature_transform) and [SURF](https://en.wikipedia.org/wiki/Speeded_up_robust_features). It is programmed in C++ with the [Qt framework](http://qt.io). To calculate features IFD makes use of the [OpenCV libraries](http://opencv.org).

<img align="center" src="http://i.imgur.com/KH48YeK.png" />

This program was originally part of the end of degree project _Image Features Extraction for Mobile Robots Navigation_ of my Telecommunications Degree, presented at the Cáceres Polytechnic School of the [University of Extremadura](http://unex.es), Spain, on February 2011.

The project memory and presentation are available to download on PDF from the [doc folder](http://github.com/AntonioRedondo/ImageFeatureDetector/tree/master/doc). If you want to have a laugh and guess how terrible and nonsensical were the English phrases and expressions I was using just a few years ago these PDFs are the right reading material to find out.

In the beginning the project was hosted for 4 years on Google Code. From version 2.0 I moved it to GitHub and since then I have been carrying on maintaining it.

The image detectors used by IFD have been developed thanks to complex and long research projects. The papers which presented them to the scientific community are:

-  **Harris**: [A Combined Corner And Edge Detector](http://www.bmva.org/bmvc/1988/avc-88-023.pdf), by Chris Harris and Mike Stephens, [Roke Manor Research](http://roke.co.uk).
-  **FAST**: [Machine learning for high-speed corner detection](http://www.edwardrosten.com/work/fast.html), by Edward Rosten and Tom Drummond, [Cambridge University](http://cam.ac.uk).
-  **SIFT**: [Distinctive Image Features from Scale-Invariant Keypoints](http://www.cs.ubc.ca/~lowe/papers/ijcv04.pdf), by David G. Lowe, [University of British Columbia](http://ubc.ca).
-  **SURF**: [SURF: Speeded Up Robust Features](http://www.vision.ee.ethz.ch/~surf/), by Herbert Bay, Andreas Ess, Tinne Tuytelaars and Luc Van Gool, [ETH Zurich](http://ethz.ch) and [K. U. Leuven](http://kuleuven.be).



## Details

The program let you choose an image stored on your computer and apply one of the four feature detectors. Then the obtained descriptors are overimposed in red on the image, and the number of them and the time it took to calculated them is shown on the status bar.

<img align="center" src="http://i.imgur.com/8tgjA4y.png" />

Pictures can be taken directly from a webcam.

<img align="center" src="http://i.imgur.com/TstkzkV.png" />

FAST features can be calculated in real time from the video stream of a webcam.

<img align="center" src="http://i.imgur.com/eZme6eH.png" />

The input parameters for every detector can be tweaked from the _Detector parameters_ top bar.

**Harris**

<img align="center" src="http://i.imgur.com/yGZJoFX.png" />

**FAST**

<img align="center" src="http://i.imgur.com/h9RJ5P9.png" />

**SIFT**

<img align="center" src="http://i.imgur.com/iGbdBiL.png" />

**SURF**

<img align="center" src="http://i.imgur.com/OQldIfg.png" />

Finally, all four detectors can be compared at the same time with the _Do4!_ option. A new window will pop up showing four images with a different detector applied to each picture. The parameter values of every detector are those ones set on each detector parameters bar.

<img align="center" src="http://i.imgur.com/KcqSRY0.png" />



## Compiling the project

IFD is a simple C++ project. It only has 9 `cpp` classes, 8 `h` headers and 12 `ui` Qt GUI files. Once you have the dependencies solved to compile the code is a matter of seconds.

To configure the project building and binary linking IFD makes uses of CMake. Thanks to this IFD can be built on Linux, Mac or Windows. The screenshots shown here are taken from a computer running KDE Plasma 5.

#### Resolving dependencies

IFD only makes use of two external libraries: Qt framework and OpenCV. There are other direct but minor dependencies, like the compiler and the C++ Standard Library, but they are solved by the fact Qt and OpenCV (and almost every single C++ project) also depend on them. So once the first two libraries are fulfiled you do not have to worry anymore.

- [Install Qt](http://doc.qt.io/qt-5/linux.html): On Linux and Debian-based distributions you don't need to compile Qt. The easiest and fastest way to install Qt is with `apt-get` (details on the above link).
- [Install OpenCV](http://opencv.org/quickstart.html): due to some OpenCV copyrighted code, the modules containing SIFT and SURF detectors aren't available on Debian repositories like the rest of [OpenCV modules](https://packages.debian.org/search?keywords=opencv). This means you will have to compile OpenCV libraries by yourself. By and large you will need to download the main [OpenCV modules](https://github.com/Itseez/opencv) plus the [OpenCV contrib](https://github.com/Itseez/opencv_contrib) ones (the code containing SIFT and SURF detectors), and when you are about to compile the main OpenCV modules set the `OPENCV_EXTRA_MODULES_PATH` CMake variable to `<opencv_contrib>/modules`. Better explained [here](https://github.com/Itseez/opencv_contrib).

Once you have these two dependencies solved, from the command shell just run:

```
$ cd <imagefeaturedetector_root_folder>
$ mkdir CMakeFiles
$ cd CMakeFiles
$ cmake ..
$ make
```

and in a minute IFD should be compiled and ready to be executed on the `<imagefeaturedetector_root_folder>/bin` folder. `CMakeFiles` is an empty folder created to stored the temporary files created by CMake so that IFD root folder remains clean.



## Do you provide some ready-to-use file like a `deb` package or a Windows `exe`?

I have tried to build a Linux Debian `deb` package for amd64 machines. But after compiling statically OpenCV libraries to include them together with the IFD binary, when building IFD and at linking time `make` complains time after time about endless number of missing libraries. Once all missing libraries have been added to the `make -lanotherlibrary` command, again a new dependency misses, thus meaning I'm probably in the middle of a deep [dependency hell](https://en.wikipedia.org/wiki/Dependency_hell). I guess eventually I will find out the right order of library inclusion and will be able to generate a `deb` package.



## Contribute!

If you have an awesome pull request no matter whether it's a big or small change, send it over! Your contribution to the computer vision community is priceless.



## License

Image Feature Detector makes use of the [GNU GPL v3.0](http://choosealicense.com/licenses/gpl-3.0/) license. Remember to make public your project source code when reusing IFD.

Icons come from the KDE Plasma 5 Breeze theme: https://github.com/NitruxSA/plasma-next-icons.
