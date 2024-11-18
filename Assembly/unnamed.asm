; Rewritten clang-compiled Hello World app (C++)

main:
    ; Allocate stack frame
    allocframe(r29, #8):raw

    ; Reserve space for local variable
    r0 = add(r30, #-4)
    memw(r0 + #0) = #0  ; Initialize local variable to 0

    ; Load address of std::cout from the Global Offset Table
    r0 = add(pc, ##_GLOBAL_OFFSET_TABLE_@PCREL)
    r0 = memw(r0 + ##_ZNSt3__14coutE@GOT)  ; Load std::cout

    ; Load address of the string literal "Hello World"
    r1 = add(pc, ##.L.str@PCREL)

    ; Call operator<< to output the string
    call std::__1::basic_ostream<char, std::__1::char_traits<char>>& std::__1::operator<<[abi:v160005]<std::__1::char_traits<char>>(std::__1::basic_ostream<char, std::__1::char_traits<char>>&, char const*)

    ; Call std::endl to flush the output
    r1 = add(pc, ##_ZNSt3__14endlB7v160005IcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_@PCREL)
    call std::__1::basic_ostream<char, std::__1::char_traits<char>>::operator<<[abi:v160005](std::__1::basic_ostream<char, std::__1::char_traits<char>>& (*)(std::__1::basic_ostream<char, std::__1::char_traits<char>>&))

    ; Return 0 from main
    r0 = #0
    r31:30 = dealloc_return(r30):raw  ; Deallocate stack frame and return

__clang_call_terminate:
    ; Handle termination of exceptions
    allocframe(r29, #0):raw
    call __cxa_begin_catch
    call std::terminate()

.L.str:
    .string "Hello World"

DW.ref.__gxx_personality_v0:
    .word __gxx_personality_v0  ; Reference to the personality function for exception handling