CC ?= cc
BUILD_DIR := build-make
TARGET := $(BUILD_DIR)/tarea_so_1

CPPFLAGS += -D_POSIX_C_SOURCE=200809L -Iinclude
CFLAGS ?= -std=gnu99 -g

SOURCES := \
    src/main.c \
    src/parseador.c \
    src/ejecutar.c \
    src/ej_background.c \
    src/comandos_internos.c \
    src/redireccion.c \
    src/senales.c \
    src/pmon.c

OBJECTS := $(SOURCES:src/%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJECTS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR)