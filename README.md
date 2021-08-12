# Verilog Generator

> Verilog generator是一個 EDA工具，這個工具可以將 benchmark轉換成 Verilog的形式。

### 檔案用途
  * main.cpp是主程式
  * cases資料夾裡面的資料是用來測試的資料
  * ex_pic1、ex_pic2、ex_pic3和ex_pic4是用來作範例的圖片

### 使用方式
  1. 假設在桌面有一個叫做abc的資料夾，資料夾裡有main.cpp(主程式)和c17.bench(測試資料)<br>
  ![ex_pic1](https://github.com/tplin1999/Verilog_Generator/blob/main/ex_pic1.png)
  
  2. 先用VScode將main.cpp編譯成main.exe<br>
  ![ex_pic1](https://github.com/tplin1999/Verilog_Generator/blob/main/ex_pic2.png)
  
  3. 再到powershell輸入"cd Desktop\abc"，以及"./main.exe c17.bench c17.v"<br>
  ![ex_pic1](https://github.com/tplin1999/Verilog_Generator/blob/main/ex_pic3.png)
  
  4. 即可獲得c17.v 這個檔案<br>
  ![ex_pic1](https://github.com/tplin1999/Verilog_Generator/blob/main/ex_pic4.png)
