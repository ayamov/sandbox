#include <jni.h>

int StartApplication(int argc,char** argv);

int ghl_android_app_main(int argc,char** argv) {
	 return StartApplication(argc,argv);
}

extern "C" JNIEXPORT
void JNICALL Java_com_sandbox_Activity_nativeOnActivityCreated(
                                                                  JNIEnv *env,
                                                                  jobject thiz,
                                                                  jobject activity,
                                                                  jobject saved_instance_state);
extern "C" JNIEXPORT
void JNICALL Java_com_sandbox_Activity_nativeOnActivityDestroyed(
                                                                    JNIEnv *env, jobject thiz, jobject activity);

extern "C" JNIEXPORT
void JNICALL Java_com_sandbox_Activity_nativeOnActivityPaused(
                                                                 JNIEnv *env, jobject thiz, jobject activity);

extern "C" JNIEXPORT
void JNICALL Java_com_sandbox_Activity_nativeOnActivityResumed(
                                                                  JNIEnv *env, jobject thiz, jobject activity);

extern "C" JNIEXPORT
void JNICALL Java_com_sandbox_Activity_nativeOnActivitySaveInstanceState(
                                                                            JNIEnv *env, jobject thiz, jobject activity, jobject out_state) ;
extern "C" JNIEXPORT
void JNICALL Java_com_sandbox_Activity_nativeOnActivityStarted(
                                                                  JNIEnv *env, jobject thiz, jobject activity) ;
extern "C" JNIEXPORT
void JNICALL Java_com_sandbox_Activity_nativeOnActivityStopped(
                                                                  JNIEnv *env, jobject thiz, jobject activity) ;
extern "C" JNIEXPORT
void JNICALL Java_com_sandbox_Activity_nativeOnActivityResult(
                                                                 JNIEnv *env,
                                                                 jobject thiz,
                                                                 jobject activity,
                                                                 jint request_code,
                                                                 jint result_code,
                                                                 jobject data) ;

