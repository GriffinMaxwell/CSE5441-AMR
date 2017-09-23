BUILD_DIR=Build
SRC_DIR=Source

SRC_FILES = \

.PHONY: all
all:
	# nothing yet to build :(

.PHONY: package
package:
	# for packaging up the source so it is ready to submit

.PHONY: clean
clean:
	@rm -f $(BUILD_DIR)/*.o
