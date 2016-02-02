#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "jni.h"
#include "dexit_SigTerm.h"
#include "dexit_SigTermExtLib.h"

extern jmp_buf ljenv;

// these are the risky codes that call exit function
void externalRiskyCode();
void externalRiskyCodeCompiled();

// riskCode protected by handler
// this method will not exit from JVM
// instead, it will throw exception
JNIEXPORT void JNICALL Java_dexit_SigTerm_callExitCode
  (JNIEnv *env, jclass obj) {

  // set the long jump for the exit call handler
  // if handler is called it will jump
  // here with the error code specified
  // as parameter of siglongjmp
  // first call to sigsetjmp returns 0
  if( sigsetjmp(ljenv,1) == 0) {
    // call the code that will try to exit
    externalRiskyCodeCompiled();
  } else {
    // if the code tries to call exit, we are handling it
    // we can allocate a little bit more than we require
    char exceptionBuffer[1024];
    sprintf(exceptionBuffer, "Error");
    (*env)->ThrowNew(	env, 
		(*env)->FindClass( 	env, "java/lang/Exception"), 
											exceptionBuffer);
  }
}

// unprotected call to riskyCode; JVM will exit after this call
JNIEXPORT void JNICALL Java_dexit_SigTermExtLib_callExitCodeExtLib
  (JNIEnv *env, jclass obj) {

  // set the long jump for the exit call handler
  // if handler is called it will jump
  // here with the error code specified
  // as parameter of siglongjmp
  // first call to sigsetjmp returns 0
  if( sigsetjmp(ljenv,1) == 0) {
    // call the code that will try to exit
    externalRiskyCode();
  } else {
    // if the code tries to call exit, we are handling it
    // we can allocate a little bit more than we require
    char exceptionBuffer[1024];
    sprintf(exceptionBuffer, "Error");
    (*env)->ThrowNew(   env,
                (*env)->FindClass(      env, "java/lang/Exception"),
                                                                                        exceptionBuffer);
  }
}
