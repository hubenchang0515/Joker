#ifndef FILE_INFO_H
#define FILE_INFO_H

#ifdef __cplusplus
    extern "C" {
#endif


const char* fileinfo_name(const char* path, char* output, size_t size);
const char* fileinfo_directory(const char* path, char* output, size_t size);
const char* fileinfo_fullExtension(const char* path, char* output, size_t size);
const char* fileinfo_lastExtension(const char* path, char* output, size_t size);


#ifdef __cplusplus
    }
#endif

#endif // FILE_INFO_H