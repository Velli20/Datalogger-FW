#ifndef littlefs_trace_h
#define littlefs_trace_h

#ifdef __cplusplus
extern "C" {
#endif

void lfs_trace_impl(const char* file, size_t line, const char* fmt, ...);

#if !defined(LFS_TRACE_ENABLED)
#define LFS_TRACE_IMPL(...)
#define LFS_ASSERT_IMPL(test)
#endif /* not LFS_TRACE */

#if defined(LFS_TRACE_ENABLED)

#define LFS_TRACE_IMPL(file, line, fmt, ...)             \
    do                                                   \
    {                                                    \
        lfs_trace_impl(file, line, (fmt), __VA_ARGS__);    \
    }                                                    \
    while ( 0 )

#define LFS_ASSERT_IMPL(test)                                              \
    do                                                                     \
    {                                                                      \
        if ( !(test) )                                                      \
        {                                                                   \
            lfs_trace_impl(__FILE__, __LINE__, "Condition: ("#test") failed"); \
        }   \
    }                                                                      \
    while ( 0 )

#endif /* LFS_TRACE_ENABLED */

#define LFS_TRACE(fmt, ...) LFS_TRACE_IMPL(__FILE__, __LINE__, (fmt), __VA_ARGS__)
#define LFS_DEBUG(fmt, ...) LFS_TRACE_IMPL(__FILE__, __LINE__, (fmt), __VA_ARGS__)
#define LFS_WARN(fmt, ...)  LFS_TRACE_IMPL(__FILE__, __LINE__, (fmt), __VA_ARGS__)
#define LFS_ERROR(fmt, ...) LFS_TRACE_IMPL(__FILE__, __LINE__, (fmt), __VA_ARGS__)
#define LFS_ASSERT(test)    LFS_ASSERT_IMPL(test)

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* littlefs_trace_h */
