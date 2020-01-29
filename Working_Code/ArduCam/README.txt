1/29/20
"ArduCam_Soft_SPI_to_SD" : the first file that I tried to make to take photoes and save the on an SD card

The problem that I encountered was with the sd card module. For some reason, it does not function on the SPI bus like it should. It does not share it with other devices (the camera). So I have to use a software SPI to create a secondary SPI to the existing dedicated lines on the arduino.

The file is kinda jank and was being problematic. So I set out to redo this to hopefully create a more reliable file.

"ArduCam_Soft_SPI_to_SD_RAW" and "ArduCam_Soft_SPI_to_SD_JPG" were birthed on the same day. The RAW file (in theory) should hold more information. I thought this might be useful for the machine imaging but I have no idea. It resulted in longer save times and I can't defaultly open RAW files on my computer so I made the JPG file. Both these files are adapted from example files.

"ArduCam_Soft_SPI_to_SD_JPG_withTrigger" is a copy of its mother with the ability to trigger an image capture/save from pin 9 and confirm it over pin 8. (By sending a pulse of HIGH)

-William