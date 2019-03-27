## MultiCharts_Dll Example

### Import to use in MC
```
DefineDLLFunc: "PATH\TO\YOUR\MultiCharts_Dll\x64\Debug\MyDll.dll",  int, "jAdd", int, int; 
DefineDLLFunc: "PATH\TO\YOUR\MultiCharts_Dll\x64\Debug\MyDll.dll",  int, "jSub", int, int; 

if date = lastcalcdate and t = lastcalctime then text_new(d, t, H+15, NumToStr(jAdd(H,C),0)); 
if date = lastcalcdate and t = lastcalctime then text_new(d, t, H+10, NumToStr(jSub(H,C),0)); 
```

![Screenshot](/../master/ScreenShots/display-on-chart.png?raw=true "display-on-chart")
