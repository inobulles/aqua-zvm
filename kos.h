
int64_t zvm_noop() {
	printf("This is a no-operation function. If you reach this, either your current KOS is incomplete, either you have a severe problem in your code\n");
	return 1;
	
} void zvm_print(uint64_t self, uint64_t string) {
	printf("%s", (const char*) string);
	fflush(stdout);
	
} int64_t zvm_exit(uint64_t self, int64_t error_code) {
	((zvm_program_t*) self)->state.registers[REGISTER_IP] = (((zvm_program_t*) self)->meta->length - ((zvm_program_t*) self)->meta->text_section_start) * ZVM_SIZE + 1;
	return error_code;
	
}

// note that these memory functions are not ideal (for security reasons)
// ideally, these would all be in a sandboxed heap,
// but this is "example code", and for readability's sake, I've opted for this

void* zvm_malloc(uint64_t self, uint64_t bytes) {
	uint64_t* pointer = (uint64_t*) malloc(bytes + sizeof(uint64_t));
	*pointer = 0; // set first value to zero to tell zvm_mfree to use "free" instead of "munmap"
	              // this is nothing standard, just how i decided to write these functions in this particular implementation of the zvm
	return (void*) pointer + sizeof(uint64_t);
}

void  zvm_mfree(uint64_t self, uint64_t __pointer, uint64_t bytes) { // bytes argument is obsolete
	uint64_t* pointer = (uint64_t*) (__pointer - sizeof(uint64_t));
	
	if (*pointer) {
		long page_bytes = sysconf(_SC_PAGESIZE);
		
		munmap((void*) __pointer, *pointer);
		munmap((void*) __pointer - page_bytes, *pointer + page_bytes);
		
	} else {
		free(pointer);
	}
}

void  zvm_mcpy  (uint64_t self, uint64_t dst, uint64_t src,  uint64_t bytes) { memcpy((void*) dst, (void*) src, bytes); }
void  zvm_mset  (uint64_t self, uint64_t dst, uint64_t byte, uint64_t bytes) { memset((void*) dst, (char) byte, bytes); }

static void* prereserved[] = { // prereserved functions from the kos (some of these are integrated in this file as they are common between all kos's)
	(void*) zvm_noop,           (void*) zvm_print,            (void*) zvm_exit,                                                                        // base functions
	(void*) zvm_malloc,         (void*) zvm_mfree,            (void*) zvm_mcpy,          (void*) zvm_mset,                                             // memory functions
	(void*) kos_get_platform,   (void*) kos_platform_command, (void*) kos_create_device, (void*) kos_send_device,         (void*) kos_native,          // platform and device functions
	(void*) kos_video_width,    (void*) kos_video_height,     (void*) kos_video_fps,     (void*) kos_video_clear,         (void*) kos_video_flip,      // basic video functions
	(void*) kos_create_machine, (void*) kos_execute_machine,  (void*) kos_kill_machine,  (void*) kos_give_machine_events, (void*) kos_current_machine, // machine functions
	(void*) kos_get_events,                                                                                                                            // misc functions
};
