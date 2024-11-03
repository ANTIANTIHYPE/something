main:
        {
                allocframe(r29,#8):raw
        }
        {
                r0 = add(r30,#-4)
        }
        {
                memw(r0+#0) = #0
        }
        {
                r0 = add(pc,##_GLOBAL_OFFSET_TABLE_@PCREL)
        }
        {
                r0 = memw(r0+##_ZNSt3__14coutE@GOT)
        }
        {
                r1 = add(pc,##.L.str@PCREL)
        }
        {
                call std::__1::basic_ostream<char, std::__1::char_traits<char>>& std::__1::operator<<[abi:v160005]<std::__1::char_traits<char>>(std::__1::basic_ostream<char, std::__1::char_traits<char>>&, char const*)
        }
        {
                r1 = add(pc,##_ZNSt3__14endlB7v160005IcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_@PCREL)
        }
        {
                call std::__1::basic_ostream<char, std::__1::char_traits<char>>::operator<<[abi:v160005](std::__1::basic_ostream<char, std::__1::char_traits<char>>& (*)(std::__1::basic_ostream<char, std::__1::char_traits<char>>&))
        }
        {
                r0 = #0
        }
        {
                r31:30 = dealloc_return(r30):raw
        }

__clang_call_terminate:
        {
                allocframe(r29,#0):raw
        }
        {
                call __cxa_begin_catch
        }
        {
                call std::terminate()
        }

.L.str:
        .string "Hello World"

DW.ref.__gxx_personality_v0:
        .word   __gxx_personality_v0