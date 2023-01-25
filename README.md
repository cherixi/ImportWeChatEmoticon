# ImportWeChatEmoticon
将表情批量导入进手机微信中，只支持Windows x64 + Android
## 注意
在生成的debug或release中或导出后需要手动添加 adb.exe AdbWinApi.dll AdbWinUsbApi.dll 三个文件，在 ImportWeChatEmoticon 文件夹中可以找到  
需要手动在运行目录新建一个名为 screenshoot 文件夹
## 安装
软件已打包放在了 ImportWeChatEmoticon 文件夹和 tag 中，只需下载此文件夹即可
## 实现
基于QT配合adb实现对手机微信的控制
