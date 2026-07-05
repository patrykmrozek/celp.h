OUT = celp_test
$(OUT):
	gcc \
		-DLOG_MODE_ALL \
		tests/celp_test.c \
		tests/celp_da.c \
		tests/celp_ll.c \
		tests/celp_map.c \
		tests/celp_math.c \
		-o $(OUT)

run: $(OUT)
	./$(OUT)
