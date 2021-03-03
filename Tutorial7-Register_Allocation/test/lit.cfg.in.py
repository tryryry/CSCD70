import lit
import lit.llvm

lit.llvm.initialize(lit_config, config)

from lit.llvm import llvm_config


config.name = 'CSCD70RegAlloc'
config.test_format = lit.formats.ShTest()
config.test_source_root = "@CMAKE_CURRENT_SOURCE_DIR@"
config.test_exec_root   = "@CMAKE_CURRENT_BINARY_DIR@"
config.suffixes = ['.c', '.ll']

config.substitutions.append((r'%dylibext', "@CMAKE_SHARED_LIBRARY_SUFFIX@"))
config.substitutions.append((r'%dylibdir', "@CMAKE_LIBRARY_OUTPUT_DIR@"))

config.llvm_config_bindir = "@LLVM_BINDIR@"
llvm_config.add_tool_substitutions(
        ["llc", "FileCheck"],
        config.llvm_config_bindir)