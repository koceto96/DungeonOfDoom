/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class GameLogic */

#ifndef _Included_GameLogic
#define _Included_GameLogic
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     GameLogic
 * Method:    startGame
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_GameLogic_startGame
  (JNIEnv *, jobject, jstring);

/*
 * Class:     GameLogic
 * Method:    setMap
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_GameLogic_setMap
  (JNIEnv *, jobject, jstring);

/*
 * Class:     GameLogic
 * Method:    hello
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_GameLogic_hello
  (JNIEnv *, jobject);

/*
 * Class:     GameLogic
 * Method:    move
 * Signature: (CZ)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_GameLogic_move
  (JNIEnv *, jobject, jchar, jboolean);

/*
 * Class:     GameLogic
 * Method:    resetPosition
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_GameLogic_resetPosition
  (JNIEnv *, jobject);

/*
 * Class:     GameLogic
 * Method:    pickup
 * Signature: (Z)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_GameLogic_pickup
  (JNIEnv *, jobject, jboolean);

/*
 * Class:     GameLogic
 * Method:    look
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_GameLogic_look
  (JNIEnv *, jobject);

/*
 * Class:     GameLogic
 * Method:    printMap
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_GameLogic_printMap
  (JNIEnv *, jobject);

/*
 * Class:     GameLogic
 * Method:    isWall
 * Signature: (C)Z
 */
JNIEXPORT jboolean JNICALL Java_GameLogic_isWall
  (JNIEnv *, jobject, jchar);

/*
 * Class:     GameLogic
 * Method:    getMapHeight
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_GameLogic_getMapHeight
  (JNIEnv *, jobject);

/*
 * Class:     GameLogic
 * Method:    getMapWidth
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_GameLogic_getMapWidth
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
