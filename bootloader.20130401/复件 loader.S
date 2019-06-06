# 1 "loader.S"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "loader.S"
.extern lmain
.global _start
.code16
.section .text
.section .data
_start:
 call lmain
