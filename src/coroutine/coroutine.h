#ifndef COROUTINE_H
#define COROUTINE_H

#ifdef __cplusplus
    extern "C" {
#endif

typedef struct Coroutine Coroutine;
typedef void* (*CoFunc)(Coroutine*, void*);

struct Coroutine
{
    int state;
    CoFunc function;
};

#define CoBegin(co)               switch((co)->state) { case 0:
#define CoEnd()                   }
#define CoResume(co, userdate)    (co)->function(co, userdate)
#define CoYield(co, value)        (co)->state = __LINE__; return value; case __LINE__:

static void CoCreate(Coroutine* output, CoFunc func)
{
    output->state = 0;
    output->function = func;
}

#ifdef __cplusplus
    }
#endif

#endif // COROUTINE_H