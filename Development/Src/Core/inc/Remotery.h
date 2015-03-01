#include "coredll.h"

#ifndef RMT_INCLUDED_H
#define RMT_INCLUDED_H

#if USE_PROFILER

// Used by the Celtoys TinyCRT library (not released yet)
//#define RMT_USE_TINYCRT

// Assuming CUDA headers/libs are setup, allow CUDA profiling
//#define RMT_USE_CUDA

// Assuming Direct3D 11 headers/libs are setup, allow D3D11 profiling
//#define RMT_USE_D3D11

// Allow OpenGL profiling
//#define RMT_USE_OPENGL


/*
------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------
   Compiler/Platform Detection and Preprocessor Utilities
------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------
*/



// Compiler identification
#if defined(_MSC_VER)
    #define RMT_COMPILER_MSVC
#elif defined(__GNUC__)
    #define RMT_COMPILER_GNUC
#elif defined(__clang__)
    #define RMT_COMPILER_CLANG
#endif


// Platform identification
#if defined(_WINDOWS) || defined(_WIN32)
    #define RMT_PLATFORM_WINDOWS
#elif defined(__linux__)
    #define RMT_PLATFORM_LINUX
    #define RMT_PLATFORM_POSIX
#elif defined(__APPLE__)
    #define RMT_PLATFORM_MACOS
    #define RMT_PLATFORM_POSIX
#endif


// Allows macros to be written that can work around the inability to do: #define(x) #ifdef x
// with the C preprocessor.
#ifdef USE_PROFILER
    #define IFDEF_USE_PROFILER(t, f) t
#else
    #define IFDEF_USE_PROFILER(t, f) f
#endif
#if defined(USE_PROFILER) && defined(RMT_USE_CUDA)
    #define IFDEF_RMT_USE_CUDA(t, f) t
#else
    #define IFDEF_RMT_USE_CUDA(t, f) f
#endif
#if defined(USE_PROFILER) && defined(RMT_USE_D3D11)
    #define IFDEF_RMT_USE_D3D11(t, f) t
#else
    #define IFDEF_RMT_USE_D3D11(t, f) f
#endif
#if defined(USE_PROFILER) && defined(RMT_USE_OPENGL)
#define IFDEF_RMT_USE_OPENGL(t, f) t
#else
#define IFDEF_RMT_USE_OPENGL(t, f) f
#endif


// Public interface is written in terms of these macros to easily enable/disable itself
#define RMT_OPTIONAL(macro, x) IFDEF_ ## macro(x, )
#define RMT_OPTIONAL_RET(macro, x, y) IFDEF_ ## macro(x, (y))



/*
------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------
   Types
------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------
*/



// Boolean
typedef unsigned int rmtBool;
#define RMT_TRUE ((rmtBool)1)
#define RMT_FALSE ((rmtBool)0)


// Unsigned integer types
typedef unsigned char rmtU8;
typedef unsigned short rmtU16;
typedef unsigned int rmtU32;
typedef unsigned long long rmtU64;


// Signed integer types
typedef char rmtS8;
typedef short rmtS16;
typedef int rmtS32;
typedef long long rmtS64;


// Const, null-terminated string pointer
typedef const char* rmtPStr;


// Handle to the main remotery instance
typedef struct Remotery Remotery;


// All possible error codes
typedef enum rmtError
{
    RMT_ERROR_NONE,

    // System errors
    RMT_ERROR_MALLOC_FAIL,                      // Malloc call within remotery failed
    RMT_ERROR_TLS_ALLOC_FAIL,                   // Attempt to allocate thread local storage failed
    RMT_ERROR_VIRTUAL_MEMORY_BUFFER_FAIL,       // Failed to create a virtual memory mirror buffer
    RMT_ERROR_CREATE_THREAD_FAIL,               // Failed to create a thread for the server

    // Network TCP/IP socket errors
    RMT_ERROR_SOCKET_INIT_NETWORK_FAIL,         // Network initialisation failure (e.g. on Win32, WSAStartup fails)
    RMT_ERROR_SOCKET_CREATE_FAIL,               // Can't create a socket for connection to the remote viewer
    RMT_ERROR_SOCKET_BIND_FAIL,                 // Can't bind a socket for the server
    RMT_ERROR_SOCKET_LISTEN_FAIL,               // Created server socket failed to enter a listen state
    RMT_ERROR_SOCKET_SET_NON_BLOCKING_FAIL,     // Created server socket failed to switch to a non-blocking state
    RMT_ERROR_SOCKET_INVALID_POLL,              // Poll attempt on an invalid socket
    RMT_ERROR_SOCKET_SELECT_FAIL,               // Server failed to call select on socket
    RMT_ERROR_SOCKET_POLL_ERRORS,               // Poll notified that the socket has errors
    RMT_ERROR_SOCKET_ACCEPT_FAIL,               // Server failed to accept connection from client
    RMT_ERROR_SOCKET_SEND_TIMEOUT,              // Timed out trying to send data
    RMT_ERROR_SOCKET_SEND_FAIL,                 // Unrecoverable error occured while client/server tried to send data
    RMT_ERROR_SOCKET_RECV_NO_DATA,              // No data available when attempting a receive
    RMT_ERROR_SOCKET_RECV_TIMEOUT,              // Timed out trying to receive data
    RMT_ERROR_SOCKET_RECV_FAILED,               // Unrecoverable error occured while client/server tried to receive data

    // WebSocket errors
    RMT_ERROR_WEBSOCKET_HANDSHAKE_NOT_GET,      // WebSocket server handshake failed, not HTTP GET
    RMT_ERROR_WEBSOCKET_HANDSHAKE_NO_VERSION,   // WebSocket server handshake failed, can't locate WebSocket version
    RMT_ERROR_WEBSOCKET_HANDSHAKE_BAD_VERSION,  // WebSocket server handshake failed, unsupported WebSocket version
    RMT_ERROR_WEBSOCKET_HANDSHAKE_NO_HOST,      // WebSocket server handshake failed, can't locate host
    RMT_ERROR_WEBSOCKET_HANDSHAKE_BAD_HOST,     // WebSocket server handshake failed, host is not allowed to connect
    RMT_ERROR_WEBSOCKET_HANDSHAKE_NO_KEY,       // WebSocket server handshake failed, can't locate WebSocket key
    RMT_ERROR_WEBSOCKET_HANDSHAKE_BAD_KEY,      // WebSocket server handshake failed, WebSocket key is ill-formed
    RMT_ERROR_WEBSOCKET_HANDSHAKE_STRING_FAIL,  // WebSocket server handshake failed, internal error, bad string code
    RMT_ERROR_WEBSOCKET_DISCONNECTED,           // WebSocket server received a disconnect request and closed the socket
    RMT_ERROR_WEBSOCKET_BAD_FRAME_HEADER,       // Couldn't parse WebSocket frame header
    RMT_ERROR_WEBSOCKET_BAD_FRAME_HEADER_SIZE,  // Partially received wide frame header size
    RMT_ERROR_WEBSOCKET_BAD_FRAME_HEADER_MASK,  // Partially received frame header data mask
    RMT_ERROR_WEBSOCKET_RECEIVE_TIMEOUT,        // Timeout receiving frame header

    RMT_ERROR_REMOTERY_NOT_CREATED,             // Remotery object has not been created
    RMT_ERROR_SEND_ON_INCOMPLETE_PROFILE,       // An attempt was made to send an incomplete profile tree to the client

    // CUDA error messages
    RMT_ERROR_CUDA_DEINITIALIZED,               // This indicates that the CUDA driver is in the process of shutting down
    RMT_ERROR_CUDA_NOT_INITIALIZED,             // This indicates that the CUDA driver has not been initialized with cuInit() or that initialization has failed
    RMT_ERROR_CUDA_INVALID_CONTEXT,             // This most frequently indicates that there is no context bound to the current thread
    RMT_ERROR_CUDA_INVALID_VALUE,               // This indicates that one or more of the parameters passed to the API call is not within an acceptable range of values
    RMT_ERROR_CUDA_INVALID_HANDLE,              // This indicates that a resource handle passed to the API call was not valid
    RMT_ERROR_CUDA_OUT_OF_MEMORY,               // The API call failed because it was unable to allocate enough memory to perform the requested operation
    RMT_ERROR_ERROR_NOT_READY,                  // This indicates that a resource handle passed to the API call was not valid

    // Direct3D 11 error messages
    RMT_ERROR_D3D11_FAILED_TO_CREATE_QUERY,     // Failed to create query for sample

    // OpenGL error messages
    RMT_ERROR_OPENGL_ERROR,                     // Generic OpenGL error, no real need to expose more detail since app will probably have an OpenGL error callback registered

    RMT_ERROR_CUDA_UNKNOWN,
} rmtError;



/*
------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------
   Public Interface
------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------
*/



// Can call remotery functions on a null pointer
// TODO: Can embed extern "C" in these macros?

#define rmt_Settings()																\
    RMT_OPTIONAL_RET(USE_PROFILER, _rmt_Settings(), NULL )

#define rmt_CreateGlobalInstance(rmt)                                               \
    RMT_OPTIONAL_RET(USE_PROFILER, _rmt_CreateGlobalInstance(rmt), RMT_ERROR_NONE)

#define rmt_DestroyGlobalInstance(rmt)                                              \
    RMT_OPTIONAL(USE_PROFILER, _rmt_DestroyGlobalInstance(rmt))

#define rmt_SetGlobalInstance(rmt)                                                  \
    RMT_OPTIONAL(USE_PROFILER, _rmt_SetGlobalInstance(rmt))

#define rmt_GetGlobalInstance()                                                     \
    RMT_OPTIONAL_RET(USE_PROFILER, _rmt_GetGlobalInstance(), NULL)

#define rmt_SetCurrentThreadName(rmt)                                               \
    RMT_OPTIONAL(USE_PROFILER, _rmt_SetCurrentThreadName(rmt))

#define rmt_LogText(text)                                                           \
    RMT_OPTIONAL(USE_PROFILER, _rmt_LogText(text))

#define rmt_BeginCPUSample(name)                                                    \
    RMT_OPTIONAL(USE_PROFILER, {                                                     \
        static rmtU32 rmt_sample_hash_##name = 0;                                   \
        _rmt_BeginCPUSample(#name, &rmt_sample_hash_##name);                        \
    })

#define rmt_EndCPUSample()                                                          \
    RMT_OPTIONAL(USE_PROFILER, _rmt_EndCPUSample())


// Memory allocation functions
typedef void* (*rmtMallocPtr )(void* mm_context, rmtU32 size);
typedef void(*rmtFreePtr)(void* mm_context, void* ptr);


// Struture to fill in to modify Remotery default settings
typedef struct rmtSettings
{
    rmtU32 port;

    // How long to sleep between server updates, hopefully trying to give
    // a little CPU back to other threads.
    rmtU32 msSleepBetweenServerUpdates;

    // Size of the internal message queues Remotery uses
    // Will be rounded to page granularity of 64k
    rmtU32 messageQueueSizeInBytes;

    // If the user continuously pushes to the message queue, the server network
    // code won't get a chance to update unless there's an upper-limit on how
    // many messages can be consumed per loop.
    rmtU32 maxNbMessagesPerUpdate;

    rmtMallocPtr malloc;
    rmtFreePtr free;
    void* mm_context;
    
    rmtPStr logFilename;
} rmtSettings;


// Structure to fill in when binding CUDA to Remotery
typedef struct rmtCUDABind
{
    // The main context that all driver functions apply before each call
    void* context;

    // Driver API function pointers that need to be pointed to
    // Untyped so that the CUDA headers are not required in this file
    // NOTE: These are named differently to the CUDA functions because the CUDA API has a habit of using
    // macros to point function calls to different versions, e.g. cuEventDestroy is a macro for
    // cuEventDestroy_v2.
    void* CtxSetCurrent;
    void* CtxGetCurrent;
    void* EventCreate;
    void* EventDestroy;
    void* EventRecord;
    void* EventQuery;
    void* EventElapsedTime;

} rmtCUDABind;


// Call once after you've initialised CUDA to bind it to Remotery
#define rmt_BindCUDA(bind)                                                  \
    RMT_OPTIONAL(RMT_USE_CUDA, _rmt_BindCUDA(bind))

// Mark the beginning of a CUDA sample on the specified asynchronous stream
#define rmt_BeginCUDASample(name, stream)                                   \
    RMT_OPTIONAL(RMT_USE_CUDA, {                                            \
        static rmtU32 rmt_sample_hash_##name = 0;                           \
        _rmt_BeginCUDASample(#name, &rmt_sample_hash_##name, stream);       \
    })

// Mark the end of a CUDA sample on the specified asynchronous stream
#define rmt_EndCUDASample(stream)                                           \
    RMT_OPTIONAL(RMT_USE_CUDA, _rmt_EndCUDASample(stream))


#define rmt_BindD3D11(device, context)                                      \
    RMT_OPTIONAL(RMT_USE_D3D11, _rmt_BindD3D11(device, context))

#define rmt_UnbindD3D11()                                                   \
    RMT_OPTIONAL(RMT_USE_D3D11, _rmt_UnbindD3D11())

#define rmt_BeginD3D11Sample(name)                                          \
    RMT_OPTIONAL(RMT_USE_D3D11, {                                           \
        static rmtU32 rmt_sample_hash_##name = 0;                           \
        _rmt_BeginD3D11Sample(#name, &rmt_sample_hash_##name);              \
    })

#define rmt_EndD3D11Sample()                                                \
    RMT_OPTIONAL(RMT_USE_D3D11, _rmt_EndD3D11Sample())


#define rmt_BindOpenGL()                                                    \
    RMT_OPTIONAL(RMT_USE_OPENGL, _rmt_BindOpenGL())

#define rmt_UnbindOpenGL()                                                  \
    RMT_OPTIONAL(RMT_USE_OPENGL, _rmt_UnbindOpenGL())

#define rmt_BeginOpenGLSample(name)                                         \
    RMT_OPTIONAL(RMT_USE_OPENGL, {                                          \
        static rmtU32 rmt_sample_hash_##name = 0;                           \
        _rmt_BeginOpenGLSample(#name, &rmt_sample_hash_##name);             \
    })

#define rmt_EndOpenGLSample()                                               \
    RMT_OPTIONAL(RMT_USE_OPENGL, _rmt_EndOpenGLSample())

/*
------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------
Private Interface - don't directly call these
------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------
*/

CORE_API rmtSettings* _rmt_Settings(void);
CORE_API enum rmtError _rmt_CreateGlobalInstance(Remotery** remotery);
CORE_API void _rmt_DestroyGlobalInstance(Remotery* remotery);
CORE_API void _rmt_SetGlobalInstance(Remotery* remotery);
CORE_API Remotery* _rmt_GetGlobalInstance(void);
CORE_API void _rmt_SetCurrentThreadName(rmtPStr thread_name);
CORE_API void _rmt_LogText(rmtPStr text);
CORE_API void _rmt_BeginCPUSample(rmtPStr name, rmtU32* hash_cache);
// Types that end samples in their destructors
CORE_API void _rmt_EndCPUSample(void);

#ifdef RMT_USE_CUDA
void _rmt_BindCUDA(const rmtCUDABind* bind);
void _rmt_BeginCUDASample(rmtPStr name, rmtU32* hash_cache, void* stream);
void _rmt_EndCUDASample(void* stream);
#endif

#ifdef RMT_USE_D3D11
void _rmt_BindD3D11(void* device, void* context);
void _rmt_UnbindD3D11(void);
void _rmt_BeginD3D11Sample(rmtPStr name, rmtU32* hash_cache);
void _rmt_EndD3D11Sample(void);
#endif

#ifdef RMT_USE_OPENGL
void _rmt_BindOpenGL();
void _rmt_UnbindOpenGL(void);
void _rmt_BeginOpenGLSample(rmtPStr name, rmtU32* hash_cache);
void _rmt_EndOpenGLSample(void);
#endif

#define PROFILER_ENABLE() PROFILER_LOG("start profiling");
#define PROFILER_DISABLE() PROFILER_LOG("end profiling");
#define PROFILER_START(x) rmt_BeginCPUSample(x)
#define PROFILER_END() rmt_EndCPUSample();
#define PROFILER_LOG(x) rmt_LogText(x)
#define PROFILER_SET_CATEGORY(x) rmt_SetCurrentThreadName(x)

struct rmt_EndCPUSampleOnScopeExit
{
    ~rmt_EndCPUSampleOnScopeExit()
    {
        _rmt_EndCPUSample();
    }
};
#ifdef RMT_USE_CUDA
struct rmt_EndCUDASampleOnScopeExit
{
    rmt_EndCUDASampleOnScopeExit(void* stream) : stream(stream)
    {
    }
    ~rmt_EndCUDASampleOnScopeExit()
    {
        _rmt_EndCUDASample(stream);
    }
    void* stream;
};
#endif
#ifdef RMT_USE_D3D11
struct rmt_EndD3D11SampleOnScopeExit
{
    ~rmt_EndD3D11SampleOnScopeExit()
    {
        _rmt_EndD3D11Sample();
    }
};
#endif

#ifdef RMT_USE_OPENGL
struct rmt_EndOpenGLSampleOnScopeExit
{
    ~rmt_EndOpenGLSampleOnScopeExit()
    {
        _rmt_EndOpenGLSample();
    }
};
#endif

// Pairs a call to rmt_Begin<TYPE>Sample with its call to rmt_End<TYPE>Sample when leaving scope
#define rmt_ScopedCPUSample(name)                                                                       \
        RMT_OPTIONAL(USE_PROFILER, rmt_BeginCPUSample(name));                                            \
        RMT_OPTIONAL(USE_PROFILER, rmt_EndCPUSampleOnScopeExit rmt_ScopedCPUSample##name);
#define rmt_ScopedCUDASample(name, stream)                                                              \
        RMT_OPTIONAL(RMT_USE_CUDA, rmt_BeginCUDASample(name, stream));                                  \
        RMT_OPTIONAL(RMT_USE_CUDA, rmt_EndCUDASampleOnScopeExit rmt_ScopedCUDASample##name(stream));
#define rmt_ScopedD3D11Sample(name)                                                                     \
        RMT_OPTIONAL(RMT_USE_D3D11, rmt_BeginD3D11Sample(name));                                        \
        RMT_OPTIONAL(RMT_USE_D3D11, rmt_EndD3D11SampleOnScopeExit rmt_ScopedD3D11Sample##name);
#define rmt_ScopedOpenGLSample(name)                                                                    \
        RMT_OPTIONAL(RMT_USE_OPENGL, rmt_BeginOpenGLSample(name));                                      \
        RMT_OPTIONAL(RMT_USE_OPENGL, rmt_EndOpenGLSampleOnScopeExit rmt_ScopedOpenGLSample##name);



#else // USE_PROFILER

#define PROFILER_LOG()

#define PROFILER_ENABLE()
#define PROFILER_DISABLE()
#define PROFILER_START(x)
#define PROFILER_END()
#define PROFILER_SET_CATEGORY()

#endif // USE_PROFILER

#endif