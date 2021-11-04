## Description

These 2 projects were completed for the Operating Systems course of the [Department of Computer Science of the Athens University of Economics and Business](https://www.dept.aueb.gr/el/cs), during the Spring semester of 2019-2020.

In both projects, a system of pizza order and delivery is implemented with the use of
[POSIX](https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html) threads package (pthreads). In this system orders are entered, each order is prepared
and when the preparation is done it is delivered to the customer.

In the first task
a simplified system is built, which in the second task becomes more complex. In these systems we
have a large number of orders which are served by a limited service points, so
mutual exclusion (with mutexes) and synchronization (with condition
variables) must be implemented.

More details on each project below:

- [1st Project](1st-Assignment)
- [2nd Project](2nd-Assignment)

## Environment

The POSIX library maintains compatibility with UNIX like systems, so a Unix-Linux distribution is needed in order for these programs to work. If your native OS is not a Linux distribution, you can try running one in Virtual Box or VMware, so that these projects can then be tested. GCC compiler is also required for the compilation of the C programs. Most Linux distros have it installed by default.

<ins>Requirements</ins>

- [Lubuntu](https://lubuntu.net/) or any other Linux distribution
- [Virtualbox](https://www.virtualbox.org/) or [VMware](https://www.vmware.com/)
- [GCC](https://linuxize.com/post/how-to-install-gcc-compiler-on-ubuntu-18-04)

## Contributors

- [nevwalkalone](https://github.com/nevwalkalone)
- [Petros247](https://github.com/Petros247)
- [frostedpenguin](https://github.com/frostedpenguin)
