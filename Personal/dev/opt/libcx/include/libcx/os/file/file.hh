/** @file cx/os/file/file.hh **/

#ifndef CX_OS_FILE_FILE_HH
#define CX_OS_FILE_FILE_HH

#include "libcx/uti/utilities.hh"
#include "libcx/mem/_allocator.hh"

namespace cx::file {

// −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
//  Forward declarations

struct File;
struct FileOperations;

union FileDescriptor;

using FileTime = u64;

// −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
//  Definitions

enum FileMode {
    FileMode_Read = BIT<0>,
    FileMode_Write = BIT<1>,
    FileMode_Append = BIT<2>,
    FileMode_ReadWrite = BIT<3>,
    FileMode_Mask = FileMode_Read | FileMode_Write | FileMode_Append | FileMode_ReadWrite
};

enum FileWhence {
    FileWhenceBegin,
    FileWhenceCurrent,
    FileWhenceEnd,
};

enum FileError {
    FileError_None,
    FileError_Invalid,
    FileError_Filename,
    FileError_Exists,
    FileError_NotExists,
    FileError_Permission,
    FileError_TruncFail,
};

enum FileStandard {
    FileStandardInput,
    FileStandardOutput,
    FileStandardError,

    FileStandardCount,
};

union FileDescriptor {
    void* p;
    iptr i;
    uptr u;
};

#define CX_FILE_OPEN_PROC(name) \
  func name(FileDescriptor* fd, FileOperations* ops, FileMode mode, cstring name) noexce -> FileError
#define CX_FILE_READ_AT_PROC(name) \
  func name(FileDescriptor fd, void* buf, isize size, i64 off, isize* read) noexce -> i32
#define CX_FILE_WRITE_AT_PROC(name) \
  func name(FileDescriptor fd, void const* buf, isize size, i64 off, isize* written) noexce -> i32
#define CX_FILE_SEEK_PROC(name) \
  func name(FileDescriptor fd, i64 cur_off, FileWhence whence, i64* new_off) noexce -> i32
#define CX_FILE_CLOSE_PROC(name) \
  func name(FileDescriptor fd) noexce -> void

typedef CX_FILE_OPEN_PROC(FileOpenProc);
typedef CX_FILE_READ_AT_PROC(FileReadProc);
typedef CX_FILE_WRITE_AT_PROC(FileWriteProc);
typedef CX_FILE_SEEK_PROC(FileSeekProc);
typedef CX_FILE_CLOSE_PROC(FileCloseProc);

struct FileOperations {
    FileReadProc* read_at;
    FileWriteProc* write_at;
    FileSeekProc* seek;
    FileCloseProc* close;
};

struct File {
    FileOperations ops;
    FileDescriptor fd;
    char const* name;
    FileTime mtime;
};

struct FileStandardConfig {
    b32 are_std_set = false;
    File files[FileStandardCount]{};
};

priv FileStandardConfig cx__std_cfg{};

inln fn get_standard_config() noexce -> FileStandardConfig*
{
    return &cx__std_cfg;
}

inln fn are_standard_streams_set() noexce -> i32
{
    return get_standard_config()->are_std_set;
}

// Operations (platform specific)

#if CX_SYSTEM_WIN

// TODO:

#else  // POSIX

priv fn cx__posix_file_seek(
    FileDescriptor fd, i64 cur_off, FileWhence whence, i64* new_off
) noexce -> i32 {
#if CX_SYSTEM_OSX
    i64 res = lseek(fd.i, cur_off,  whence);
#else  // LINUX
    i64 res = lseek64(fd.s, cur_off, whence);
#endif

    if (res < 0) {
        return false;
    }
    if (new_off) {
        *new_off = res;
    }
    return true;
}

priv fn cx__posix_file_read(
    FileDescriptor fd, void* buf, isize size, i64 off, isize* read
) noexce -> i32 {
    isize res = pread(fd.i, buf, size, off);
    if (res < 0) {
        return false;
    }
    if (read) {
        *read = res;
    }
    return true;
}

priv fn cx__posix_file_write(
    FileDescriptor fd, void const* buf, isize size, i64 off, isize* written
) noexce -> i32 {
    isize res;
    i64 cur_off = 0;
    cx__posix_file_seek(fd, 0, FileWhenceCurrent, &cur_off);
    if (cur_off == off) {
        res = write(int(fd.i), buf, size);
    } else {
        res = pwrite(fd.i, buf, size, off);
    }
    if (res < 0) {
        return false;
    }
    if (written) {
        *written = res;
    }
    return true;
}

priv fn cx__posix_file_close(FileDescriptor fd) noexce -> void { close(fd.i); }

priv fn default_ops() noexce -> FileOperations
{
    persist FileOperations default_file_ops{
        cx__posix_file_read, cx__posix_file_write, cx__posix_file_seek, cx__posix_file_close
    };
    return default_file_ops;
}

noinln fn cx__posix_file_open(
    FileDescriptor* fd, FileOperations* ops, FileMode mode, cstring name
) noexce -> FileError {
    i32 os_mode;
    switch (mode & FileMode_Mask) {
    case FileMode_Read:
        os_mode = O_RDONLY;
        break;
    case FileMode_Write:
        os_mode = O_WRONLY | O_CREAT | O_TRUNC;
        break;
    case FileMode_Append:
        os_mode = O_WRONLY | O_CREAT | O_APPEND;
        break;
    case FileMode_Read | FileMode_ReadWrite:
        os_mode = O_RDWR;
        break;
    case FileMode_Write | FileMode_ReadWrite:
        os_mode = O_RDWR | O_CREAT | O_TRUNC;
        break;
    case FileMode_Append | FileMode_ReadWrite:
        os_mode = O_RDWR | O_APPEND | O_CREAT;
        break;
    default:
        cx_panic("invalid file mode");
        return FileError_Invalid;
    }

    fd->i = open(name, os_mode, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (fd->i < 0) {
        return FileError_Invalid;
    }

    *ops = default_ops();
    return FileError_None;
}

#endif

inln fn set_standard_to_default() noexce -> void
{
  if (are_standard_streams_set()) {
    return;
  }
  FileStandardConfig* fsc = get_standard_config();

  #define CX_SET_STD_FILE(type, val)         \
    fsc->files[type].fd.i = val;             \
    fsc->files[type].ops = default_ops()

  CX_SET_STD_FILE((i8) FileStandardInput, 0);
  CX_SET_STD_FILE((i8) FileStandardOutput, 1);
  CX_SET_STD_FILE((i8) FileStandardError, 2);

  #undef CX_SET_STD_FILE

  fsc->are_std_set = 1u;
}

inln fn get_standard(FileStandard type) noexce -> File* 
{
  if (!are_standard_streams_set()) {
    set_standard_to_default();
  }
  return &get_standard_config()->files[(i8) type];
}

fn file_init(File *file, FileDescriptor fd, FileOperations ops, cstring fname) noexce -> FileError
{
    isize len = strlen(fname); // TODO: custom strlen!!!
    file->ops = ops;
    file->fd = fd;
    auto [ptr, _] = aligned_alloc(heap_allocator(), len * size_of(char));
    mem_copy(ptr, fname, len);
    file->name = cstring(ptr);
    return FileError_None;
}

fn file_open_mode(File *file, FileMode mode, cstring fname) noexce -> FileError
{
    FileError err = FileError_None;
#if CX_SYSTEM_WIN
    err = cx__win_file_open(&file->fd, &file->ops, mode, fname);
#else
    err = cx__posix_file_open(&file->fd, &file->ops, mode, fname);
#endif
    if (err != FileError_None) {
        return err;
    }
    return file_init(file, file->fd, file->ops, fname);
}

fn cx_file_close(File *file) noexce -> FileError {
	if (file == null) {
		return FileError_Invalid;
	}
	if (file->name != null) {
        mem::aligned_free(heap_allocator(), cast(char *, file->name));
    }

#if CX_SYSTEM_WIN
	if (file->fd.p == INVALID_HANDLE_VALUE) {
		return FileError_Invalid;
	}
#else
	if (file->fd.i < 0) {
		return FileError_Invalid;
	}
#endif

    if (not file->ops.read_at) {
        file->ops = default_ops();
    }
    file->ops.close(file->fd);

	return FileError_None;
}


}       // namespace cx::file
#endif  // CX_OS_FILE_FILE_HH
        
// gb_inline b32 gb_file_read_at_check(gbFile *f, void *buffer, isize size, i64 offset, isize *bytes_read) {
// 	if (!f->ops.read_at) f->ops = gbDefaultFileOperations;
// 	return f->ops.read_at(f->fd, buffer, size, offset, bytes_read);
// }


// gb_inline b32 gb_file_write_at_check(gbFile* f, void const* buffer, isize size,
//                                      i64 offset, isize* bytes_written) {
//   if (!f->ops.read_at)
//     f->ops = gbDefaultFileOperations;
//   return f->ops.write_at(f->fd, buffer, size, offset, bytes_written);
// }
//

// gb_inline b32 gb_file_write_at(gbFile* f, void const* buffer, isize size, i64 offset) {
//   return gb_file_write_at_check(f, buffer, size, offset, NULL);
// }

// gb_inline i64 gb_file_tell(gbFile* f) {
//   i64 new_offset = 0;
//   if (!f->ops.read_at)
//     f->ops = gbDefaultFileOperations;
//   f->ops.seek(f->fd, 0, gbSeekWhence_Current, &new_offset);
//   return new_offset;
// }

// gb_inline b32 gb_file_write(gbFile* f, void const* buffer, isize size) {
//   return gb_file_write_at(f, buffer, size, gb_file_tell(f));
// }

// gb_inline isize gb_fprintf_va(struct gbFile* f, char const* fmt, va_list va) {
//   char buf[4096];
//   va_list va_save;
//   va_copy(va_save, va);
//   isize len = gb_snprintf_va(buf, gb_size_of(buf), fmt, va_save);
//   va_end(va_save);
//   char* new_buf = NULL;
//   isize n = gb_size_of(buf);
//   while (len < 0) {
//     va_copy(va_save, va);
//     defer(va_end(va_save));
//     n <<= 1;
//     gb_free(gb_heap_allocator(), new_buf);
//     new_buf = gb_alloc_array(gb_heap_allocator(), char, n);
//     ;
//     len = gb_snprintf_va(new_buf, n, fmt, va_save);
//   }
//   if (new_buf != NULL) {
//     gb_file_write(f, new_buf,
//                   len - 1);  // NOTE(bill): prevent extra whitespace
//     gb_free(gb_heap_allocator(), new_buf);
//   } else {
//     gb_file_write(f, buf,
//                   len - 1);  // NOTE(bill): prevent extra whitespace
//   }
//   return len;
// }

// gb_no_inline isize gb_snprintf_va(char* text, isize max_len, char const* fmt, va_list va) {
//   char const* text_begin = text;
//   isize remaining = max_len - 1, res;
//
//   while (*fmt && remaining > 0) {
//     gbprivFmtInfo info = {0};
//     isize len = 0;
//     info.precision = -1;
//
//     while (remaining > 0 && *fmt && *fmt != '%') {
//       *text++ = *fmt++;
//       remaining--;
//     }
//
//     if (*fmt == '%') {
//       do {
//         switch (*++fmt) {
//         case '-':
//           info.flags |= gbFmt_Minus;
//           break;
//         case '+':
//           info.flags |= gbFmt_Plus;
//           break;
//         case '#':
//           info.flags |= gbFmt_Alt;
//           break;
//         case ' ':
//           info.flags |= gbFmt_Space;
//           break;
//         case '0':
//           info.flags |= gbFmt_Zero;
//           break;
//         default:
//           info.flags |= gbFmt_Done;
//           break;
//         }
//       } while (!(info.flags & gbFmt_Done));
//     }
//
//     // NOTE(bill): Optional Width
//     if (*fmt == '*') {
//       int width = va_arg(va, int);
//       if (width < 0) {
//         info.flags |= gbFmt_Minus;
//         info.width = -width;
//       } else {
//         info.width = width;
//       }
//       fmt++;
//     } else {
//       info.width = cast(i32) gb_str_to_i64(fmt, cast(char**) & fmt, 10);
//     }
//
//     // NOTE(bill): Optional Precision
//     if (*fmt == '.') {
//       fmt++;
//       if (*fmt == '*') {
//         info.precision = va_arg(va, int);
//         fmt++;
//       } else {
//         info.precision = cast(i32) gb_str_to_i64(fmt, cast(char**) & fmt, 10);
//       }
//       info.flags &= ~gbFmt_Zero;
//     }
//
//
//     switch (*fmt++) {
//     case 'h':
//       if (*fmt == 'h') {  // hh => char
//         info.flags |= gbFmt_Char;
//         fmt++;
//       } else {  // h => short
//         info.flags |= gbFmt_Short;
//       }
//       break;
//
//     case 'l':
//       if (*fmt == 'l') {  // ll => long long
//         info.flags |= gbFmt_Llong;
//         fmt++;
//       } else {  // l => long
//         info.flags |= gbFmt_Long;
//       }
//       break;
//
//       break;
//
//     case 'z':  // NOTE(bill): usize
//       info.flags |= gbFmt_Unsigned;
//       // fallthrough
//     case 't':  // NOTE(bill): isize
//       info.flags |= gbFmt_Size;
//       break;
//
//     default:
//       fmt--;
//       break;
//     }
//
//
//     switch (*fmt) {
//     case 'u':
//       info.flags |= gbFmt_Unsigned;
//       // fallthrough
//     case 'd':
//     case 'i':
//       info.base = 10;
//       break;
//
//     case 'o':
//       info.base = 8;
//       break;
//
//     case 'x':
//       info.base = 16;
//       info.flags |= (gbFmt_Unsigned | gbFmt_Lower);
//       break;
//
//     case 'X':
//       info.base = 16;
//       info.flags |= (gbFmt_Unsigned | gbFmt_Upper);
//       break;
//
//     case 'f':
//     case 'F':
//     case 'g':
//     case 'G':
//       len = gb__print_f64(text, remaining, &info, va_arg(va, f64));
//       break;
//
//     case 'a':
//     case 'A':
//       // TODO(bill):
//       break;
//
//     case 'c':
//       len = gb__print_char(text, remaining, &info, cast(char) va_arg(va, int));
//       break;
//
//     case 's':
//       len = gb__print_string(text, remaining, &info, va_arg(va, char*));
//       break;
//
//     case 'p':
//       info.base = 16;
//       info.flags |= (gbFmt_Lower | gbFmt_Unsigned | gbFmt_Alt | gbFmt_Intptr);
//       break;
//
//     case '%':
//       len = gb__print_char(text, remaining, &info, '%');
//       break;
//
//     default:
//       fmt--;
//       break;
//     }
//
//     fmt++;
//
//     if (info.base != 0) {
//       if (info.flags & gbFmt_Unsigned) {
//         u64 value = 0;
//         switch (info.flags & gbFmt_Ints) {
//         case gbFmt_Char:
//           value = cast(u64) cast(u8) va_arg(va, int);
//           break;
//         case gbFmt_Short:
//           value = cast(u64) cast(u16) va_arg(va, int);
//           break;
//         case gbFmt_Long:
//           value = cast(u64) va_arg(va, unsigned long);
//           break;
//         case gbFmt_Llong:
//           value = cast(u64) va_arg(va, unsigned long long);
//           break;
//         case gbFmt_Size:
//           value = cast(u64) va_arg(va, usize);
//           break;
//         case gbFmt_Intptr:
//           value = cast(u64) va_arg(va, uintptr);
//           break;
//         default:
//           value = cast(u64) va_arg(va, unsigned int);
//           break;
//         }
//
//         len = gb__print_u64(text, remaining, &info, value);
//
//       } else {
//         i64 value = 0;
//         switch (info.flags & gbFmt_Ints) {
//         case gbFmt_Char:
//           value = cast(i64) cast(i8) va_arg(va, int);
//           break;
//         case gbFmt_Short:
//           value = cast(i64) cast(i16) va_arg(va, int);
//           break;
//         case gbFmt_Long:
//           value = cast(i64) va_arg(va, long);
//           break;
//         case gbFmt_Llong:
//           value = cast(i64) va_arg(va, long long);
//           break;
//         case gbFmt_Size:
//           value = cast(i64) va_arg(va, usize);
//           break;
//         case gbFmt_Intptr:
//           value = cast(i64) va_arg(va, uintptr);
//           break;
//         default:
//           value = cast(i64) va_arg(va, int);
//           break;
//         }
//
//         len = gb__print_i64(text, remaining, &info, value);
//       }
//     }
//
//
//     text += len;
//     if (len >= remaining) {
//       remaining = 0;
//     } else {
//       remaining -= len;
//     }
//   }
//
//   *text++ = '\0';
//   res = (text - text_begin);
//   return (res >= max_len || res < 0) ? -1 : res;
// }
