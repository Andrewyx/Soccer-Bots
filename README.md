![image](https://github.com/Andrewyx/Soccer-Bots/assets/72371419/e18146f2-7ecb-4a88-b530-f78e7331e05d)# Soccer Bots
 Complete project file for autonomous soccer bots

Download and run the AsyncServer folder for the most recent project file.

## Requirements:
HiLetgo ESP-WROOM-32 ESP32
L298N Motor Driver
Platform.io on VSCode to upload file system information & src files

PLEASE Read main.cpp for instructions to configure your robot

Example Photos
![PXL_20230616_203337592 MP](https://github.com/Andrewyx/Soccer-Bots/assets/72371419/586abee7-04b6-4707-ab48-1429a8a97d2b)

![PXL_20230616_203253528 MP](https://github.com/Andrewyx/Soccer-Bots/assets/72371419/18e91bba-7743-4c31-be4e-e0dc86ef0f23)

![PXL_20230616_203233763 MP](https://github.com/Andrewyx/Soccer-Bots/assets/72371419/93b06661-aa5d-4c4d-8998-dd2d3a06114b)

## Uploading SPIFFS/Code:

Two uploading processes must be completed to run this project. One concerns the web interface (front end) and the other concerns the backend (C++ code).
For both, you must first:
- In VSCode with platform.io installed, navigate to the alien head highlighted in purple (see below)
- Flash the frontend code
- Flash the backend code
  
## Front End (FileSystem/SPIFFS)
Once you have arrived at the menu below by clicking the alien head, flash the data you have saved in the *data* folder of your project by clicking the ***Upload Filesystem Image ...*** button boxed in red. This process must be done every time you change the filesystem data (and must be followed by also completing the following step).
![Screenshot 2023-11-08 163351](https://github.com/Andrewyx/Soccer-Bots/assets/72371419/4a4a8eb1-0778-4719-99fc-8929832cf3f8)

## Back End (C++)
Then, to upload your C++ code, ***Build*** it first (yellow box button), then ***Upload*** (blue box).
![Screenshot 2023-11-08 163351](https://github.com/Andrewyx/Soccer-Bots/assets/72371419/4a4a8eb1-0778-4719-99fc-8929832cf3f8)

