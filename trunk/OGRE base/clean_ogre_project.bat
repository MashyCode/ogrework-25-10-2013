REM Delete Garbage Files
del solution\*.aps
del solution\*.ilk
del solution\*.map
del solution\*.ncb
del solution\*.opt
del solution\*.plg
del solution\*.pbi
del solution\*.pbo
del solution\*.pbt
del solution\*.pdb
del solution\*.tmp
del solution\*._xe
del solution\*.old
del solution\*.xml
del solution\*.log
del solution\*.user
del solution\*.sdf

REM Delete hidden archived garbage files, first changing their attributes.
attrib -h -a solution\*.gid
del solution\*.gid

REM Optional: Delete Visual Studio Solution user options, which can be useful to keep.
attrib -h -a solution\*.suo
del solution\*.suo

REM Delete Garbage Directories
del solution\enc_temp_folder\*.*
rmdir solution\enc_temp_folder

ECHO Y | del solution\DEBUG\*.*
ECHO Y | rmdir solution\DEBUG
ECHO Y | del solution\RELEASE\*.*
ECHO Y | rmdir solution\RELEASE

ECHO Y | del solution\_UpgradeReport_Files\*.*
ECHO Y | rmdir solution\_UpgradeReport_Files

ECHO Y | del bin\debug\*.*
ECHO Y | rmdir bin\debug
ECHO Y | del bin\release\*.*
ECHO Y | rmdir bin\release



REM Optional: delete the executables that the project has written to root.
REM del *.exe
