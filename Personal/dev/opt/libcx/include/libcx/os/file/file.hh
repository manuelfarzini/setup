/// \file cx/os/file/file.hh
#pragma once
#include <libcx/uti/utilities.hh>

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
  FileModeRead = cx_bit(0),
  FileModeWrite = cx_bit(1),
  FileModeAppend = cx_bit(2),
  FileModeReadWrite = cx_bit(3),
  FileModeMask = FileModeRead | FileModeWrite | FileModeAppend | FileModeReadWrite
};

// // NOTE:(manu)
// // Idea
// // Explicit passing to a function, implicit conversion to be used in switch-case
// struct FileMode {
//   glob cexpr i32 Read = cx_bit(0);
//   glob cexpr i32 Write = cx_bit(1);
//   glob cexpr i32 Append = cx_bit(2);
//   glob cexpr i32 ReadWrite = cx_bit(3);
//   glob cexpr i32 Mask = Read | Write | Append | ReadWrite;
//
//   i32 v{0};
//   implicit cexpr operator i32() const { return v; }
//   implicit cexpr operator int() const { return v; }
//   implicit cexpr operator=(int u) const { v = u; }
//   implicit cexpr operator=(i32 u) const { v = u; }
// 
//    
// };

enum FileWhence {
  FileWhenceBegin,
  FileWhenceCurrent,
  FileWhenceEnd,
};

enum FileError {
  FileErrorNone,
  FileErrorInvalid,
  FileErrorFilename,
  FileErrorExists,
  FileErrorNotExists,
  FileErrorPermission,
  FileErrorTruncFail,
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

#define CX_FILE_OPEN_PROC(fn)                                                                   \
  proc fn(FileDescriptor* fd, FileOperations* ops, FileMode mode, cstr name) noexce -> FileError
#define CX_FILE_READ_AT_PROC(fn)                                                   \
  proc fn(FileDescriptor fd, void* buf, isize size, i64 off, isize* read) noexce -> i32
#define CX_FILE_WRITE_AT_PROC(fn)                                                           \
  proc fn(FileDescriptor fd, void const* buf, isize size, i64 off, isize* written) noexce -> i32
#define CX_FILE_SEEK_PROC(fn)                                                           \
  proc fn(FileDescriptor fd, i64 cur_off, FileWhence whence, i64* new_off) noexce -> i32
#define CX_FILE_CLOSE_PROC(fn)             \
  proc fn(FileDescriptor fd) noexce -> void

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

onedef FileStandardConfig ___std_cfg{};

finline proc get_standard_config() noexce -> FileStandardConfig*
{
  return &___std_cfg;
}

finline proc are_standard_streams_set() noexce -> i32
{
  return get_standard_config()->are_std_set;
}

// −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
//  Operations (platform specific)

#if defined(CX_SYSTEM_WIN)

// TODO:

#else  // POSIX

intern proc ___posix_file_seek(
  FileDescriptor fd, i64 cur_off, FileWhence whence, i64* new_off) noexce -> i32
{
  #if defined(CX_SYSTEM_OSX)
    i64 res = lseek(fd.i, cur_off, (int)whence);
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

intern proc ___posix_file_read(
  FileDescriptor fd, void* buf, isize size, i64 off, isize* read) noexce -> i32
{
  isize res = pread(fd.i, buf, size, off);
  if (res < 0) {
    return false;
  }
  if (read) {
    *read = res;
  }
  return true;
}

intern proc ___posix_file_write(
  FileDescriptor fd, void const* buf, isize size, i64 off, isize* written) noexce -> i32
{
  isize res;
  i64 cur_off = 0;
  ___posix_file_seek(fd, 0, FileWhenceCurrent, &cur_off);
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


intern proc ___posix_file_close(FileDescriptor fd) noexce -> void { close(fd.i); }

intern proc get_default_ops() noexce -> FileOperations
{
  persist cexpr FileOperations default_file_ops{
      ___posix_file_read, ___posix_file_write, 
      ___posix_file_seek, ___posix_file_close};
  return default_file_ops;
}

// TODO: implement
noinline proc ___posix_file_open(
    FileDescriptor* fd, FileOperations* ops, FileMode mode, ptrcch8 name
) noexce -> FileError {
    i32 os_mode;
    switch (mode & FileModeMask) {
    case FileModeRead:
        os_mode = O_RDONLY;
        break;
    case FileModeWrite:
        os_mode = O_WRONLY | O_CREAT | O_TRUNC;
        break;
    case FileModeAppend:
        os_mode = O_WRONLY | O_CREAT | O_APPEND;
        break;
    case FileModeRead | FileModeReadWrite:
        os_mode = O_RDWR;
        break;
    case FileModeWrite | FileModeReadWrite:
        os_mode = O_RDWR | O_CREAT | O_TRUNC;
        break;
    case FileModeAppend | FileModeReadWrite:
        os_mode = O_RDWR | O_APPEND | O_CREAT;
        break;
    default:
        // CX_PANIC("invalid file mode");
        return FileErrorInvalid;
    }

    fd->i = open(
        name, os_mode, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
    );
    if (fd->i < 0) {
        // TODO:(manu): More file errors
        return FileErrorInvalid;
    }

    *ops = get_default_ops();
    return FileErrorNone;
}

#endif  // 

finline proc set_standard_to_default() noexce -> void
{
  if (are_standard_streams_set()) {
    return;
  }
  FileStandardConfig* fsc = get_standard_config();

  #define CX_SET_STD_FILE(type, val)        \
    fsc->files[type].fd.i = val;             \
    fsc->files[type].ops = get_default_ops()

  CX_SET_STD_FILE((i8) FileStandardInput, 0);
  CX_SET_STD_FILE((i8) FileStandardOutput, 1);
  CX_SET_STD_FILE((i8) FileStandardError, 2);

  #undef CX_SET_STD_FILE

  fsc->are_std_set = 1u;
}

finline proc get_standard(FileStandard type) noexce -> File* 
{
  if (!are_standard_streams_set()) {
    set_standard_to_default();
  }
  return &get_standard_config()->files[(i8) type];
}

}  // namespace cx::file

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
