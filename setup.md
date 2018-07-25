# IPD C++ Setup

## Installation

Before we can get started we need to install our development environment. You’ll need a C++ compiler, the CLion IDE, and Git for version control. You should be able to start on to the next section, [GitHub Setup](#github-setup), while files are downloading.

### C++ compiler and Git

#### Mac

OS X automatically installs its toolchain, including Git, when you attempt to use it from the command line for the first time. Thus, to install developer tools, run the *Terminal* program (from `Applications/Utilities`) to get a command prompt. At the prompt, type

```
    clang
```

and press return. If it prints `clang: error: no input files` then you have it installed already. Otherwise, a dialog box will pop up and offer to install the command-line developer tools for you. Say yes.

(Alternatively, you can install the latest version of *Command Line Tools for OS X* manually [from Apple](https://developer.apple.com/downloads/), or install *XCode* from the App Store.)

#### Windows

On Windows, you need to install MinGW-w64 (the C++ compiler) and Git separately:

  - MinGW-w64 with SDL2

    1.  Download and run [our custom installer](https://users.eecs.northwestern.edu/~jesse/course/ipd/MinGW-SDL2.exe).
    2.  Follow the prompts to install MinGW—w64. You should usually install it to <tt>C:\MinGW</tt>, but wherever you install it, take note, as you will have to configure CLion to find it.

  - Git

    1.  Download and run the installer from [git-scm.com](https://git-scm.com/download/win).
    2.  Click “Next” for each question until “Adjusting your PATH environment” appears. Select “Use Git from the Windows Command Prompt” instead of the default option. Then continue clicking “Next” until the installation completes.

#### Linux, etc.

Make sure you have Git and a working C++14 toolchain installed. You
should also install the development packages for SDL2, SDL2_image,
SDL2_ttf, and SDL2_mixer.

### CLion (all platforms)

1.  Register for a student account at [www.jetbrains.com/shop/eform/students](https://www.jetbrains.com/shop/eform/students)

2.  Follow the instructions in your email to activate your account.

3.  Download CLion from [jetbrains.com](https://www.jetbrains.com/clion/download)

4.  Run the installer—defaults should be fine. (Windows: check all of the “Create associations” boxes when they appear.)

5.  Windows only: Set the toolchain in CLion to the location where you installed MinGW. The folder you select should contain subfolders with names like `bin` and `lib`. Ignore the warnings about version numbers.

