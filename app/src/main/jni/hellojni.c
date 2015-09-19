#include "lemon_hellojni_MainActivity.h"

jstring JNICALL Java_lemon_hellojni_MainActivity_hellojni
        (JNIEnv *env, jobject obj) {
    return (*env)->NewStringUTF(env, "Hello, Jni world~");
}
