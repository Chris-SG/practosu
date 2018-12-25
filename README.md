# practosu

-------------------------
Starting the Program
-------------------------
practosu relies on the osu! process to run. As such, if osu! is closed, practosu will silently exit. Please ensure osu! is currently running before opening practosu.
In addition, upon execution, practosu will load all of your beatmaps. This may take 15-20 seconds if you have many beatmaps, so be patient.

-------------------------
Loading Beatmaps
-------------------------
practosu offers three ways to load a beatmap.
1) While at the song select screen in osu!, hitting F11 will automatically load the currently selected map and bring practosu to the foreground. This will only work on songs that have a Beatmap ID, and therefore does not support practice difficulties or unsubmitted maps.
2) Clicking 'Reload Map' will have the same effect as above, including the Beatmap ID limitation.
3) Clicking 'Manually Load' will allow you to browse for a particular .osu file. This will allow you to create practice difficulties for both other practice difficulties as well as unsubmitted maps.

-------------------------
Utilizing Presets
-------------------------
Presets allow an easy way to apply commonly used settings to maps quickly.
You can create a preset by clicking the 'New Preset' button. Checking a box and typing information into the provided boxes allows you to create presets to your own liking.
You may not yet delete or modify presets from within practosu. If you wish to do so, you may manually edit the presets.txt file created in the same folder as practosu.exe. Be careful editing however, as this may cause program instability. For best results, only delete presets manually, avoid modifications.

-------------------------
Saving Beatmaps
-------------------------
After loading a beatmap and changing the settings as desired, you may save the new beatmap by using the 'Save Beatmap' button. Ensure the 'Version' field differs from the original value to avoid name conflicts. practosu will notify you if there is an issue.
If changing the speed of the beatmap, practosu will utilize ffmpeg to create a new audio file with the required speed. This will open a blank console window that will close after completion. Please wait for this to close before trying to play the practice difficulty.

-------------------------
Audio File Cleaner
-------------------------
Creating practice difficulties with changed speeds will result in a new audio file, however removing these difficulties will not remove the audio file automatically. The Audio File Cleaner is used to clear these files.
As a word of caution, this process may take many minutes if you have a large amount of beatmaps.
After running the tool, a list of all practosu audio files will be displayed. By clicking 'Delete Inactive Audio Files', the program will scan all relevant beatmaps to see if any audio files are no longer in use, and delete them if so.

-------------------------
Manual Pointer Search
-------------------------
Automatic pointer search seems to fail on some computers. Unfortunately I have not yet been able to work out the reason for this, so instead I have added a manual pointer search. This uses set address patterns from a few beatmap difficulties to manually find the beatmap pointer offset. This will only need to be done once per load of the program.


-----------------------------------------------------------------------------------------------------

-------------------------
Building
-------------------------
1. Clone repository.
2. Add a QTDIR environmental variable for your 32bit QT install (eg. C:\Qt\5.12.0\msvc2017).
3. Install Qt Visual Studio Tools.
4. Set Qt version to the same version used in QTDIR within VS.
5. Enable required modules (Core, GUI, Widgets).
