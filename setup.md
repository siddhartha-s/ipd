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

On Windows, you need to install MinGW (the C++ compiler) and Git separately:

  - MinGW

    1.  Download and run the 64-bit installer from [mingw.org](http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe/download).
    2.  Follow the prompts to install MinGW—all the default options should suffice. Take note of where you install it, as you will have to configure CLion to find it.

  - Git

    1.  Download and run the installer from [git-scm.com](https://git-scm.com/download/win).
    2.  Click “Next” for each question until “Adjusting your PATH environment” appears. Select “Use Git from the Windows Command Prompt” instead of the default option. Then continue clicking “Next” until the installation completes.

#### Linux, etc.

Make sure you have Git and a working C++14 toolchain installed.

### CLion (all platforms)

1.  Register for a student account at [www.jetbrains.com/shop/eform/students](https://www.jetbrains.com/shop/eform/students)

2.  Follow the instructions in your email to activate your account.

3.  Download CLion from [jetbrains.com](https://www.jetbrains.com/clion/download)

4.  Run the installer—defaults should be fine. (Windows: check all of the “Create associations” boxes when they appear.)

5.  Windows only: Set the toolchain in CLion to the location where you installed MinGW. The folder you select should contain subfolders with names like `bin` and `lib`. Ignore the warnings about version numbers.

## GitHub Setup

Git is a source control tool that you’ll be learning more about later in this lab. In short, it keeps track of each version of your files, so that you can:

  - Go back to any previous version. This is useful because it means that mistakes are easy to undo, and it’s safe to experiment.

  - Have multiple versions (*branches*) simultaneously. You may use separate branches for different features you are developing or ideas you are trying, so that you can switch between them without them interfering with one another. This also allows multiple developers to work on the same project concurrently without stepping on each other’s toes.

  - Copy changes from one version to another. For example, if you are happy with a feature that you developed in a *feature branch*, then you can copy the changes into the main (*master*) branch.

GitHub is a service that hosts Git repositories (where all the versions are stored) online to facilitate collaboration. In particular, you will (probably) use GitHub when handing in your homework. Thus, you will need a GitHub account, and we will need to know your username:

1.  If you don’t already have a GitHub account, go to [github.com](https://github.com/) and sign up for one.

2.  Let us know your GitHub username by [TBD].

