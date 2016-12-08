#CAPP Reporter

Are you an RPI **dual** major? Have you ever noticed that if you try to view your CAPP report on SIS you only get the CAPP report for your *primary* major? We have, and that is why we are creating CAPP Reporter!

This application is being made to allow single, and even dual majors to get an *accurate* CAPP report! As you enter the courses you have taken, this *open source* application will allow you to view what courses you must take to graduate! No more needlessly worrying and fussing that you may have made a mistake while trying to discover what you need by hand!

This application will have a user friendly GUI and should update the courses that need to be taken as you type. Furthermore, since you enter the courses manually you can enter the courses you will have taken by the end of next semester if you wish!

Currently this application is being made only CS, MATH, and CS/MATH dual majors (due to time restrictions) however we intend to extend it in the future. The process for adding majors is simple, but very detalied and requites meticulous work. To extend this project to incorporate many majors, it would take the effort of a few dedicated developers as each combination of majors has their specific requirements and special rules. *However*, the language that is used to implement the logic is documented well enough that *anyone* that desires to do so could make a pull request and implement whatever majors they wish.

##Disclaimer

This application only handles general cases. Many 'corner cases' may occur, many of which are not listed RPI's websites. Please be aware, because of this, the results may not be 100% accurate. However, we are making this application pessimistic, so it will likely not tell you that you have satisfied a requirement which you have not. Regardless, as this application is still in it's infancy, we *strongly* recommend you double check your results by hand. One important, still un-implemented, feature of this is recognizing cross listed classes. If you enter a cross listed class, it will only consider it for the major you entered it as.

##Requirements

This application requires the use of the graphics library QT. It requires qmake 5.7 and C++ 11 to build.

##Installation Instructions:

First, cd into the directory you would like to install this application in

Then git clone this repository
```bash
git clone https://github.com/foxe2/CAPP-Reporter
```

Create your build directory as follows
```bash
mkdir CAPP-Reporter/build && cd CAPP-Reporter/build/
```

After that run qmake and make with the command below
```bash
qmake ../GUI/CAPP_Reporter.pro && make
```

Finally relocate the 'Database' to the location of the created __binary__. With that, you should have a functional CAPP_Reporter application!

##Usage
This application takes no arguments. Just open it like you would any application.
