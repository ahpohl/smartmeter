# get version info from git and compile into the program
# git tags have format vMAJOR.MINOR.RELEASE
# https://embeddedartistry.com/blog/2016/10/27/giving-you-build-a-version
version := $(subst -, ,$(shell git describe --long --dirty --tags))
COMMIT := $(strip $(word 3, $(version)))
COMMITS_PAST := $(strip $(word 2, $(version)))
DIRTY := $(strip $(word 4, $(version)))
ifneq ($(COMMITS_PAST), 0)
    BUILD_INFO_COMMITS := "."$(COMMITS_PAST)
endif
ifneq ($(DIRTY),)
    BUILD_INFO_DIRTY :="+"
endif

export BUILD_TAG :=$(strip $(word 1, $(version)))
export BUILD_INFO := $(COMMIT)$(BUILD_INFO_COMMITS)$(BUILD_INFO_DIRTY)

# define the C compiler to use
CPP = g++

# define any compile-time flags
CPPFLAGS = -Wall -g -std=c++17 -pthread

CPPFLAGS += -DVERSION_BUILD_DATE=\""$(shell date "+%F %T")"\" \
            -DVERSION_BUILD_MACHINE=\""$(shell echo `whoami`@`hostname`)"\" \
            -DVERSION_TAG=\"$(BUILD_TAG)\" \
            -DVERSION_BUILD=\"$(BUILD_INFO)\"

# define any directories containing header files other than /usr/include
INCLUDES = -I./include

# define library paths in addition to /usr/lib
LFLAGS =

# define any libraries to link into executable:
LIBS = -lstdc++fs

# define src and obj directories
SRC_DIR = src

# define build directory
OBJ_DIR = build

# define the C source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# define the C object files 
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# define the executable file 
MAIN = smartmeter

###############
### targets ###
###############

.PHONY: build clean install

all: build $(MAIN)

build:
	-@ mkdir -p $(OBJ_DIR)

$(MAIN): $(OBJS) 
	$(CPP) $(CPPFLAGS) $(INCLUDES) -o $(OBJ_DIR)/$(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	-@ $(RM) $(OBJS) $(OBJ_DIR)/$(MAIN) *~

# define install directories
ifeq ($(PREFIX),)
  PREFIX = /usr/local
endif

install: all
	install -d $(DESTDIR)$(PREFIX)/bin/ 
	install -m 755 $(MAIN) $(DESTDIR)$(PREFIX)/bin/
