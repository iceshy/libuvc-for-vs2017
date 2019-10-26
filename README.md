How to prepare your system for uvc on Windows 
=============================================
(please refer to https://github.com/pupil-labs/pyuvc/blob/master/WINDOWS_USER.md)

# LibUVC will work with x64/x86
1. Download and install [libusbk 3.0.7.0] (https://sourceforge.net/projects/libusbk/files/libusbK-release/3.0.7.0/libusbK-3.0.7.0-setup.exe/download)
2. Download and install [Zadig] (https://github.com/pbatard/libwdi/releases/download/v1.2.5/zadig-2.2.exe)
3. Plug in your web camera
4. Start ZadiG. From options menu uncheck "Hide Composite Devices" or "Ignore Hubs or Composite Devices", and check "List All Devices"
6. Locate your web camera composite device. It is crucial you select the "composite parent" and not one of the interfaces, otherwise libusb will not work
6. Set the driver replacement to libusbK (v3.0.7.0) and click "Replace driver"
7. Verify in device manager it is correctly replaced. Should be listed under a new category libusbK USB Devices
8. untar the 7z, and run libuvc/exmaples/example

