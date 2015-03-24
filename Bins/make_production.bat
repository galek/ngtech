XCOPY Bin32 !production\Bin32 /D /E /C /R /I /K /Y
XCOPY Bin64 !production\Bin64 /D /E /C /R /I /K /Y
XCOPY data !production\data /D /E /C /R /I /K /Y
COPY production_user.ltx !production\user.ltx
del "!production\*.pdb"  /F /S
del "!production\*.bsc"  /F /S
del "!production\*.dmp"  /F /S
del "!production\*.log"  /F /S