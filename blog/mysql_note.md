#mysql笔记

#安装mySQL 8(在wsl里面无法安装)
在官网(https://dev.mysql.com/downloads/repo/apt/)下载apt repo mysql-apt-config_0.8.23-1_all.deb

dpkg -i安装后，apt update

然后可以通过apt安装，但速度太慢，为apt使用代理。
echo 'Acquire::http::Proxy "http://192.168.119.1:7890";' > /etc/apt/apt.conf.d/apt_proxy.conf

apt install mysql-server

mysql_secure_installation, 按照提示操作。

mysql -uroot -p , 登录

#索引
后端程序员必备：索引失效的十大杂症  https://juejin.cn/post/6844904015872917517