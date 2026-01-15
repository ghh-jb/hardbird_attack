CC = clang
SDK = $(shell xcrun --sdk iphoneos --show-sdk-path)
CFLAGS = -isysroot $(SDK) -arch arm64 -Wno-string-plus-int -fno-stack-protector
CFLAGS += -D_FORTIFY_SOURCE=0 -nostdlib
LDFLAGS = -Xlinker -kext

all : hardbird_attack

hardbird : hardbird.c
	$(CC) $(CFLAGS) $(LDFLAGS) hardbird.c -o hardbird_attack

clean:
	rm -rf hardbird_attack