# use this file to know how to clone a system.
# 由于配置环境中暂时无法解决的错误而产生以下的解决方式
## 复刻能跑代码的ubuntu16.04的系统
* 制作再生龙镜像，有关再生龙这个工具，自行百度了解一下
  * 推荐使用Windows系统上的Ultraso（大概是这个名字）根据再生龙的iso镜像制作再生龙镜像，这个操作需要一个u盘，刻录会清空整个u盘上的文件，有关这个软件的使用自行百度
  * 再生龙镜像的下载详情请见再生龙的官方网站[再生龙官方镜像的下载2020.7.20记录](https://www.clonezilla.org/downloads.php)
  * 使用再生龙复刻整个硬盘的教程[再生龙复刻整个镜像](https://www.cnblogs.com/linux-37ge/p/12793788.html)
  * 由于上述教程复刻的是整个硬盘，所以复刻的时候最好保证一个系统装在一个完整的硬盘中。比如我的电脑中有两块硬盘，第一块硬盘只装了windows，该硬盘的名称为sda，第二块硬盘中只装了ubuntu，该硬盘的名称为sdb，这两个硬盘互不影响，那么我们使用再生龙复刻整个ubuntu系统的时候，需要复刻的是ubuntu系统所在那一块sdb盘。
  * 执行上述再生龙复刻系统的时候，记得辨认清楚哪一个是原盘，哪一个是目的盘。
  * 由于再生龙主要为windows设置的软件，用来复刻ubuntu的时候有一定的问题。
    * 主要的问题有：
    * 如果使用再生龙复刻ubuntu+windows双系统（相当于你的一个完整的机器），在另一台机器上还原的时候，可能会出现windows把ubuntu引导弄丢失的问题，这个时候可以制作一个ubuntu的启动盘使用ubuntu的自我修复找回引导。有关ubuntu的自我修复[ubuntu的采用try install的way to repair boot](https://www.jianshu.com/p/0741770f1720)
* some successful cases(i don't want to use english,but there may be some problems with my input)
    1. i have a machine with only ubuntu on it, and all ubuntu was installed in only one disk,so i clone this disk with clonezilla(zaishenglong),the coures i followed was mentioned above[再生龙复刻整个镜像](https://www.cnblogs.com/linux-37ge/p/12793788.html). and after finishing cloning and installing it on a new machine i miss a new problem,as you see,system can't find boot manager,so you need to find grub by yourself, you can reference course [here](https://www.jianshu.com/p/0741770f1720).
    2. if you try to copy windows+ubuntu, you need to copy all disks without any remained, as you see,we don't need windows at all,and this process consumes too much time,and there may exist some problems when you try to install.
    3. **an important problem you need to pay attention to when you try to copy system to a new machine*, is that you need to ensure size of source disk where you system from mast be smaller than destionate disk you want to install on**

# as you see, problems still exist when you want to configure your own environment with a brand new ubuntu. 
* we take solutions above to avoid wasting too much time as competiton may soon start,you need to know why adn where these problems exist,and you need to solve them.
# as you see, ubutnu has updated to ubuntu20.04,so you need to update your project to ubuntu18.04,hoping for your excellent job.\

# 2020.7.20
* the system you want to copy is in a solid state disk owned by base(noun.),directory 'ubuntu' is its name,you may need it when you want to install it on your new nuc,these dir is created by  clonezilla.
* you need to use a USB flash disk to make a clonezilla starter disk, as you can see it [here](## 复刻能跑代码的ubuntu16.04的系统)
* 