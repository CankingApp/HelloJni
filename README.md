# AS中实战Jni开发

 Jni(Java Native Interface )其实是Java平台与C／C＋＋及其他语言交互的接口，基于Android SDK开发的应用用的事java语言，自然可以通过Jni调用C／C++代码满足一些特殊需求。

为了便于提高安卓平台JNI开发模式，Google提供了一套工具集Native Development Kit（NDK）来调用动态库，使得开发者可以快速方便的完成JNI调用。

本文中演示在Android Studio中怎样创建一个`JNI Hello`工程
###1.创建一个普通安卓工程
目录结构如下，为AS默认创建的一个工程

![目录结构](http://img.blog.csdn.net/20150920005920898)

##2.添加native接口
在将要使用JNI接口的类中添加以下代码。为了调用方便，本例中直接在activity中添加。
```java
  public native String hellojni();
```

##3.生成 .h 文件
命令格式：
```
#操作命令：
javah -d jni -classpath <SDK_android.jar>;<APP_classes> lab.sodino.jnitest.MainActivity
```
为了方便可以写个脚本帮我们完成该步骤：

```shell
#!/bin/sh
export ProjectPath=$(cd "../$(dirname "$1")"; pwd)
export TargetClassName="lemon.hellojni.MainActivity"

export SourceFile="${ProjectPath}/app/src/main/java"
export TargetPath="${ProjectPath}/app/src/main/jni"

cd "${SourceFile}"
javah -d ${TargetPath} -classpath "${SourceFile}" "${TargetClassName}"
echo -d ${TargetPath} -classpath "${SourceFile}" "${TargetClassName}"
```
使用该代码时只需修改包名和路径即可。
如果你的AS中装有Bash Plugin，可以直接执行该脚步，会生成目标文件`lemon_hellojni_MainActivity.h`
![项目目录](http://img.blog.csdn.net/20150920011405157)

##4.创建.c/.cpp 文件，实现我们的c／c＋＋代码
此处参考了[googlesample hello-jni](https://github.com/googlesamples/android-ndk/blob/master/hello-jni/app/src/main/jni/hello-jni.c)
```c++
#include "lemon_hellojni_MainActivity.h"

jstring JNICALL Java_lemon_hellojni_MainActivity_hellojni
        (JNIEnv* env, jobject obj)
{
    return (*env)->NewStringUTF(env, "Hello, Jni world~");
}
```
C++代码与C借口有一点不同，如下：
```c++
#include "lemon_hellojni_MainActivity.h"

jstring JNICALL Java_lemon_hellojni_MainActivity_hellojni
        (JNIEnv* env, jobject obj)
{
    return env->NewStringUTF("Hello, Jni world~");
}
```

##5.配置相关参数，Built Project
如果没有配置ndk环境变量的化需要在`local.properties` 中添加：
```
ndk.dir=/Users/lemon/ndk/android-ndk-r10e
```


gradew 2.5后对配置有所变动，更加系统化，参考[google](http://tools.android.com/tech-docs/new-build-system/gradle-experimental)
本例中采用兼容模式，在`grade.properties` 中添加：
```
android.useDeprecatedNdk=true
```
在app module 中的build.gradle 中找到defaultconfig，添加：
```java
defaultConfig {                   
  applicationId "lemon.hellojni"  
  minSdkVersion 9                 
  targetSdkVersion 23             
  versionCode 1                   
  versionName "1.0"               
  ndk {                           
    moduleName "hellojni"         
  }                               
}                                 
```
编译后生成build目录：
![生成so文件目录](http://img.blog.csdn.net/20150920014834184)

###6.总结
运行后，可以看到输出了我们在c代码中返回的字符，说明jni调用成功了。
![效果](http://img.blog.csdn.net/20150920014928108)

以上是一种jni调用方法，在以上过程中，我们编译生产的so文件可以直接放到项目中复用。
在main目录下建立`jnlLibs` 文件夹，把生产的so文件放入即可。

[实例源码Git查看](https://github.com/CankingApp/HelloJni)