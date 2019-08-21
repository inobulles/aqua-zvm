
int64_t zvm_noop() {
	printf("This is a no-operation function. If you reach this, either your current KOS is incomplete, either you have a severe problem in your code\n");
	return 1;
	
} void zvm_print(uint64_t self, uint64_t string) {
	printf("%s", (const char*) string);
	fflush(stdout);
	
} void zvm_exit(uint64_t self, int64_t error_code) {
	((zvm_program_t*) self)->state.registers[REGISTER_G0] = error_code;
	((zvm_program_t*) self)->state.registers[REGISTER_IP] = ((zvm_program_t*) self)->meta->length * ZVM_SIZE + 1;
	
}

// note that these memory functions are not ideal (for security reasons)
// ideally, these would all be in a sandboxed heap,
// but this is "example code", and for readability's sake, I've opted for this

void* zvm_malloc(uint64_t self, uint64_t bytes) { return malloc(bytes); }
void  zvm_mfree (uint64_t self, uint64_t pointer, uint64_t bytes) { free((void*) pointer); }

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
