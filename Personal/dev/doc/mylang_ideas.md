---
markdown_extensions: +bracketed_spans
---

# Misc

```
--- I think this is the best

+-- Multine non documentationc comment
+-- this is the last line, still part of comment until\n

--+ Multiline documentation coment:
--+ this is the last line, still part of comment until\n
```

```
--- good
@inln @priv
cons proc generic_test[
    T: type where T is cpyble & dtble,
    n: sint where n > 0
](&[T, n] x) -> T

--- good
@inln @priv
cons proc generic_test[
    T: type
    n: sint
](&[T, n] x) -> T, where:
    T is cpyble & dtble,
    n > 0

--- good
@inln @priv 
cons proc generic_test(
    $T:    type
    $n:    sint
    x:     *[T, n]
) -> T, where:
    cpyble(T) & dtble(T)
    0 < n and n < 100
  
inln priv cons proc generic_test(
    type       $T
    sint       $n
    &[T, n]    x
) -> T, where:
    T is cpyble & dtble,
    n > 0

@inln @priv
cons proc generic_test[type T, sint n](&[T, n] arr) -> T, where:
    cpyble[T] & dtble[T],
    n > 0

```

```
@inln @priv
proc visit(
    *DeclareVarInst inst
) -> void, where: some_cons_predicate() {
    // if arr_t := dycast(*ArrayTyped, inst.t_info); arr_t:
    if arr_t := dyn *ArrayTyped(inst.t_info); arr_t:
        dst <- endln <- tab <- accept(inst.addr) <- " = ";
        if inst.value:
            dst <- inst.accept(inst.value);
        elif inst.something:
            do_something();
        else:
            zero_initializer(inst.t_info);
}
```

```
    fn create_remote_dsp_factory_from_string(
        *cons string    name_app,
        *cons string    dsp_content,
        sint            argc,
        [*cons string]  argv,
        *cons string    ip_server,
        sint            port_server,
        *cons string    error_msg,
        sint            opt_level    = -1
    ) -> RemoteDspFactory;
```

```
@inln @cons @priv
(string, string, anyerr) collect_results(sint a, uint b)
{
    printn(a + b);
    return to_string(a), to_string(b), none;
}
```

```
@inln @cons @priv
fn print_something_generic: ($T: type, x: T) -> void
with: 
    T: printble
{ 
    for i in 0 .. 10
        if i % 2 == 0
            print(i)
        elif i % 3 == 0
            print(get_something(i))
        else
            print(get_value(i))
        endif
    endfor
} 
   
type Person:
    name:     string,
    age:      sint,
    is_alive: bool,
```

```
Cns Uint8
Cns Sint64
Cns Float32

// cpp/rust like
inline cexpr proc< 
    Movble    First,
    Copble    Second,
    Type      ReturnErr
> align_up(
    uptr    ptr,
    sint    off
) -> (
    Uptr, 
    ReturnErr
) 
where:
    is_power_of_two(align_of.<First>)
    is_power_of_two(align_of.<Second>)
{
    return align_up(ptr, align_of.<T>);
}

// c + odin
inln cons proc align_up(
    Movable  $First
    Copyable $Second
    Any      $ReturnErr
    uptr     ptr
    sint     off
) -> (
    uptr,
    ReturnErr
) where (
    is_power_of_two(align_of(First)),
    is_power_of_two(align_of(Second))
) {
    x := get_value();
    return align_up(ptr, align_of(T));
}

// c3 with trailing return
@inln @cons 
fn align_up(
    Movable  $First
    Copyable $Second
    Any      $ReturnErr
    uptr     ptr
    sint     off
) -> (
    uptr, 
    ReturnErr
) where (
    is_power_of_two(align_of(First)),
    is_power_of_two(align_of(Second))
) {
    x := get_value();
    return align_up(ptr, align_of(T));
}

// c3 multple return
fn(sint) add(sint a, sint b)
{
    return a + b
}
inln cons
fn(uptr, anyerr) align_up(mveble $Type1, cpyble $Type2, uptr ptr, sint off)
where:
    is_power_of_two(align_of(Type1)),
    is_power_of_two(align_of(Type2))
{
    auto x = get_value();
    return align_up(ptr + x, align_of(Type1)), none;
}

// custom
@inln @cons @priv
fun align_up(
    $First:  Movable
    $Second: Copyable
    $ReturnErr: Any
    ptr:     uptr
    off:     sint
) -> (
    uptr,
    ReturnErr
) where:
    is_power_of_two(align_of(First)),
    is_power_of_two(align_of(Second))
{
    x := get_value();
    return align_up(ptr, align_of(T)), none;
}

@inln @cons @priv
fn do_some_operation(
    $First:        mveble,
    $Second:       cpyble,
    $ReturnErr:    anyerr,
    ptr:           uptr,
    off:           sint
) -> (uptr, sint, ReturnErr)
where:
    is_some_predicate(some_propriety(First)),
    is_some_predicate(some_propriety(Second))
{
    x := get_value();
    return do_something_else(ptr, some_property(First)), 42, none
}
```

```
begfor i := 0..10
    begif i > 3
        print("> 3")
    endif
    printn(i)
endfor
```

```
Person :: struct {
    age:     s32
    name:    string
}

usize_of(s32)
ssize_of(u8)

fn push_elem(type $T, uint $N, *[T, N] arr, T elm) -> s32!
{
}

push_elem :: ($T: type, $N: usz, arr: *[T, N], elm: T) -> (s32, ArrayError)
where
    T: Copy, N: Gne(0)
{
    arr.len == N ! "arr is full"
    // if (arr.len == N) {
    //     return _, "arr is full"
    // }

    arr[len] = elm
    len = len + 1
}

main :: (args: [string]) -) void
{
    arr: [string, 8]
    for s in args {
        push_elem(&arr, s)
    }
}
```

```
from:
  template<typename T, CTupleType Tup, typename L = Tup::Types>
  finline inline constexpr auto get(Tup const& tup) -> auto const&
to:
  <T, TupleType Tup> inline cexpr proc get(tup: &const Tup) -> &const auto 
  {
    return get<get_index<T, Tup.Types>>(tup);
  }
or:
  inline cexpr proc get($T, $Tup: TupleType, tup: &cns Tup) -> &cns auto 
  {
    return get(type_idx(T, Tup.Types), tup);
  }

```

```
inline priv proc forward_five_variables(
    $FirstType, $SecondType, $ThirdType, $FourthType, $FifthType,
    *FirstType a, *SecondType b, *ThirdType c, *FourthType d, *FifthType e
) -> (FirstType, SecondType, ThirdType, FourthType, FifthType)
where
    FirstType, SecondType, ThirdType, FourthType, FifthType: Copy + Move
{
  return *a, *b, *c, *d, *e;
}

inline priv forward_five_variables :: proc (
    $FirstType, $SecondType, $ThirdType, $FourthType, $FifthType \ Copy,
    a: *FirstType, b:  *SecondType, c: *ThirdType, d: *FourthType, e: *FifthType
) -> (FirstType, SecondType, ThirdType, FourthType, FifthType)
{
  return *a, *b, *c, *d, *e;
}

```

```
inline private
proc<FirstType, SecondType, ThirdType, FourthType, FifthType: Copy + Move>
forward_five_variables(a: ^FirstType, b: ^const SecondType, c: ^ThirdType,
                       d: ^const FourthType, e: ^FifthType)
-> (FirstType, SecondType, ThirdType, FourthType, FifthType)
{ 
  return a^^, b^, c^^, d^, e^^; 
}

inline priv proc<FirstType, SecondType>
forward_five_variables(a: ^FirstType, b: ^const SecondType)
  -> (FirstType, SecondType)
{ 
  return a^^, b^, c^^, d^, e^^; 
}
```

```
&[T]: slice READ WRITE elems, NO: realloc, append, etc.
&[cons T]: slice READ only elems, NO: realloc, append
^[T]: pointer to dynarray, READ WRITE elems, YES realloc, append, etc.
^[cons T]: pointer to dynarray, read only elems, YES realloc, append, etc.
^[T, N]: pointer to static array, READ WRITE elems, but NO dynamics
^[cons T, N]: pointer to static array, READ WRITE elems, but NO dynamics
```

```
nodisc inln cons proc at(i: cons u8) -> (&[u8]!IdxErr) {
  i >= usize_of(Lens) ? "index out of bounds"
  return buf[begs[i] : begs[i + 1]]
}
```

```
proc<Ts..: Copy>
memcopy_multi(dst: &[&Ts..], src: &[&Ts..], num: usz) -> void
{
  for i, T : Ts {
    mem::memcopy::<T>(dst[i], src[i], num);
  }
}
```

```
const priv proc persist foreign struct enum union
rune void
u8 f16 i16 …
usz isz 
uptr iptr

type Person {
  age: i32
  name: &[u8]
}

enum Color: u8 {
  Red,
  Green,
  Blue
}

union FileDescriptor {
  i: i32
  p: *void
}

walt: Person = {50, Walter}
jes := Person{18, Jesse}
gus: cons Person = {53, Gus}
sky := cons Person{42, Skyler}
```

```
// not good
cons proc<..Type Ts, type K> bool find(*Cons MultiArray<Ts..> arr, Cons K key);

cons proc<Ts: ..type, K: type> 
find(arr: *cons MultiArray<Ts..>, key: cons K) -> bool;
``` 

```
proc<..Ts> memcopy_multi(dst: const *Ts.., src: const *Ts.., num: usz) -> void
```

```
inline priv cexpr
proc<
    Ts: ..type,
    I: ..usz,
    J: ..usz,
    K: ..type,
    V: ..type
> bind_multi(
    ptr: *void, 
    new_num: usz
) -> (
    [*void, pack_len(Ts)],
    usz
) {
}
```

```
inline priv cexpr
proc<
    Ts: ..type,
    I: ..usz,
    J: ..usz,
    K: ..type,
    V: ..type
> 
bind_multi(
    ptr: *void, 
    new_num: usz
)
-> (
    [*void, pack_len(Ts)],
    usz
) {
}
```

```
inline priv cexpr proc<Ts: ..type, I: ..usz>
bind_multi(ptr: const *void, new_num: const usz) -> [*void, pack_len(Ts)]
{
  bind_and_advance := proc<T>() -> *void {
    ptr = mem::align_up::<T>(ptr);
    beg := ptr as *T
    ptr = beg + new_num;
    return beg;
  };
  return {bind_and_advance::<Ts>()..};
}
```

# Ideas

## Idea 1: kind first

### Example 1: ...

```
proc Ast!Allocator.Error parse(Allocator gpa, [:0]cons uint8 source, Mode mode){
  auto tokens = Ast.TokenList{}
  cons auto toks = Ast.TokenList{}
  auto sum = fn[](i32 a, i32 b) i32 { return a + b }
  auto y = sum(42, 0)
}
```

### Example 2: ... 

```
inln priv cons proc<..type Ts, ..usz I>
void! a_simple_test_proc(std.IndexSequence<I..>, *cons std.Tuple<*Ts..> dst,
                         *cons std.Tuple<*Ts..> src, usz num);
```

### Example 3: ...

```
inln cons proc *cons char!SomeError test_exampl(*cons char str, uint32 x) {
  test_example_side(str, uint32)!
  return 'c'
}
```

### Example 4: ...

```
inln cons proc<type PtrCharType, type ErrType>
(char, uint32)!ErrType an_example_with_long_name_and_many_args(
    *cons char this_is_a_char_ptr, uint32 this_is_an_unsigned) {
  return str.*, x
}
```

### Example 5: ...

```{=latex}
\monostyle{
  \kw{proc} (\ty{uint8}, \ty{uint8}) returning\_a\_tuple(\ty{uint8} a, \ty{uint8} b) \{ \\
    \tab \kw{return} a + b, a - b \\
  \}
}
```


### Example 6: ...

```
proc void main() {
  val, err := example.test_example("test", 42)
  //= alternatives =//
  char! val, err = examples.test_example("test", 42);    

  void! val, err = mem.memcpy_multi<uint8, f64, i32>(/*args*/);
}
```

\newpage

### Example 7: very crowded declaration

```{=latex}
\monostyle{
\kw{noinline} \kw{priv} \kw{cons} \kw{proc}\text{<} \\
    \tab \ty{SomeLongTraitName} \tp{LongType1}, \ty{TraitName} \tp{LongTypeName}, \ty{type} \tp{T}, \\
    \tab \ty{YetAnotherEvenLongerTraitName} \tp{LongType2}, \ty{type} \tp{U}, \\
    \tab \ty{SomeLongTraitName} \tp{LongType3}\text{>} \\
(\ty{VeryVeryLongReturnTypeName1}, \tp{LongTypeName}, \tp{LongTypeName}, \\
\s \ty{VeryVeryLongReturnTypeName2}) a\_super\_long\_function\_identifier( \\
    \tab *\ty{cons} \tp{LongType1} argument\_name, \\
    \tab *\ty{cons} \tp{LongType2} long\_argument\_name) \{ \\
  \duo \kw{return} \\
\}
}
```

### Example 8: ...
```
noinline priv cons proc<T: Orderable>
(^T rv, ssize ri) largest(cons &[T] list) {
  rv = &list[0]
  for i, x in list {
    if x^ > rv { 
      rv = x
      ri = ssize(i)
    }
  }
}
```

## Idea 2: identifier first

### Example 1: ...

```
proc parse(gpa: Allocator, source: [:0]cons uint8, mode: Mode) -> Ast!Allocator.Error {
  tokens := Ast.TokenList{}
  toks := cons Ast.TokenList{}
  sum := fn[](i32 a, i32 b) i32 { return a + b }
  y := sum(42, 0)
}
```

### Example 2: ... 

```
inln priv cons proc<Ts: ..type, I: ..usz>
a_simple_test_proc(std.IndexSequence<I..>, dst: *cons std.Tuple<*Ts..>,
                   src: *cons std.Tuple<*Ts..>, num: usz) -> void!;
```

### Example 3:

```
inln cons proc 
test_example_side(str: *cons char, x: uint32) -> i32!SomeError {
  if y := do_domething(str, x); == 0 {
    return 0, "error occured"
  }
  return y
}

inln cons proc test_example(str: *cons char, x: uint32) -> i32!SomeError {
  y := test_example_side(str, x)! // rethrow (as a couple, no maybe type here)
  return y + 1
}
```

### Example 4: ...

```
inln cons proc<PtrCharType: type, ErrType: type>
an_example_with_long_name_and_many_args(
    *cons char this_is_a_char_ptr, uint32 this_is_an_unsigned)
-> (char, uint32)!ErrType {
  return str.*, x
}
```

```
inln proc test_function(n: ^usize) -> ssize {
  m: ssize = -n
  return -n^
}
```

```
inln cons proc<SomeCustomType> 
simple_identity_function(x: ^cons SomeCustomType) -> SomeCustomType {
  y := x
  return y
}
```

```
noinline cons proc<T: PartialOrder> largest(list: cons &[T]) -> (^T, ssize) {
  rv := &list[0]
  ri := 0sz
  for i, x in list {
    if x^ > rv {
      rv = x
      ri = ssize(i)
    }
  }
  return rv, ri
}
```

###  Example 9: traits

Below two traits definitions.

```
trait Orderable {
  inln cons oper<T> lne(a: ^cons T, b: ^cons T) -> bool;
}
trait OrderableWith {
  inln cons oper<T, U> lne(a: ^cons T, b: ^cons U) -> bool;
}
```

The following snippets implements `Orderable` for `Book`.

```
inln cons oper lne(b1: Book, b2: Book) -> bool {
  return b1.price < b2.price
}
```

\newpage

### Example 7: very crowded declaration

```{=latex}
\monostyle{
\kw{noinline} \kw{priv} \kw{cons} \kw{proc}\textless \\
    \tab \tp{LongType1}: \ty{SomeLongTraitName}, \tp{LongTypeName}: \ty{TraitName}, \tp{T}: \ty{type}, \\
    \tab \tp{LongType2}: \ty{YetAnotherEvenLongerTraitName}, \tp{U}: \ty{type}, \\
    \tab \tp{LongType3}: \ty{SomeLongTraitName}\textgreater \\
a\_super\_long\_function\_identifier( \\
    \tab argument\_name: *\ty{cons} \tp{LongType1},  \\
    \tab long\_argument\_name: *\ty{cons} \tp{LongType2}) \\
\text{->} (\ty{VeryVeryLongReturnTypeName1}, \tp{LongTypeName}, \tp{LongTypeName},  \\
    \tab \ty{VeryVeryLongReturnTypeName2}) \{ \\
  \duo \kw{return} 0 \\
\}
}
```

### Example 8: ...

```
noinline priv cons proc<T: Orderable>
largest(list: cons &[T]) -> (rv: ^T, ri: ssize) {
  rv = &list[0]
  for i, x in list {
    if x^ > rv { 
      rv = x
      ri = ssize(i)
    }
  }
}
```

```{=latex}
\monostyle{
\kw{noinline} \kw{priv} \kw{cons} \kw{proc}\text{<}\tp{T}: \ty{Orderable}\text{>} \\
largest(list: \ty{cons} \&[\tp{T}]) \text{->} (rv: *\tp{T}, ri: \ty{ssize}) \{ \\
  \duo rv = \&list[0] \\
  \duo \kw{for} i, x \kw{in} list \{ \\
    \duo\duo \kw{if} x* \textgreater\s rv \{ \\
      \duo\duo\duo rv = x \\
      \duo\duo\duo ri = \ty{ssize}(i) \newline
    \duo\duo \} \\
  \duo \} \\
\}
}
```


### Appendix A: types

```
usize
ssize
uptr
sptr
sint8
uint8 
uint32
sint64
float64
float32
bool
void
```
