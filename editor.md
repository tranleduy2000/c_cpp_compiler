# Native Activity

Sample: native-activity
The native-activity sample resides under the NDK installation root, in samples/native-activity. It is a very simple example of a purely native application, with no Java source code. In the absence of any Java source, the Java compiler still creates an executable stub for the virtual machine to run. The stub serves as a wrapper for the actual, native program, which is located in the .so file.

The app itself simply renders a color onto the entire screen, and then changes the color partly in response to movement that it detects.

Example from https://developer.android.com/ndk/samples/sample_na

See video demo

1. Click File menu, then select **Open file**
<img src="https://github.com/tranleduy2000/c_cpp_compiler/blob/master/wiki/native_activity/images/Screenshot_20180608-025406_CPP%20N-IDE.jpg" width="600"/>

2. Go to directory ``/sdcard/CPlusPlusNIDE/Examples/native-activity`` and select ``native-activity.c`` to open it
<img src="https://github.com/tranleduy2000/c_cpp_compiler/blob/master/wiki/native_activity/images/Screenshot_20180608-025432_CPP%20N-IDE.jpg" width="600"/>

3. Select ``native-activity.c`` to open it
<img src="https://github.com/tranleduy2000/c_cpp_compiler/blob/master/wiki/native_activity/images/Screenshot_20180608-025627_CPP%20N-IDE.jpg" width="600"/>

4. Now click **Run** button on the action bar
5. You can enter ``CFLAGS``, ``CXXFLAGS`` or ``LDFLAGS`` if need needed. In this case, just click **Build** button
<img src="https://github.com/tranleduy2000/c_cpp_compiler/blob/master/wiki/native_activity/images/Screenshot_20180608-030204_CPP%20N-IDE.jpg" width="600"/>

6. Wait for build complete and see result, perfect.
<img src="https://github.com/tranleduy2000/c_cpp_compiler/blob/master/wiki/native_activity/images/Screenshot_20180608-025751_CPP%20N-IDE.jpg" width="600"/>


