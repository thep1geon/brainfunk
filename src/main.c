#include <stdio.h>
#include <stdlib.h>
#include <Pigeon/string.h>

int main(i32 argc, char** argv) {
    if (argc != 2) return 1;
    String program = string_slurp_file_malloc(string(argv[1]));
    u8* c = (u8*)program.data;
    u8 buffer[20000]; u8* buf_ptr = buffer;
    u8** table = malloc(sizeof(u8*) * program.len); 
    u8* stack[1024];
    u8** stack_ptr = stack;
    while (*c) {
        if (*c == '[') *stack_ptr++ = c;
        if (*c == ']') {
            if (stack_ptr == stack) return 2;
            table[(usize)(*(--stack_ptr) - (u8*)program.data)] = c;
        }
        c++;
    }
    c = (u8*)program.data;
    while (*c) {
        switch (*c) {
            case '+': *buf_ptr+=1; break;
            case '-': *buf_ptr-=1; break;
            case '>': buf_ptr++; break; 
            case '<': buf_ptr--; break;
            case '.': putchar(*buf_ptr); break;
            case ',': *buf_ptr = getc(stdin); break;
            case '[': { if (!*buf_ptr) c = table[(usize)(c-(u8*)program.data)]; else *stack_ptr++ = c; } break;
            case ']': { if (*buf_ptr) c = *(stack_ptr-1); else stack_ptr--;} break;
        }
        c++;
    }
    free(table);
    string_free(&program);
    return 0;
}
