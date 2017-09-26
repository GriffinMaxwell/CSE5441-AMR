TARGET_EXEC := amr

BUILD_DIR := Build
SRC_DIRS := Source

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS = -O3
CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -lrt -o $@ $(LDFLAGS)

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


.PHONY: clean package

clean:
	@echo Cleaning build files...
	@$(RM) -r $(BUILD_DIR)
	@$(RM) $(TARGET_EXEC)

package:
	@echo Packaging up project for submission...
	@mkdir -p cse5441_lab1
	@cp Source/*.c Source/*.h cse5441_lab1
	@cp submit.mk cse5441_lab1
	@mv cse5441_lab1/submit.mk cse5441_lab1/Makefile
	
-include $(DEPS)

MKDIR_P ?= mkdir -p
