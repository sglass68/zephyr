.. _hello_world:

Chrome OS EC
############

Overview
********
A simple demo of a few chunks of the Chrome OS EC code base running in Zephyr.

Building and Running
********************

This project outputs 'Hello World' to the console.  It can be built and executed
on QEMU as follows:

.. zephyr-app-commands::
   :zephyr-app: samples/cros_ec
   :host-os: unix
   :board: native_posix
   :goals: run
   :compact:

Sample Output
=============

.. code-block:: console

    Chrome OS EC
