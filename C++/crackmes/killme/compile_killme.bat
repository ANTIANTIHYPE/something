@echo off

set COMPILER=g++
set FLAGS=-std=c++23 -I/i/ -static -fno-tree-loop-distribute-patterns -fno-loop-block -fexceptions -fno-cse-follow-jumps -fno-combine-stack-adjustments -fno-early-inlining -fno-cse-skip-blocks -fexpensive-optimizations -fno-hoist-adjacent-loads -fno-if-conversion -fno-if-conversion2 -fno-loop-interchange -fno-loop-unroll-and-jam -fno-move-loop-invariants -fno-tree-dse -fno-tree-forwprop -fno-tree-coalesce-vars -fno-tree-copy-prop -fno-sched-stalled-insns-dep -fno-split-wide-types -fno-tree-dominator-opts -fno-tree-cselim -fno-section-anchors -fno-tree-ch -fno-tree-ccp -fno-tree-bit-ccp -finline-functions -fno-merge-constants -fwrapv -ftrapv -g0 -fno-builtin -fomit-frame-pointer -fno-tree-pta -fno-tree-pre -fno-tree-switch-conversion -fno-tree-tail-merge -fno-tree-phiprop -fno-tree-sink -fno-tree-fre -fno-tree-dce -fno-dce -fno-split-ivs-in-unroller -fno-thread-jumps -fno-shrink-wrap -fno-sched-spec -fno-sched-stalled-insns -fno-reorder-functions -fno-rerun-cse-after-loop -fno-ipa-icf -s -fno-reorder-blocks-and-partition -fno-ipa-ra -fno-regmove -fpeel-loops -fno-gcse -fno-gcse-lm -fno-modulo-sched -fno-math-errno -fno-ipa-sra -flto -fno-lifetime-dse -fno-ipa-cp -fno-cprop-registers -fno-combine-stack-adjustments -falign-functions -falign-jumps -fno-peephole -fno-peephole2 -fno-reorder-blocks -fno-reorder-functions -fno-dwarf2-cfi-asm -fno-schedule-insns -fno-schedule-insns2 -fno-data-sections -fno-delete-null-pointer-checks -fno-strict-aliasing -fno-strict-overflow -fno-tree-vrp -O1 -Wall -fno-ident -fno-debug-types-section -fno-merge-all-constants -frtti -D_FORTIFY_SOURCE=3 -fstack-protector -feliminate-unused-debug-types -feliminate-unused-debug-symbols -finline -finline-functions-called-once
%COMPILER% %FLAGS% killme.cpp -liphlpapi -lws2_32 -o killme.exe

if %ERRORLEVEL% neq 0 (
    echo Compilation failed!
    pause
    exit /b 1
)

.\killme.exe