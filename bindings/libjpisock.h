/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_gnu_pilotlink_PilotLink */

#ifndef _Included_org_gnu_pilotlink_PilotLink
#define _Included_org_gnu_pilotlink_PilotLink
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_gnu_pilotlink_PilotLink
 * Method:    connect
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_gnu_pilotlink_PilotLink_connect
  (JNIEnv *, jobject, jstring);

JNIEXPORT jobject JNICALL Java_org_gnu_pilotlink_PilotLink_readAppInfo
  (JNIEnv *env, jobject, jint, jint);
/*
 * Class:     org_gnu_pilotlink_PilotLink
 * Method:    readSysInfo
 * Signature: (I)Lorg/gnu/pilotlink/SysInfo;
 */
JNIEXPORT jobject JNICALL Java_org_gnu_pilotlink_PilotLink_readSysInfo
  (JNIEnv *, jobject, jint);

/*
 * Class:     org_gnu_pilotlink_PilotLink
 * Method:    readUserInfo
 * Signature: (I)Lorg/gnu/pilotlink/User;
 */
JNIEXPORT jobject JNICALL Java_org_gnu_pilotlink_PilotLink_readUserInfo
  (JNIEnv *, jobject, jint);

/*
 * Class:     org_gnu_pilotlink_PilotLink
 * Method:    writeUserInfo
 * Signature: (Lorg/gnu/pilotlink/User;)V
 */
JNIEXPORT void JNICALL Java_org_gnu_pilotlink_PilotLink_writeUserInfo
  (JNIEnv *, jobject, jint, jobject);

/*
 * Class:     org_gnu_pilotlink_PilotLink
 * Method:    openConduit
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_org_gnu_pilotlink_PilotLink_openConduit
  (JNIEnv *, jobject, jint);

/*
 * Class:     org_gnu_pilotlink_PilotLink
 * Method     writeAppBlock
 * Signature: Unknown
 */
JNIEXPORT int JNICALL Java_org_gnu_pilotlink_PilotLink_writeAppBlock
	(JNIEnv *, jobject, jint, jint, jbyteArray, jint);

/*
 * Class:     org_gnu_pilotlink_PilotLink
 * Method:    openDB
 * Signature: (ILjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_gnu_pilotlink_PilotLink_openDB
  (JNIEnv *, jobject, jint, jstring);

/*
 * Class:     org_gnu_pilotlink_PilotLink
 * Method:    createDB
 * Signature: Unknown
 */
JNIEXPORT jint JNICALL Java_org_gnu_pilotlink_PilotLink_createDB
	(JNIEnv *env, jobject obj, jint handle, jlong creator, jstring jdbname, jlong type);

/*
 * Class:     org_gnu_pilotlink_PilotLink
 * Method:    deleteDB
 * Signature: Unknown
 */
JNIEXPORT jint JNICALL Java_org_gnu_pilotlink_PilotLink_deleteDB
	(JNIEnv *env, jobject obj, jint handle, jstring jdbname);

/*
 * Class:     org_gnu_pilotlink_PilotLink
 * Method:    getRecordCount
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_org_gnu_pilotlink_PilotLink_getRecordCount
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     org_gnu_pilotlink_PilotLink
 * Method:    getRecordByIndex
 * Signature: (III)Lorg/gnu/pilotlink/Record;
 */
JNIEXPORT jobject JNICALL Java_org_gnu_pilotlink_PilotLink_getRecordByIndex
  (JNIEnv *, jobject, jint, jint, jint);

/*
 * Class:     org_gnu_pilotlink_PilotLink
 * Method:    deleteRecordByIndex
 * Signature: (III)I
 */
JNIEXPORT jint JNICALL Java_org_gnu_pilotlink_PilotLink_deleteRecordByIndex
  (JNIEnv *, jobject, jint, jint, jint);

/*
 * Class:     org_gnu_pilotlink_PilotLink
 * Method:    writeRecord
 * Signature: (IIILorg/gnu/pilotlink/Record;)I
 */
JNIEXPORT jint JNICALL Java_org_gnu_pilotlink_PilotLink_writeRecord
  (JNIEnv *, jobject, jint, jint,  jobject);

/*
 * Class:     org_gnu_pilotlink_PilotLink
 * Method:    closeDB
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_org_gnu_pilotlink_PilotLink_closeDB
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     org_gnu_pilotlink_PilotLink
 * Method:    endSync
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_gnu_pilotlink_PilotLink_endSync
  (JNIEnv *, jobject, jint);

/*
 * Class:     org_gnu_pilotlink_PilotLink
 * Method:    close
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_org_gnu_pilotlink_PilotLink_close
  (JNIEnv *, jobject, jint);

#ifdef __cplusplus
}
#endif
#endif