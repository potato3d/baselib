#-----------------------------------------------------------------------------------------------------------------------
# end configuring default project
#-----------------------------------------------------------------------------------------------------------------------
CONFIG += no_include_pwd

#-----------------------------------------------------------------------------------------------------------------------
# include directories
#-----------------------------------------------------------------------------------------------------------------------
INCLUDEPATH += $${ROOT_DIR}/sources
DEPENDPATH  += $$INCLUDEPATH

#-----------------------------------------------------------------------------------------------------------------------
# watch for changes in dependencies
#-----------------------------------------------------------------------------------------------------------------------
PRE_TARGETDEPS = $${LIBS}

#-----------------------------------------------------------------------------------------------------------------------
# intermediate output configuration
#-----------------------------------------------------------------------------------------------------------------------
OBJECTS_DIR = $${ROOT_DIR}/temp/$${TARGET}/$${TARGET_SUFFIX}$${TARGET_DEBUG}
MOC_DIR     = $${ROOT_DIR}/temp/$${TARGET}/$${TARGET_SUFFIX}$${TARGET_DEBUG}
RCC_DIR     = $${ROOT_DIR}/temp/$${TARGET}/$${TARGET_SUFFIX}$${TARGET_DEBUG}
UI_DIR      = $${ROOT_DIR}/temp/$${TARGET}/$${TARGET_SUFFIX}$${TARGET_DEBUG}

#-----------------------------------------------------------------------------------------------------------------------
# final output configuration
#-----------------------------------------------------------------------------------------------------------------------
TARGET = $${TARGET}_$${TARGET_SUFFIX}$${TARGET_DEBUG}

#-----------------------------------------------------------------------------------------------------------------------
# compiler architecture switch
#-----------------------------------------------------------------------------------------------------------------------
isEmpty(BUILD_32_BITS) {
	QMAKE_CXXFLAGS 	+= -m64
} else {
	QMAKE_CXXFLAGS 	+= -m32
}

#-----------------------------------------------------------------------------------------------------------------------
# enable compiler options
#-----------------------------------------------------------------------------------------------------------------------
# -std=c++11		= enable C++11
# -pipe				= Use pipes rather than temporary files for communication between the various stages of compilation
# -fconserve-space	= Put uninitialized or runtime-initialized global variables into the common segment
# -Wfatal-errors	= halt on first error
QMAKE_CXXFLAGS += -std=c++11 -pipe -fconserve-space -Wfatal-errors

#-----------------------------------------------------------------------------------------------------------------------
# enable compiler warning flags
#-----------------------------------------------------------------------------------------------------------------------
# Ref : http://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
# Ref : http://linux.die.net/man/1/gcc

# -Wall 						= enables all the warnings
# -Werror						= make all warnings into errors
# -Wextra						= enables some extra warning flags that are not enabled by -Wall
# -Wunused						= Warn whenever a function parameter is unused aside from its declaration. warn if a caller of a function does not use its return value
# -Wredundant-decls				= Warn if anything is declared more than once in the same scope
# -Wuninitialized				= Warn if an automatic variable is used without first being initialized
# -Wswitch-default				= Warn whenever a switch statement does not have a default case
# -Winit-self					= Warn about uninitialized variables which are initialized with themselves
# -Wignored-qualifiers			= Warn if the return type of a function has a type qualifier such as const
# -Wmissing-include-dirs		= Warn if a user-supplied include directory does not exist
# -Wsync-nand					= Warn when __sync_fetch_and_nand and __sync_nand_and_fetch built-in functions are used
# DISABLED -Wfloat-equal		= Warn if floating point values are used in equality comparisons
# -Wundef						= Warn if an undefined identifier is evaluated in an `#if' directive
# -Wpointer-arith				= Warn about anything that depends on the "size of" a function type or of void
# -Wcast-qual					= Warn whenever a pointer is cast so as to remove a type qualifier from the target type
# -Wcast-align					= Warn whenever a pointer is cast such that the required alignment of the target is increased
# -Wlogical-op					= Warn about suspicious uses of logical operators in expressions
# DISABLED -Wmissing-declarations		= Warn if a global function is defined without a previous declaration (problem - generated qrc.cpp)
# -Wmissing-format-attribute	= Warn about function pointers which might be candidates for format attributes
# -Wpacked						= Warn if a structure is given the packed attribute, but the packed attribute has no effect on the layout or size of the structure
# -Wdisabled-optimization		= Warn if a requested optimization pass is disabled (code is too big or too complex)
# -Wabi							= Warn when G++ generates code that is probably not compatible with the vendor-neutral C ++ ABI
# -Wctor-dtor-privacy			= Warn when a class seems unusable because all the constructors or destructors in that class are private, and it has neither friends nor public static member functions
# -Wstrict-null-sentinel		= Warn also about the use of an uncasted "NULL" as sentinel
# -Woverloaded-virtual			= Warn when a function declaration hides virtual functions from a base class
# -Wsign-promo					= Warn when overload resolution chooses a promotion from unsigned or enumerated type to a signed type, over a conversion to an unsigned type of the same size
# -Wparentheses					= Warn if parentheses are omitted in certain contexts
# -Wendif-labels				= Warn whenever an #else or an #endif are followed by text
# -Wswitch-enum					= Warn whenever a switch statement has an index of enumerated type and lacks a case for one or more of the named codes of that enumeration
# -pedantic-errors				= Issue all the mandatory diagnostics, and make all mandatory diagnostics into errors.
# DISABLED -Weffc++				= Warn about violations of the following style guidelines from Scott Meyers' Effective C ++ book
# DISABLED -Wsystem-headers		= Print warning messages for constructs found in system header files
# -Wshadow						= Warn whenever a local variable or type declaration shadows another variable, parameter, type, or class member (in C++), or whenever a built-in function is shadowed
# DISABLED -Wconversion			= Warn for implicit conversions that may alter a value
# DISABLED -Wsign-conversion	= Warn for implicit conversions that may change the sign of an integer value, like assigning a signed integer expression to an unsigned integer variable
# DISABLED -Waggregate-return	= Warn if any functions that return structures or unions are defined or called
# DISABLED -Wpadded				= Warn if padding is included in a structure, either to align an element of the structure or to align the whole structure
# -Wvla							= Warn if variable length array is used in the code
# DISABLED -fstack-protector -Wstack-protector = It warns about functions that will not be protected against stack smashing
# DISABLED -Wunused-macros		= Warn about macros defined in the main file that are unused
# DISABLED -Winline				= Warn if a function can not be inlined and it was declared as inline.
# DISABLED -Wold-style-cast		= Warn if an old-style (C-style) cast to a non-void type is used within a C ++ program
# -Wnon-virtual-dtor			= Warn when a class has virtual functions and accessible non-virtual destructor, in which case it would be possible but unsafe to delete an instance of a derived class through a pointer to the base class

QMAKE_CXXFLAGS += -Wall -Werror -Wextra -Wunused-parameter -Wunused -Wredundant-decls -Wuninitialized -Wswitch-default -Winit-self -Wignored-qualifiers
QMAKE_CXXFLAGS += -Wmissing-include-dirs -Wsync-nand -Wundef -Wpointer-arith -Wcast-qual -Wcast-align -Wlogical-op -Wmissing-format-attribute
QMAKE_CXXFLAGS += -Wpacked -Wdisabled-optimization -Wctor-dtor-privacy -Wstrict-null-sentinel -Woverloaded-virtual -Wsign-promo -Wparentheses -Wendif-labels
QMAKE_CXXFLAGS += -Wswitch-enum -pedantic-errors -Wshadow -Wvla -Wnon-virtual-dtor

CFLAGS += -pipe -Wfatal-errors

CFLAGS += -Wall -Werror -Wextra -Wunused-parameter -Wunused -Wuninitialized -Wswitch-default -Winit-self -Wignored-qualifiers
CFLAGS += -Wmissing-include-dirs -Wsync-nand -Wundef -Wpointer-arith -Wcast-qual -Wcast-align -Wlogical-op -Wmissing-format-attribute
CFLAGS += -Wpacked -Wdisabled-optimization -Wparentheses -Wendif-labels
CFLAGS += -Wswitch-enum -pedantic-errors -Wshadow -Wvla

QMAKE_CFLAGS_DEBUG += -O0 $$CFLAGS
QMAKE_CFLAGS_RELEASE += -O3 $$CFLAGS

# enable me on gcc 4.6
# DISABLED -Wdouble-promotion	= Give a warning when a value of type float is implicitly promoted to double
# DISABLED -Wsuggest-attribute=[pure|const|noreturn] = Warn for cases where adding an attribute may be beneficial
