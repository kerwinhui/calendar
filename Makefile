INCLUDE_DIR=./include/
CXXFLAGS=-Wall -fexceptions -I$(INCLUDE_DIR)
LDFLAGS=-Wl,--as-needed,-O1
COMMONOBJS=\
	calendar.o \
	date.o \
	main.o
COMMONBIN=calendar
DATEHEAD=\
	beautyexception.h \
	date.h \
	debug.h \
	exception.h
CAL_HEAD=$(DATEHEAD) \
	calendar.h
MAINHEAD=$(CAL_HEAD)
COMMONHEAD=$(DATEHEAD) $(CAL_HEAD) $(MAINHEAD)
DEBUGDIR=debug/

.PHONY: all .all-debug .all-release .release-executable .all-documentation
.PHONY: .debug-executable .debug-directory
.PHONY: release
.PHONY: debug
.PHONY: .all-documentation

all: .all-debug .all-release documentation
	@echo Building target $@

release: .all-release
debug: .all-debug

.all-documentation: documentation
	@echo Building target $@

documentation: Doxyfile $(sort $(addprefix include/,$(COMMONHEAD)) $(COMMONOBJS:.o=.cc))
	@echo Building target $@
	mkdir -pv documentation
	doxygen Doxyfile > /dev/null

.all-release: CXXFLAGS += -O2 -DNDEBUG
.all-release: .release-executable
	@echo Building target $@

.all-debug: CXXFLAGS += -g -DDEBUG
.all-debug: LDFLAGS  += -g
.all-debug: .debug-directory .debug-executable
	@echo Building target $@

.debug-directory:
	@echo Building target $@
	mkdir -pv $(DEBUGDIR)

.release-executable: $(COMMONBIN)
	@echo Building target $@

.debug-executable: $(addprefix $(DEBUGDIR),$(COMMONBIN))
	@echo Building target $@

calendar: $(COMMONOBJS)
	@echo Building target $@
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@
	strip $@

$(DEBUGDIR)calendar: $(addprefix $(DEBUGDIR),$(COMMONOBJS))
	@echo Building target $@
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

$(DEBUGDIR)calendar.o calendar.o: calendar.cc $(addprefix include/,$(CAL_HEAD))
	@echo Building target $@
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DEBUGDIR)date.o date.o: date.cc $(addprefix include/,$(DATEHEAD))
	@echo Building target $@
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DEBUGDIR)main.o main.o: main.cc $(addprefix include/,$(MAINHEAD))
	@echo Building target $@
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean .clean-debug .clean-release

clean: .clean-debug .clean-release
	@echo Building target $@

.clean-debug:
	@echo Building target $@
	rm -f $(DEBUGDIR)*.o $(DEBUGDIR)calendar $(DEBUGDIR)*.log

.clean-release:
	@echo Building target $@
	rm -f *.o calendar *.log
