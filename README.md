# SteamGameLocator  （Steam游戏路径定位工具）
### 功能/特点：
- 为其它应用提供的API，可通过名称定位游戏安装路径.
- 提供**C++**、**Python**、**JavaScript**三种语言的API.
### 使用方法：
1. 获取游戏安装目录名称：在steam中右键相应游戏-管理-浏览本地文件-复制游戏安装目录名称（**gameFolderName**）
  > 例如：“D:\steam\steamapps\common\Half-Life”，gameFolderName=Half-Life.
2. 调用API并填入**gameFolderName**作为参数
> 具体使用方法参见各API文件夹中的“**example**”

**注意**：API返回的是一个string列表，可能包含多个路径。需要自行验证哪个是正确的安装目录。  
原因：游戏卸载后并不会移除安装目录文件夹。

 


