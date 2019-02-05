/*
 * Copyright 2012 Adam Murdoch
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/*
 * Generic cross-platform functions.
 */
#include "native.h"
#include "generic.h"
#include <stdlib.h>
#include <string.h>

void mark_failed_with_message(JNIEnv *env, const char* message, jobject result) {
    mark_failed_with_code(env, message, 0, NULL, result);
}

void mark_failed_with_code(JNIEnv *env, const char* message, int error_code, const char* error_code_message, jobject result) {
    jclass destClass = env->GetObjectClass(result);
    jmethodID method = env->GetMethodID(destClass, "failed", "(Ljava/lang/String;IILjava/lang/String;)V");
    jstring message_str = env->NewStringUTF(message);
    jstring error_code_str = error_code_message == NULL ? NULL : env->NewStringUTF(error_code_message);
    jint failure_code = map_error_code(error_code);
    env->CallVoidMethod(result, method, message_str, failure_code, error_code, error_code_str);
}

char* byte_array_to_char(JNIEnv* env, jbyteArray byteArray, jobject result) {
    jboolean isCopy;
    jbyte* bytes = env->GetByteArrayElements(byteArray, &isCopy);
    size_t length = env->GetArrayLength(byteArray);
    char* chars = (char*)malloc(length + 1);
    for (int i=0; i<length; i++) {
        chars[i] = (char)bytes[i];
    }
    chars[length] = '\0';
    env->ReleaseByteArrayElements(byteArray, bytes, 0);
    return chars;
}

jbyteArray char_to_byte_array(JNIEnv* env, const char* chars, jobject result) {
    size_t length = strlen(chars);
    jbyteArray bytes = env->NewByteArray(length);
    env->SetByteArrayRegion(bytes, 0, length, (jbyte *)chars);
    return bytes;
}

JNIEXPORT jint JNICALL
Java_net_rubygrapefruit_platform_internal_jni_NativeLibraryFunctions_getVersion(JNIEnv *env, jclass target) {
    return NATIVE_VERSION;
}
