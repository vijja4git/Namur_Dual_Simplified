# MS51 NAMUR dual-channel firmware — SDCC build + host unit tests

SDCC     ?= sdcc
PACKIHX  ?= packihx
CC_HOST  ?= gcc

TARGET    = firmware
BUILD_DIR = build

MCU_FLAGS = -mmcs51 --model-small --std-sdcc99
MCU_FLAGS += --iram-size 256 --xram-size 1024 --code-size 16384

INCLUDES = -Iinclude \
           -Isrc/app \
           -Isrc/bsp/ms51 \
           -Iexternal/nuvoton/ms51/include

FW_SRCS = src/main.c \
          src/app/namur_logic.c \
          src/app/namur_app.c \
          src/bsp/ms51/clock.c \
          src/bsp/ms51/gpio.c \
          src/bsp/ms51/adc.c \
          src/bsp/ms51/delay.c \
          src/bsp/ms51/platform_ms51.c

FW_OBJS = $(addprefix $(BUILD_DIR)/,$(notdir $(FW_SRCS:.c=.rel)))

CFLAGS_FW = $(MCU_FLAGS) $(INCLUDES) --opt-code-size

TEST_LOGIC_BIN = $(BUILD_DIR)/test_namur_logic
TEST_INT_BIN   = $(BUILD_DIR)/test_channel_integration

TEST_CFLAGS = -std=c99 -Wall -Wextra -Iinclude -Isrc/app -Itests/mocks
APP_SRCS = src/app/namur_logic.c src/app/namur_app.c

.PHONY: all clean test hex dirs

all: dirs $(BUILD_DIR)/$(TARGET).ihx $(BUILD_DIR)/$(TARGET).hex

hex: $(BUILD_DIR)/$(TARGET).hex

dirs:
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET).ihx: $(FW_OBJS)
	$(SDCC) $(MCU_FLAGS) $(INCLUDES) --out-fmt-ihx -o $@ $(FW_OBJS)

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).ihx
	$(PACKIHX) $< > $@

$(BUILD_DIR)/%.rel: src/main.c | dirs
	$(SDCC) -c $(CFLAGS_FW) -o $@ $<

$(BUILD_DIR)/namur_logic.rel: src/app/namur_logic.c | dirs
	$(SDCC) -c $(CFLAGS_FW) -o $@ $<

$(BUILD_DIR)/namur_app.rel: src/app/namur_app.c | dirs
	$(SDCC) -c $(CFLAGS_FW) -o $@ $<

$(BUILD_DIR)/clock.rel: src/bsp/ms51/clock.c | dirs
	$(SDCC) -c $(CFLAGS_FW) -o $@ $<

$(BUILD_DIR)/gpio.rel: src/bsp/ms51/gpio.c | dirs
	$(SDCC) -c $(CFLAGS_FW) -o $@ $<

$(BUILD_DIR)/adc.rel: src/bsp/ms51/adc.c | dirs
	$(SDCC) -c $(CFLAGS_FW) -o $@ $<

$(BUILD_DIR)/delay.rel: src/bsp/ms51/delay.c | dirs
	$(SDCC) -c $(CFLAGS_FW) -o $@ $<

$(BUILD_DIR)/platform_ms51.rel: src/bsp/ms51/platform_ms51.c | dirs
	$(SDCC) -c $(CFLAGS_FW) -o $@ $<

test: dirs $(TEST_LOGIC_BIN) $(TEST_INT_BIN)
	./$(TEST_LOGIC_BIN)
	./$(TEST_INT_BIN)

$(TEST_LOGIC_BIN): tests/test_namur_logic.c src/app/namur_logic.c | dirs
	$(CC_HOST) $(TEST_CFLAGS) tests/test_namur_logic.c src/app/namur_logic.c -o $@

$(TEST_INT_BIN): tests/test_channel_integration.c tests/mocks/mock_platform.c $(APP_SRCS) | dirs
	$(CC_HOST) $(TEST_CFLAGS) tests/test_channel_integration.c tests/mocks/mock_platform.c $(APP_SRCS) -o $@

clean:
	rm -rf $(BUILD_DIR)
