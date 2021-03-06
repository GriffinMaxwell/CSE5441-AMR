TARGET_EXEC_1 := disposable
TARGET_EXEC_2 := persistent

BUILD_DIR := Build
SRC_DIRS := Source

SRCS := $(shell find $(SRC_DIRS) ! -name maxwell_griffin* -and \( -name *.cpp -or -name *.c -or -name *.s \))
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS := -O3 -lrt -fopenmp
CPPFLAGS := $(INC_FLAGS) -MMD -MP

all: $(TARGET_EXEC_1) $(TARGET_EXEC_2)

$(TARGET_EXEC_1): $(OBJS)
	$(CC) -c -o $(BUILD_DIR)/maxwell_griffin_disposable.o Source/maxwell_griffin_disposable.c $(CFLAGS)
	$(CC) $(BUILD_DIR)/maxwell_griffin_disposable.o $(OBJS) $(CFLAGS) -o $@ $(LDFLAGS)

$(TARGET_EXEC_2): $(OBJS)
	$(CC) -c -o $(BUILD_DIR)/maxwell_griffin_persistent.o Source/maxwell_griffin_persistent.c $(CFLAGS)
	$(CC) $(BUILD_DIR)/maxwell_griffin_persistent.o $(OBJS) $(CFLAGS) -o $@ $(LDFLAGS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean package disposable persistent

clean:
	@echo Cleaning build files...
	@$(RM) -r $(BUILD_DIR)
	@$(RM) $(TARGET_EXEC_1)
	@$(RM) $(TARGET_EXEC_2)

package:
	@echo "Packaging up project for submission..."
	@mkdir -p cse5441_lab3
	@cp Source/*.c Source/*.h cse5441_lab3
	@cp submit.mk cse5441_lab3
	@mv cse5441_lab3/submit.mk cse5441_lab3/Makefile

-include $(DEPS)

MKDIR_P ?= mkdir -p
