# Verilog Generator

> Verilog generator是一個 EDA工具，這個工具可以將 benchmark轉換成 Verilog的形式。

## 檔案用途
  1. main.cpp是主程式
  2. cases資料夾裡面的資料是用來測試的資料
  3. results資料夾裡面的資料是轉換後的資料<br><br>
  例如:  _c17.bench透過主程式轉換後會變成 c17.v_ 

### 使用範例
  1. 假設在桌面有一個叫做example的資料夾，資料夾裡有main.cpp(主程式)和original.v(測試資料)<br>
  ![ex_pic1](https://github.com/tplin1999/2021_CAD_Contest_Problem_F/blob/main/ex_pic1.png)
  
  2. 先用VScode將main.cpp編譯成main.exe<br>
  ![ex_pic1](https://github.com/tplin1999/2021_CAD_Contest_Problem_F/blob/main/ex_pic2.png)
  
  3. 再到powershell輸入"cd Desktop\example"，以及"./main.exe original.v original_ans.v"<br>
  ![ex_pic1](https://github.com/tplin1999/2021_CAD_Contest_Problem_F/blob/main/ex_pic3.png)
  
  4. 即可獲得original_ans.v 這個檔案<br>
  ![ex_pic1](https://github.com/tplin1999/2021_CAD_Contest_Problem_F/blob/main/ex_pic4.png)
