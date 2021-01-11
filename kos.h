
uint64_t zvm_noop() {
	printf("This is a no-operation function. If you reach this, either your current KOS is incomplete, either you have a severe problem in your code\n");
	return 1;
}

uint64_t zvm_print(zvm_program_t* self, uint64_t string) {
	printf("%s", (const char*) string);
	fflush(stdout);
	
	return 0;
}

uint64_t zvm_exit(zvm_program_t* self, uint64_t error_code) {
	self->state.registers[ZED_REGISTER_IP] = self->meta_section->logic_section_words;
	return error_code;
}

// note that these memory functions are not ideal (for security reasons)
// ideally, these would all be in a sandboxed heap,
// but this is "example code", and for readability's sake, I've opted for this

uint64_t zvm_allocate(zvm_program_t* self, uint64_t bytes) {
	uint64_t* pointer = (uint64_t*) malloc(bytes + sizeof(uint64_t));
	*pointer = 0; // set first value to zero to tell zvm_free to use "free" instead of "munmap"
	              // this is nothing standard, just how i decided to write these functions in this particular implementation of the zvm
	return (uint64_t) pointer + sizeof(uint64_t);
}

#include <sys/mman.h>

uint64_t zvm_free(zvm_program_t* self, uint64_t __pointer, __attribute__((unused)) uint64_t bytes) { // bytes argument is obsolete
	uint64_t* pointer = (uint64_t*) (__pointer - sizeof(uint64_t));
	
	if (*pointer) {
		long page_bytes = sysconf(_SC_PAGESIZE);
		
		munmap((void*) __pointer, *pointer);
		munmap((void*) __pointer - page_bytes, *pointer + page_bytes);
		
	} else {
		free(pointer);
	}

	return 0;
}

uint64_t zvm_copy(zvm_program_t* self, uint64_t dst, uint64_t src, uint64_t bytes) {
	memcpy((void*) dst, (void*) src, bytes);
	return 0;
}

uint64_t zvm_zero(zvm_program_t* self, uint64_t dst, uint64_t bytes) {
	memset((void*) dst, 0, bytes);
	return 0;
}

typedef uint64_t (*zvm_kfunc_t) (zvm_program_t* self, ...); // technically should be called 'zvm_kfunc_pointer_t' but whatever

static zvm_kfunc_t zvm_kfunc_pointers[] = { // kfunc pointers (some of these are integrated in this file as they are common between all kos')
	(zvm_kfunc_t) zvm_noop,          (zvm_kfunc_t) zvm_print,            (zvm_kfunc_t) zvm_exit,                                                                        // base functions
	(zvm_kfunc_t) zvm_allocate,      (zvm_kfunc_t) zvm_free,             (zvm_kfunc_t) zvm_copy,   (zvm_kfunc_t) zvm_zero,
	(zvm_kfunc_t) kos_get_platform,  (zvm_kfunc_t) kos_platform_command, (zvm_kfunc_t) kos_native, (zvm_kfunc_t) kos_get_requests,
	(zvm_kfunc_t) kos_create_device, (zvm_kfunc_t) kos_send_device
};

#define ZVM_KFUNC_COUNT (sizeof(zvm_kfunc_pointers) / sizeof(zvm_kfunc_t))