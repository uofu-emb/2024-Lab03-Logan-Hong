#ifndef UNITY_CONFIG_H
#define UNITY_CONFIG_H

#ifndef NULL
#ifndef __cplusplus
#define NULL (void*)0
#else
#define NULL 0
#endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

void unityOutputStart();
void unityOutputChar(char);
void unityOutputFlush();
void unityOutputComplete();

// NOTE: Commented out due to multipe definition error. Seems to be working wtihout these.
// #define UNITY_OUTPUT_START()    unityOutputStart()
// #define UNITY_OUTPUT_CHAR(c)    unityOutputChar(c)
// #define UNITY_OUTPUT_FLUSH()    unityOutputFlush()
// #define UNITY_OUTPUT_COMPLETE() unityOutputComplete()

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* UNITY_CONFIG_H */
