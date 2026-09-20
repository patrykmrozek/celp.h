OUT_TEST = celp_test
$(OUT_TEST):
	gcc \
		-DLOG_MODE_ALL \
		tests/celp_test.c \
		tests/celp_da.c \
		tests/celp_ll.c \
		tests/celp_ll_noerror.c \
		tests/celp_map.c \
		tests/celp_str.c \
		tests/celp_links.c \
		tests/celp_arena.c \
		tests/celp_math.c \
		-o $(OUT_TEST)

test: $(OUT_TEST)
	./$(OUT_TEST)

OUT_PROFILE = celp_profile
$(OUT_PROFILE):
	gcc \
		-DLOG_MODE_ALL \
		profiles/celp_profile_malloc.c \
		profiles/celp_profile.c \
		-o $(OUT_PROFILE)

profile: $(OUT_PROFILE)
	./$(OUT_PROFILE)
