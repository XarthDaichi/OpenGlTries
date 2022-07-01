/*

        Copyright 2014 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <iostream>
#include <fstream>
#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>

#include "ogldev_util.h"

bool ReadFile(const char* pFileName, string& outFile)
{
    ifstream f(pFileName);

    bool ret = false;

    if (f.is_open()) {
        string line;
        while (getline(f, line)) {
            outFile.append(line);
            outFile.append("\n");
        }

        f.close();

        ret = true;
    }
    else {
        OGLDEV_FILE_ERROR(pFileName);
    }

    return ret;
}


#ifdef _WIN32

char* ReadBinaryFile(const char* pFileName, int& size)
{
    HANDLE f = CreateFileA(pFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (f == INVALID_HANDLE_VALUE) {
        OGLDEV_FILE_ERROR(pFileName);
        return NULL;
    }

    size = GetFileSize(f, NULL);

    if (size == INVALID_FILE_SIZE) {
        OGLDEV_ERROR("Invalid file size %s\n", pFileName);
        return NULL;
    }

    char* p = (char*)malloc(size);

    DWORD bytes_read = 0;
    bool b = ReadFile(f, p, size, &bytes_read, NULL);

    return p;
}

#else
char* ReadBinaryFile(const char* pFileName, int& size)
{
    int f = open(pFileName, O_RDONLY);

    if (f == -1) {
        OGLDEV_ERROR("Error opening '%s': %s\n", pFileName, strerror(errno));
        return NULL;
    }

    struct stat stat_buf;
    int error = stat(pFileName, &stat_buf);

    if (error) {
        OGLDEV_ERROR("Error getting file stats: %s\n", strerror(errno));
        return NULL;
    }

    size = stat_buf.st_size;

    char* p = (char*)malloc(size);
    assert(p);

    int read_len = read(f, p, size);

    if (read_len != size) {
        OGLDEV_ERROR("Error reading file: %s\n", strerror(errno));
        return NULL;
    }

    close(f);

    return p;
}
#endif

void OgldevError(const char* pFileName, uint line, const char* format, ...)
{
    char msg[1000];
    va_list args;
    va_start(args, format);
    VSNPRINTF(msg, sizeof(msg), format, args);
    va_end(args);

#ifdef _WIN32
    char msg2[1000];
    _snprintf_s(msg2, sizeof(msg2), "%s:%d: %s", pFileName, line, msg);
    MessageBoxA(NULL, msg2, NULL, 0);
#else
    fprintf(stderr, "%s:%d - %s", pFileName, line, msg);
#endif
}


void OgldevFileError(const char* pFileName, uint line, const char* pFileError)
{
#ifdef _WIN32
    char msg[1000];
    _snprintf_s(msg, sizeof(msg), "%s:%d: unable to open file `%s`", pFileName, line, pFileError);
    MessageBoxA(NULL, msg, NULL, 0);
#else
    fprintf(stderr, "%s:%d: unable to open file `%s`\n", pFileName, line, pFileError);
#endif
}


long long GetCurrentTimeMillis()
{
#ifdef _WIN32
    return GetTickCount();
#else
    timeval t;
    gettimeofday(&t, NULL);

    long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
    return ret;
#endif
}


#define EXIT_ON_GL_ERROR
void gl_check_error(const char* function, const char *file, int line)
{
    GLenum error = 0;
    bool is_error = false;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        is_error = true;
        switch (error)
        {
        case GL_INVALID_ENUM: printf("GL_INVALID_ENUM");
            break;
        case GL_INVALID_VALUE: printf("GL_INVALID_VALUE");
            break;
        case GL_INVALID_OPERATION: printf("INVALID_OPERATION");
            break;
        case GL_STACK_OVERFLOW: printf("STACK_OVERFLOW");
            break;
        case GL_STACK_UNDERFLOW: printf("STACK_UNDERFLOW");
            break;
        case GL_OUT_OF_MEMORY: printf("OUT_OF_MEMORY");
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: printf("INVALID_FRAMEBUFFER_OPERATION");
            break;
        case GL_CONTEXT_LOST: printf("GL_CONTEXT_LOST");
            break;
        case GL_TABLE_TOO_LARGE: printf("GL_TABLE_TOO_LARGE");
            break;
        default:
            printf("Unknown error code %d", error);
        }
        printf(" encountered at function '%s' (%s:%d)\n", function, file, line);
    }
#ifdef EXIT_ON_GL_ERROR
    if (is_error) {
        exit(1);
    }
#endif
}


void glDebugOutput(GLenum source,
                   GLenum type,
                   unsigned int id,
                   GLenum severity,
                   GLsizei length,
                   const char *message,
                   const void *userParam)
{
    printf("!!! Debug callback !!!\n");
    printf("Debug message: id %d, %s\n", id, message);

    printf("Message source: ");
    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             printf("API\n"); break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   printf("Window System\n"); break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: printf("Shader Compiler\n"); break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     printf("Third Party\n"); break;
    case GL_DEBUG_SOURCE_APPLICATION:     printf("Application\n"); break;
    case GL_DEBUG_SOURCE_OTHER:           printf("Other\n"); break;
    }

    printf("Error type: ");
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               printf("Error\n"); break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: printf("Deprecated Behaviour\n"); break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  printf("Undefined Behaviour\n"); break;
    case GL_DEBUG_TYPE_PORTABILITY:         printf("Portability\n"); break;
    case GL_DEBUG_TYPE_PERFORMANCE:         printf("Performance\n"); break;
    case GL_DEBUG_TYPE_MARKER:              printf("Marker\n"); break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          printf("Push Group\n"); break;
    case GL_DEBUG_TYPE_POP_GROUP:           printf("Pop Group\n"); break;
    case GL_DEBUG_TYPE_OTHER:               printf("Other\n"); break;
    }

    printf("Severity: ");
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         printf("High\n"); break;
    case GL_DEBUG_SEVERITY_MEDIUM:       printf("Medium\n"); break;
    case GL_DEBUG_SEVERITY_LOW:          printf("Low\n"); break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: printf("Notification\n"); break;
    }
}